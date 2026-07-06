#include <unity.h>
#include "nvs_storage.h"
#include "nvs_flash.h"
#include <string.h>

void setUp(void) {
    // Inicializa NVS limpa antes de cada teste
    ESP_ERROR_CHECK(nvs_flash_erase());
    ESP_ERROR_CHECK(nvs_storage_init());
}

void tearDown(void) {
    // Limpa NVS depois de cada teste
    nvs_flash_deinit();
}

void test_save_load_wifi_credentials(void) {
    const char *test_ssid = "MeuWiFi_Teste";
    const char *test_pass = "SenhaForte123";

    TEST_ASSERT_EQUAL(ESP_OK, nvs_save_wifi_credentials(test_ssid, test_pass));

    char load_ssid[32] = {0};
    char load_pass[64] = {0};
    TEST_ASSERT_EQUAL(ESP_OK, nvs_load_wifi_credentials(load_ssid, sizeof(load_ssid), load_pass, sizeof(load_pass)));

    TEST_ASSERT_EQUAL_STRING(test_ssid, load_ssid);
    TEST_ASSERT_EQUAL_STRING(test_pass, load_pass);
}

void test_save_load_irrigation_mode(void) {
    irrigation_mode_t load_mode;
    
    TEST_ASSERT_EQUAL(ESP_OK, nvs_save_irrigation_mode(MODE_MANUAL));
    TEST_ASSERT_EQUAL(ESP_OK, nvs_load_irrigation_mode(&load_mode));
    TEST_ASSERT_EQUAL(MODE_MANUAL, load_mode);

    TEST_ASSERT_EQUAL(ESP_OK, nvs_save_irrigation_mode(MODE_AUTO));
    TEST_ASSERT_EQUAL(ESP_OK, nvs_load_irrigation_mode(&load_mode));
    TEST_ASSERT_EQUAL(MODE_AUTO, load_mode);
}

void test_save_load_moisture_threshold(void) {
    int load_th;
    
    TEST_ASSERT_EQUAL(ESP_OK, nvs_save_moisture_threshold(45));
    TEST_ASSERT_EQUAL(ESP_OK, nvs_load_moisture_threshold(&load_th));
    TEST_ASSERT_EQUAL(45, load_th);
}

void app_main() {
    UNITY_BEGIN();
    RUN_TEST(test_save_load_wifi_credentials);
    RUN_TEST(test_save_load_irrigation_mode);
    RUN_TEST(test_save_load_moisture_threshold);
    UNITY_END();
}
