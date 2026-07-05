#include "display_task.h"
#include "config.h"
#include "common.h"
#include "ssd1306.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_log.h"
#include <stdio.h>

static const char *TAG = "DisplayTask";

typedef enum {
    SCREEN_MAIN = 0,
    SCREEN_DETAILS = 1,
    SCREEN_SYSTEM = 2,
    SCREEN_COUNT
} display_screen_t;

static display_screen_t current_screen = SCREEN_MAIN;
static int64_t last_screen_change = 0;

void display_next_screen() {
    current_screen = (current_screen + 1) % SCREEN_COUNT;
    last_screen_change = esp_timer_get_time() / 1000;
}

static void display_task(void *pvParameters) {
    esp_err_t err = ssd1306_init(I2C_MASTER_NUM, OLED_I2C_ADDR);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao inicializar OLED");
        vTaskDelete(NULL);
    }

    last_screen_change = esp_timer_get_time() / 1000;

    while (1) {
        int64_t now = esp_timer_get_time() / 1000;
        if (now - last_screen_change > DISPLAY_AUTO_CYCLE_MS) {
            display_next_screen();
        }

        ssd1306_clear();
        char buf[32];

        // Obter copias dos dados
        xSemaphoreTake(sensor_data_mutex, portMAX_DELAY);
        sensor_data_t sd = g_sensor_data;
        xSemaphoreGive(sensor_data_mutex);

        xSemaphoreTake(irrigation_state_mutex, portMAX_DELAY);
        irrigation_state_t is = g_irrigation_state;
        xSemaphoreGive(irrigation_state_mutex);

        switch (current_screen) {
            case SCREEN_MAIN:
                ssd1306_draw_string(0, 0, "SmartFlora", 2);
                snprintf(buf, sizeof(buf), "Solo: %d%%", sd.soil_moisture);
                ssd1306_draw_string(0, 24, buf, 1);
                
                snprintf(buf, sizeof(buf), "Temp: %.1f C", sd.air_temperature);
                ssd1306_draw_string(0, 34, buf, 1);

                snprintf(buf, sizeof(buf), "Modo: %s", is.mode == MODE_AUTO ? "AUTO" : "MANUAL");
                ssd1306_draw_string(0, 44, buf, 1);
                
                snprintf(buf, sizeof(buf), "Bomba: %s", is.pump_state == PUMP_ON ? "LIGADA" : "DESLIG");
                ssd1306_draw_string(0, 54, buf, 1);
                break;

            case SCREEN_DETAILS:
                ssd1306_draw_string(0, 0, "Detalhes", 2);
                snprintf(buf, sizeof(buf), "Umid. Ar: %.1f%%", sd.air_humidity);
                ssd1306_draw_string(0, 24, buf, 1);
                
                if (sd.bme_available) {
                    snprintf(buf, sizeof(buf), "Pres: %.0f hPa", sd.bme_pressure);
                } else {
                    snprintf(buf, sizeof(buf), "Pres: N/A");
                }
                ssd1306_draw_string(0, 34, buf, 1);
                
                snprintf(buf, sizeof(buf), "Dist. Agua: %.1fcm", sd.water_distance_cm);
                ssd1306_draw_string(0, 44, buf, 1);
                
                snprintf(buf, sizeof(buf), "Nivel OK: %s", sd.water_level_ok ? "SIM" : "NAO");
                ssd1306_draw_string(0, 54, buf, 1);
                break;

            case SCREEN_SYSTEM:
                ssd1306_draw_string(0, 0, "Sistema", 2);
                snprintf(buf, sizeof(buf), "Simular: %s", SIMULATE_SOIL_WITH_POT ? "SIM" : "NAO");
                ssd1306_draw_string(0, 24, buf, 1);
                
                snprintf(buf, sizeof(buf), "Limiar: %d%%", is.moisture_threshold);
                ssd1306_draw_string(0, 34, buf, 1);

                EventBits_t bits = xEventGroupGetBits(wifi_mqtt_event_group);
                snprintf(buf, sizeof(buf), "WiFi: %s", (bits & WIFI_CONNECTED_BIT) ? "OK" : "ERR");
                ssd1306_draw_string(0, 44, buf, 1);

                snprintf(buf, sizeof(buf), "MQTT: %s", (bits & MQTT_CONNECTED_BIT) ? "OK" : "ERR");
                ssd1306_draw_string(0, 54, buf, 1);
                break;
            default:
                break;
        }

        ssd1306_display(I2C_MASTER_NUM, OLED_I2C_ADDR);
        vTaskDelay(pdMS_TO_TICKS(DISPLAY_UPDATE_INTERVAL_MS));
    }
}

void display_task_init() {
    xTaskCreatePinnedToCore(display_task, "display_task", 4096, NULL, 2, NULL, 0);
}
