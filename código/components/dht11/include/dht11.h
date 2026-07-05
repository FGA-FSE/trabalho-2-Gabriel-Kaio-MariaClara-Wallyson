#ifndef DHT11_H
#define DHT11_H

#include "driver/gpio.h"
#include "esp_err.h"

typedef struct {
    float temperature;
    float humidity;
} dht11_reading_t;

/**
 * Lê temperatura e umidade do sensor DHT11.
 * @param gpio Pino GPIO conectado ao DATA do DHT11.
 * @param out  Ponteiro para struct de resultado.
 * @return ESP_OK em caso de sucesso, ESP_FAIL em caso de erro.
 */
esp_err_t dht11_read(gpio_num_t gpio, dht11_reading_t *out);

#endif // DHT11_H
