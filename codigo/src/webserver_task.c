#include "webserver_task.h"
#include "config.h"
#include "common.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "cJSON.h"
#include <string.h>

static const char *TAG = "WebServer";

static const char *dashboard_html =
"<!DOCTYPE html>"
"<html lang='pt-BR'>"
"<head>"
"<meta charset='UTF-8'>"
"<meta name='viewport' content='width=device-width,initial-scale=1'>"
"<title>SmartFlora Dashboard</title>"
"<style>"
"*{margin:0;padding:0;box-sizing:border-box}"
"body{font-family:'Segoe UI',sans-serif;background:#0f1923;color:#e0e6ed;min-height:100vh}"
".header{background:linear-gradient(135deg,#1a3a2a,#2d5a3d);padding:20px;text-align:center;box-shadow:0 2px 10px rgba(0,0,0,.3)}"
".header h1{font-size:1.6em;color:#6fcf97}"
".header p{color:#8fa;font-size:.85em;margin-top:4px}"
".container{max-width:700px;margin:0 auto;padding:16px}"
".cards{display:grid;grid-template-columns:1fr 1fr;gap:12px;margin-top:16px}"
".card{background:#1a2733;border-radius:12px;padding:16px;border:1px solid #2a3a4a;transition:transform .2s}"
".card:hover{transform:translateY(-2px)}"
".card .label{font-size:.75em;color:#7a8fa0;text-transform:uppercase;letter-spacing:1px}"
".card .value{font-size:1.8em;font-weight:700;margin:6px 0;color:#fff}"
".card .unit{font-size:.7em;color:#5a7a90}"
".card.soil .value{color:#4fc3f7}"
".card.temp .value{color:#ff8a65}"
".card.hum .value{color:#81c784}"
".card.water .value{color:#64b5f6}"
".card.pump .value{color:#f06292}"
".card.press .value{color:#ce93d8}"
".status-bar{margin-top:16px;background:#1a2733;border-radius:12px;padding:16px;border:1px solid #2a3a4a;display:flex;justify-content:space-between;align-items:center;flex-wrap:wrap;gap:8px}"
".mode-badge{padding:6px 16px;border-radius:20px;font-size:.85em;font-weight:600}"
".mode-auto{background:#1b5e20;color:#a5d6a7}"
".mode-manual{background:#4a148c;color:#ce93d8}"
".pump-on{background:#b71c1c;color:#ef9a9a}"
".pump-off{background:#1a2733;color:#7a8fa0;border:1px solid #2a3a4a}"
".controls{margin-top:16px;display:grid;grid-template-columns:1fr 1fr;gap:10px}"
"button{padding:14px;border:none;border-radius:10px;font-size:.95em;font-weight:600;cursor:pointer;transition:all .2s}"
"button:active{transform:scale(.96)}"
".btn-auto{background:#2e7d32;color:#fff}"
".btn-manual{background:#6a1b9a;color:#fff}"
".btn-irrigate{background:#0277bd;color:#fff}"
".btn-stop{background:#c62828;color:#fff}"
".btn-auto:hover{background:#388e3c}"
".btn-manual:hover{background:#7b1fa2}"
".btn-irrigate:hover{background:#0288d1}"
".btn-stop:hover{background:#d32f2f}"
".threshold-box{margin-top:16px;background:#1a2733;border-radius:12px;padding:16px;border:1px solid #2a3a4a}"
".threshold-box label{font-size:.8em;color:#7a8fa0}"
".threshold-box input{width:60px;padding:6px;border-radius:6px;border:1px solid #3a4a5a;background:#0f1923;color:#fff;font-size:1em;text-align:center;margin:0 8px}"
".threshold-box button{padding:8px 20px}"
".footer{text-align:center;padding:20px;color:#3a4a5a;font-size:.75em}"
"</style>"
"</head>"
"<body>"
"<div class='header'>"
"<h1>&#127793; SmartFlora</h1>"
"<p>Monitoramento em tempo real</p>"
"</div>"
"<div class='container'>"
"<div class='status-bar'>"
"<div>Modo: <span id='modeBadge' class='mode-badge mode-auto'>AUTO</span></div>"
"<div>Bomba: <span id='pumpBadge' class='mode-badge pump-off'>OFF</span></div>"
"</div>"
"<div class='cards'>"
"<div class='card soil'><div class='label'>Umidade do Solo</div><div class='value' id='soil'>--</div><div class='unit'>%</div></div>"
"<div class='card temp'><div class='label'>Temperatura</div><div class='value' id='temp'>--</div><div class='unit'>&deg;C</div></div>"
"<div class='card hum'><div class='label'>Umidade do Ar</div><div class='value' id='hum'>--</div><div class='unit'>%</div></div>"
"<div class='card water'><div class='label'>Dist. Reservatorio</div><div class='value' id='water'>--</div><div class='unit'>cm</div></div>"
"<div class='card press'><div class='label'>Pressao</div><div class='value' id='press'>--</div><div class='unit'>hPa</div></div>"
"<div class='card pump'><div class='label'>Threshold</div><div class='value' id='thresh'>--</div><div class='unit'>%</div></div>"
"</div>"
"<div class='controls'>"
"<button class='btn-auto' onclick='cmd(\"setMode\",{mode:\"AUTO\"})'>Modo AUTO</button>"
"<button class='btn-manual' onclick='cmd(\"setMode\",{mode:\"MANUAL\"})'>Modo MANUAL</button>"
"<button class='btn-irrigate' onclick='cmd(\"irrigate\",{duration:20})'>Regar 20s</button>"
"<button class='btn-stop' onclick='cmd(\"stop\",{})'>Parar Bomba</button>"
"</div>"
"<div class='threshold-box'>"
"<label>Limiar de umidade:</label>"
"<input type='number' id='threshInput' min='0' max='100' value='30'>"
"<button class='btn-auto' onclick='setTh()'>Aplicar</button>"
"</div>"
"</div>"
"<div class='footer'>SmartFlora v" FIRMWARE_VERSION " &mdash; FSE 2026/1</div>"
"<script>"
"function update(){"
"fetch('/api/status').then(r=>r.json()).then(d=>{"
"document.getElementById('soil').textContent=d.soil_moisture;"
"document.getElementById('temp').textContent=d.air_temperature.toFixed(1);"
"document.getElementById('hum').textContent=d.air_humidity.toFixed(0);"
"document.getElementById('water').textContent=d.water_distance_cm.toFixed(1);"
"document.getElementById('press').textContent=d.bme_available?d.bme_pressure.toFixed(0):'N/A';"
"document.getElementById('thresh').textContent=d.moisture_threshold;"
"document.getElementById('threshInput').value=d.moisture_threshold;"
"var mb=document.getElementById('modeBadge');"
"mb.textContent=d.mode;mb.className='mode-badge '+(d.mode==='AUTO'?'mode-auto':'mode-manual');"
"var pb=document.getElementById('pumpBadge');"
"pb.textContent=d.pump_on?'LIGADA':'OFF';pb.className='mode-badge '+(d.pump_on?'pump-on':'pump-off');"
"}).catch(e=>console.log(e))}"
"function cmd(m,p){fetch('/api/command',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({method:m,params:p})}).then(()=>setTimeout(update,500))}"
"function setTh(){var v=parseInt(document.getElementById('threshInput').value);cmd('setThreshold',{threshold:v})}"
"update();setInterval(update,3000);"
"</script>"
"</body></html>";

