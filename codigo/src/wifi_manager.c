#include "wifi_manager.h"
#include "config.h"
#include "nvs_storage.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include <string.h>

static const char *TAG = "WiFi";

static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        xEventGroupClearBits(wifi_mqtt_event_group, WIFI_CONNECTED_BIT);
        ESP_LOGI(TAG, "Reconectando ao WiFi...");
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Conectado! IP: " IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(wifi_mqtt_event_group, WIFI_CONNECTED_BIT);
    }
}


static const char* index_html = 
    "<!DOCTYPE html><html><body>"
    "<h2>SmartFlora - Configuracao WiFi</h2>"
    "<form action=\"/save\" method=\"POST\">"
    "SSID: <input type=\"text\" name=\"ssid\"><br><br>"
    "Senha: <input type=\"password\" name=\"pass\"><br><br>"
    "<input type=\"submit\" value=\"Salvar e Reiniciar\">"
    "</form></body></html>";

static esp_err_t get_handler(httpd_req_t *req) {
    httpd_resp_send(req, index_html, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t post_handler(httpd_req_t *req) {
    char buf[128];
    int ret, remaining = req->content_len;
    if (remaining > sizeof(buf)) return ESP_FAIL;
    
    if ((ret = httpd_req_recv(req, buf, remaining)) <= 0) return ESP_FAIL;
    buf[ret] = '\0';
    
    // Extrair SSID e Senha
    char ssid[32] = {0};
    char pass[64] = {0};
    
    char *ssid_ptr = strstr(buf, "ssid=");
    if (ssid_ptr) {
        ssid_ptr += 5;
        char *end = strchr(ssid_ptr, '&');
        if (end) {
            strncpy(ssid, ssid_ptr, end - ssid_ptr);
        } else {
            strcpy(ssid, ssid_ptr);
        }
    }
    
    char *pass_ptr = strstr(buf, "pass=");
    if (pass_ptr) {
        pass_ptr += 5;
        strcpy(pass, pass_ptr);
    }
    
    if (strlen(ssid) > 0) {
        nvs_save_wifi_credentials(ssid, pass);
        httpd_resp_send(req, "Salvo! Reiniciando...", HTTPD_RESP_USE_STRLEN);
        vTaskDelay(pdMS_TO_TICKS(1000));
        esp_restart();
    } else {
        httpd_resp_send(req, "Erro: SSID vazio", HTTPD_RESP_USE_STRLEN);
    }
    
    return ESP_OK;
}

static void start_webserver(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t uri_get = { .uri = "/", .method = HTTP_GET, .handler = get_handler, .user_ctx = NULL };
        httpd_uri_t uri_post = { .uri = "/save", .method = HTTP_POST, .handler = post_handler, .user_ctx = NULL };
        httpd_register_uri_handler(server, &uri_get);
        httpd_register_uri_handler(server, &uri_post);
    }
}


void wifi_manager_init(bool start_provisioning) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    if (start_provisioning) {

        esp_netif_create_default_wifi_ap();
        
        wifi_config_t wifi_config = {
            .ap = {
                .ssid = SOFTAP_SSID,
                .ssid_len = strlen(SOFTAP_SSID),
                .password = SOFTAP_PASSWORD,
                .max_connection = SOFTAP_MAX_CONN,
                .authmode = WIFI_AUTH_OPEN
            },
        };
        
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
        ESP_ERROR_CHECK(esp_wifi_start());
        
        ESP_LOGI(TAG, "SoftAP iniciado. Conecte-se a '%s' para configurar.", SOFTAP_SSID);
        start_webserver();
        
    } else {

        esp_netif_create_default_wifi_sta();
        
        ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));
        ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL));

        char ssid[32] = {0};
        char pass[64] = {0};
        
        if (nvs_load_wifi_credentials(ssid, sizeof(ssid), pass, sizeof(pass)) != ESP_OK || strlen(ssid) == 0) {
            ESP_LOGW(TAG, "Sem credenciais no NVS, usando padrão");
            strcpy(ssid, WIFI_DEFAULT_SSID);
            strcpy(pass, WIFI_DEFAULT_PASSWORD);
        }

        wifi_config_t wifi_config = {0};
        strcpy((char*)wifi_config.sta.ssid, ssid);
        strcpy((char*)wifi_config.sta.password, pass);

        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
        ESP_ERROR_CHECK(esp_wifi_start());
        
        ESP_LOGI(TAG, "Modo STA iniciado. Conectando a '%s'...", ssid);
    }
}
