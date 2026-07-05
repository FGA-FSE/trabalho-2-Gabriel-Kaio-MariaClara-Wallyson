#include "ultrasonic_task.h"
#include "config.h"
#include "common.h"
#include "hcsr04.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "UltrasonicTask";

static void ultrasonic_task(void *pvParameters) {
    hcsr04_init(HC_SR04_TRIG_PIN, HC_SR04_ECHO_PIN);

    while (1) {
        float distance = 0;
        esp_err_t err = hcsr04_read_distance(HC_SR04_TRIG_PIN, HC_SR04_ECHO_PIN, &distance);
        
        if (err == ESP_OK) {
            bool ok = (distance < WATER_ALARM_DISTANCE_CM);
            
            xSemaphoreTake(sensor_data_mutex, portMAX_DELAY);
            g_sensor_data.water_distance_cm = distance;
            g_sensor_data.water_level_ok = ok;
            xSemaphoreGive(sensor_data_mutex);
            
            ESP_LOGI(TAG, "Distancia: %.1f cm | Nivel OK: %d", distance, ok);
        } else {
            ESP_LOGW(TAG, "Erro lendo HC-SR04");
        }

        vTaskDelay(pdMS_TO_TICKS(WATER_LEVEL_READ_INTERVAL_MS));
    }
}

void ultrasonic_task_init() {
    xTaskCreatePinnedToCore(ultrasonic_task, "ultrasonic_task", 2048, NULL, 3, NULL, 1);
}