static esp_err_t dashboard_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, dashboard_html, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t api_status_handler(httpd_req_t *req) {
    cJSON *json = cJSON_CreateObject();

    xSemaphoreTake(sensor_data_mutex, portMAX_DELAY);
    cJSON_AddNumberToObject(json, "soil_moisture", g_sensor_data.soil_moisture);
    cJSON_AddNumberToObject(json, "air_temperature", g_sensor_data.air_temperature);
    cJSON_AddNumberToObject(json, "air_humidity", g_sensor_data.air_humidity);
    cJSON_AddNumberToObject(json, "bme_pressure", g_sensor_data.bme_pressure);
    cJSON_AddNumberToObject(json, "water_distance_cm", g_sensor_data.water_distance_cm);
    cJSON_AddBoolToObject(json, "water_level_ok", g_sensor_data.water_level_ok);
    cJSON_AddBoolToObject(json, "bme_available", g_sensor_data.bme_available);
    xSemaphoreGive(sensor_data_mutex);

    xSemaphoreTake(irrigation_state_mutex, portMAX_DELAY);
    cJSON_AddStringToObject(json, "mode", g_irrigation_state.mode == MODE_AUTO ? "AUTO" : "MANUAL");
    cJSON_AddBoolToObject(json, "pump_on", g_irrigation_state.pump_state == PUMP_ON);
    cJSON_AddNumberToObject(json, "moisture_threshold", g_irrigation_state.moisture_threshold);
    xSemaphoreGive(irrigation_state_mutex);

    char *json_str = cJSON_PrintUnformatted(json);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, json_str, HTTPD_RESP_USE_STRLEN);
    free(json_str);
    cJSON_Delete(json);
    return ESP_OK;
}

