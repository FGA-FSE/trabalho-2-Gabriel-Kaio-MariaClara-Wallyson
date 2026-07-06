#include "button_task.h"
#include "config.h"
#include "common.h"
#include "buzzer_task.h"
#include "display_task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "ButtonTask";

#define DEBOUNCE_TIME_MS 50
#define LONG_PRESS_TIME_MS 1000
#define DOUBLE_CLICK_TIME_MS 400

bool button_is_pressed() {
    // O botão é ativo baixo (resistor pull-up)
    return gpio_get_level(BUTTON_PIN) == 0;
}

static void button_task(void *pvParameters) {
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .pull_down_en = 0,
        .pull_up_en = 1
    };
    gpio_config(&io_conf);

    bool last_state = false;
    bool is_long_press = false;
    int64_t press_time = 0;
    int64_t release_time = 0;
    int click_count = 0;
    int64_t last_click_time = 0;

    while (1) {
        bool current_state = button_is_pressed();
        int64_t now = esp_timer_get_time() / 1000; // ms

        if (current_state && !last_state) {
            // Button pressed
            press_time = now;
            is_long_press = false;
        } else if (!current_state && last_state) {
            // Button released
            if ((now - press_time) > DEBOUNCE_TIME_MS) {
                if (!is_long_press) {
                    click_count++;
                    last_click_time = now;
                }
            }
        } else if (current_state && last_state) {
            // Button held down
            if (!is_long_press && (now - press_time) > LONG_PRESS_TIME_MS) {
                is_long_press = true;
                ESP_LOGI(TAG, "Long Press Detectado - Mudando Modo");
                buzzer_beep(2, 100, 100);
                
                // Toggle mode
                irrigation_mode_t new_mode = MODE_AUTO;
                xSemaphoreTake(irrigation_state_mutex, portMAX_DELAY);
                if (g_irrigation_state.mode == MODE_AUTO) new_mode = MODE_MANUAL;
                xSemaphoreGive(irrigation_state_mutex);
                
                irrigation_cmd_t cmd = { .type = CMD_SET_MODE, .param = new_mode };
                xQueueSend(irrigation_cmd_queue, &cmd, 0);
            }
        }

        // Process clicks after timeout
        if (click_count > 0 && !current_state && (now - last_click_time) > DOUBLE_CLICK_TIME_MS) {
            if (click_count == 1) {
                ESP_LOGI(TAG, "Single Click Detectado - Trocando Tela");
                buzzer_beep(1, 50, 0);
                display_next_screen();
            } else if (click_count == 2) {
                ESP_LOGI(TAG, "Double Click Detectado - Irrigar Manual");
                buzzer_beep(2, 50, 50);
                irrigation_cmd_t cmd = { .type = CMD_START_MANUAL, .param = DEFAULT_IRRIGATION_DURATION };
                xQueueSend(irrigation_cmd_queue, &cmd, 0);
            }
            click_count = 0;
        }

        last_state = current_state;
        vTaskDelay(pdMS_TO_TICKS(BUTTON_POLL_INTERVAL_MS));
    }
}

void button_task_init() {
    xTaskCreatePinnedToCore(button_task, "button_task", 2048, NULL, 4, NULL, 0);
}
