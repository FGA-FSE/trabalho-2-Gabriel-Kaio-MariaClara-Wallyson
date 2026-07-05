#ifndef NVS_STORAGE_H
#define NVS_STORAGE_H

#include "common.h"
#include "esp_err.h"

esp_err_t nvs_storage_init();

// WiFi Credentials
esp_err_t nvs_save_wifi_credentials(const char *ssid, const char *password);
esp_err_t nvs_load_wifi_credentials(char *ssid, size_t ssid_len, char *password, size_t pass_len);

// Settings
esp_err_t nvs_save_irrigation_mode(irrigation_mode_t mode);
esp_err_t nvs_load_irrigation_mode(irrigation_mode_t *mode);

esp_err_t nvs_save_moisture_threshold(int threshold);
esp_err_t nvs_load_moisture_threshold(int *threshold);

#endif // NVS_STORAGE_H
