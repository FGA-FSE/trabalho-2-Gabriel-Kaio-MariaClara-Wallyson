#include "mqtt_client.h"
#include <stdlib.h>
#include <string.h>

esp_mqtt_client_handle_t esp_mqtt_client_init(const esp_mqtt_client_config_t *config) {
    (void)config;
    return (esp_mqtt_client_handle_t)malloc(1);
}

esp_err_t esp_mqtt_client_register_event(esp_mqtt_client_handle_t client, int event_id, void (*cb)(void*, esp_event_base_t, int32_t, void*), void *ctx) {
    (void)client; (void)event_id; (void)cb; (void)ctx;
    return 0;
}

esp_err_t esp_mqtt_client_start(esp_mqtt_client_handle_t client) {
    (void)client;
    return 0;
}

esp_err_t esp_mqtt_client_publish(esp_mqtt_client_handle_t client, const char *topic, const char *data, int len, int qos, int retain) {
    (void)client; (void)topic; (void)data; (void)len; (void)qos; (void)retain;
    return 0;
}

esp_err_t esp_mqtt_client_subscribe(esp_mqtt_client_handle_t client, const char *topic, int qos) {
    (void)client; (void)topic; (void)qos;
    return 0;
}

esp_err_t esp_mqtt_client_destroy(esp_mqtt_client_handle_t client) {
    free(client);
    return 0;
}
