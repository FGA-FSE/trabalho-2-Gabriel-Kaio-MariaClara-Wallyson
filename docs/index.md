<div class="hero">
  <div class="hero-content">
    <div class="hero-text">
      <p class="hero-tag">Fundamentos de Sistemas Embarcados • UnB Gama • Trabalho 2 (2026/1)</p>
      <h1>Controlador de Irrigação Inteligente com ESP32</h1>
      <p class="hero-description">
        Estudo teórico e proposta de reprodução de um controlador de irrigação inteligente usando ESP32,
        sensores de umidade do solo, temperatura/umidade do ar e nível de água, com integração a bot no Telegram
        e dashboard web de telemetria.
      </p>

      <div class="hero-actions">
        <a class="md-button md-button--primary" href="descricao-produto">Ver descrição do produto</a>
        <a class="md-button" href="reproducao-esp32">Ver proposta com ESP32</a>
      </div>
    </div>

    <div class="hero-image">
      <img src="/img/logounb.png" alt="Logo UnB Gama" width="220"/>
    </div>
  </div>
</div>

---

## Visão Geral

O sistema proposto automatiza a irrigação de uma zona (vaso, jardineira ou pequeno canteiro) com base na
umidade do solo, monitorando também o microclima e o nível do reservatório de água.  
O ESP32 atua como unidade de controle, acionando uma mini bomba DC e disponibilizando os dados em tempo real via Telegram e interface web.

<div class="grid cards" markdown>

- **Automação de irrigação**  
  Acionamento automático da bomba quando o solo está abaixo de um limite configurável.

- **Monitoramento ambiental**  
  Leitura contínua da umidade do solo e da temperatura/umidade do ar.

- **Segurança do reservatório**  
  Sensor de nível com boia impede que a bomba funcione com o reservatório vazio.

- **Conectividade**  
  ESP32 conectado ao Wi‑Fi, integrando bot no Telegram e dashboard web hospedado no próprio microcontrolador.

</div>

---

## Marcos do Trabalho

| Marco | Entrega | Data Limite | Status |
|:--|:--|:--|:--|
| **T2-Planejamento** | Escolha do produto e definição da arquitetura | **11/06/2026** | Finalizado |
| **T2-Relatório** | Descrição, análise técnica e artigos | **01/07/2026** | Finalizado |
| **T2-Protótipo** | Implementação com ESP32 (Entrega 03) | **11/07/2026** | Pendente |

---

## Destaques da Solução

<div class="grid cards" markdown>

- **Sensores simples e acessíveis**  
  Sensor capacitivo de umidade do solo, DHT11/DHT22 e boia de nível em PP, todos compatíveis com ESP32.

- **Atuador de baixo custo**  
  Mini bomba submersível 3–5 V controlada via relé/MOSFET, ideal para protótipos acadêmicos e irrigação de pequenos volumes.

- **Bot no Telegram**  
  Alertas de solo seco, fim de irrigação e reservatório baixo, além de comandos como `/status` e `/regar_20s` para controle manual.

- **Dashboard Web de Telemetria**  
  Interface web com gráficos de umidade do solo e temperatura/umidade do ar, log de eventos de irrigação e visualização do estado atual do sistema.

</div>

---

## Acesso Rápido

<div class="grid cards" markdown>

- **Documentação do Trabalho**

  - [Descrição do produto](descricao-produto.md)
  - [Funcionamento técnico](funcionamento.md)
  - [Proposta de reprodução com ESP32](reproducao-esp32.md)
  - [Requisitos funcionais e não funcionais](requisitos.md)
  - [Estrutura Analítica do Projeto (EAP)](eap.md)
  - [Normas técnicas aplicadas](normas.md)
  - [Análise de riscos](analiseriscos.md)
  - [Comparativo com produtos similares](comparativo.md)
  - [Pesquisa bibliográfica](pesquisa-bibliografica.md)
  - [Protótipo de site de telemetria](prototipo-site.md)

- **Arquivos técnicos (Entrega 03)**

  - Diagrama conceitual do sistema (sensores → ESP32 → Telegram/Web)
  - Esquemático de ligação da bomba e sensores (a definir na implementação)
  - Código-fonte ESP32 (firmware de controle e web server)
  - Scripts de integração com Telegram (API bot)

</div>

---

## Equipe

<div style="display: flex; flex-wrap: wrap; justify-content: center; gap: 15px;">

  <div class="photo-card leader">
    <a href="https://github.com/GabrielSMonteiro" target="_blank">
      <img src="https://avatars.githubusercontent.com/u/100171830?v=4" />
      <h4>Gabriel Monteiro<br/><small>Responsável Software / Web</small></h4>
    </a>
  </div>

  <div class="photo-card">
    <a href="https://github.com/bigkaio" target="_blank">
      <img src="https://avatars.githubusercontent.com/u/121952063?v=4" />
      <h4>Kaio<br/><small>Pesquisa Bibliográfica</small></h4>
    </a>
  </div>

  <div class="photo-card">
    <a href="https://github.com/alvezclari" target="_blank">
      <img src="https://avatars.githubusercontent.com/u/145598542?v=4" />
      <h4>Maria Clara<br/><small>Descrição / Comparativo</small></h4>
    </a>
  </div>

  <div class="photo-card">
    <a href="https://github.com/devwallyson" target="_blank">
      <img src="https://avatars.githubusercontent.com/u/120031974?v=4" />
      <h4>Wallyson<br/><small>Análise Técnica</small></h4>
    </a>
  </div>

</div>



---

## Tecnologias Utilizadas

<div class="badges">
  <img src="https://img.shields.io/badge/Hardware-ESP32-blue?style=for-the-badge" alt="ESP32" />
  <img src="https://img.shields.io/badge/Sensores-Umidade%20Solo%20%7C%20DHT11%20%7C%20Boia-orange?style=for-the-badge" alt="Sensores" />
  <img src="https://img.shields.io/badge/Comunicação-Wi--Fi%20%7C%20Telegram%20Bot%20%7C%20HTTP-lightgrey?style=for-the-badge" alt="Comunicação" />
  <img src="https://img.shields.io/badge/Docs-Markdown%20%7C%20Docusaurus-purple?style=for-the-badge" alt="Docs" />
</div>