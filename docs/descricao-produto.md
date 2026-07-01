# 1. Descrição do Produto

## 1.1 Visão geral

O produto é um **Controlador de Irrigação Inteligente** para uma zona de cultivo (vaso, canteiro ou pequena horta), capaz de monitorar as condições do solo e do ambiente em tempo real e acionar automaticamente a irrigação quando necessário, eliminando a necessidade de regar manualmente em horários fixos e independente da real necessidade da planta.

O sistema combina sensoriamento ambiental, lógica de decisão embarcada e múltiplos canais de interação com o usuário (bot no Telegram e dashboard web), reproduzindo em escala reduzida as funcionalidades centrais de controladores de irrigação comerciais (ex. Rain Bird, Rachio, Orbit B-Hyve), descritos na [Seção 5 — Comparativo](./comparativo.md).

## 1.2 Funções principais

- **Monitoramento contínuo** da umidade do solo, temperatura e umidade do ar, e nível do reservatório de água.
- **Irrigação automática**: aciona a bomba quando a umidade do solo cai abaixo de um limiar configurável (threshold), respeitando histerese para evitar acionamentos repetidos.
- **Irrigação manual**: permite ao usuário regar por um tempo determinado via comando no Telegram ou botão no dashboard web, desde que o reservatório esteja em nível seguro.
- **Bloqueio de segurança**: suspende a irrigação automática em caso de reservatório com nível baixo ou leituras de sensores inválidas/inconsistentes.
- **Notificações automáticas** de eventos relevantes (solo seco, irrigação concluída, reservatório baixo, falhas de sensor).
- **Histórico e telemetria**: registra e disponibiliza amostras de sensores e eventos de irrigação para consulta posterior.

## 1.3 Público-alvo

- Pessoas que cultivam plantas em casa, apartamento ou pequenos jardins e querem automatizar a rega sem depender de memória ou disponibilidade para regar manualmente.
- Pequenos produtores e entusiastas de hortas domésticas/urbanas interessados em economia de água e em soluções de automação de baixo custo.
- Estudantes e makers interessados em projetos de IoT/sistemas embarcados aplicados à agricultura, por ser uma solução open-source, replicável e de baixo custo em comparação aos produtos comerciais do mercado.

## 1.4 Componentes do sistema

| Componente | Função |
|---|---|
| **ESP32 DevKit V1** | Microcontrolador central: lê sensores, controla a bomba, conecta-se ao Wi-Fi, hospeda o dashboard web e integra com o Telegram. |
| **Sensor capacitivo de umidade do solo** | Mede a umidade do solo (saída analógica via ADC), entrada principal da lógica de decisão. |
| **Sensor de temperatura e umidade do ar (DHT11/DHT22)** | Fornece dados do microclima ao redor da planta (saída digital via GPIO). |
| **Sensor de nível de água (boia de nível)** | Indica se o reservatório está em nível seguro ou baixo, usado como trava de segurança. |
| **Mini bomba de água submersível (3–5 V)** | Atuador responsável por bombear a água até a planta. |
| **Módulo relé / driver MOSFET** | Interface entre o ESP32 (3,3 V) e a bomba (5 V), permitindo o acionamento seguro do atuador. |
| **Mangueira de PVC** | Conduz a água da bomba até o vaso ou canteiro. |

Mais detalhes sobre a arquitetura de hardware e software estão na [Seção 3 — Reprodução com ESP32](./reproducao-esp32.md).

## 1.5 Tecnologias de comunicação

- **Wi-Fi**: conectividade principal do ESP32, utilizada tanto para hospedar o dashboard web local quanto para a comunicação com a API do Telegram.
- **HTTP**: protocolo usado pelo dashboard web (servidor embarcado no ESP32) e pela integração com a API do Telegram (bot).
- **API REST/JSON**: o ESP32 expõe endpoints (`/api/status`, `/api/history`) que retornam o estado do sistema e o histórico de leituras em formato JSON, consumidos pelo próprio dashboard.
- **Telegram Bot API**: canal de comunicação remota por mensagens, permitindo consultar status (`/status`), alternar modos de operação (`/modo_auto`, `/modo_manual`) e acionar irrigação manual (`/regar_10s`, `/regar_20s`).

Essas escolhas combinam sensoriamento local e protocolos leves como HTTP para comunicação direta com o usuário, sem dependência de serviços em nuvem (ver [Seção 4 — Pesquisa Bibliográfica](./pesquisa-bibliografica.md)).

---

| Versão | Autor | Data |
| --- | --- | --- |
| 0.1 | [Wallyson](https://github.com/devwallyson) | 30/06/2026 |
