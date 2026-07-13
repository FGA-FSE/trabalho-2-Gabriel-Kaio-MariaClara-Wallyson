#ifndef CONFIG_H
#define CONFIG_H
#include "sdkconfig.h"

// ======================== Telegram Bot ========================
#define TELEGRAM_BOT_TOKEN CONFIG_TELEGRAM_BOT_TOKEN
#define TELEGRAM_CHAT_ID                                                       \
  CONFIG_TELEGRAM_CHAT_ID // Opcional, se quiser enviar alertas ativos
#define TELEGRAM_POLL_INTERVAL_MS 3000





// ======================== WiFi ========================
// Credenciais padrão (podem ser sobrescritas via NVS)
#define WIFI_DEFAULT_SSID CONFIG_WIFI_DEFAULT_SSID
#define WIFI_DEFAULT_PASSWORD CONFIG_WIFI_DEFAULT_PASSWORD

// SoftAP para provisioning
#define SOFTAP_SSID "SmartFlora-Setup"
#define SOFTAP_PASSWORD "" // Aberto
#define SOFTAP_MAX_CONN 2

// ======================== GPIO Pins ========================

// Potenciômetro (simula umidade do solo) — ADC1
#define SOIL_SENSOR_PIN GPIO_NUM_34

// DHT11
#define DHT_PIN GPIO_NUM_4

// HC-SR04 (nível do reservatório)
#define HC_SR04_TRIG_PIN GPIO_NUM_5
#define HC_SR04_ECHO_PIN GPIO_NUM_18

// Relé da bomba (ativo-LOW)
#define RELAY_PIN GPIO_NUM_26

// Buzzer ativo
#define BUZZER_PIN GPIO_NUM_13

// Botão físico
#define BUTTON_PIN GPIO_NUM_14

// I²C (OLED + BME280)
#define I2C_SDA_PIN GPIO_NUM_21
#define I2C_SCL_PIN GPIO_NUM_22
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 400000

// OLED SSD1306
#define OLED_I2C_ADDR 0x3C
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

// BME280/BMP280
#define BME280_I2C_ADDR 0x76

// LED de status (onboard)
#define LED_PIN GPIO_NUM_2

// ======================== Modo Simulação ========================
#define SIMULATE_SOIL_WITH_POT 1 // 1=potenciômetro, 0=sensor real

// ======================== Calibração do Solo ========================
#define SOIL_AIR_VALUE 3500
#define SOIL_WATER_VALUE 1500
#define POT_MIN 0
#define POT_MAX 4095

// ======================== HC-SR04 ========================
#define WATER_FULL_DISTANCE_CM 5
#define WATER_EMPTY_DISTANCE_CM 25
#define WATER_ALARM_DISTANCE_CM 20

// ======================== Irrigação ========================
#define DEFAULT_MOISTURE_THRESHOLD 30
#define DEFAULT_IRRIGATION_DURATION 20
#define IRRIGATION_COOLDOWN_S 120
#define MAX_PUMP_ON_TIME_S 60

// ======================== Intervalos (ms) ========================
#define SENSOR_READ_INTERVAL_MS 30000
#define WATER_LEVEL_READ_INTERVAL_MS 5000
#define DISPLAY_UPDATE_INTERVAL_MS 1000
#define DISPLAY_AUTO_CYCLE_MS 5000
#define BUTTON_POLL_INTERVAL_MS 10

// ======================== Relé ========================
#define RELAY_ACTIVE_LOW 1

// ======================== NTP ========================
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC (-3 * 3600)

// ======================== Erros ========================
#define MAX_CONSECUTIVE_ERRORS 5

// ======================== Firmware ========================
#define FIRMWARE_VERSION "2.0.0"

#endif // CONFIG_H
