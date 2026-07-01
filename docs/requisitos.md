# Levantamento de Requisitos Funcionais e Não Funcionais

## 1. Visão Geral da Missão

O objetivo deste projeto é desenvolver um **sistema de irrigação inteligente** capaz de monitorar umidade do solo, temperatura/umidade do ar e nível do reservatório, acionando uma bomba de água de forma automática ou manual para irrigar uma zona específica (vaso, jardineira ou pequeno canteiro).  

A missão do sistema consiste em:

- Ler continuamente os sensores;
- Decidir quando irrigar com base em thresholds configuráveis;
- Garantir que o reservatório possui água suficiente;
- Expor o estado via **bot no Telegram** e **dashboard web**;
- Permitir intervenção manual segura pelo usuário.

Os requisitos funcionais (Seção 2) descrevem as funcionalidades que o sistema deve oferecer; os requisitos não funcionais (Seção 3) tratam de atributos de qualidade, restrições técnicas e desempenho.

### 1.1. Identificação dos Requisitos

Por convenção, cada requisito é identificado pelo código da categoria e um número sequencial:

- Requisitos funcionais: **[RF01]**, **[RF02]**, ...
- Requisitos não funcionais: **[RNF01]**, **[RNF02]**, ...

A referência será feita no formato:

> [nome da subseção / identificador do requisito]

### 1.2. Prioridade dos Requisitos

As prioridades adotadas são:

- **Essencial:** indispensável para o funcionamento básico do sistema.
- **Importante:** relevante para desempenho e usabilidade, mas não inviabiliza totalmente o funcionamento se ausente.
- **Desejável:** agrega valor, mas pode ser postergado para versões futuras.

---

## 2. Requisitos Funcionais (RF)

### 2.1. RF01 – Leitura de Umidade do Solo
O sistema deve ler periodicamente o valor de um **sensor capacitivo de umidade do solo**, convertendo-o para uma escala interpretável (por exemplo, 0–100%).  

### 2.2. RF02 – Leitura de Temperatura e Umidade do Ar
O sistema deve ler periodicamente dados de um sensor do tipo **DHT11 ou DHT22**, obtendo temperatura em graus Celsius e umidade relativa do ar.

### 2.3. RF03 – Monitoramento do Nível do Reservatório
O sistema deve monitorar o estado do reservatório por meio de um **sensor de boia de nível**, indicando se há água suficiente para irrigação.

### 2.4. RF04 – Irrigação Automática
O sistema deve acionar a bomba de água automaticamente quando a umidade do solo estiver abaixo de um threshold configurável e o reservatório estiver em nível seguro.

### 2.5. RF05 – Controle Manual de Irrigação
O sistema deve permitir o acionamento manual da bomba por meio de comandos via **bot no Telegram** e/ou botões na interface web.

### 2.6. RF06 – Alternância de Modos de Operação
O sistema deve permitir alternar entre modo **AUTOMÁTICO** e **MANUAL**, mantendo registro do modo atual.

### 2.7. RF07 – Proteção contra Reservatório Vazio
O sistema deve impedir o acionamento da bomba (automático ou manual) quando o sensor de nível indicar reservatório vazio ou abaixo do mínimo.

### 2.8. RF08 – Notificações via Telegram
O sistema deve enviar notificações via bot no Telegram em eventos relevantes, como:
- solo seco detectado;
- início e fim de irrigação;
- reservatório em nível baixo;
- erro de leitura de sensor.

### 2.9. RF09 – Comando de Status via Telegram
O sistema deve responder ao comando `/status` no Telegram com um resumo dos principais parâmetros (umidade do solo, temperatura, umidade do ar, modo, estado da bomba e nível do reservatório).

### 2.10. RF10 – Dashboard Web
O sistema deve disponibilizar uma interface web embarcada, acessível em rede local, mostrando valores atuais de sensores, estado do sistema e gráfico de histórico recente.

### 2.11. RF11 – Registro de Eventos de Irrigação
O sistema deve registrar em memória (histórico) os eventos de irrigação (tipo AUTO/MANUAL, duração, umidade antes/depois), para consulta posterior na interface web.

### 2.12. RF12 – Configuração de Thresholds
O usuário deve poder configurar, através de interface (Telegram ou web), o valor do threshold de umidade do solo utilizado na irrigação automática.

---

## 3. Requisitos Não Funcionais (RNF)

### 3.1. RNF01 – Dimensões do Protótipo
O conjunto (reservatório pequeno + vaso + estrutura eletrônica) deve possuir dimensões adequadas à bancada/laboratório, com footprint máximo aproximado de 50 × 50 cm.

### 3.2. RNF02 – Autonomia de Operação
O sistema deve operar continuamente por, no mínimo, 1 hora em condições normais de uso (ciclos de leitura e alguns ciclos de irrigação).

### 3.3. RNF03 – Capacidade de Bombeamento
A bomba deve ser capaz de fornecer vazão suficiente para irrigar a zona de interesse em poucos minutos, mantendo uma coluna d’água adequada às especificações típicas de bombas 3–5 V usadas em projetos com ESP32.  

### 3.4. RNF04 – Estabilidade das Leituras de Solo
As leituras do sensor de umidade devem apresentar estabilidade adequada após filtragem (por exemplo, sem variações bruscas não justificadas), garantindo decisões confiáveis de irrigação.  

### 3.5. RNF05 – Plataforma de Processamento
O sistema deve utilizar um microcontrolador **ESP32 DevKit** como plataforma principal.

