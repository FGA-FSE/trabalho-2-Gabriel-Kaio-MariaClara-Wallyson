#include "hcsr04.h"
#include "esp_timer.h"
#include "rom/ets_sys.h"
#include "esp_log.h"

static const char *TAG = "HCSR04";

esp_err_t hcsr04_init(gpio_num_t trig_pin, gpio_num_t echo_pin) {
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << trig_pin),
        .pull_down_en = 0,
        .pull_up_en = 0
    };
    gpio_config(&io_conf);

    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << echo_pin);
    gpio_config(&io_conf);

    gpio_set_level(trig_pin, 0);
    return ESP_OK;
}

esp_err_t hcsr04_read_distance(gpio_num_t trig_pin, gpio_num_t echo_pin, float *distance_cm) {
    // Garantir que o trig está em 0
    gpio_set_level(trig_pin, 0);
    ets_delay_us(2);

    // Enviar pulso de 10us
    gpio_set_level(trig_pin, 1);
    ets_delay_us(10);
    gpio_set_level(trig_pin, 0);

    // Esperar o pino echo subir (timeout de ~10ms)
    int64_t start_wait = esp_timer_get_time();
    while (gpio_get_level(echo_pin) == 0) {
        if ((esp_timer_get_time() - start_wait) > 10000) {
            ESP_LOGD(TAG, "Timeout waiting for echo HIGH");
            return ESP_ERR_TIMEOUT;
        }
    }

    // Gravar o tempo inicial
    int64_t start_time = esp_timer_get_time();

    // Esperar o pino echo descer (timeout de ~30ms -> ~5m)
    while (gpio_get_level(echo_pin) == 1) {
        if ((esp_timer_get_time() - start_time) > 30000) {
             ESP_LOGD(TAG, "Timeout waiting for echo LOW");
             return ESP_ERR_TIMEOUT;
        }
    }

    int64_t end_time = esp_timer_get_time();
    int64_t duration = end_time - start_time;

    // Distância em cm = (Duração em us * 0.0343) / 2
    *distance_cm = (float)duration * 0.01715f;

    return ESP_OK;
}
