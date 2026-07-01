# Levantamento de Normas Técnicas

## Introdução

O desenvolvimento de um sistema de irrigação inteligente baseado em ESP32, com sensores de umidade de solo, temperatura/umidade do ar e nível de água, exige a convergência de conhecimentos em Eletrônica, Energia, Mecânica/Hidráulica e Engenharia de Software.  

Nesse cenário, a observância de normas técnicas nacionais e internacionais é fundamental para garantir **segurança elétrica**, **proteção mecânica**, **qualidade do software embarcado** e **confiabilidade** do protótipo acadêmico, aproximando o projeto dos requisitos da indústria.

Este documento cataloga as principais diretrizes normativas aplicáveis ao controlador de irrigação, abrangendo desde instalações elétricas de baixa tensão até qualidade de software e documentação técnica.

## Metodologia

A seleção das normas foi feita a partir de uma análise sistêmica do sistema proposto, segmentada nos seguintes domínios:

1. **Sistemas de Potência e Eletrônica:** requisitos de baixa tensão e proteção de circuitos;
2. **Segurança Ocupacional e de Máquinas:** mitigação de riscos envolvendo bomba, reservatório e partes móveis;
3. **Engenharia de Software:** qualidade do firmware e processos de ciclo de vida;
4. **Documentação Estrutural e Hidráulica:** padronização de desenhos e montagem;
5. **Sistemas Ciberfísicos em IoT:** diretrizes gerais para dispositivos conectados.

## Aplicação no Projeto

A aplicação prática deste levantamento se reflete em:

- **Segurança Operacional:** redução de riscos de curto‑circuito, aquecimento excessivo da bomba/driver e vazamento de água em componentes eletrônicos;
- **Confiabilidade do Firmware:** utilização de boas práticas e métricas de qualidade para o código de controle e telemetria;
- **Manutenibilidade:** padronização da documentação e do layout físico para facilitar reparos e melhorias futuras;
- **Conformidade:** aproximação do protótipo aos padrões de produtos comerciais de irrigação inteligente.  

---

# Normas Técnicas Aplicadas

### ABNT NBR ISO 9001:2015 – Sistema de Gestão da Qualidade
Apesar de voltada a processos organizacionais, seus princípios de **foco no cliente** (no caso, requisitos da disciplina e dos usuários do sistema) e **melhoria contínua** orientam o planejamento das atividades, a rastreabilidade das modificações e as revisões do documento e do firmware.

### ABNT NBR 5410:2005 – Instalações Elétricas de Baixa Tensão
Aplica‑se ao dimensionamento de condutores, proteção da alimentação 5 V e organização do barramento de energia do sistema, incluindo a separação entre circuitos da bomba (potência) e circuitos de sinal (ESP32 e sensores).

### NR 10 – Segurança em Instalações e Serviços em Eletricidade
Norma regulamentadora essencial para atividades de bancada: montagem, testes de alimentação, medições elétricas e integração de fonte/bateria, garantindo procedimentos que minimizam risco de choque elétrico.

### ABNT NBR IEC 60204‑1 – Segurança de Máquinas: Equipamentos Elétricos
Relacionada aos requisitos de segurança em máquinas, utilizada aqui para definir métodos de seccionamento de energia do sistema, inclusão de chave geral e eventuais rotinas de parada segura em caso de falha.

### ABNT NBR ISO 12100 – Segurança de Máquinas: Princípios Gerais de Projeto
Orienta a identificação e redução de riscos mecânicos e físicos. No projeto, é aplicada para avaliar:
- pontos de esmagamento relacionados ao reservatório e suporte;
- riscos de queda do conjunto ou de objetos com o movimento da bomba e tubulação.

### ABNT NBR IEC 60529 – Graus de Proteção de Invólucros (Código IP)
Define o grau de proteção contra ingresso de poeira e umidade. É relevante para especificar a proteção da caixa eletrônica onde o ESP32 ficará, evitando respingos de água comuns em sistemas de irrigação.

### ABNT NBR ISO 31000 – Gestão de Riscos: Diretrizes
Estrutura a metodologia utilizada na análise de riscos do projeto, permitindo priorizar falhas críticas, como vazamento de água sobre eletrônica, superaquecimento de componentes e erros de irrigação.

### ABNT NBR ISO/IEC 25010 – Engenharia de Software (SQuaRE)
Define características de qualidade de software (funcionalidade, confiabilidade, eficiência, manutenibilidade, etc.). Aplicada ao firmware do ESP32 para avaliar desempenho da leitura de sensores, controle da bomba e telemetria.

### ISO/IEC/IEEE 12207 – Processos de Ciclo de Vida de Software
Padroniza processos de ciclo de vida, servindo de referência para organizar as etapas de análise de requisitos, design, implementação, testes e implantação do código no microcontrolador.

---

# Conclusão

A integração das normas técnicas ao projeto de irrigação inteligente com ESP32 vai além de uma exigência formal, funcionando como ferramental de engenharia para aumentar **segurança**, **confiabilidade** e **profissionalismo** do sistema.

Além disso, as normas levantadas se relacionam diretamente com a análise de riscos, servindo como base para definir estratégias de mitigação e contingência, especialmente nos aspectos de segurança elétrica (NR 10), segurança mecânica (ISO 12100) e qualidade de software (ISO/IEC 25010).

# Referências Bibliográficas

* ASSOCIAÇÃO BRASILEIRA DE NORMAS TÉCNICAS. NBR ISO 12100: Segurança de máquinas. Rio de Janeiro: ABNT.
* ASSOCIAÇÃO BRASILEIRA DE NORMAS TÉCNICAS. NBR 5410: Instalações elétricas de baixa tensão.
* INTERNATIONAL ORGANIZATION FOR STANDARDIZATION. ISO/IEC 25010: Systems and software quality models.

## Histórico de Versões

<font size="3"><p style={{textAlign: "left"}}>**Tabela 1** - Histórico de versões.</p></font>

| Versão | Descrição | Autor(es) | Data | Revisor | Data de revisão |
| :----: | :-------: | :-------: | :--: | :-----: | :-------------: |
|  1.0   | Criação do documento (normas aplicadas ao sistema de irrigação) | [Gabriel Santos Monteiro](https://github.com/GabrielSMonteiro) | 30/06/2026 |  |  |