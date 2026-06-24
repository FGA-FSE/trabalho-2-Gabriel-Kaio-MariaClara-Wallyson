## 3. Proposta de reprodução com ESP32

### 3.1 Visão geral da solução

Propomos reproduzir as principais funcionalidades de um controlador de irrigação inteligente comercial (uma zona de irrigação) utilizando um microcontrolador **ESP32**, sensores simples e uma bomba de água DC de baixa potência. [irjiet](https://irjiet.com/common_src/article_file/IRJIET1020191772610552.pdf)

O sistema fará leitura contínua da umidade do solo, temperatura e umidade do ar, além do nível do reservatório, e usará esses dados para acionar automaticamente a irrigação quando necessário, permitindo também controle manual via **bot no Telegram** e um **dashboard web** hospedado pelo próprio ESP32. [github](https://github.com/mistersomov/IoT-AutoIrrigation)

***

### 3.2 Arquitetura de hardware proposta

**Componentes principais:**

- **ESP32 DevKit V1**  
  - Responsável por ler sensores, controlar a bomba, conectar ao Wi‑Fi, servir o site de telemetria e integrar com o Telegram. [youtube](https://www.youtube.com/watch?v=P63km6uiEK8)

- **Sensor capacitivo de umidade do solo**  
  - Alimentado em 3,3–5 V, saída analógica conectada a um canal ADC do ESP32.  
  - Permite medir a umidade do solo em tempo real, sendo amplamente utilizado em projetos de irrigação com ESP32. [github](https://github.com/Circuit-Digest/Smart-Irrigation-System-Using-ESP32-Blynk-App)

- **Sensor de temperatura e umidade do ar (DHT11 ou DHT22)**  
  - Saída digital ligada a um GPIO do ESP32.  
  - Fornece temperatura ambiente e umidade relativa, dados usados para monitorar o microclima da planta. [ijirt](https://ijirt.org/publishedpaper/IJIRT182347_PAPER.pdf)

- **Sensor de nível de água (boia de nível em PP)**  
  - Boia horizontal em polipropileno, contato seco, com especificação típica de até 10 W / 0,5 A em 110 VDC. [evelta](https://evelta.com/horizontal-water-level-switch-liquid-level-sensor-pp-plastic-ball-float-switch/)
  - Ligada entre GND e um GPIO configurado como `INPUT_PULLUP` no ESP32, indicando se o reservatório está em nível seguro ou baixo.

- **Mini bomba de água submersível 3–5 V**  
  - Bomba DC com tensão de operação 3–5 V, corrente ~100–200 mA, vazão ~1,2–1,6 L/min e altura de coluna d’água de 0,3–0,8 m. [einstronic](https://einstronic.com/product/5v-mini-water-pump-submersible/)
  - Alimentada por 5 V e comutada por um módulo relé ou driver MOSFET, nunca diretamente pelo GPIO do ESP32. [robotique](https://www.robotique.site/tutorial/using-water-pump-by-esp32-card/)

- **Módulo relé 1 canal 5 V (ou driver MOSFET)**  
  - Interface entre o ESP32 (3,3 V) e a bomba (5 V), permitindo ligar/desligar a bomba de forma segura. [youtube](https://www.youtube.com/watch?v=QL3q8tBXPGk)

- **Mangueira de PVC 1 m**  
  - Diâmetro interno compatível com a saída da bomba (~5–5,5 mm), utilizada para conduzir a água até o vaso/canteiro. [shopee.com](https://shopee.com.my/3-5-VDC-Mini-Submersible-Water-Pump-for-Arduino-Raspberry-Pi-Hobby-School-STEM-FYP-Projects-i.132184430.9469777039)

Essa arquitetura segue o padrão de projetos de irrigação inteligentes com ESP32 presentes em tutoriais e artigos, que combinam sensores de solo, bomba DC e relé. [ceur-ws](https://ceur-ws.org/Vol-3058/Paper-031.pdf)

***

### 3.3 Arquitetura de software e lógica de controle

O software será dividido em três camadas principais:

1. **Camada de aquisição de dados (sensores)**  
   - Rotinas periódicas (por exemplo, a cada 30–60 s) para:
     - Ler o valor analógico do sensor de umidade do solo e normalizar para uma escala de 0–100%.  
     - Ler temperatura e umidade do ar via DHT.  
     - Ler o estado do sensor de nível (boia) para determinar se o reservatório está OK ou baixo. [electronicwings](https://www.electronicwings.com/esp32/water-level-detection-interfacing-with-esp32)

2. **Camada de decisão e controle da bomba**  
   - Dois modos de operação:
     - **Automático:**  
       - Se `umidade_solo < threshold` (por ex. 30%), reservatório OK e bomba desligada, aciona irrigação automática por um tempo fixo (ex. 20 s), desliga e relê o valor de umidade. [journal.aira.or](https://journal.aira.or.id/cosie/article/view/1492)
       - Pode usar histerese simples para evitar liga/desliga frequente (só reativar após um intervalo mínimo).  
     - **Manual:**  
       - Bomba ligada apenas por ação do usuário (botão local, comando no Telegram ou botão no site), mantendo as condições de segurança (reservatório não pode estar baixo). [irjiet](https://irjiet.com/common_src/article_file/IRJIET1020191772610552.pdf)
   - Em caso de falha de sensores (várias leituras inválidas) ou reservatório baixo, o sistema bloqueia irrigação automática e registra o evento.

3. **Camada de comunicação (Telegram + Web)**  
   - Responsável por expor o estado do sistema, receber comandos e enviar eventos relevantes, seguindo modelos de sistemas IoT com ESP32 que integram nuvem e dashboards. [docs.cirkitdesigner](https://docs.cirkitdesigner.com/project/published/9a2a1cf5-c4a6-4ecf-af69-09c04d7ba81c/esp32-based-wi-fi-controlled-water-pump-with-lcd-interface)

***

### 3.4 Integração com bot no Telegram

O ESP32 será configurado para se conectar à API do Telegram utilizando HTTP sobre Wi‑Fi, atuando como cliente do bot. [youtube](https://www.youtube.com/watch?v=e4mtwrwoLnU&vl=it)

**Comandos previstos:**

- `/status`  
  - Retorna um resumo textual do estado atual:
    - Umidade do solo (%).  
    - Temperatura e umidade do ar.  
    - Estado da bomba (ligada/desligada).  
    - Modo de operação (AUTO/MANUAL).  
    - Nível do reservatório (OK/baixo).  
    - Hora e tipo da última irrigação.  

- `/modo_auto` e `/modo_manual`  
  - Alternam o modo de operação e respondem com confirmação, informando o threshold de umidade configurado. [youtube](https://www.youtube.com/watch?v=P63km6uiEK8)

- `/regar_10s`, `/regar_20s`  
  - Acionam a bomba no modo manual por um tempo específico, respeitando a condição de reservatório. [youtube](https://www.youtube.com/watch?v=wZrRpZkEkos)

- Opcional: `/set_threshold 30`  
  - Ajusta o valor do threshold de umidade do solo.

**Notificações automáticas:**

- Solo seco detectado (antes da irrigação automática).  
- Fim da irrigação automática (informando valores antes/depois).  
- Reservatório em nível baixo (bloqueio da irrigação).  
- Erros persistentes de sensores ou falhas críticas.  

Essa integração segue a lógica de projetos de ESP32 em que eventos de sensores disparam mensagens em apps móveis ou na nuvem (Blynk, ThingSpeak, SMS), adaptada aqui para Telegram. [youtube](https://www.youtube.com/watch?v=UDo5p880uPk)

***

### 3.5 Interface web de telemetria

A proposta é que o **próprio ESP32** hospede um servidor HTTP simples, fornecendo uma página web com dashboard e endpoints de API em JSON, como visto em projetos de ESP32 que controlam bomba via web e exibem dados de sensores. [hackster](https://www.hackster.io/pradeeplogu01/building-a-real-time-soil-moisture-dashboard-a91787)

**Endpoints sugeridos:**

- `GET /`  
  - Retorna a página HTML/JS/CSS do dashboard.

- `GET /api/status`  
  - JSON com estado atual:
    - `soil_moisture` (0–100%).  
    - `air_temperature` (°C).  
    - `air_humidity` (%).  
    - `pump_state` (ON/OFF).  
    - `mode` (AUTO/MANUAL).  
    - `water_level_ok` (boolean).  
    - `moisture_threshold` atual. [youtube](https://www.youtube.com/watch?v=g2n7lSDIjzA)

- `GET /api/history`  
  - JSON com:
    - Lista das últimas N amostras (timestamp, umidade do solo, temperatura, umidade, estado da bomba).  
    - Lista de eventos de irrigação (tipo AUTO/MANUAL, duração, umidade antes/depois). [etd.repository.ugm.ac](https://etd.repository.ugm.ac.id/penelitian/detail/263946)

- Opcional: `POST /api/control`  
  - Para ações como ligar bomba manualmente e alternar modo (com validação simples).

**Elementos do dashboard:**

- Gráfico de linha de umidade do solo vs. tempo (últimas horas).  
- Gráfico de temperatura e umidade do ar vs. tempo.  
- Indicadores de estado (bomba, modo, reservatório).  
- Tabela de eventos de irrigação (hora, tipo, duração, umidade antes/depois).  
- Botões de ação (regar por X segundos, mudar modo), se o grupo optar por permitir controle via web. [scribd](https://www.scribd.com/document/849095717/3)

Essa abordagem se alinha com trabalhos e tutoriais que utilizam ESP32 para hospedar dashboards próprios de irrigação e controle de bomba via web. [github](https://github.com/mistersomov/IoT-AutoIrrigation)

***

### 3.6 Diagrama conceitual do sistema (descrição textual)

O diagrama conceitual proposto é um diagrama de blocos com os seguintes elementos e relações:

- **Bloco “Sensores”**  
  - Sensor de umidade do solo  
  - DHT11/DHT22  
  - Boia de nível  

- **Bloco “ESP32”**  
  - Módulo de aquisição de dados (ADC + GPIO).  
  - Módulo de lógica de controle (modo AUTO/MANUAL, thresholds).  
  - Módulos de comunicação (Wi‑Fi, Telegram, servidor HTTP).  

- **Bloco “Atuadores”**  
  - Módulo relé/MOSFET.  
  - Bomba de água submersível.  

- **Bloco “Interface Telegram”**  
  - Usuário interage com o bot para receber status e enviar comandos.  

- **Bloco “Interface Web”**  
  - Usuário acessa o dashboard para visualizar telemetria e histórico, e opcionalmente enviar comandos.  

Fluxos principais:

- Sensores → ESP32 (leituras periódicas).  
- ESP32 → Atuadores (comando liga/desliga bomba).  
- ESP32 ↔ Telegram (mensagens de status/comando).  
- ESP32 ↔ Navegador (API JSON + página web).  

***

### 3.7 Limitações e desafios esperados

- **Precisão e calibração dos sensores de umidade do solo**  
  - Sensores capacitivos simples são influenciados por tipo de solo, salinidade e posicionamento, exigindo calibração prática para mapear a leitura analógica para percentual de umidade confiável. [youtube](https://www.youtube.com/watch?v=N9fWEPzLq40)

- **Cobertura de Wi‑Fi**  
  - O ESP32 depende de uma rede Wi‑Fi estável; em ambientes externos ou rurais, pode haver perda de conectividade, afetando o envio de dados para o Telegram e acesso ao dashboard. [ceur-ws](https://ceur-ws.org/Vol-3058/Paper-031.pdf)

- **Latência e uso de recursos na ESP32**  
  - Hospedar servidor HTTP, lógica de controle e integração com Telegram em um único ESP32 exige cuidado com uso de memória, tempo de CPU e tratamento de falhas. [docs.cirkitdesigner](https://docs.cirkitdesigner.com/project/published/9a2a1cf5-c4a6-4ecf-af69-09c04d7ba81c/esp32-based-wi-fi-controlled-water-pump-with-lcd-interface)

- **Proteção elétrica e robustez**  
  - Bomba e relé geram ruído elétrico; é necessário projetar o hardware com proteção mínima (diodo de flyback, fonte adequada) para evitar resets aleatórios do ESP32. [robotique](https://www.robotique.site/tutorial/using-water-pump-by-esp32-card/)

- **Escalabilidade**  
  - O protótipo é pensado para uma única zona de irrigação; a extensão para múltiplas zonas exigiria mais sensores, válvulas e lógica mais complexa de agendamento, como mostrado em sistemas multi‑zona em literatura. [youtube](https://www.youtube.com/watch?v=UDo5p880uPk)

Essa proposta, apesar dessas limitações, é coerente com o que trabalhos acadêmicos e projetos open‑source têm demonstrado em smart irrigation com ESP32, e é compatível com o escopo prático da disciplina. [github](https://github.com/Circuit-Digest/Smart-Irrigation-System-Using-ESP32-Blynk-App)

<div align="center">

| Versão | Autor | Data |
| --- | --- | --- |
| 0.1 | [Gabriel Monteiro](https://github.com/GabrielSMonteiro) | 24/06/2026 |

</div>