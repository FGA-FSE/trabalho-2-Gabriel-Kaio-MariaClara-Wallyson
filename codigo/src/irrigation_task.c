#include "irrigation_task.h"
#include "config.h"
#include "common.h"
#include "mqtt_manager.h"
#include "buzzer_task.h"
#include "nvs_storage.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "IrrigationTask";

static void set_pump(pump_state_t state) {
    xSemaphoreTake(irrigation_state_mutex, portMAX_DELAY);
    g_irrigation_state.pump_state = state;
    xSemaphoreGive(irrigation_state_mutex);
    
    // O relé geralmente é ativo baixo, mas verificamos a configuração
    int level = (state == PUMP_ON) ? (RELAY_ACTIVE_LOW ? 0 : 1) : (RELAY_ACTIVE_LOW ? 1 : 0);
    gpio_set_level(RELAY_PIN, level);
    
    mqtt_publish_attributes();
}

static void irrigation_task(void *pvParameters) {
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << RELAY_PIN),
        .pull_down_en = 0,
        .pull_up_en = 0
    };
    gpio_config(&io_conf);
    gpio_set_level(RELAY_PIN, RELAY_ACTIVE_LOW ? 1 : 0); // Off

    // Load saved settings
    irrigation_mode_t saved_mode = MODE_AUTO;
    int saved_threshold = DEFAULT_MOISTURE_THRESHOLD;
    nvs_load_irrigation_mode(&saved_mode);
    nvs_load_moisture_threshold(&saved_threshold);

    xSemaphoreTake(irrigation_state_mutex, portMAX_DELAY);
    g_irrigation_state.mode = saved_mode;
    g_irrigation_state.moisture_threshold = saved_threshold;
    g_irrigation_state.phase = PHASE_IDLE;
    g_irrigation_state.pump_state = PUMP_OFF;
    xSemaphoreGive(irrigation_state_mutex);

    int64_t cooldown_start_us = 0;

    while (1) {
        irrigation_cmd_t cmd;
        // Aguarda até 100ms por um comando
        if (xQueueReceive(irrigation_cmd_queue, &cmd, pdMS_TO_TICKS(100)) == pdTRUE) {
            xSemaphoreTake(irrigation_state_mutex, portMAX_DELAY);
            switch (cmd.type) {
                case CMD_START_MANUAL:
                    if (g_irrigation_state.phase == PHASE_IDLE || g_irrigation_state.phase == PHASE_COOLDOWN) {
                        g_irrigation_state.phase = PHASE_IRRIGATING;
                        g_irrigation_state.pump_duration_s = cmd.param;
                        g_irrigation_state.pump_start_time_us = esp_timer_get_time();
                        
                        xSemaphoreTake(sensor_data_mutex, portMAX_DELAY);
                        g_irrigation_state.soil_before_irrigation = g_sensor_data.soil_moisture;
                        xSemaphoreGive(sensor_data_mutex);
                        
                        ESP_LOGI(TAG, "Iniciando irrigação manual por %ds", cmd.param);
                    }
                    break;
                case CMD_STOP:
                    if (g_irrigation_state.phase == PHASE_IRRIGATING) {
                        g_irrigation_state.phase = PHASE_COOLDOWN;
                        cooldown_start_us = esp_timer_get_time();
                        ESP_LOGI(TAG, "Irrigação parada via comando");
                    }
                    break;
                case CMD_SET_MODE:
                    g_irrigation_state.mode = (irrigation_mode_t)cmd.param;
                    nvs_save_irrigation_mode(g_irrigation_state.mode);
                    ESP_LOGI(TAG, "Modo alterado para %d", g_irrigation_state.mode);
                    break;
                case CMD_SET_THRESHOLD:
                    g_irrigation_state.moisture_threshold = cmd.param;
                    nvs_save_moisture_threshold(g_irrigation_state.moisture_threshold);
                    ESP_LOGI(TAG, "Threshold alterado para %d%%", g_irrigation_state.moisture_threshold);
                    break;
            }
            xSemaphoreGive(irrigation_state_mutex);
            mqtt_publish_attributes();
        }

        // --- State Machine ---
        xSemaphoreTake(irrigation_state_mutex, portMAX_DELAY);
        irrigation_phase_t phase = g_irrigation_state.phase;
        irrigation_mode_t mode = g_irrigation_state.mode;
        int threshold = g_irrigation_state.moisture_threshold;
        int duration_s = g_irrigation_state.pump_duration_s;
        int64_t start_time_us = g_irrigation_state.pump_start_time_us;
        xSemaphoreGive(irrigation_state_mutex);

        xSemaphoreTake(sensor_data_mutex, portMAX_DELAY);
        int current_moisture = g_sensor_data.soil_moisture;
        bool water_ok = g_sensor_data.water_level_ok;
        xSemaphoreGive(sensor_data_mutex);

        int64_t now_us = esp_timer_get_time();

        switch (phase) {
            case PHASE_IDLE:
                set_pump(PUMP_OFF);
                if (mode == MODE_AUTO) {
                    if (current_moisture < threshold) {
                        if (water_ok) {
                            xSemaphoreTake(irrigation_state_mutex, portMAX_DELAY);
                            g_irrigation_state.phase = PHASE_IRRIGATING;
                            g_irrigation_state.pump_duration_s = DEFAULT_IRRIGATION_DURATION;
                            g_irrigation_state.pump_start_time_us = now_us;
                            g_irrigation_state.soil_before_irrigation = current_moisture;
                            xSemaphoreGive(irrigation_state_mutex);
                            ESP_LOGI(TAG, "Iniciando irrigação automática (solo = %d%% < %d%%)", current_moisture, threshold);
                            buzzer_beep(3, 100, 100);
                        } else {
                            // Alerta de falta de água se precisar irrigar
                            ESP_LOGW(TAG, "Falta d'agua! Nao e possivel irrigar.");
                            buzzer_beep(5, 50, 50);
                            vTaskDelay(pdMS_TO_TICKS(5000)); // Esperar para não apitar sem parar
                        }
                    }
                }
                break;

            case PHASE_IRRIGATING:
                if (!water_ok) {
                    ESP_LOGW(TAG, "Nível de água baixo durante irrigação. Abortando.");
                    xSemaphoreTake(irrigation_state_mutex, portMAX_DELAY);
                    g_irrigation_state.phase = PHASE_COOLDOWN;
                    xSemaphoreGive(irrigation_state_mutex);
                    cooldown_start_us = now_us;
                    buzzer_beep(3, 300, 100);
                    break;
                }

                set_pump(PUMP_ON);
                
                int64_t elapsed_s = (now_us - start_time_us) / 1000000;
                if (elapsed_s >= duration_s || elapsed_s >= MAX_PUMP_ON_TIME_S) {
                    xSemaphoreTake(irrigation_state_mutex, portMAX_DELAY);
                    g_irrigation_state.phase = PHASE_COOLDOWN;
                    xSemaphoreGive(irrigation_state_mutex);
                    cooldown_start_us = now_us;
                    ESP_LOGI(TAG, "Irrigação finalizada. Tempo: %d s", (int)elapsed_s);
                }
                break;

            case PHASE_COOLDOWN:
                set_pump(PUMP_OFF);
                if ((now_us - cooldown_start_us) / 1000000 >= IRRIGATION_COOLDOWN_S) {
                    xSemaphoreTake(irrigation_state_mutex, portMAX_DELAY);
                    g_irrigation_state.phase = PHASE_IDLE;
                    xSemaphoreGive(irrigation_state_mutex);
                    ESP_LOGI(TAG, "Cooldown finalizado. Voltando para IDLE.");
                }
                break;
        }
    }
}

void irrigation_task_init() {
    xTaskCreatePinnedToCore(irrigation_task, "irrigation_task", 4096, NULL, 4, NULL, 1);
}
