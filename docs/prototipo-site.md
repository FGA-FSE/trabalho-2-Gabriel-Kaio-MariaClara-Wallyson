# 6. Interface de Controle

## Visão Geral

A interface de controle do **SmartFlora** é uma aplicação web servida pelo próprio ESP32 do sistema de irrigação. O microcontrolador hospeda um servidor HTTP na rede Wi‑Fi local; o usuário acessa o painel pelo navegador (computador ou celular) digitando o IP do dispositivo. O dashboard é entregue diretamente a partir da memória do microcontrolador.

Não há dependência de servidor externo ou banco de dados para o funcionamento básico — os arquivos da interface (`index.html`, `style.css`, `app.js`) ficam armazenados na memória do ESP32 e são entregues diretamente ao cliente. Opcionalmente, o sistema pode enviar dados para outros serviços (por exemplo, bot do Telegram), mas a visualização local é sempre garantida.

A interface foi projetada para resolver quatro necessidades principais:

1. **Monitoramento em tempo real**  
   Visualizar umidade do solo, temperatura, umidade do ar, status da bomba e nível do reservatório em um único dashboard.

2. **Controle de irrigação**  
   Alternar entre modo **Automático** e **Manual**, acionar a bomba por janelas de tempo configuradas e ajustar o gatilho de umidade mínima.

3. **Histórico e diagnósticos**  
   Acompanhar histórico de umidade, condições climáticas e registros de ciclos de irrigação (quando irrigou, por quanto tempo, e com qual resultado).

4. **Configuração e manutenção**  
   Ajustar parâmetros de rede, integração com Telegram e calibração do sensor de umidade, além de reiniciar o ESP32 ou restaurar configurações de fábrica.

---

## Prévias da Interface

### Protótipo no Figma

O design da interface foi desenvolvido em Figma, com foco em um layout escuro, organizado em **sidebar à esquerda** (navegação entre Dashboard e Configurações) e **conteúdo principal à direita**, como ilustrado nas imagens abaixo:

- Tela "Dashboard – Automático":  
  ![Dashboard Automático](./images/Página%20inicial%20-%20Automático.png)

- Tela "Dashboard – Manual":  
  ![Dashboard Manual](./images/Página%20inicial%20-%20Manual.png)

- Tela "Configurações":  
  ![Configurações](./images/Configurações.png)

A navegação entre os modos **Automático** e **Manual** é feita por um seletor no canto superior direito do dashboard, enquanto a mudança de seção (Dashboard / Configurações) é feita pela barra lateral.

O protótipo navegável pode ser acessado pelo seguinte link:

<iframe style={{border: "1px solid rgba(0, 0, 0, 0.1)", display: "block", margin: "0 auto"}} width="800" height="450" src="https://www.figma.com/embed?embed_host=share&url=https%3A%2F%2Fwww.figma.com%2Fproto%2F2k8TpvARyq0MmV2ibipGOG%2FEmbarcados%3Fpage-id%3D0%253A1%26node-id%3D3-351%26p%3Df%26viewport%3D456%252C468%252C0.37%26t%3DYSa6C8cXv9V4IlLl-1%26scaling%3Dmin-zoom%26content-scaling%3Dfixed%26starting-point-node-id%3D3%253A34" allowfullscreen></iframe>

No contexto do sistema de irrigação, esse protótipo representa:

- A página inicial com **cards de status** (umidade do solo, temperatura, umidade do ar, reservatório, bomba).
- Controles de irrigação (modo automático/manual, botões de tempo, slider de threshold de umidade).
- Gráficos de histórico de umidade e condições climáticas.
- Tela de configuração de rede, Telegram, calibração do sensor e gerenciamento do sistema.

---

## Guia de Estilo

  ![Style Guide](./images/StyleGuide.png)

Para garantir a consistência visual e facilitar a posterior implementação do código CSS, o projeto adota um **Design System** simplificado, conforme ilustrado no Guia de Estilo acima. Os principais fundamentos definidos são:

- **Paleta de Cores:** Focada em um tema escuro (*Dark Mode*) moderno. Utiliza tons de cinza escuro para fundos e painéis (reduzindo o cansaço visual), e cores semânticas vibrantes para destaques: verde (sucesso, conectado, ativo), vermelho (alertas, desconectado, ação de parar) e azul (ações primárias).
- **Tipografia:** Uso de uma fonte sem serifa limpa e legível (como *Inter* ou *Roboto*), estabelecendo uma hierarquia clara entre títulos (cabeçalhos dos cards), valores de destaque (ex: a porcentagem de umidade em tamanho ampliado) e textos auxiliares.
- **Componentes Visuais:** Padronização de botões (com estados de *hover* e ativo), sliders, e cards com bordas suavemente arredondadas e sombras discretas para agrupar as informações sem poluir a interface.
- **Iconografia:** Ícones minimalistas para representar métricas (gotas para umidade, termômetros para temperatura) e ações, permitindo o rápido escaneamento visual dos dados do dashboard.

