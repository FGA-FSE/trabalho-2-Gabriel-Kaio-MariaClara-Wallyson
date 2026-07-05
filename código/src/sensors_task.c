#include "sensors_task.h"
#include "config.h"
#include "common.h"
#include "dht11.h"
#include "bme280_driver.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"

static const char *TAG = "SensorsTask";
static adc_oneshot_unit_handle_t adc1_handle;
static bool bme280_present = false;

static void init_adc() {
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
        .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc1_handle));

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_12,
    };
    // GPIO34 é ADC1_CHANNEL_6
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_6, &config));
}

static int map(int x, int in_min, int in_max, int out_min, int out_max) {
    if (x <= in_min) return out_min;
    if (x >= in_max) return out_max;
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static void sensors_task(void *pvParameters) {
    init_adc();
    
    // Init BME280
    esp_err_t err = bme280_init(I2C_MASTER_NUM, BME280_I2C_ADDR, &bme280_present);
    if (err == ESP_OK) {
        xSemaphoreTake(sensor_data_mutex, portMAX_DELAY);
        g_sensor_data.bme_available = true;
        xSemaphoreGive(sensor_data_mutex);
    } else {
        ESP_LOGW(TAG, "BME280 nao inicializado. Usando DHT11 para temperatura/umidade.");
    }

    while (1) {
        // 1. Ler potenciômetro (simulando umidade do solo)
        int pot_raw = 0;
        adc_oneshot_read(adc1_handle, ADC_CHANNEL_6, &pot_raw);
        int soil_moisture = map(pot_raw, SOIL_WATER_VALUE, SOIL_AIR_VALUE, 100, 0);
        if (soil_moisture < 0) soil_moisture = 0;
        if (soil_moisture > 100) soil_moisture = 100;

        // 2. Ler DHT11
        dht11_reading_t dht_data = {0};
        bool dht_ok = (dht11_read(DHT_PIN, &dht_data) == ESP_OK);

        // 3. Ler BME280 se disponível
        float bme_temp = 0, bme_press = 0, bme_hum = 0;
        bool bme_ok = false;
        if (bme280_present) {
            bme_ok = (bme280_read_data(I2C_MASTER_NUM, BME280_I2C_ADDR, &bme_temp, &bme_press, &bme_hum) == ESP_OK);
        }

        // Atualizar struct protegida por mutex
        xSemaphoreTake(sensor_data_mutex, portMAX_DELAY);
        g_sensor_data.soil_moisture = soil_moisture;
        if (dht_ok) {
            g_sensor_data.air_humidity = dht_data.humidity;
            if (!bme280_present) {
                g_sensor_data.air_temperature = dht_data.temperature;
            }
        }
        if (bme_ok) {
            g_sensor_data.bme_temperature = bme_temp;
            g_sensor_data.bme_pressure = bme_press;
            if (bme280_present) { // Se for BME280 e não BMP280, usar umidade
                g_sensor_data.bme_humidity = bme_hum;
                g_sensor_data.air_humidity = bme_hum;
            }
            g_sensor_data.air_temperature = bme_temp; // BME tem prioridade
        }
        xSemaphoreGive(sensor_data_mutex);

        ESP_LOGI(TAG, "Sensores lidos: Solo=%d%%, T=%.1fC, H=%.1f%%", 
                 soil_moisture, g_sensor_data.air_temperature, g_sensor_data.air_humidity);

        vTaskDelay(pdMS_TO_TICKS(SENSOR_READ_INTERVAL_MS));
    }
}

void sensors_task_init() {
    xTaskCreatePinnedToCore(sensors_task, "sensors_task", 4096, NULL, 3, NULL, 1);
}
