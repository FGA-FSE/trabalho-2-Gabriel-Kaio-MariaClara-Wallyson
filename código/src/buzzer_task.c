#include "buzzer_task.h"
#include "config.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static void buzzer_task(void *pvParameters) {
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << BUZZER_PIN),
        .pull_down_en = 0,
        .pull_up_en = 0
    };
    gpio_config(&io_conf);
    gpio_set_level(BUZZER_PIN, 0);

    buzzer_cmd_t cmd;
    while (1) {
        if (xQueueReceive(buzzer_queue, &cmd, portMAX_DELAY) == pdTRUE) {
            for (int i = 0; i < cmd.beep_count; i++) {
                gpio_set_level(BUZZER_PIN, 1);
                vTaskDelay(pdMS_TO_TICKS(cmd.duration_ms));
                gpio_set_level(BUZZER_PIN, 0);
                if (i < cmd.beep_count - 1) {
                    vTaskDelay(pdMS_TO_TICKS(cmd.gap_ms));
                }
            }
        }
    }
}

void buzzer_beep(int count, int duration_ms, int gap_ms) {
    buzzer_cmd_t cmd = { .beep_count = count, .duration_ms = duration_ms, .gap_ms = gap_ms };
    xQueueSend(buzzer_queue, &cmd, 0);
}

void buzzer_task_init() {
    xTaskCreatePinnedToCore(buzzer_task, "buzzer_task", 2048, NULL, 1, NULL, 0);
}
