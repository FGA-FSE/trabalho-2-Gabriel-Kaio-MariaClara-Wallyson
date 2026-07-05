#ifndef COMMON_H
#define COMMON_H

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include <stdbool.h>
#include <stdint.h>

// ======================== Dados de Sensores (protegido por mutex) ========================

typedef struct {
    int   soil_moisture;        // 0-100 %
    float air_temperature;      // °C (do BME280 se disponível, senão DHT11)
    float air_humidity;         // % UR (do DHT11)
    float bme_temperature;      // °C (BME280 direto)
    float bme_pressure;         // hPa
    float bme_humidity;         // % UR (somente BME280, -1 se BMP280)
    float water_distance_cm;    // Distância HC-SR04
    bool  water_level_ok;       // true = nível OK
    bool  bme_available;        // true = BME280/BMP280 detectado
    bool  simulate_soil;        // true = potenciômetro
} sensor_data_t;

// ======================== Estado da Irrigação (protegido por mutex) ========================

typedef enum {
    MODE_AUTO = 0,
    MODE_MANUAL = 1
} irrigation_mode_t;

typedef enum {
    PUMP_OFF = 0,
    PUMP_ON = 1
} pump_state_t;

typedef enum {
    PHASE_IDLE = 0,
    PHASE_IRRIGATING = 1,
    PHASE_COOLDOWN = 2
} irrigation_phase_t;

typedef struct {
    irrigation_mode_t  mode;
    pump_state_t       pump_state;
    irrigation_phase_t phase;
    int                moisture_threshold;
    int                pump_duration_s;         // Duração programada
    int64_t            pump_start_time_us;      // esp_timer_get_time() quando bomba ligou
    int                soil_before_irrigation;
} irrigation_state_t;

// ======================== Comandos para Tasks ========================

// Comandos para a fila de irrigação
typedef enum {
    CMD_START_MANUAL = 0,      // Iniciar irrigação manual (param = duração em s)
    CMD_STOP = 1,              // Parar bomba
    CMD_SET_MODE = 2,          // Alterar modo (param = 0 AUTO, 1 MANUAL)
    CMD_SET_THRESHOLD = 3,     // Alterar threshold (param = valor %)
} irrigation_cmd_type_t;

typedef struct {
    irrigation_cmd_type_t type;
    int param;
} irrigation_cmd_t;

// Comandos para a fila do buzzer
typedef struct {
    int beep_count;      // Número de beeps
    int duration_ms;     // Duração de cada beep
    int gap_ms;          // Intervalo entre beeps
} buzzer_cmd_t;

// ======================== Event Group Bits ========================
#define WIFI_CONNECTED_BIT    BIT0
#define MQTT_CONNECTED_BIT    BIT1

// ======================== Handles Globais ========================
// Definidos em main.c, extern aqui

extern SemaphoreHandle_t    sensor_data_mutex;
extern SemaphoreHandle_t    irrigation_state_mutex;
extern QueueHandle_t        buzzer_queue;
extern QueueHandle_t        irrigation_cmd_queue;
extern EventGroupHandle_t   wifi_mqtt_event_group;

extern sensor_data_t        g_sensor_data;
extern irrigation_state_t   g_irrigation_state;

#endif // COMMON_H
