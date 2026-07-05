#include "config.h"
#include "common.h"
#include "nvs_storage.h"
#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "sensors_task.h"
#include "ultrasonic_task.h"
#include "buzzer_task.h"
#include "button_task.h"
#include "display_task.h"
#include "irrigation_task.h"

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "Main";

// Global variables (defined as extern in common.h)
SemaphoreHandle_t    sensor_data_mutex;
SemaphoreHandle_t    irrigation_state_mutex;
QueueHandle_t        buzzer_queue;
QueueHandle_t        irrigation_cmd_queue;
EventGroupHandle_t   wifi_mqtt_event_group;

sensor_data_t        g_sensor_data;
irrigation_state_t   g_irrigation_state;

static void init_globals() {
    sensor_data_mutex = xSemaphoreCreateMutex();
    irrigation_state_mutex = xSemaphoreCreateMutex();
    
    buzzer_queue = xQueueCreate(10, sizeof(buzzer_cmd_t));
    irrigation_cmd_queue = xQueueCreate(5, sizeof(irrigation_cmd_t));
    
    wifi_mqtt_event_group = xEventGroupCreate();

    // Valores padrão
    g_sensor_data.soil_moisture = 0;
    g_sensor_data.air_temperature = 0;
    g_sensor_data.air_humidity = 0;
    g_sensor_data.bme_temperature = 0;
    g_sensor_data.bme_pressure = 0;
    g_sensor_data.bme_humidity = 0;
    g_sensor_data.water_distance_cm = 0;
    g_sensor_data.water_level_ok = true;
    g_sensor_data.bme_available = false;
    g_sensor_data.simulate_soil = SIMULATE_SOIL_WITH_POT;
}

static void init_i2c_master() {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0));
}

void app_main(void) {
    ESP_LOGI(TAG, "Iniciando SmartFlora v%s", FIRMWARE_VERSION);

    // 1. Inicializar Globais e Mutexes/Filas
    init_globals();

    // 2. Inicializar NVS
    ESP_ERROR_CHECK(nvs_storage_init());

    // 3. Inicializar barramento I2C (OLED e BME280 dependem dele)
    init_i2c_master();

    // 4. Checar botão no boot para modo Provisioning (SoftAP)
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .pull_down_en = 0,
        .pull_up_en = 1
    };
    gpio_config(&io_conf);
    
    // Pequeno delay para debounce inicial
    vTaskDelay(pdMS_TO_TICKS(100));
    
    bool start_provisioning = false;
    if (gpio_get_level(BUTTON_PIN) == 0) { // Se botão estiver pressionado no boot
        ESP_LOGW(TAG, "Botão pressionado no boot. Iniciando modo Provisioning (SoftAP)!");
        start_provisioning = true;
    }

    // 5. Inicializar WiFi (SoftAP ou STA)
    wifi_manager_init(start_provisioning);

    // Se estiver em modo provisioning, parar por aqui.
    // O ESP vai servir a página e reiniciar ao salvar credenciais.
    if (start_provisioning) {
        while(1) vTaskDelay(pdMS_TO_TICKS(1000)); 
    }

    // 6. Iniciar Tasks do FreeRTOS
    // Core 0: WiFi, MQTT, Display, Buzzer, Button
    // Core 1: Sensors, Ultrasonic, Irrigation
    
    mqtt_manager_init();
    display_task_init();
    buzzer_task_init();
    button_task_init();
    
    sensors_task_init();
    ultrasonic_task_init();
    irrigation_task_init();

    // Task principal fica em loop de monitoramento/idle
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10000));
        // Pode-se implementar log de memória livre (Heap) aqui
        ESP_LOGI(TAG, "Free heap: %" PRIu32 " bytes", esp_get_free_heap_size());
    }
}
