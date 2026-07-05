#include "mqtt_manager.h"
#include "config.h"
#include "mqtt_client.h"
#include "esp_log.h"
#include "cJSON.h"
#include "freertos/task.h"
#include "esp_wifi.h"

static const char *TAG = "MQTT";
static esp_mqtt_client_handle_t client = NULL;

static void process_rpc_request(const char *topic, int topic_len, const char *data, int data_len) {
    // Tópico é do tipo: v1/devices/me/rpc/request/{request_id}
    char topic_str[128];
    snprintf(topic_str, sizeof(topic_str), "%.*s", topic_len, topic);
    
    char *request_id_str = strrchr(topic_str, '/');
    if (!request_id_str) return;
    request_id_str++; // Pular o '/'
    int request_id = atoi(request_id_str);

    char data_str[256];
    snprintf(data_str, sizeof(data_str), "%.*s", data_len, data);
    
    cJSON *json = cJSON_Parse(data_str);
    if (!json) return;

    cJSON *method = cJSON_GetObjectItem(json, "method");
    if (!method || !cJSON_IsString(method)) {
        cJSON_Delete(json);
        return;
    }

    cJSON *params = cJSON_GetObjectItem(json, "params");
    cJSON *response_json = cJSON_CreateObject();
    bool send_response = true;

    if (strcmp(method->valuestring, "irrigate") == 0) {
        int duration = 20;
        if (params && cJSON_IsNumber(cJSON_GetObjectItem(params, "duration"))) {
            duration = cJSON_GetObjectItem(params, "duration")->valueint;
        }
        
        irrigation_cmd_t cmd = { .type = CMD_START_MANUAL, .param = duration };
        xQueueSend(irrigation_cmd_queue, &cmd, 0);
        cJSON_AddStringToObject(response_json, "status", "ok");
        
    } else if (strcmp(method->valuestring, "stop") == 0) {
        irrigation_cmd_t cmd = { .type = CMD_STOP, .param = 0 };
        xQueueSend(irrigation_cmd_queue, &cmd, 0);
        cJSON_AddStringToObject(response_json, "status", "ok");
        
    } else if (strcmp(method->valuestring, "setMode") == 0) {
        if (params && cJSON_IsString(cJSON_GetObjectItem(params, "mode"))) {
            const char *m = cJSON_GetObjectItem(params, "mode")->valuestring;
            irrigation_cmd_t cmd = { .type = CMD_SET_MODE, .param = (strcmp(m, "AUTO") == 0 ? 0 : 1) };
            xQueueSend(irrigation_cmd_queue, &cmd, 0);
            cJSON_AddStringToObject(response_json, "status", "ok");
        } else {
            cJSON_AddStringToObject(response_json, "error", "invalid param");
        }
    } else if (strcmp(method->valuestring, "setThreshold") == 0) {
        if (params && cJSON_IsNumber(cJSON_GetObjectItem(params, "threshold"))) {
            int th = cJSON_GetObjectItem(params, "threshold")->valueint;
            irrigation_cmd_t cmd = { .type = CMD_SET_THRESHOLD, .param = th };
            xQueueSend(irrigation_cmd_queue, &cmd, 0);
            cJSON_AddStringToObject(response_json, "status", "ok");
        } else {
            cJSON_AddStringToObject(response_json, "error", "invalid param");
        }
    } else if (strcmp(method->valuestring, "getStatus") == 0) {
        xSemaphoreTake(irrigation_state_mutex, portMAX_DELAY);
        cJSON_AddStringToObject(response_json, "mode", g_irrigation_state.mode == MODE_AUTO ? "AUTO" : "MANUAL");
        cJSON_AddNumberToObject(response_json, "threshold", g_irrigation_state.moisture_threshold);
        cJSON_AddBoolToObject(response_json, "pump_on", g_irrigation_state.pump_state == PUMP_ON);
        xSemaphoreGive(irrigation_state_mutex);
    } else {
        cJSON_AddStringToObject(response_json, "error", "method not found");
    }

    if (send_response) {
        char response_topic[128];
        snprintf(response_topic, sizeof(response_topic), TB_RPC_RESPONSE_TOPIC_FMT, request_id);
        char *resp_str = cJSON_PrintUnformatted(response_json);
        esp_mqtt_client_publish(client, response_topic, resp_str, 0, 1, 0);
        free(resp_str);
    }

    cJSON_Delete(response_json);
    cJSON_Delete(json);
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Conectado ao Broker MQTT");
            xEventGroupSetBits(wifi_mqtt_event_group, MQTT_CONNECTED_BIT);
            esp_mqtt_client_subscribe(client, "v1/devices/me/rpc/request/+", 1);
            mqtt_publish_attributes();
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "Desconectado do Broker MQTT");
            xEventGroupClearBits(wifi_mqtt_event_group, MQTT_CONNECTED_BIT);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "Dados recebidos, topico: %.*s", event->topic_len, event->topic);
            if (strncmp(event->topic, "v1/devices/me/rpc/request/", 26) == 0) {
                process_rpc_request(event->topic, event->topic_len, event->data, event->data_len);
            }
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            break;
    }
}

