# Estrutura Analítica do Projeto (EAP)

## Introdução

Este documento apresenta a Estrutura Analítica do Projeto (EAP) para o desenvolvimento de um **controlador de irrigação inteligente com ESP32**, organizando as principais atividades necessárias para sua execução.

A EAP divide o projeto em quatro áreas principais — **Estrutura Hidráulica**, **Eletrônica**, **Energia** e **Software & Comunicação** — permitindo uma visão clara das etapas, melhor planejamento e acompanhamento das atividades.

Essa organização contribui para o controle do escopo, a integração entre os subsistemas e a condução eficiente do projeto até sua entrega prática na disciplina.

---

```mermaid
%%{init: {"theme": "default", "themeVariables": { "fontSize": "18px" }}}%%
graph TD
    classDef rootStyle fill:#f5f5dc,stroke:#000,color:#000,rx:5,ry:5,stroke-width:2px,font-size:22px;
    classDef estStyle fill:#e8f5e9,stroke:#4caf50,color:#1b5e20,rx:5,ry:5,stroke-width:2px,font-size:16px;
    classDef eleStyle fill:#e3f2fd,stroke:#2196f3,color:#0d47a1,rx:5,ry:5,font-size:16px;
    classDef eneStyle fill:#fff3e0,stroke:#ff9800,color:#e65100,rx:5,ry:5,font-size:16px;
    classDef sofStyle fill:#fce4ec,stroke:#f44336,color:#b71c1c,rx:5,ry:5,font-size:16px;

    Root["<b>Sistema de Irrigação Inteligente</b>"]:::rootStyle

    Root --> Est["<b>Estrutura Hidráulica</b>"]:::estStyle
    Root --> Ele["<b>Eletrônica</b>"]:::eleStyle
    Root --> Ene["<b>Energia</b>"]:::eneStyle
    Root --> Sof["<b>Software & Comunicação</b>"]:::sofStyle

    %% ESTRUTURA HIDRÁULICA
    Est --- Est1["<b>Projeto do circuito de água</b><br>- Definição da zona de irrigação<br>- Layout de mangueiras e pontos de saída"]:::estStyle
    Est1 --- Est2["<b>Seleção de componentes hidráulicos</b><br>- Mini bomba 3–5 V<br>- Mangueira de PVC<br>- Reservatório de água"]:::estStyle
    Est2 --- Est3["<b>Integração mecânica com sensores</b><br>- Posicionamento do sensor de umidade de solo<br>- Instalação da boia de nível no reservatório"]:::estStyle
    Est3 --- Est4["<b>Montagem física do protótipo</b><br>- Fixação da bomba<br>- Organização das mangueiras<br>- Suporte para reservatório e vaso"]:::estStyle
    Est4 --- Est5["<b>Ajustes e testes hidráulicos</b><br>- Verificação de vazamentos<br>- Testes de vazão e tempo de irrigação"]:::estStyle

    %% ELETRÔNICA
    Ele --- Ele1["<b>Arquitetura eletrônica</b><br>- Mapeamento de pinos da ESP32<br>- Definição de interfaces de sensores e atuadores"]:::eleStyle
    Ele1 --- Ele2["<b>Ligação de sensores</b><br>- Sensor capacitivo de umidade de solo (ADC)<br>- DHT11/DHT22 (GPIO)<br>- Boia de nível (GPIO)"]:::eleStyle
    Ele2 --- Ele3["<b>Controle da bomba</b><br>- Driver de relé/MOSFET<br>- Interface 5 V / 3,3 V<br>- Proteções básicas"]:::eleStyle
    Ele3 --- Ele4["<b>Protótipo em bancada</b><br>- Testes individuais de sensores<br>- Testes de acionamento da bomba"]:::eleStyle
    Ele4 --- Ele5["<b>Integração eletrônica completa</b><br>- Montagem do circuito final<br>- Organização de cabos e conectores"]:::eleStyle

    %% ENERGIA
    Ene --- Ene1["<b>Dimensionamento de alimentação</b><br>- Consumo da ESP32<br>- Consumo da bomba<br>- Margem de segurança"]:::eneStyle
    Ene1 --- Ene2["<b>Seleção de fonte/bateria</b><br>- Fonte 5 V estabilizada ou pack de baterias<br>- Critérios de autonomia mínima"]:::eneStyle
    Ene2 --- Ene3["<b>Distribuição de energia</b><br>- Barramento 5 V para bomba e relé<br>- 5 V / 3,3 V para ESP32 e sensores"]:::eneStyle
    Ene3 --- Ene4["<b>Proteção elétrica</b><br>- Fusível ou proteção simples<br>- Verificação de aquecimento de componentes"]:::eneStyle

    %% SOFTWARE & COMUNICAÇÃO
    Sof --- Sof1["<b>Planejamento e requisitos</b><br>- Definição de modos (AUTO/MANUAL)<br>- Requisitos de Telegram e Web<br>- Critérios de telemetria"]:::sofStyle
    Sof1 --- Sof2["<b>Arquitetura de software</b><br>- Máquina de estados para o controle<br>- Módulos de leitura de sensores<br>- Módulo de controle da bomba"]:::sofStyle
    Sof2 --- Sof3["<b>Integração com Telegram</b><br>- Bot para comandos e alertas<br>- Formato de mensagens<br>- Tratamento de erros"]:::sofStyle
    Sof3 --- Sof4["<b>Servidor web embarcado</b><br>- Endpoints JSON (/status, /history)<br>- Dashboard web com gráficos e log de eventos"]:::sofStyle
    Sof4 --- Sof5["<b>Integração e testes finais</b><br>- Testes de irrigação automática<br>- Testes de comandos via Telegram<br>- Testes de telemetria na web"]:::sofStyle
```

## Histórico de Versões

<font size="3"><p style={{textAlign: "left"}}>**Tabela 1** - Histórico de versões.</p></font>

| Versão | Descrição | Autor(es) | Data |
| :----: | :-------: | :-------: | :--: |
|  1.0   | Criação do documento (EAP do sistema de irrigação) | [Gabriel Santos Monteiro](https://github.com/GabrielSMonteiro) | 30/06/2026 |