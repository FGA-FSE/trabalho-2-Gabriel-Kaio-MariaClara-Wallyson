#include "bme280_driver.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "BME280";


// Registradores
#define BME280_REG_CHIPID 0xD0
#define BME280_REG_RESET 0xE0
#define BME280_REG_CTRL_HUM 0xF2
#define BME280_REG_CTRL_MEAS 0xF4
#define BME280_REG_CONFIG 0xF5
#define BME280_REG_DATA 0xF7

// Estrutura de calibração

struct {
    uint16_t dig_T1; int16_t dig_T2; int16_t dig_T3;
    uint16_t dig_P1; int16_t dig_P2; int16_t dig_P3; int16_t dig_P4;
    int16_t dig_P5; int16_t dig_P6; int16_t dig_P7; int16_t dig_P8; int16_t dig_P9;
    uint8_t dig_H1; int16_t dig_H2; uint8_t dig_H3; int16_t dig_H4; int16_t dig_H5; int8_t dig_H6;
} cal_data;

int32_t t_fine;

static esp_err_t i2c_write_reg(i2c_port_t i2c_num, uint8_t i2c_addr, uint8_t reg, uint8_t data) {
    uint8_t write_buf[2] = {reg, data};
    return i2c_master_write_to_device(i2c_num, i2c_addr, write_buf, sizeof(write_buf), pdMS_TO_TICKS(100));
}

static esp_err_t i2c_read_regs(i2c_port_t i2c_num, uint8_t i2c_addr, uint8_t reg, uint8_t *data, size_t len) {
    return i2c_master_write_read_device(i2c_num, i2c_addr, &reg, 1, data, len, pdMS_TO_TICKS(100));
}

static void read_calibration_data(i2c_port_t i2c_num, uint8_t i2c_addr, bool is_bme280) {
    uint8_t cal1[24];
    i2c_read_regs(i2c_num, i2c_addr, 0x88, cal1, 24);
    cal_data.dig_T1 = (cal1[1] << 8) | cal1[0];
    cal_data.dig_T2 = (cal1[3] << 8) | cal1[2];
    cal_data.dig_T3 = (cal1[5] << 8) | cal1[4];
    cal_data.dig_P1 = (cal1[7] << 8) | cal1[6];
    cal_data.dig_P2 = (cal1[9] << 8) | cal1[8];
    cal_data.dig_P3 = (cal1[11] << 8) | cal1[10];
    cal_data.dig_P4 = (cal1[13] << 8) | cal1[12];
    cal_data.dig_P5 = (cal1[15] << 8) | cal1[14];
    cal_data.dig_P6 = (cal1[17] << 8) | cal1[16];
    cal_data.dig_P7 = (cal1[19] << 8) | cal1[18];
    cal_data.dig_P8 = (cal1[21] << 8) | cal1[20];
    cal_data.dig_P9 = (cal1[23] << 8) | cal1[22];

    if (is_bme280) {
        uint8_t h1;
        i2c_read_regs(i2c_num, i2c_addr, 0xA1, &h1, 1);
        cal_data.dig_H1 = h1;

        uint8_t cal2[7];
        i2c_read_regs(i2c_num, i2c_addr, 0xE1, cal2, 7);
        cal_data.dig_H2 = (cal2[1] << 8) | cal2[0];
        cal_data.dig_H3 = cal2[2];
        cal_data.dig_H4 = (cal2[3] << 4) | (cal2[4] & 0x0F);
        cal_data.dig_H5 = (cal2[5] << 4) | (cal2[4] >> 4);
        cal_data.dig_H6 = (int8_t)cal2[6];
    }
}

