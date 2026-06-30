# 5. Comparativo com Produtos Similares

Esta seção compara o produto proposto (protótipo baseado em ESP32, descrito na [Seção 1](./descricao-produto.md) e [Seção 3](./reproducao-esp32.md)) com cinco controladores de irrigação inteligente comerciais consolidados no mercado.

## 5.1 Tabela comparativa

| Característica | **Nosso Protótipo (ESP32)** | **Rain Bird ST8I-2.0** | **Orbit B-hyve** | **Hunter Hydrawise** | **Xiaomi Mi Smart Garden** | **Rachio 3** |
|---|---|---|---|---|---|---|
| **Zonas suportadas** | 1 zona | Até 8 zonas | 4 a 16 zonas (conforme modelo indoor/outdoor) | 4 a 32 zonas (conforme modelo, ex. Pro-HC até 24) | Múltiplos gotejadores em 1 circuito (até ~16 vasos) | 4, 8 ou 16 zonas |
| **Conectividade** | Wi-Fi | Wi-Fi (módulo LNK2 em alguns modelos) | Wi-Fi + Bluetooth | Wi-Fi (módulo dedicado em alguns modelos) | Wi-Fi | Wi-Fi dual-band (2,4/5 GHz) |
| **Sensor de umidade do solo** | Sim, sensor capacitivo dedicado | Não (apenas dados climáticos) | Estimativa indireta via dados climáticos; aceita sensor externo na entrada de sensor | Não (ajuste por dados climáticos/Predictive Watering) | Não (timer de gotejamento fixo) | Não (ajuste por dados climáticos/Weather Intelligence) |
| **Sensor de temperatura/umidade do ar** | Sim (DHT11/DHT22) | Não | Não | Não | Não | Não |
| **Sensor de nível de reservatório** | Sim (boia de nível) | Não aplicável (ligado à rede hidráulica) | Não aplicável | Não aplicável | Não aplicável (depende de garrafa/reservatório manual) | Não aplicável |
| **Controle automático por threshold local** | Sim, lógica embarcada no ESP32 | Não (ajuste sazonal/clima via nuvem) | Parcial (WeatherSense, baseado em clima) | Sim, Predictive Watering baseado em previsão do tempo | Não (agendamento fixo) | Sim, Weather Intelligence Plus (skip por chuva/vento/geada) |
| **Interface de controle** | Bot no Telegram + dashboard web hospedado no próprio ESP32 | App mobile (iOS/Android) + Alexa | App mobile + Alexa/Google Assistant | App mobile + web + tela touchscreen (modelos Pro-HC) | App Xiaomi Home / Mi Home | App mobile + Alexa |
| **Histórico de eventos/telemetria** | Sim, via API JSON (`/api/history`) | Relatórios de rega no app | Limitado | Sim, monitoramento de uso de água | Não | Sim, relatórios de uso de água |
| **Custo aproximado (USD)** | Baixo (~US$ 25–40 em componentes) | ~US$ 175 (8 zonas) | ~US$ 80–150 (conforme modelo) | ~US$ 130–300+ (conforme modelo/zonas) | ~US$ 20–40 | US$ 190 (8 zonas) / US$ 240 (16 zonas) |
| **Foco de aplicação** | Vaso/canteiro único, doméstico, educacional | Gramados e jardins residenciais/comerciais | Gramados residenciais | Gramados residenciais/comerciais (paisagismo profissional) | Plantas em vaso/varanda | Gramados residenciais |
| **Código aberto / customizável** | Sim (projeto próprio, totalmente customizável) | Não | Não | Não | Não | Não |

## 5.2 Principais diferenciais do protótipo

- **Sensoriamento direto do solo e do ambiente**: diferente da maioria dos concorrentes comerciais (Rain Bird, Hydrawise, Rachio), que inferem a necessidade de água a partir de dados climáticos externos, o protótipo mede diretamente a umidade do solo, temperatura e umidade do ar na própria planta, possibilitando decisões mais precisas para aplicações pontuais (vaso/canteiro).
- **Trava de segurança por nível de reservatório**: funcionalidade pouco comum nos produtos comparados, que em geral assumem conexão direta à rede hidráulica e não a um reservatório local.
- **Baixo custo**: o protótipo utiliza componentes de poucas dezenas de dólares, frente a controladores comerciais que custam de US$ 80 a mais de US$ 250.
- **Abertura e customização**: por ser um projeto próprio em ESP32, toda a lógica de controle, limiares e integrações (Telegram, dashboard) podem ser livremente modificadas, ao contrário das soluções comerciais fechadas.

## 5.3 Principais limitações frente aos concorrentes

- **Escalabilidade**: o protótipo atende a apenas uma zona de irrigação, enquanto os produtos comerciais suportam de 4 a 32 zonas.
- **Inteligência climática**: não há integração com previsão do tempo (diferente de Hydrawise e Rachio), o que pode levar a irrigações desnecessárias em dias de chuva, salvo extensão futura do projeto.
- **Robustez e suporte**: produtos comerciais oferecem garantia, suporte técnico, certificações e maior tolerância a falhas de campo, o que não se aplica a um protótipo educacional.

---

| Versão | Autor | Data |
| --- | --- | --- |
| 0.1 | [Wallyson](https://github.com/devwallyson) | 30/06/2026 |
