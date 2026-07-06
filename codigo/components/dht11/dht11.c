#include "dht11.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "rom/ets_sys.h"
#include <string.h>

static const char *TAG = "DHT11";

static int wait_for_level(gpio_num_t gpio, int level, int timeout_us) {
    int64_t start = esp_timer_get_time();
    while (gpio_get_level(gpio) != level) {
        if ((esp_timer_get_time() - start) > timeout_us) {
            return -1;
        }
    }
    return (int)(esp_timer_get_time() - start);
}

esp_err_t dht11_read(gpio_num_t gpio, dht11_reading_t *out) {
    uint8_t data[5] = {0};

    // Enviar sinal de start: pull LOW por 20ms, depois HIGH
    gpio_set_direction(gpio, GPIO_MODE_OUTPUT);
    gpio_set_level(gpio, 0);
    ets_delay_us(20000); // 20ms
    gpio_set_level(gpio, 1);
    ets_delay_us(30);    // 30us

    // Mudar para input e esperar resposta do DHT11
    gpio_set_direction(gpio, GPIO_MODE_INPUT);

    // DHT11 responde com LOW 80us + HIGH 80us
    if (wait_for_level(gpio, 0, 100) < 0) {
        ESP_LOGW(TAG, "Timeout esperando resposta LOW");
        return ESP_FAIL;
    }
    if (wait_for_level(gpio, 1, 100) < 0) {
        ESP_LOGW(TAG, "Timeout no pulso LOW de resposta");
        return ESP_FAIL;
    }
    if (wait_for_level(gpio, 0, 100) < 0) {
        ESP_LOGW(TAG, "Timeout no pulso HIGH de resposta");
        return ESP_FAIL;
    }

    // Ler 40 bits (5 bytes)
    for (int i = 0; i < 40; i++) {
        // Cada bit começa com LOW ~50us
        if (wait_for_level(gpio, 1, 70) < 0) {
            ESP_LOGW(TAG, "Timeout no bit %d (LOW)", i);
            return ESP_FAIL;
        }

        // Duração do HIGH determina o bit:
        // ~26-28us = 0, ~70us = 1
        int duration = wait_for_level(gpio, 0, 100);
        if (duration < 0) {
            ESP_LOGW(TAG, "Timeout no bit %d (HIGH)", i);
            return ESP_FAIL;
        }

        data[i / 8] <<= 1;
        if (duration > 40) {
            data[i / 8] |= 1;
        }
    }

    // Verificar checksum
    uint8_t checksum = data[0] + data[1] + data[2] + data[3];
    if (checksum != data[4]) {
        ESP_LOGW(TAG, "Checksum inválido: %d != %d", checksum, data[4]);
        return ESP_FAIL;
    }

    out->humidity    = (float)data[0] + (float)data[1] * 0.1f;
    out->temperature = (float)data[2] + (float)data[3] * 0.1f;

    return ESP_OK;
}
