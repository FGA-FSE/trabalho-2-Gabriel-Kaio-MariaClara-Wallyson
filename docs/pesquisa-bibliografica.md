# 4. Pesquisa Bibliográfica e Tecnológica

> **Tecnologias pesquisadas:** MQTT · ESP32 · Sensores de Umidade · IoT  
> **Bases consultadas:** [IEEE Xplore](https://ieeexplore.ieee.org) · [MDPI Sensors](https://www.mdpi.com/journal/sensors) · [IEEE Latin America Transactions](https://ieeexplore.ieee.org/xpl/RecentIssue.jsp?punumber=9907) · [IEEE Access](https://ieeeaccess.ieee.org)

---

## 4.1 Artigos sobre Tecnologias Habilitadoras do Produto

---

### Artigo T1

| Campo | Informação |
|-------|-----------|
| **Título** | [Tsukamoto Fuzzy Inference System on Internet of Things-Based for Room Temperature and Humidity Control](https://ieeexplore.ieee.org/document/10015014) |
| **Autores** | Sunardi; Anton Yudhana; Furizal |
| **Revista** | *IEEE Access*, Vol. 11, 2023 |
| **DOI** | [10.1109/ACCESS.2023.3240093](https://doi.org/10.1109/ACCESS.2023.3240093) |
| **Link direto** | https://ieeexplore.ieee.org/document/10015014 |
| **Base** | IEEE Xplore |
| **Citações** | 21 |

#### Resumo

O artigo apresenta um sistema IoT para controle automático de temperatura e umidade de ambientes internos utilizando o microcontrolador **ESP32** integrado com o sensor **DHT22** para aquisição dos dados ambientais. A lógica de controle é implementada via **sistema de inferência fuzzy de Tsukamoto**, cujas regras processam as leituras do sensor e determinam a ação de atuadores (ventiladores). A comunicação dos dados é realizada por Wi-Fi para uma base de dados na nuvem (Firebase Realtime Database), permitindo monitoramento e controle remoto via aplicativo mobile. Os resultados validaram que o sistema responde corretamente a variações ambientais, aumentando a confortabilidade e reduzindo intervenção manual.

#### Relevância para Sistemas Embarcados

Demonstra a viabilidade do **ESP32 como microcontrolador embarcado de baixo custo** para aquisição de dados de sensores de umidade/temperatura, processamento de lógica de controle local e conectividade Wi-Fi — as três funções centrais do produto. O trabalho documenta a arquitetura de hardware e firmware típica de produtos IoT baseados em ESP32 e sensores de umidade do tipo DHT/SHT.

---

### Artigo T2

| Campo | Informação |
|-------|-----------|
| **Título** | [Secure Home Automation System Based on ESP-NOW Mesh Network, MQTT and Home Assistant Platform](https://ieeexplore.ieee.org/document/10244182) |
| **Autores** | Joel A. Cujilema Paguay; Gustavo A. Hidalgo Brito; Dixys L. Hernandez Rojas; Joffre J. Cartuche Calva |
| **Revista** | *IEEE Latin America Transactions*, Vol. 21, Issue 7, pp. 829–838, Jul. 2023 |
| **DOI** | [10.1109/TLA.2023.10244182](https://doi.org/10.1109/TLA.2023.10244182) |
| **Link direto** | https://ieeexplore.ieee.org/document/10244182 |
| **Base** | IEEE Xplore |
| **Citações** | 28 |

#### Resumo

Este trabalho desenvolve e valida um sistema de automação residencial seguro utilizando dispositivos IoT de baixo custo baseados em **ESP32**, com protocolo de rede em malha **ESP-NOW** para comunicação entre nós e integração ao **protocolo MQTT** via broker seguro. O sistema é integrado à plataforma Home Assistant para visualização e controle domótico. Um algoritmo de criação de rede em malha garante comunicação segura com criptografia de dados. Os testes em ambiente real demonstraram latência média de **75 ms** e taxa de perda de pacotes de **9,25%**, comprovando desempenho satisfatório para aplicações IoT residenciais.

#### Relevância para Sistemas Embarcados

Documenta como o **ESP32 opera em arquiteturas distribuídas de sistemas embarcados**, combinando protocolo ESP-NOW (comunicação entre dispositivos) e MQTT (comunicação com broker/nuvem). Evidencia a escalabilidade desta plataforma para múltiplos nós sensores e valida quantitativamente o desempenho do MQTT como camada de comunicação IoT.

---

### Artigo T3

| Campo | Informação |
|-------|-----------|
| **Título** | [Optimal Distributed MQTT Broker and Services Placement for SDN-Edge Based Smart City Architecture](https://www.mdpi.com/1424-8220/22/9/3431) |
| **Autores** | Dzaky Zakiyal Fawwaz; Sang-Hwa Chung; Chang-Woo Ahn; Won-Suk Kim |
| **Revista** | *Sensors* (MDPI), Vol. 22, Issue 9, Art. 3431, Abr. 2022 |
| **DOI** | [10.3390/s22093431](https://doi.org/10.3390/s22093431) |
| **Link direto** | https://www.mdpi.com/1424-8220/22/9/3431 |
| **Base** | MDPI — Open Access |
| **Citações** | 26 |

#### Resumo

O artigo realiza uma análise técnica aprofundada do **protocolo MQTT como tecnologia central de comunicação para infraestruturas IoT** em larga escala (cidades inteligentes). O trabalho caracteriza o MQTT como um protocolo leve de troca de mensagens, baseado em *publish/subscribe*, que utiliza um broker centralizado para compartilhamento de dados entre dispositivos de baixo consumo. Os autores propõem uma arquitetura de **broker MQTT distribuído** otimizada por programação inteiro-não-linear para posicionamento em recursos de *edge computing*, demonstrando redução significativa no tráfego de rede e na latência de entrega de dados.

#### Relevância para Sistemas Embarcados

Define claramente o papel do **MQTT na pilha de comunicação dos sistemas embarcados IoT**: o microcontrolador (ESP32) publica dados dos sensores para o broker MQTT, que os encaminha para servidores ou dashboards. Essa arquitetura publish/subscribe é a base de funcionamento do produto descrito, e o artigo fornece fundamentação científica sólida para sua adoção.

---

### Artigo T4

| Campo | Informação |
|-------|-----------|
| **Título** | [Implementation of an Internet of Things Architecture to Monitor Indoor Air Quality: A Case Study During Sleep Periods](https://www.mdpi.com/1424-8220/25/6/1683) |
| **Autores** | Afonso Mota; Carlos Serôdio; Ana Briga-Sá; Antonio Valente |
| **Revista** | *Sensors* (MDPI), Vol. 25, Issue 6, Art. 1683, Mar. 2025 |
| **DOI** | [10.3390/s25061683](https://doi.org/10.3390/s25061683) |
| **Link direto** | https://www.mdpi.com/1424-8220/25/6/1683 |
| **Base** | MDPI — Open Access *(Editor's Choice)* |
| **Citações** | 6 |

#### Resumo

Apresenta a implementação completa de uma arquitetura IoT para monitoramento de qualidade do ar interno (IAQ) utilizando o **ESP32-C6** como dispositivo embarcado de aquisição. O sistema utiliza o **protocolo MQTT** para transmissão dos dados ao banco de dados **InfluxDBv2**, visualizado via Grafana. O sistema foi validado em dois cenários durante períodos de sono (porta ligeiramente aberta vs. fechada), demonstrando que a ventilação mantém os níveis de CO₂ abaixo dos limites recomendados. Selecionado como *Editor's Choice* pelo MDPI.

#### Relevância para Sistemas Embarcados

Documenta precisamente a cadeia tecnológica completa: **sensor → ESP32-C6 → Wi-Fi → MQTT → InfluxDB → Grafana → análise preditiva**. Representa a mesma arquitetura de hardware e firmware aplicável ao produto em estudo (ESP32 + sensor de umidade + MQTT), validada em ambiente real com métricas de desempenho e análise de dados.

---

## 4.2 Artigos sobre Aplicação / Uso do Produto (Irrigação Inteligente e Uso em Campo)

---

### Artigo A1

| Campo | Informação |
|-------|-----------|
| **Título** | [Internet of Things (IoT)-Based Smart Agriculture Irrigation and Monitoring System Using Ubidots Server](https://www.mdpi.com/2673-4591/82/1/99 ) |
| **Autores** | Mohammad Mohiuddin; Md. Saiful Islam; Shaila Shanjida |
| **Revista** | *Engineering Proceedings* (MDPI), Vol. 82, Issue 1, Art. 99, 2024 |
| **DOI** | [10.3390/ecsa-11-20528](https://doi.org/10.3390/ecsa-11-20528 ) |
| **Link direto** | https://www.mdpi.com/2673-4591/82/1/99 |
| **Base** | MDPI — Open Access |
| **Citações** | 14 |

#### Resumo

O artigo propõe um sistema de irrigação inteligente baseado em IoT para agricultura, utilizando o microcontrolador **ESP32** integrado a sensores de umidade do solo, temperatura, umidade do ar (DHT11 ) e detecção de chuva. Os dados são transmitidos via Wi-Fi para o servidor **Ubidots**, onde são processados e comparados com limiares predefinidos. O sistema automatiza a ativação de bombas d'água via relés quando a umidade do solo cai abaixo do nível crítico, permitindo também o controle manual remoto pelo agricultor via dashboard web ou mobile.

#### Relevância para Sistemas Embarcados

Demonstra a aplicação prática do **ESP32 em ambientes agrícolas**, validando sua capacidade de gerenciar múltiplos sensores e atuar sobre o hardware (bombas) em tempo real. O uso de protocolos como **HTTP/MQTT** para comunicação com servidores de nuvem (Ubidots) reforça a viabilidade de soluções de baixo custo para monitoramento e automação de campo.

---

### Artigo A2

| Campo | Informação |
|-------|-----------|
| **Título** | [Research and Development of an IoT Smart Irrigation System for Farmland Based on LoRa and Edge Computing](https://www.mdpi.com/2073-4395/15/2/366 ) |
| **Autores** | Li-Li Zhangzhong; Hao-Ran Gao; Wen-An Zheng; Guang-Fei Chen |
| **Revista** | *Agronomy* (MDPI), Vol. 15, Issue 2, Art. 366, 2025 |
| **DOI** | [10.3390/agronomy15020366](https://doi.org/10.3390/agronomy15020366 ) |
| **Link direto** | https://www.mdpi.com/2073-4395/15/2/366 |
| **Base** | MDPI — Open Access |
| **Citações** | - |

#### Resumo

Este trabalho desenvolve um sistema de irrigação inteligente para grandes áreas agrícolas utilizando comunicação **LoRa** e **Edge Computing**. A arquitetura utiliza nós sensores para coletar dados de umidade e temperatura, enviando-os a um gateway que executa um algoritmo de decisão baseado na fórmula de Penman-Monteith modificada e na equação de balanço hídrico. Os testes de campo demonstraram uma cobertura de comunicação de até 4 km com taxa de perda de pacotes zero em 3,5 km, garantindo irrigação precisa adaptada às características do solo e estágios de crescimento da cultura (trigo ).

#### Relevância para Sistemas Embarcados

Valida o uso de tecnologias de longo alcance (**LoRa**) integradas a sistemas embarcados para superar as limitações de distância em aplicações rurais. O artigo destaca a importância do **processamento na borda (Edge Computing)** para reduzir a latência e otimizar o consumo de água, fornecendo uma base sólida para o desenvolvimento de gateways IoT robustos.

---

### Artigo A3

| Campo | Informação |
|-------|-----------|
| **Título** | [Sustainable Irrigation System for Farming Supported by Machine Learning and Real-Time Sensor Data](https://www.mdpi.com/1424-8220/21/9/3079 ) |
| **Autores** | Abel Glória; João Cardoso; Pedro Sebastião |
| **Revista** | *Sensors* (MDPI), Vol. 21, Issue 9, Art. 3079, 2021 |
| **DOI** | [10.3390/s21093079](https://doi.org/10.3390/s21093079 ) |
| **Link direto** | https://www.mdpi.com/1424-8220/21/9/3079 |
| **Base** | MDPI — Open Access |
| **Citações** | 105 |

#### Resumo

O artigo apresenta um sistema de irrigação sustentável que combina dados de sensores em tempo real com modelos de **Machine Learning**. Utilizando o microcontrolador **ESP32**, o sistema coleta dados de umidade do solo e parâmetros meteorológicos, transmitindo-os via **LoRa e MQTT** para um servidor central. Um modelo de regressão prevê a necessidade de água para as próximas 24 horas, otimizando o agendamento da irrigação. O trabalho foca na eficiência energética, utilizando o modo *deep sleep* do ESP32 para prolongar a vida útil da bateria em dispositivos de campo.

#### Relevância para Sistemas Embarcados

Explora técnicas avançadas de **gerenciamento de energia em sistemas embarcados** (ESP32 ), essenciais para dispositivos alimentados por bateria em campo. Além disso, demonstra a integração de protocolos de comunicação híbridos (**MQTT sobre LoRa**) para garantir a entrega confiável de dados em arquiteturas IoT agrícolas.

---

### Artigo A4

| Campo | Informação |
|-------|-----------|
| **Título** | [Smart & Green: An Internet-of-Things Framework for Smart Irrigation](https://www.mdpi.com/1424-8220/20/1/190 ) |
| **Autores** | Junior M. Talavera; Luis E. Tobón; J. A. Gómez; Maria A. Culman; et al. |
| **Revista** | *Sensors* (MDPI), Vol. 20, Issue 1, Art. 190, 2020 |
| **DOI** | [10.3390/s20010190](https://doi.org/10.3390/s20010190 ) |
| **Link direto** | https://www.mdpi.com/1424-8220/20/1/190 |
| **Base** | MDPI — Open Access |
| **Citações** | 120+ |

#### Resumo

O trabalho introduz o *framework* "Smart & Green", uma arquitetura IoT completa para gestão de irrigação inteligente. O sistema suporta múltiplos protocolos de comunicação, incluindo **MQTT e CoAP**, para integrar nós sensores de umidade e estações meteorológicas. O diferencial é a camada de serviço que permite a configuração personalizada de tipos de solo, culturas e sistemas de irrigação (ex: microaspersão ), além de algoritmos para detecção e remoção de *outliers* nos dados coletados, garantindo decisões de irrigação mais precisas e economia de água.

#### Relevância para Sistemas Embarcados

Fornece um modelo de referência para a **arquitetura de software de sistemas IoT**, destacando a necessidade de interoperabilidade entre diferentes protocolos (**MQTT/CoAP**) e a importância do pré-processamento de dados diretamente nos dispositivos embarcados ou gateways para melhorar a confiabilidade do sistema final.



## 4.3 Síntese Geral

| # | Artigo | Tipo | Revista | Tecnologias Centrais | Link |
|---|--------|------|---------|----------------------|------|
| T1 | Sunardi et al., 2023 | Tecnologia Habilitadora | IEEE Access | ESP32, DHT22, IoT, Fuzzy | [🔗](https://ieeexplore.ieee.org/document/10015014) |
| T2 | Cujilema et al., 2023 | Tecnologia Habilitadora | IEEE Latin America Trans. | ESP32, ESP-NOW, MQTT | [🔗](https://ieeexplore.ieee.org/document/10244182) |
| T3 | Fawwaz et al., 2022 | Tecnologia Habilitadora | MDPI Sensors | MQTT, SDN, Edge Computing | [🔗](https://www.mdpi.com/1424-8220/22/9/3431) |
| T4 | Mota et al., 2025 | Tecnologia Habilitadora | MDPI Sensors | ESP32-C6, MQTT, InfluxDB | [🔗](https://www.mdpi.com/1424-8220/25/6/1683) |
| A1 | Mohiuddin et al., 2024 | Aplicação | Engineering Proc. | ESP32, DHT11, IoT, Ubidots | [🔗](https://www.mdpi.com/2673-4591/82/1/99 ) |
| A2 | Zhangzhong et al., 2025 | Aplicação | Agronomy | LoRa, Edge Computing, Penman-Monteith | [🔗](https://www.mdpi.com/2073-4395/15/2/366 ) |
| A3 | Glória et al., 2021 | Aplicação | Sensors | ESP32, LoRa, MQTT, Machine Learning | [🔗](https://www.mdpi.com/1424-8220/21/9/3079 ) |
| A4 | Talavera et al., 2020 | Aplicação | Sensors | MQTT, CoAP, IoT Framework | [🔗](https://www.mdpi.com/1424-8220/20/1/190 ) |

---

## 4.4 Referências 

**[T1]** SUNARDI; YUDHANA, Anton; FURIZAL. Tsukamoto Fuzzy Inference System on Internet of Things-Based for Room Temperature and Humidity Control. **IEEE Access**, v. 11, 2023. DOI: [10.1109/ACCESS.2023.3240093](https://doi.org/10.1109/ACCESS.2023.3240093). Disponível em: https://ieeexplore.ieee.org/document/10015014.

**[T2]** CUJILEMA PAGUAY, Joel A. et al. Secure Home Automation System Based on ESP-NOW Mesh Network, MQTT and Home Assistant Platform. **IEEE Latin America Transactions**, v. 21, n. 7, p. 829–838, jul. 2023. DOI: [10.1109/TLA.2023.10244182](https://doi.org/10.1109/TLA.2023.10244182). Disponível em: https://ieeexplore.ieee.org/document/10244182.

**[T3]** FAWWAZ, Dzaky Zakiyal et al. Optimal Distributed MQTT Broker and Services Placement for SDN-Edge Based Smart City Architecture. **Sensors**, v. 22, n. 9, art. 3431, abr. 2022. DOI: [10.3390/s22093431](https://doi.org/10.3390/s22093431). Disponível em: https://www.mdpi.com/1424-8220/22/9/3431.

**[T4]** MOTA, Afonso et al. Implementation of an Internet of Things Architecture to Monitor Indoor Air Quality: A Case Study During Sleep Periods. **Sensors**, v. 25, n. 6, art. 1683, mar. 2025. DOI: [10.3390/s25061683](https://doi.org/10.3390/s25061683). Disponível em: https://www.mdpi.com/1424-8220/25/6/1683.

**[A1]** MOHIUDDIN, Mohammad; ISLAM, Md. Saiful; SHANJIDA, Shaila. Internet of Things (IoT )-Based Smart Agriculture Irrigation and Monitoring System Using Ubidots Server. **Engineering Proceedings**, v. 82, n. 1, art. 99, 2024. DOI: [10.3390/ecsa-11-20528](https://doi.org/10.3390/ecsa-11-20528 ). Disponível em: https://www.mdpi.com/2673-4591/82/1/99.

**[A2]** ZHANGZHONG, Li-Li; GAO, Hao-Ran; ZHENG, Wen-An; CHEN, Guang-Fei. Research and Development of an IoT Smart Irrigation System for Farmland Based on LoRa and Edge Computing. **Agronomy**, v. 15, n. 2, art. 366, 2025. DOI: [10.3390/agronomy15020366](https://doi.org/10.3390/agronomy15020366 ). Disponível em: https://www.mdpi.com/2073-4395/15/2/366.

**[A3]** GLÓRIA, Abel; CARDOSO, João; SEBASTIÃO, Pedro. Sustainable Irrigation System for Farming Supported by Machine Learning and Real-Time Sensor Data. **Sensors**, v. 21, n. 9, art. 3079, 2021. DOI: [10.3390/s21093079](https://doi.org/10.3390/s21093079 ). Disponível em: https://www.mdpi.com/1424-8220/21/9/3079.

**[A4]** TALAVERA, Junior M. et al. Smart & Green: An Internet-of-Things Framework for Smart Irrigation. **Sensors**, v. 20, n. 1, art. 190, 2020. DOI: [10.3390/s20010190](https://doi.org/10.3390/s20010190 ). Disponível em: https://www.mdpi.com/1424-8220/20/1/190.

---

| Versão | Autor | Data |
| --- | --- | --- |
| 0.1 (Artigos sobre Tecnologias Habilitadoras do Produto) | [Kaio Macedo](https://github.com/bigkaio) | 25/06/2026 |
| 0.2 (Artigos sobre Aplicação / Uso do Produto (Irrigação Inteligente e Uso em Campo)) | [Maria Clara](https://github.com/alvezclari) | 25/06/2026 |