void mqtt_publish_attributes() {
    if (!client || !(xEventGroupGetBits(wifi_mqtt_event_group) & MQTT_CONNECTED_BIT)) return;

    cJSON *json = cJSON_CreateObject();
    
    xSemaphoreTake(irrigation_state_mutex, portMAX_DELAY);
    cJSON_AddStringToObject(json, "mode", g_irrigation_state.mode == MODE_AUTO ? "AUTO" : "MANUAL");
    cJSON_AddNumberToObject(json, "moisture_threshold", g_irrigation_state.moisture_threshold);
    xSemaphoreGive(irrigation_state_mutex);

    cJSON_AddBoolToObject(json, "simulate_soil", SIMULATE_SOIL_WITH_POT);
    cJSON_AddStringToObject(json, "firmware_version", FIRMWARE_VERSION);

    wifi_ap_record_t ap_info;
    if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
        cJSON_AddNumberToObject(json, "wifi_rssi", ap_info.rssi);
    }

    char *json_str = cJSON_PrintUnformatted(json);
    esp_mqtt_client_publish(client, TB_ATTRIBUTES_TOPIC, json_str, 0, 1, 0);
    free(json_str);
    cJSON_Delete(json);
}

static void mqtt_telemetry_task(void *pvParameters) {
    while (1) {
        EventBits_t bits = xEventGroupWaitBits(wifi_mqtt_event_group, MQTT_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
        if (bits & MQTT_CONNECTED_BIT) {
            cJSON *json = cJSON_CreateObject();
            
            xSemaphoreTake(sensor_data_mutex, portMAX_DELAY);
            cJSON_AddNumberToObject(json, "soil_moisture", g_sensor_data.soil_moisture);
            cJSON_AddNumberToObject(json, "air_temperature", g_sensor_data.air_temperature);
            cJSON_AddNumberToObject(json, "air_humidity", g_sensor_data.air_humidity);
            if (g_sensor_data.bme_available) {
                cJSON_AddNumberToObject(json, "bme_pressure", g_sensor_data.bme_pressure);
            }
            cJSON_AddNumberToObject(json, "water_distance_cm", g_sensor_data.water_distance_cm);
            cJSON_AddBoolToObject(json, "water_level_ok", g_sensor_data.water_level_ok);
            xSemaphoreGive(sensor_data_mutex);

            xSemaphoreTake(irrigation_state_mutex, portMAX_DELAY);
            cJSON_AddBoolToObject(json, "pump_state", g_irrigation_state.pump_state == PUMP_ON);
            xSemaphoreGive(irrigation_state_mutex);

            char *json_str = cJSON_PrintUnformatted(json);
            esp_mqtt_client_publish(client, TB_TELEMETRY_TOPIC, json_str, 0, 1, 0);
            free(json_str);
            cJSON_Delete(json);
        }
        vTaskDelay(pdMS_TO_TICKS(MQTT_TELEMETRY_INTERVAL_MS));
    }
}

void mqtt_manager_init() {
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = TB_BROKER_URI,
        .broker.address.port = TB_BROKER_PORT,
        .credentials.username = TB_ACCESS_TOKEN,
        .credentials.client_id = "ESP32_SmartFlora"
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);

    xTaskCreatePinnedToCore(mqtt_telemetry_task, "mqtt_telemetry", 4096, NULL, 3, NULL, 0);
}
