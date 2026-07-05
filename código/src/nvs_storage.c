#include "nvs_storage.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "NVS";
static const char *NVS_NAMESPACE = "smartflora";

esp_err_t nvs_storage_init() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    return ret;
}

esp_err_t nvs_save_wifi_credentials(const char *ssid, const char *password) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    err = nvs_set_str(handle, "wifi_ssid", ssid);
    if (err == ESP_OK) {
        err = nvs_set_str(handle, "wifi_pass", password);
    }
    
    if (err == ESP_OK) {
        err = nvs_commit(handle);
    }
    nvs_close(handle);
    return err;
}

esp_err_t nvs_load_wifi_credentials(char *ssid, size_t ssid_len, char *password, size_t pass_len) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) return err;

    err = nvs_get_str(handle, "wifi_ssid", ssid, &ssid_len);
    if (err == ESP_OK) {
        err = nvs_get_str(handle, "wifi_pass", password, &pass_len);
    }
    nvs_close(handle);
    return err;
}

esp_err_t nvs_save_irrigation_mode(irrigation_mode_t mode) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    err = nvs_set_u8(handle, "mode", (uint8_t)mode);
    if (err == ESP_OK) nvs_commit(handle);
    nvs_close(handle);
    return err;
}

esp_err_t nvs_load_irrigation_mode(irrigation_mode_t *mode) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) return err;

    uint8_t m = 0;
    err = nvs_get_u8(handle, "mode", &m);
    if (err == ESP_OK) *mode = (irrigation_mode_t)m;
    nvs_close(handle);
    return err;
}

esp_err_t nvs_save_moisture_threshold(int threshold) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    err = nvs_set_i32(handle, "threshold", threshold);
    if (err == ESP_OK) nvs_commit(handle);
    nvs_close(handle);
    return err;
}

esp_err_t nvs_load_moisture_threshold(int *threshold) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) return err;

    int32_t t = 0;
    err = nvs_get_i32(handle, "threshold", &t);
    if (err == ESP_OK) *threshold = t;
    nvs_close(handle);
    return err;
}
