#pragma once
#include <stdint.h>

typedef int esp_err_t;
typedef const char* esp_event_base_t;

typedef struct {
    struct { const char *uri; int port; } address;
} esp_mqtt_broker_t;

typedef struct {
    struct { const char *username; const char *client_id; } credentials;
    struct { const char *uri; int port; } broker_address; // fallback
} mqtt_credentials_t;

typedef struct {
    struct {
        struct { const char *uri; int port; } address;
    } broker;
    struct {
        const char *username;
        const char *client_id;
    } credentials;
} esp_mqtt_client_config_t;

typedef void *esp_mqtt_client_handle_t;

typedef enum {
    MQTT_EVENT_CONNECTED = 0,
    MQTT_EVENT_DISCONNECTED,
    MQTT_EVENT_DATA,
    MQTT_EVENT_ERROR
} esp_mqtt_event_id_t;

typedef struct {
    const char *topic;
    int topic_len;
    const char *data;
    int data_len;
} esp_mqtt_event_t;

typedef esp_mqtt_event_t *esp_mqtt_event_handle_t;

esp_mqtt_client_handle_t esp_mqtt_client_init(const esp_mqtt_client_config_t *config);
esp_err_t esp_mqtt_client_register_event(esp_mqtt_client_handle_t client, int event_id, void (*cb)(void*, esp_event_base_t, int32_t, void*), void *ctx);
esp_err_t esp_mqtt_client_start(esp_mqtt_client_handle_t client);
esp_err_t esp_mqtt_client_publish(esp_mqtt_client_handle_t client, const char *topic, const char *data, int len, int qos, int retain);
esp_err_t esp_mqtt_client_subscribe(esp_mqtt_client_handle_t client, const char *topic, int qos);
esp_err_t esp_mqtt_client_destroy(esp_mqtt_client_handle_t client);
