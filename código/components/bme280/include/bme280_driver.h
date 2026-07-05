#ifndef BME280_DRIVER_H
#define BME280_DRIVER_H

#include "driver/i2c.h"
#include "esp_err.h"

#define BME280_CHIP_ID 0x60
#define BMP280_CHIP_ID 0x58

/**
 * @brief Initialize BME280/BMP280 sensor
 * 
 * @param i2c_num I2C port number
 * @param i2c_addr I2C address of the device (usually 0x76 or 0x77)
 * @param is_bme280 Pointer to bool, set to true if it's BME280, false if BMP280
 * @return esp_err_t ESP_OK on success
 */
esp_err_t bme280_init(i2c_port_t i2c_num, uint8_t i2c_addr, bool *is_bme280);

/**
 * @brief Read temperature, pressure, and humidity (if BME280)
 * 
 * @param i2c_num I2C port number
 * @param i2c_addr I2C address of the device
 * @param temperature Pointer to store temperature in Celsius
 * @param pressure Pointer to store pressure in hPa
 * @param humidity Pointer to store humidity in %RH (can be NULL if BMP280)
 * @return esp_err_t ESP_OK on success
 */
esp_err_t bme280_read_data(i2c_port_t i2c_num, uint8_t i2c_addr, float *temperature, float *pressure, float *humidity);

#endif // BME280_DRIVER_H