static esp_err_t api_command_handler(httpd_req_t *req) {
    char buf[256];
    int ret = httpd_req_recv(req, buf, sizeof(buf) - 1);
    if (ret <= 0) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Corpo vazio");
        return ESP_FAIL;
    }
    buf[ret] = '\0';

    cJSON *json = cJSON_Parse(buf);
    if (!json) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "JSON invalido");
        return ESP_FAIL;
    }

    cJSON *method = cJSON_GetObjectItem(json, "method");
    cJSON *params = cJSON_GetObjectItem(json, "params");

    if (!method || !cJSON_IsString(method)) {
        cJSON_Delete(json);
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "method ausente");
        return ESP_FAIL;
    }

    if (strcmp(method->valuestring, "irrigate") == 0) {
        int duration = 20;
        if (params && cJSON_GetObjectItem(params, "duration")) {
            duration = cJSON_GetObjectItem(params, "duration")->valueint;
        }
        irrigation_cmd_t cmd = { .type = CMD_START_MANUAL, .param = duration };
        xQueueSend(irrigation_cmd_queue, &cmd, 0);

    } else if (strcmp(method->valuestring, "stop") == 0) {
        irrigation_cmd_t cmd = { .type = CMD_STOP, .param = 0 };
        xQueueSend(irrigation_cmd_queue, &cmd, 0);

    } else if (strcmp(method->valuestring, "setMode") == 0) {
        if (params && cJSON_GetObjectItem(params, "mode") && cJSON_IsString(cJSON_GetObjectItem(params, "mode"))) {
            const char *m = cJSON_GetObjectItem(params, "mode")->valuestring;
            irrigation_cmd_t cmd = { .type = CMD_SET_MODE, .param = (strcmp(m, "AUTO") == 0 ? 0 : 1) };
            xQueueSend(irrigation_cmd_queue, &cmd, 0);
        }

    } else if (strcmp(method->valuestring, "setThreshold") == 0) {
        if (params && cJSON_GetObjectItem(params, "threshold")) {
            int th = cJSON_GetObjectItem(params, "threshold")->valueint;
            irrigation_cmd_t cmd = { .type = CMD_SET_THRESHOLD, .param = th };
            xQueueSend(irrigation_cmd_queue, &cmd, 0);
        }
    }

    cJSON_Delete(json);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, "{\"status\":\"ok\"}", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

void webserver_task_init(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 8;
    config.stack_size = 8192;

    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t uri_dashboard = { .uri = "/", .method = HTTP_GET, .handler = dashboard_handler };
        httpd_uri_t uri_status = { .uri = "/api/status", .method = HTTP_GET, .handler = api_status_handler };
        httpd_uri_t uri_command = { .uri = "/api/command", .method = HTTP_POST, .handler = api_command_handler };
        httpd_register_uri_handler(server, &uri_dashboard);
        httpd_register_uri_handler(server, &uri_status);
        httpd_register_uri_handler(server, &uri_command);
        ESP_LOGI(TAG, "Dashboard web iniciado na porta 80");
    } else {
        ESP_LOGE(TAG, "Falha ao iniciar o servidor HTTP");
    }
}