Essa padronização não só entrega uma experiência premium ao usuário, como também permite traduzir as decisões de design diretamente para variáveis globais no arquivo `style.css`.

---

## Arquitetura da Aplicação Web

A interface segue o modelo *single‑page application* (SPA): uma única página HTML é carregada e toda a interação é feita via JavaScript, sem recarregar o navegador.

Os arquivos principais são:

- **`index.html`**  
  Estrutura base da página: sidebar (logo do SmartFlora, itens "Dashboard" e "Configurações"), cabeçalho com título "Visão Geral" e seletor de modo, cards de métricas, área de gráficos e tabela de atividades, além da página de configurações com os painéis de rede, Telegram, calibração e gerenciamento.

- **`style.css`**  
  Folha de estilos responsável pelo tema escuro, tipografia, espaçamento e responsividade. Usa variáveis CSS para cores (primária, secundária, estados de alerta), bordas arredondadas e sombras discretas para destacar os cards.

- **`app.js`**  
  Implementa a lógica da aplicação:
  - gerenciamento de estado (modo automático/manual, valores de sensores, status da bomba, configurações);
  - comunicação com o firmware do ESP32 via HTTP (endpoints como `/api/status`, `/api/history`, `/api/control`, `/api/config`);
  - atualização periódica dos cards e gráficos;
  - manipulação de eventos da interface (clique nos botões de controle manual, alteração do slider de threshold, ações de reboot/factory reset).

O estado é mantido em um objeto central (por exemplo `state`), contendo:

- leituras atuais de sensores (umidade do solo, temperatura, umidade do ar);
- estado lógico (`modo`, `bombaLigada`, `reservatorioOk`);
- histórico de leituras e eventos de irrigação;
- informações de rede e Telegram (RSSI, IP, status do bot, frequência de polling).

Mudanças no estado disparam funções de renderização que atualizam os cards, gráficos e indicadores de conexão.

---

## Estrutura Visual – Dashboard

O **Dashboard** é a página principal de operação e está dividido em blocos:

### 1. Barra lateral (Sidebar)

- **Logo e nome do sistema:**  
  Exibe "SmartFlora – Sistema de Irrigação".
- **Menu:**  
  - *Dashboard* (selecionado por padrão).  
  - *Configurações*.
- **Indicador de conexão:**  
  No canto inferior esquerdo há um status (ex.: ponto vermelho com texto "Desconectado" quando não há comunicação com o ESP32).

### 2. Cabeçalho do Dashboard

- Texto "Visão Geral".
- Seletor de modo no canto superior direito:
  - **Automático**
  - **Manual**

Esse seletor muda o comportamento do card de controle da bomba e do bloco de "Gatilho de Irrigação".

### 3. Cards de Métricas

Na parte superior, são exibidos cards individuais para:

- **Umidade do Solo**  
  - Mostra o valor atual em "%".  
  - Barra horizontal indica visualmente quão próximo está do threshold de irrigação.

- **Temperatura**  
  - Mostra a temperatura ambiente em "°C".

- **Umidade do Ar**  
  - Mostra a umidade relativa em "%".

- **Reservatório**  
  - Estado do nível de água (por exemplo, "OK" ou "Baixo"), baseado no sensor de boia.

- **Bomba**  
  - Estado atual "Ligada"/"Desligada".  
  - No modo manual, muda em tempo real quando a bomba é acionada pelos botões de controle.

### 4. Controle de Irrigação

- **Modo Manual**  
  Quando o seletor está em "Manual", o card de "Controle Manual" fica ativo, com botões:
  - `10s` – ligar a bomba por 10 segundos.
  - `20s` – ligar a bomba por 20 segundos.
  - `30s` – ligar a bomba por 30 segundos.
  - `Parar` – interromper imediatamente a bomba.

  Esse card é desabilitado no modo automático (mostrando indicações como "Requer modo Manual").

- **Modo Automático**  
  Quando o seletor está em "Automático", o card principal passa a ser:
  - **Gatilho de Irrigação (Auto)** – slider que define o threshold de umidade do solo (ex.: 30%).  
  - Botão **Aplicar** – envia o novo valor para o ESP32, atualizando a configuração usada na lógica de irrigação.

### 5. Gráficos e Atividade