### 3.6. RNF06 – Alimentação Elétrica
O sistema deve utilizar fonte/bateria com tensão e corrente compatíveis com ESP32 e bomba, além de reguladores adequados para evitar queda excessiva de tensão.

### 3.7. RNF07 – Latência de Comandos via Telegram
O tempo entre o envio de um comando de irrigação (por exemplo, `/regar_20s`) e o acionamento da bomba não deve exceder 2 segundos em condições de Wi‑Fi estável.

### 3.8. RNF08 – Estabilidade da Conexão Web
A interface web deve se manter acessível enquanto o ESP32 estiver ligado e conectado ao Wi‑Fi, com tempo máximo de resposta de 1 segundo para consultas de status.

### 3.9. RNF09 – Segurança Operacional
O sistema deve interromper automaticamente a irrigação em caso de falha crítica detectada (por exemplo, leitura impossível dos sensores de solo ou nível).

### 3.10. RNF10 – Proteção contra Água na Eletrônica
A caixa eletrônica deve ser projetada de forma a minimizar risco de respingos diretos de água em placas, mantendo pelo menos proteção equivalente a uso em ambiente interno.

### 3.11. RNF11 – Recuperação de Falhas
Após falhas não críticas (por exemplo, perda temporária de Wi‑Fi), o sistema deve retomar seu funcionamento normal sem necessidade de intervenção física.

### 3.12. RNF12 – Registro Mínimo de Histórico
O sistema deve ser capaz de manter em memória pelo menos as últimas 100 amostras de sensores e 20 eventos de irrigação para exibição no dashboard.

### 3.13. RNF13 – Modularidade de Código
O firmware deve ser estruturado em módulos (leitura de sensores, controle de bomba, Telegram, Web) permitindo manutenção e evolução.

---

## 4. Matriz de Classificação dos Requisitos

### 4.1. Classificação dos Requisitos Funcionais

| Código | Requisito                      | Classificação  | Justificativa                                                        |
| ------ | ------------------------------ | -------------- | -------------------------------------------------------------------- |
| RF01   | Leitura de umidade do solo    | **Essencial**  | Base da lógica de irrigação automática                              |
| RF02   | Leitura de temperatura/umidade| **Importante** | Melhora o monitoramento ambiental, mas não impede irrigação         |
| RF03   | Monitoramento do reservatório | **Essencial**  | Evita funcionamento da bomba sem água                               |
| RF04   | Irrigação automática          | **Essencial**  | Principal funcionalidade inteligente do sistema                     |
| RF05   | Controle manual               | **Importante** | Necessário para testes e override humano                            |
| RF06   | Alternância de modos          | **Essencial**  | Garante flexibilidade entre operação automática e manual            |
| RF07   | Proteção contra reservatório vazio | **Essencial** | Protege bomba e evita danos                                         |
| RF08   | Notificações via Telegram     | **Importante** | Melhora observabilidade do sistema                                  |
| RF09   | Comando de status             | **Importante** | Facilita monitoramento remoto                                       |
| RF10   | Dashboard web                 | **Importante** | Atende requisito de telemetria e visualização                       |
| RF11   | Registro de eventos de irrigação | **Importante** | Permite análise de comportamento e validação                        |
| RF12   | Configuração de thresholds    | **Essencial**  | Necessário para ajustar irrigação às condições reais                |

### 4.2. Classificação dos Requisitos Não Funcionais

| Código | Requisito                             | Classificação  | Justificativa                                                |
| ------ | ------------------------------------- | -------------- | ------------------------------------------------------------ |
| RNF01  | Dimensões do protótipo               | **Importante** | Facilita uso em bancada e armazenamento                     |
| RNF02  | Autonomia de operação                | **Essencial**  | Necessária para monitoramento e irrigação ao longo do tempo |
| RNF03  | Capacidade de bombeamento            | **Essencial**  | Garante irrigação eficaz                                     |
| RNF04  | Estabilidade das leituras de solo    | **Essencial**  | Evita decisões erradas de irrigação                         |
| RNF05  | Plataforma ESP32                     | **Importante** | Define arquitetura principal, mas poderiam existir variantes |
| RNF06  | Alimentação elétrica                 | **Essencial**  | Garante funcionamento estável                                |
| RNF07  | Latência de comandos via Telegram    | **Importante** | Impacta experiência de controle remoto                       |
| RNF08  | Estabilidade da conexão web          | **Importante** | Impacta visualização de telemetria                           |
| RNF09  | Segurança operacional                 | **Essencial**  | Protege sistema e ambiente                                   |
| RNF10  | Proteção contra água na eletrônica   | **Essencial**  | Previne falhas por umidade                                   |
| RNF11  | Recuperação de falhas                | **Importante** | Aumenta robustez                                             |
| RNF12  | Registro mínimo de histórico         | **Importante** | Necessário para análise básica de telemetria                 |
| RNF13  | Modularidade de código               | **Desejável**  | Facilita manutenção, mas não impede operação                 |

---

## 5. Histórico de Versionamento

| Versão | Data       | Autor(es)                             | Descrição das Alterações                        |
| ------ | ---------- | ------------------------------------- | ----------------------------------------------- |
| 0.1    | 30/06/2026 | Gabriel Santos Monteiro               | Criação inicial do documento de requisitos      |
| 0.2    | 30/06/2026 | Gabriel Santos Monteiro               | Adição dos requisitos funcionais                |
| 0.3    | 30/06/2026 | Gabriel Santos Monteiro               | Adição dos requisitos não funcionais e matrizes |