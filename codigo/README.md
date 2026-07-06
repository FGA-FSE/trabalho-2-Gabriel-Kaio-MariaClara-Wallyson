# SmartFlora - Trabalho Final FSE (2026/1)

Este repositório contém o código-fonte do **SmartFlora**, projeto desenvolvido para o Trabalho Final da disciplina de Fundamentos de Sistemas Embarcados (2026/1).

O objetivo principal desta etapa foi migrar o escopo do projeto anterior (desenvolvido no ecossistema Arduino) para a linguagem C pura, utilizando o framework oficial da Espressif, o **ESP-IDF**, aliado ao sistema operacional de tempo real **FreeRTOS**. O sistema monitora as condições da planta e controla o acionamento de uma bomba d'água de forma automatizada, reportando os dados para a nuvem.

## Componentes de Hardware Utilizados

- **Controlador Principal:** ESP32 DevKit V1
- **Sensor de Temperatura e Umidade do Ar:** DHT11 (conectado no GPIO 4)
- **Sensor de Pressão e Temperatura:** Módulo GY-BME280 / BMP280 via I²C (SDA no 21, SCL no 22)
- **Sensor de Umidade do Solo:** Emulação via Potenciômetro ligado à porta ADC (GPIO 34)
- **Atuador da Bomba de Água:** Módulo Relé de 2 canais (sinal no GPIO 26)
- **Sensor de Segurança do Reservatório:** Ultrassônico HC-SR04 (Trigger no 5, Echo no 18)
- **Interface Visual:** Display OLED 0,96" I²C (barramento compartilhado no 21/22)
- **Alarme Sonoro:** Buzzer Ativo (GPIO 25)
- **Controle Físico:** Push Button com resistor de pull-up interno (GPIO 27)

## Arquitetura de Software (FreeRTOS)

Cumprindo as diretrizes do trabalho, o loop estrutural único foi substituído por uma arquitetura multitarefa, distribuída entre os dois núcleos do processador do ESP32 através de **7 Tasks**:

1. **`sensors_task`**: Realiza a leitura periódica (a cada 30 segundos) do DHT11, do BME280 e do canal ADC.
2. **`ultrasonic_task`**: Monitora o nível do reservatório de água a cada 5 segundos para garantir a proteção contra funcionamento a seco da bomba.
3. **`irrigation_task`**: Implementa a Máquina de Estados Finita (FSM) do sistema, avaliando as leituras de solo e controlando o relé da bomba, incluindo um tempo de _cooldown_ para evitar acionamentos repetitivos rápidos.
4. **`display_task`**: Gerencia o framebuffer do OLED e alterna o ciclo entre três telas informativas distintas.
5. **`button_task`**: Realiza o _polling_ não bloqueante do botão físico. Identifica os eventos de clique único (alternar tela), duplo clique (acionamento manual forçado da bomba) e pressionamento longo (alternância entre modo Automático e Manual).
6. **`buzzer_task`**: Task bloqueada que aguarda comandos via Fila (Queue) para emitir alertas sonoros assíncronos sem interromper outras rotinas.
7. **`mqtt_telemetry_task`**: Responsável pela conexão com o servidor e envio/recebimento de dados.

Para garantir a concorrência segura, foram implementados mecanismos IPC, como `xSemaphoreCreateMutex` para proteção dos dados globais e `xQueueCreate` para enfileiramento de comandos do buzzer e do controle de irrigação.

## Integração MQTT (ThingsBoard)

A comunicação é realizada com o servidor MQTT disponibilizado pela disciplina (`tb.fse.lappis.rocks`).
- O sistema publica pacotes de telemetria periodicamente.
- O controle remoto foi implementado utilizando a funcionalidade de chamadas **RPC**. Através do dashboard web, o usuário pode enviar comandos em formato JSON (ex: ligar bomba por tempo pré-determinado, pausar, mudar modo) que são interceptados e processados pelo ESP32 imediatamente.

## Funcionalidade Extra: Provisionamento de WiFi

Para evitar a necessidade de recompilar o código a cada alteração de ambiente, implementamos um sistema de provisionamento de rede baseado em *Captive Portal* (SoftAP):

Caso a ESP32 seja iniciada **com o botão físico pressionado**, ela não tenta conectar à rede padrão. Em vez disso, cria um Ponto de Acesso (AP) local chamado `SmartFlora-Setup`. Ao conectar-se a esta rede pelo celular, o usuário pode acessar a página web de configuração embarcada e informar o SSID e senha do roteador local. Estas credenciais são armazenadas de forma definitiva na partição flash não-volátil (NVS) do microcontrolador.

## Instruções de Compilação

1. Abra o diretório do projeto utilizando o VSCode com a extensão **PlatformIO** instalada.
2. Edite o arquivo `src/config.h` para incluir o token específico do seu dispositivo ThingsBoard na constante `TB_ACCESS_TOKEN`.
3. Inicie o processo de gravação através do menu do PlatformIO (ou executando `pio run -t upload` via linha de comando).
*(Nota: O primeiro flash apagará todo o armazenamento devido à configuração personalizada da tabela de partições e sistema NVS).*
