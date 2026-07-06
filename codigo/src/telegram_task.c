#include "telegram_task.h"
#include "config.h"
#include "common.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_tls.h"
#include "esp_crt_bundle.h"
#include "cJSON.h"
#include <string.h>
#include <stdlib.h>

static const char *TAG = "Telegram";
static int last_update_id = 0;

static void telegram_send_message(const char *chat_id, const char *text) {
    if (strlen(TELEGRAM_BOT_TOKEN) == 0 || strcmp(TELEGRAM_BOT_TOKEN, "SEU_TOKEN_TELEGRAM_AQUI") == 0) return;
    
    char url[256];
    snprintf(url, sizeof(url), "https://api.telegram.org/bot%s/sendMessage", TELEGRAM_BOT_TOKEN);
    
    esp_http_client_config_t config = {
        .url = url,
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client) return;
    
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "chat_id", chat_id);
    cJSON_AddStringToObject(root, "text", text);
    char *post_data = cJSON_PrintUnformatted(root);
    
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    
    esp_err_t err = esp_http_client_perform(client);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao enviar msg: %s", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "Mensagem enviada via Telegram");
    }
    
    free(post_data);
    cJSON_Delete(root);
    esp_http_client_cleanup(client);
}

static void handle_telegram_command(const char *chat_id, const char *text) {
    if (strcmp(text, "/status") == 0) {
        char msg[256];
        xSemaphoreTake(sensor_data_mutex, portMAX_DELAY);
        int umid = g_sensor_data.soil_moisture;
        float temp = g_sensor_data.air_temperature;
        xSemaphoreGive(sensor_data_mutex);
        
        xSemaphoreTake(irrigation_state_mutex, portMAX_DELAY);
        const char* modo = (g_irrigation_state.mode == MODE_AUTO) ? "Automático" : "Manual";
        bool pump = (g_irrigation_state.pump_state == PUMP_ON);
        xSemaphoreGive(irrigation_state_mutex);
        
        snprintf(msg, sizeof(msg), "SmartFlora Status:\nModo: %s\nBomba: %s\nUmidade Solo: %d%%\nTemp. Ar: %.1fC",
                 modo, pump ? "LIGADA" : "DESLIGADA", umid, temp);
        telegram_send_message(chat_id, msg);
        
    } else if (strcmp(text, "/modo_auto") == 0) {
        irrigation_cmd_t cmd = { .type = CMD_SET_MODE, .param = 0 };
        xQueueSend(irrigation_cmd_queue, &cmd, 0);
        telegram_send_message(chat_id, "Modo alterado para Automático.");
        
    } else if (strcmp(text, "/modo_manual") == 0) {
        irrigation_cmd_t cmd = { .type = CMD_SET_MODE, .param = 1 };
        xQueueSend(irrigation_cmd_queue, &cmd, 0);
        telegram_send_message(chat_id, "Modo alterado para Manual.");
        
    } else if (strcmp(text, "/regar_10s") == 0) {
        irrigation_cmd_t cmd = { .type = CMD_START_MANUAL, .param = 10 };
        xQueueSend(irrigation_cmd_queue, &cmd, 0);
        telegram_send_message(chat_id, "Regando por 10 segundos.");
        
    } else if (strcmp(text, "/regar_20s") == 0) {
        irrigation_cmd_t cmd = { .type = CMD_START_MANUAL, .param = 20 };
        xQueueSend(irrigation_cmd_queue, &cmd, 0);
        telegram_send_message(chat_id, "Regando por 20 segundos.");
    }
}

static void telegram_task(void *pvParameters) {
    char url[256];
    
    while (1) {
        // Wait for network connection
        xEventGroupWaitBits(wifi_mqtt_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
        
        if (strlen(TELEGRAM_BOT_TOKEN) == 0 || strcmp(TELEGRAM_BOT_TOKEN, "SEU_TOKEN_TELEGRAM_AQUI") == 0) {
            vTaskDelay(pdMS_TO_TICKS(10000));
            continue;
        }

        snprintf(url, sizeof(url), "https://api.telegram.org/bot%s/getUpdates?offset=%d&limit=3", TELEGRAM_BOT_TOKEN, last_update_id);
        
        esp_http_client_config_t config = {
            .url = url,
            .transport_type = HTTP_TRANSPORT_OVER_SSL,
            .crt_bundle_attach = esp_crt_bundle_attach,
            .timeout_ms = 5000,
        };
        
        esp_http_client_handle_t client = esp_http_client_init(&config);
        
        esp_err_t err = esp_http_client_open(client, 0);
        if (err == ESP_OK) {
            esp_http_client_fetch_headers(client);
            
            // Alocar buffer para a resposta do getUpdates
            int buffer_size = 4096;
            char *buffer = malloc(buffer_size);
            if (buffer) {
                int read_len = 0;
                int total_read = 0;
                while ((read_len = esp_http_client_read_response(client, buffer + total_read, buffer_size - total_read - 1)) > 0) {
                    total_read += read_len;
                    if (total_read >= buffer_size - 1) break; // Evita overflow
                }
                
                if (total_read >= 0) {
                    buffer[total_read] = '\0';
                    
                    cJSON *json = cJSON_Parse(buffer);
                    if (json) {
                        cJSON *ok = cJSON_GetObjectItem(json, "ok");
                        if (ok && cJSON_IsTrue(ok)) {
                            cJSON *result = cJSON_GetObjectItem(json, "result");
                            int num_results = cJSON_GetArraySize(result);
                            for (int i = 0; i < num_results; i++) {
                                cJSON *item = cJSON_GetArrayItem(result, i);
                                cJSON *update_id = cJSON_GetObjectItem(item, "update_id");
                                if (update_id) {
                                    last_update_id = update_id->valueint + 1;
                                }
                                
                                cJSON *message = cJSON_GetObjectItem(item, "message");
                                if (message) {
                                    cJSON *text = cJSON_GetObjectItem(message, "text");
                                    cJSON *chat = cJSON_GetObjectItem(message, "chat");
                                    if (text && chat && cJSON_IsString(text)) {
                                        cJSON *chat_id = cJSON_GetObjectItem(chat, "id");
                                        if (chat_id) {
                                            char cid_str[32];
                                            snprintf(cid_str, sizeof(cid_str), "%d", chat_id->valueint);
                                            handle_telegram_command(cid_str, text->valuestring);
                                        }
                                    }
                                }
                            }
                        }
                        cJSON_Delete(json);
                    }
                }
                free(buffer);
            }
        }
        esp_http_client_cleanup(client);
        
        vTaskDelay(pdMS_TO_TICKS(TELEGRAM_POLL_INTERVAL_MS));
    }
}

void telegram_task_init() {
    xTaskCreatePinnedToCore(telegram_task, "telegram_task", 6144, NULL, 3, NULL, 0);
}
