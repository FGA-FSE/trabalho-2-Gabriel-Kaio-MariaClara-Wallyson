idf_component_register(
    SRCS "main.c"
         "nvs_storage.c"
         "wifi_manager.c"
         "sensors_task.c"
         "ultrasonic_task.c"
         "irrigation_task.c"
         "display_task.c"
         "button_task.c"
         "buzzer_task.c"
         "telegram_task.c"
         "webserver_task.c"
    INCLUDE_DIRS "."
    REQUIRES driver esp_wifi esp_event esp_netif nvs_flash
             esp_http_server esp_timer esp_adc esp_http_client esp-tls
)
