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

## 4.2 Artigos sobre Aplicação / Uso do Produto

---

### Artigo A1

| Campo | Informação |
|-------|-----------|
| **Título** | [Monitoring System for Operating Variables in Incubators in the Neonatology Service of a Highly Complex Hospital through the Internet of Things (IoT)](https://www.mdpi.com/1424-8220/23/12/5719) |
| **Autores** | Pedro Antonio Aya-Parra; Andres Jacob Rodriguez-Orjuela; Viviana Rodriguez Torres; Nidia Patricia Cordoba Hernandez; Natalia Martinez Castellanos; Jefferson Sarmiento-Rojas |
| **Revista** | *Sensors* (MDPI), Vol. 23, Issue 12, Art. 5719, Jun. 2023 |
| **DOI** | [10.3390/s23125719](https://doi.org/10.3390/s23125719) |
| **Link direto** | https://www.mdpi.com/1424-8220/23/12/5719 |
| **Base** | MDPI — Open Access |
| **Citações** | 9 (Scopus) |

#### Resumo

O trabalho descreve o desenvolvimento e a implementação de um sistema IoT para monitoramento de **temperatura, umidade e ruído** em incubadoras neonatais de um hospital universitário na Colômbia. O hardware consiste em um **ESP32-C3** como microcontrolador, sensor **SHT31** de temperatura/umidade (precisão de 1,5% UR; resolução de 0,2 °C) e microfone MEMS ADMP401. Os dados são coletados a cada 5 minutos e transmitidos via Wi-Fi ao broker **EMQX** usando o **protocolo MQTT**. Uma aplicação web progressiva (PWA chamada *SiMCa-Bio*) permite acesso em tempo real, geração de alertas e registro histórico. O teste piloto de 4 meses demonstrou erro relativo de apenas **4,2% na umidade**, dentro dos limites da norma ISO/IEC 17025.

#### Relevância para Sistemas Embarcados

Caso de uso real de produto IoT baseado em **ESP32 + sensor de umidade + MQTT** em contexto hospitalar de alta criticidade. O artigo valida experimentalmente os limites de precisão dos sensores SHT31 e a robustez do protocolo MQTT em ambientes clínicos com requisitos rigorosos de confiabilidade e rastreabilidade de dados.

---

### Artigo A2

| Campo | Informação |
|-------|-----------|
| **Título** | [Deep Learning-Based IoT System for Remote Monitoring and Early Detection of Health Issues in Real-Time](https://www.mdpi.com/1424-8220/23/11/5204) |
| **Autores** | Md. Reazul Islam; Md. Mohsin Kabir; Muhammad Firoz Mridha; Sultan Alfarhood; Mejdl Safran; Dunren Che |
| **Revista** | *Sensors* (MDPI), Vol. 23, Issue 11, Art. 5204, Mai. 2023 |
| **DOI** | [10.3390/s23115204](https://doi.org/10.3390/s23115204) |
| **Link direto** | https://www.mdpi.com/1424-8220/23/11/5204 |
| **Base** | MDPI — Open Access |
| **Citações** | 161 |

#### Resumo

Este artigo propõe um sistema IoT para monitoramento remoto de saúde e detecção precoce de doenças em ambiente domiciliar. O sistema integra sensores de oximetria (MAX30100), ECG (AD8232) e temperatura corporal (MLX90614). Os dados são transmitidos a um servidor via **protocolo MQTT**, onde um modelo de *deep learning* baseado em CNN com camada de atenção classifica potenciais condições cardíacas em 5 categorias e detecta febre. O sistema conecta automaticamente o usuário ao médico mais próximo ao detectar anomalias críticas. Com **161 citações**, é um dos trabalhos mais impactantes na área de IoMT (*Internet of Medical Things*).

#### Relevância para Sistemas Embarcados

Demonstra a aplicação do **protocolo MQTT em sistemas embarcados para saúde**, onde a leveza e o modelo pub/sub são essenciais para a transmissão contínua e confiável de dados de múltiplos sensores em dispositivos com recursos computacionais limitados. Reforça que o MQTT é o protocolo de fato para sistemas embarcados IoT que exigem baixa latência e baixo consumo de banda.

---

### Artigo A3

| Campo | Informação |
|-------|-----------|
| **Título** | [Development of a Unified IoT Platform for Assessing Meteorological and Air Quality Data in a Tropical Environment](https://www.mdpi.com/1424-8220/24/9/2729) |
| **Autores** | David Kairuz-Cabrera; Victor Hernandez-Rodriguez; Olivier Schalm; Alain Martinez; Pedro Merino Laso; Daniellys Alejo-Sánchez |
| **Revista** | *Sensors* (MDPI), Vol. 24, Issue 9, Art. 2729, Abr. 2024 |
| **DOI** | [10.3390/s24092729](https://doi.org/10.3390/s24092729) |
| **Link direto** | https://www.mdpi.com/1424-8220/24/9/2729 |
| **Base** | MDPI — Open Access |
| **Citações** | 8 |

#### Resumo

O trabalho apresenta uma plataforma IoT unificada de baixo custo para monitoramento de poluentes atmosféricos e parâmetros meteorológicos — incluindo **temperatura e umidade relativa** — em ambiente tropical (Cuba). A arquitetura combina sensores Alphasense, comunicação **LoRa**, **protocolo MQTT**, Node-RED, InfluxDB e Grafana. Uma campanha de campo de 15 dias validou o sistema contra o equipamento de referência Libelium Smart Environment Pro, demonstrando resultados confiáveis com custo várias vezes inferior, aplicável a políticas de saúde pública em nações em desenvolvimento.

#### Relevância para Sistemas Embarcados

Valida a cadeia tecnológica típica de produtos IoT em campo: **sensores (incluindo umidade) → microcontrolador → MQTT → banco de dados → dashboard**. Demonstra que esta arquitetura é robusta e aplicável em condições ambientais adversas, com avaliação quantitativa de desempenho em comparação a instrumentação de referência profissional.

---

### Artigo A4

| Campo | Informação |
|-------|-----------|
| **Título** | [An Integrated IoT-Based Multi-Sensor Framework for Real-Time Indoor Environment and Safety Monitoring](https://www.mdpi.com/1424-8220/26/12/3702) |
| **Autores** | Aung Min Naing; Duaa Zuhair Al-Hamid; Anuradha Singh |
| **Revista** | *Sensors* (MDPI), Vol. 26, Issue 12, Art. 3702, Jun. 2026 |
| **DOI** | [10.3390/s26123702](https://doi.org/10.3390/s26123702) |
| **Link direto** | https://www.mdpi.com/1424-8220/26/12/3702 |
| **Base** | MDPI — Open Access |
| **Seção** | Advanced IoT Systems in Smart Cities: 3rd Edition |

#### Resumo

Este artigo apresenta um *framework* IoT multi-sensorial integrado para monitoramento em tempo real da qualidade do ambiente interno e segurança em residências e pequenos edifícios inteligentes. O sistema integra múltiplos parâmetros ambientais — temperatura, **umidade**, qualidade do ar, ruído e presença — em um único dispositivo IoT de baixo custo. A comunicação entre os nós sensores e a nuvem é feita via **protocolo MQTT**, com geração de alertas em tempo real quando os parâmetros saem das faixas de segurança, contribuindo para o bem-estar dos ocupantes em ambientes *smart home* e *smart building*.

#### Relevância para Sistemas Embarcados

Representa o **estado da arte de produtos IoT embarcados multi-sensor**, onde o microcontrolador atua como hub de aquisição de múltiplos sensores (incluindo umidade), processamento local e comunicação via MQTT. Demonstra a maturidade e a relevância atual desta arquitetura para aplicações de *smart home* e *smart building*.

---

## 4.3 Síntese Geral

| # | Artigo | Tipo | Revista | Tecnologias Centrais | Link |
|---|--------|------|---------|----------------------|------|
| T1 | Sunardi et al., 2023 | Tecnologia Habilitadora | IEEE Access | ESP32, DHT22, IoT, Fuzzy | [🔗](https://ieeexplore.ieee.org/document/10015014) |
| T2 | Cujilema et al., 2023 | Tecnologia Habilitadora | IEEE Latin America Trans. | ESP32, ESP-NOW, MQTT | [🔗](https://ieeexplore.ieee.org/document/10244182) |
| T3 | Fawwaz et al., 2022 | Tecnologia Habilitadora | MDPI Sensors | MQTT, SDN, Edge Computing | [🔗](https://www.mdpi.com/1424-8220/22/9/3431) |
| T4 | Mota et al., 2025 | Tecnologia Habilitadora | MDPI Sensors | ESP32-C6, MQTT, InfluxDB | [🔗](https://www.mdpi.com/1424-8220/25/6/1683) |
| A1 | Aya-Parra et al., 2023 | Aplicação | MDPI Sensors | ESP32-C3, SHT31, MQTT, EMQX | [🔗](https://www.mdpi.com/1424-8220/23/12/5719) |
| A2 | Islam et al., 2023 | Aplicação | MDPI Sensors | IoT, MQTT, Deep Learning, IoMT | [🔗](https://www.mdpi.com/1424-8220/23/11/5204) |
| A3 | Kairuz-Cabrera et al., 2024 | Aplicação | MDPI Sensors | IoT, MQTT, LoRa, Umidade | [🔗](https://www.mdpi.com/1424-8220/24/9/2729) |
| A4 | Naing et al., 2026 | Aplicação | MDPI Sensors | IoT, MQTT, Multi-sensor, Smart Home | [🔗](https://www.mdpi.com/1424-8220/26/12/3702) |

---

## 4.4 Referências 

**[T1]** SUNARDI; YUDHANA, Anton; FURIZAL. Tsukamoto Fuzzy Inference System on Internet of Things-Based for Room Temperature and Humidity Control. **IEEE Access**, v. 11, 2023. DOI: [10.1109/ACCESS.2023.3240093](https://doi.org/10.1109/ACCESS.2023.3240093). Disponível em: https://ieeexplore.ieee.org/document/10015014.

**[T2]** CUJILEMA PAGUAY, Joel A. et al. Secure Home Automation System Based on ESP-NOW Mesh Network, MQTT and Home Assistant Platform. **IEEE Latin America Transactions**, v. 21, n. 7, p. 829–838, jul. 2023. DOI: [10.1109/TLA.2023.10244182](https://doi.org/10.1109/TLA.2023.10244182). Disponível em: https://ieeexplore.ieee.org/document/10244182.

**[T3]** FAWWAZ, Dzaky Zakiyal et al. Optimal Distributed MQTT Broker and Services Placement for SDN-Edge Based Smart City Architecture. **Sensors**, v. 22, n. 9, art. 3431, abr. 2022. DOI: [10.3390/s22093431](https://doi.org/10.3390/s22093431). Disponível em: https://www.mdpi.com/1424-8220/22/9/3431.

**[T4]** MOTA, Afonso et al. Implementation of an Internet of Things Architecture to Monitor Indoor Air Quality: A Case Study During Sleep Periods. **Sensors**, v. 25, n. 6, art. 1683, mar. 2025. DOI: [10.3390/s25061683](https://doi.org/10.3390/s25061683). Disponível em: https://www.mdpi.com/1424-8220/25/6/1683.

**[A1]** AYA-PARRA, Pedro Antonio et al. Monitoring System for Operating Variables in Incubators in the Neonatology Service of a Highly Complex Hospital through the Internet of Things (IoT). **Sensors**, v. 23, n. 12, art. 5719, jun. 2023. DOI: [10.3390/s23125719](https://doi.org/10.3390/s23125719). Disponível em: https://www.mdpi.com/1424-8220/23/12/5719.

**[A2]** ISLAM, Md. Reazul et al. Deep Learning-Based IoT System for Remote Monitoring and Early Detection of Health Issues in Real-Time. **Sensors**, v. 23, n. 11, art. 5204, mai. 2023. DOI: [10.3390/s23115204](https://doi.org/10.3390/s23115204). Disponível em: https://www.mdpi.com/1424-8220/23/11/5204.

**[A3]** KAIRUZ-CABRERA, David et al. Development of a Unified IoT Platform for Assessing Meteorological and Air Quality Data in a Tropical Environment. **Sensors**, v. 24, n. 9, art. 2729, abr. 2024. DOI: [10.3390/s24092729](https://doi.org/10.3390/s24092729). Disponível em: https://www.mdpi.com/1424-8220/24/9/2729.

**[A4]** NAING, Aung Min; AL-HAMID, Duaa Zuhair; SINGH, Anuradha. An Integrated IoT-Based Multi-Sensor Framework for Real-Time Indoor Environment and Safety Monitoring. **Sensors**, v. 26, n. 12, art. 3702, jun. 2026. DOI: [10.3390/s26123702](https://doi.org/10.3390/s26123702). Disponível em: https://www.mdpi.com/1424-8220/26/12/3702.
