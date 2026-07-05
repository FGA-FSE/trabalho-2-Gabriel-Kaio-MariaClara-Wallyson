#ifndef HCSR04_H
#define HCSR04_H

#include "driver/gpio.h"
#include "esp_err.h"

/**
 * @brief Initialize HC-SR04 sensor pins.
 *
 * @param trig_pin GPIO pin used for TRIG
 * @param echo_pin GPIO pin used for ECHO
 * @return esp_err_t ESP_OK on success
 */
esp_err_t hcsr04_init(gpio_num_t trig_pin, gpio_num_t echo_pin);

/**
 * @brief Read distance from HC-SR04 sensor.
 *
 * @param trig_pin GPIO pin used for TRIG
 * @param echo_pin GPIO pin used for ECHO
 * @param distance_cm Pointer to store the measured distance in cm.
 * @return esp_err_t ESP_OK on success, ESP_ERR_TIMEOUT if no echo is received within timeout.
 */
esp_err_t hcsr04_read_distance(gpio_num_t trig_pin, gpio_num_t echo_pin, float *distance_cm);

#endif // HCSR04_H