- **Histórico de Umidade**  
  Gráfico de linha mostrando a evolução da umidade do solo ao longo do tempo. Pode marcar visualmente momentos em que a bomba esteve ligada (por exemplo linha/barras em outra cor).

- **Condições Climáticas**  
  Gráfico com evolução da temperatura e da umidade do ar.

- **Atividade Recente**  
  Tabela com última irrigação automática/manuais:
  - horário;
  - tipo (Automático/Manual);
  - duração;
  - resultado (ex.: "22% → 34%").

Inicialmente, a tela pode mostrar "Sem dados" até que medições e eventos sejam registrados.

---

## Estrutura Visual – Configurações

A página de **Configurações** é estruturada em cards, conforme o protótipo:

### 1. Rede e Conectividade

Card contendo:

- **Rede Wi‑Fi:** nome da rede (SSID) em que o ESP32 está conectado.
- **Endereço IP:** IP atual do ESP32 (usado para acessar o painel).
- **Força do sinal (RSSI):** ex.: `–65 dBm (Bom)`.
- **Servidor Web:** indica se o servidor HTTP está ativo (por exemplo, "Ativo na Porta 80").

### 2. Integração Telegram

Card com informações sobre a integração com o bot:

- **Status do Bot:** `Online (Polling)` / `Offline`.
- **Frequência de Checagem:** intervalo entre polls ao Telegram (ex.: "A cada 2000 ms").
- **Chat Autorizado:** se o sistema está bloqueado para um ID específico ou aberto.
- **Notificações:** ativadas/desativadas.

Essas configurações podem ser parcialmente editáveis (por exemplo, alterar intervalo de checagem) ou apenas exibidas, dependendo da implementação.

### 3. Calibração do Sensor Capacitivo (ADC)

Card dedicado à calibração do sensor de umidade do solo:

- **No Ar (0%)** – valor raw de ADC medido com o sensor completamente seco (ex.: 2850).
- **Na Água (100%)** – valor raw com o sensor imerso em água (ex.: 1250).
- **Leitura Atual (Raw)** – valor raw atual (ex.: 2050).

Esses três valores permitem ao firmware mapear o intervalo de leitura para uma escala 0–100% de forma mais precisa.

### 4. Gerenciamento do Sistema

Card com ações administrativas:

- **Reiniciar ESP32** – botão (por exemplo, "Reboot") que envia comando para reiniciar o microcontrolador; a irrigação em andamento é cancelada.

- **Resetar Configurações (Factory Reset)** – botão que apaga preferências salvas (NVS) e restaura valores padrão (rede Wi‑Fi, thresholds, integrações), exigindo nova configuração após o reinício.

---

## Comunicação com o Firmware

A interface se comunica com o ESP32 por uma pequena API HTTP REST. Uma sugestão de endpoints:

- `GET /api/status`  
  Retorna valores atuais de:
  - umidade do solo (%),
  - temperatura (°C),
  - umidade do ar (%),
  - estado da bomba,
  - estado do reservatório,
  - modo atual (AUTO/MANUAL).

- `GET /api/history`  
  Retorna histórico de leituras e eventos de irrigação para alimentar os gráficos e a tabela de atividade.

- `POST /api/control`  
  Usado para:
  - iniciar irrigação manual por X segundos;
  - parar a bomba;
  - alterar o modo (AUTO/MANUAL);
  - atualizar o threshold de umidade (modo automático).

- `GET /api/config` / `POST /api/config`  
  Leitura e atualização de parâmetros de rede, Telegram e calibração do sensor.

- `POST /api/reboot`  
  Reinicia o ESP32.

- `POST /api/factory_reset`  
  Executa o reset de configurações de fábrica.

A implementação expõe um conjunto de endpoints JSON focados em garantir latência mínima para telemetria e controle em tempo real.

---

## Indicadores de Conexão

Assim como no protótipo, o painel exibe o estado de conexão do sistema em dois níveis:

- **Sidebar (rodapé):** indicador textual/visual ("Conectado"/"Desconectado").
- **Conteúdo dos cards:** quando desconectado, cards de métricas mostram `--` em vez de valores numéricos e os botões de controle exibem estados desabilitados (por exemplo, fade).

Quando a conexão é restabelecida, os valores voltam a ser preenchidos a partir da telemetria do ESP32.

---

## Histórico de Versões

<font size="3"><p style={{textAlign: "left"}}>**Tabela 1** - Histórico de versões.</p></font>

| Versão | Descrição | Autor(es) | Data |
| :----: | :-------: | :-------: | :--: |
|  1.0   | Criação do protótipo do site | [Gabriel Santos Monteiro](https://github.com/GabrielSMonteiro) | 25/06/2026 |