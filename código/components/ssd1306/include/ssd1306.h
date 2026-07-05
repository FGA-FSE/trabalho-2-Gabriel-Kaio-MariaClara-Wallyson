#ifndef SSD1306_H
#define SSD1306_H

#include "driver/i2c.h"
#include "esp_err.h"
#include <stdbool.h>

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

/**
 * @brief Initialize SSD1306 OLED display
 * 
 * @param i2c_num I2C port number
 * @param i2c_addr I2C address of the device
 * @return esp_err_t ESP_OK on success
 */
esp_err_t ssd1306_init(i2c_port_t i2c_num, uint8_t i2c_addr);

/**
 * @brief Clear the internal display buffer
 */
void ssd1306_clear();

/**
 * @brief Send internal buffer to the display
 */
esp_err_t ssd1306_display(i2c_port_t i2c_num, uint8_t i2c_addr);

/**
 * @brief Draw a string on the display
 * 
 * @param x X coordinate (0-127)
 * @param y Y coordinate (0-63)
 * @param str String to draw
 * @param size Font size multiplier (1 or 2)
 */
void ssd1306_draw_string(int x, int y, const char *str, int size);

/**
 * @brief Draw a rectangle (outline or filled)
 * 
 * @param x X coordinate
 * @param y Y coordinate
 * @param w Width
 * @param h Height
 * @param fill True for filled, false for outline
 */
void ssd1306_draw_rect(int x, int y, int w, int h, bool fill);

/**
 * @brief Draw a horizontal line
 */
void ssd1306_draw_hline(int x, int y, int w);

/**
 * @brief Draw a small dot (2x2 square usually looks like a dot)
 */
void ssd1306_draw_dot(int x, int y, bool filled);

#endif // SSD1306_H
