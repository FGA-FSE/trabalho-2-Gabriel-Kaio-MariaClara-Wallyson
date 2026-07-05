#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include "common.h"

void mqtt_manager_init();

// Chamado pelas outras tasks para notificar que atributos mudaram
void mqtt_publish_attributes();

#endif // MQTT_MANAGER_H
