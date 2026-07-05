#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "common.h"
#include <stdbool.h>

/**
 * @brief Inicializa a rede WiFi.
 * @param start_provisioning Se true, inicia em modo SoftAP para configuração. 
 *                           Se false, conecta usando credenciais salvas (STA).
 */
void wifi_manager_init(bool start_provisioning);

#endif // WIFI_MANAGER_H
