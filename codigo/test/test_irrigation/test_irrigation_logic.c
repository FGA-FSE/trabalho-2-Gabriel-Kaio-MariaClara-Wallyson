#include <unity.h>
#include "common.h"
#include "config.h"
#include "irrigation_task.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Globais falsas para o teste
SemaphoreHandle_t    sensor_data_mutex;
SemaphoreHandle_t    irrigation_state_mutex;
QueueHandle_t        buzzer_queue;
QueueHandle_t        irrigation_cmd_queue;
EventGroupHandle_t   wifi_mqtt_event_group;

sensor_data_t        g_sensor_data;
irrigation_state_t   g_irrigation_state;

// Função simulada para o MQTT Manager
void mqtt_publish_attributes() {
    // Não faz nada durante os testes
}

// Função simulada para o Buzzer
void buzzer_beep(int count, int duration_ms, int gap_ms) {
    // Não faz nada durante os testes
}

// Emular a carga do NVS retornando falha (para usar defaults)
#include "esp_err.h"
esp_err_t nvs_load_irrigation_mode(irrigation_mode_t *mode) { return ESP_FAIL; }
esp_err_t nvs_load_moisture_threshold(int *threshold) { return ESP_FAIL; }
esp_err_t nvs_save_irrigation_mode(irrigation_mode_t mode) { return ESP_OK; }
esp_err_t nvs_save_moisture_threshold(int threshold) { return ESP_OK; }

void setUp(void) {
    // Recria mecanismos a cada teste
    sensor_data_mutex = xSemaphoreCreateMutex();
    irrigation_state_mutex = xSemaphoreCreateMutex();
    buzzer_queue = xQueueCreate(10, sizeof(buzzer_cmd_t));
    irrigation_cmd_queue = xQueueCreate(5, sizeof(irrigation_cmd_t));
    
    // Reseta estado global
    g_sensor_data.soil_moisture = 50;
    g_sensor_data.water_level_ok = true;
    
    g_irrigation_state.mode = MODE_AUTO;
    g_irrigation_state.phase = PHASE_IDLE;
    g_irrigation_state.pump_state = PUMP_OFF;
    g_irrigation_state.moisture_threshold = 30;
}

void tearDown(void) {
    vQueueDelete(buzzer_queue);
    vQueueDelete(irrigation_cmd_queue);
    vSemaphoreDelete(sensor_data_mutex);
    vSemaphoreDelete(irrigation_state_mutex);
}

void test_manual_irrigation_command(void) {
    // Inicializa a task de irrigação
    irrigation_task_init();
    vTaskDelay(pdMS_TO_TICKS(100)); // Espera a task iniciar e ler a NVS simulada
    
    // Confirma estado inicial
    TEST_ASSERT_EQUAL(PHASE_IDLE, g_irrigation_state.phase);

    // Manda comando de irrigar manual
    irrigation_cmd_t cmd = { .type = CMD_START_MANUAL, .param = 10 };
    xQueueSend(irrigation_cmd_queue, &cmd, 0);

    // Espera a máquina de estados processar o comando
    vTaskDelay(pdMS_TO_TICKS(200));

    // Confere se entrou em modo de irrigação
    TEST_ASSERT_EQUAL(PHASE_IRRIGATING, g_irrigation_state.phase);
    TEST_ASSERT_EQUAL(PUMP_ON, g_irrigation_state.pump_state);
}

void test_water_level_safety(void) {
    // Começamos dizendo que a água está OK
    g_sensor_data.water_level_ok = true;
    
    irrigation_task_init();
    vTaskDelay(pdMS_TO_TICKS(100));

    // Manda comando
    irrigation_cmd_t cmd = { .type = CMD_START_MANUAL, .param = 10 };
    xQueueSend(irrigation_cmd_queue, &cmd, 0);
    
    vTaskDelay(pdMS_TO_TICKS(200));
    TEST_ASSERT_EQUAL(PHASE_IRRIGATING, g_irrigation_state.phase);

    // Agora simulamos que a água acabou no ultrassônico
    xSemaphoreTake(sensor_data_mutex, portMAX_DELAY);
    g_sensor_data.water_level_ok = false;
    xSemaphoreGive(sensor_data_mutex);

    // Espera a máquina de estados perceber
    vTaskDelay(pdMS_TO_TICKS(200));

    // Verifica se bloqueou a bomba por segurança e entrou em cooldown
    TEST_ASSERT_EQUAL(PHASE_COOLDOWN, g_irrigation_state.phase);
    TEST_ASSERT_EQUAL(PUMP_OFF, g_irrigation_state.pump_state);
}

void app_main() {
    UNITY_BEGIN();
    RUN_TEST(test_manual_irrigation_command);
    RUN_TEST(test_water_level_safety);
    UNITY_END();
}