esp_err_t bme280_init(i2c_port_t i2c_num, uint8_t i2c_addr, bool *is_bme280) {
    uint8_t chip_id;
    esp_err_t err = i2c_read_regs(i2c_num, i2c_addr, BME280_REG_CHIPID, &chip_id, 1);
    
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Sensor não encontrado no endereço 0x%02X", i2c_addr);
        return err;
    }

    if (chip_id == BME280_CHIP_ID) {
        *is_bme280 = true;
        ESP_LOGI(TAG, "BME280 encontrado.");
    } else if (chip_id == BMP280_CHIP_ID) {
        *is_bme280 = false;
        ESP_LOGI(TAG, "BMP280 encontrado.");
    } else {
        ESP_LOGW(TAG, "Chip ID desconhecido: 0x%02X", chip_id);
        return ESP_FAIL;
    }

    // Soft reset
    i2c_write_reg(i2c_num, i2c_addr, BME280_REG_RESET, 0xB6);
    vTaskDelay(pdMS_TO_TICKS(10));

    read_calibration_data(i2c_num, i2c_addr, *is_bme280);

    // Configurar oversampling e modo normal (1x oversampling)
    if (*is_bme280) {
        i2c_write_reg(i2c_num, i2c_addr, BME280_REG_CTRL_HUM, 0x01); // osrs_h x1
    }
    
    i2c_write_reg(i2c_num, i2c_addr, BME280_REG_CTRL_MEAS, 0x27); // osrs_t x1, osrs_p x1, modo normal
    i2c_write_reg(i2c_num, i2c_addr, BME280_REG_CONFIG, 0xA0);    // t_sb 1000ms

    return ESP_OK;
}

esp_err_t bme280_read_data(i2c_port_t i2c_num, uint8_t i2c_addr, float *temperature, float *pressure, float *humidity) {
    uint8_t data[8];
    esp_err_t err = i2c_read_regs(i2c_num, i2c_addr, BME280_REG_DATA, data, 8);
    if (err != ESP_OK) return err;

    int32_t adc_P = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
    int32_t adc_T = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);
    int32_t adc_H = (data[6] << 8) | data[7];

    // Compensação da Temperatura
    int32_t var1, var2;
    var1 = ((((adc_T >> 3) - ((int32_t)cal_data.dig_T1 << 1))) * ((int32_t)cal_data.dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)cal_data.dig_T1)) * ((adc_T >> 4) - ((int32_t)cal_data.dig_T1))) >> 12) * ((int32_t)cal_data.dig_T3)) >> 14;
    t_fine = var1 + var2;
    *temperature = (t_fine * 5 + 128) >> 8;
    *temperature /= 100.0f;

    // Compensação da Pressão
    int64_t p;
    int64_t var1_p, var2_p;
    var1_p = ((int64_t)t_fine) - 128000;
    var2_p = var1_p * var1_p * (int64_t)cal_data.dig_P6;
    var2_p = var2_p + ((var1_p * (int64_t)cal_data.dig_P5) << 17);
    var2_p = var2_p + (((int64_t)cal_data.dig_P4) << 35);
    var1_p = ((var1_p * var1_p * (int64_t)cal_data.dig_P3) >> 8) + ((var1_p * (int64_t)cal_data.dig_P2) << 12);
    var1_p = (((((int64_t)1) << 47) + var1_p)) * ((int64_t)cal_data.dig_P1) >> 33;
    if (var1_p == 0) {
        *pressure = 0; // Evitar divisão por zero
    } else {
        p = 1048576 - adc_P;
        p = (((p << 31) - var2_p) * 3125) / var1_p;
        var1_p = (((int64_t)cal_data.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
        var2_p = (((int64_t)cal_data.dig_P8) * p) >> 19;
        p = ((p + var1_p + var2_p) >> 8) + (((int64_t)cal_data.dig_P7) << 4);
        *pressure = (float)p / 256.0f / 100.0f; // em hPa
    }

    // Compensação da Umidade
    if (humidity) {
        int32_t v_x1_u32r;
        v_x1_u32r = (t_fine - ((int32_t)76800));
        v_x1_u32r = (((((adc_H << 14) - (((int32_t)cal_data.dig_H4) << 20) - (((int32_t)cal_data.dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) * (((((((v_x1_u32r * ((int32_t)cal_data.dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t)cal_data.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)cal_data.dig_H2) + 8192) >> 14));
        v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)cal_data.dig_H1)) >> 4));
        v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
        v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
        *humidity = (float)(v_x1_u32r >> 12) / 1024.0f;
    }

    return ESP_OK;
}
