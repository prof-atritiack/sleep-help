# Sleep Help - Sistema de Monitoramento de SpO2

![Sleep Help Logo](https://img.shields.io/badge/Sleep%20Help-Monitoramento%20de%20SpO2-blue)
![React](https://img.shields.io/badge/React-18.2.0-blue)
![Node.js](https://img.shields.io/badge/Node.js-18.x-green)
![ESP32](https://img.shields.io/badge/ESP32-DevKit%20V1-red)
![Docker](https://img.shields.io/badge/Docker-Containerized-blue)

Interface web completa para monitoramento de SpO2 (Saturação de Oxigênio) durante o sono, com integração ESP32 e deploy em Docker.

## 📋 Sobre o Projeto

**Sleep Help** é um sistema completo de monitoramento de saúde desenvolvido como **Trabalho de Conclusão de Curso (TCC)** do curso técnico em **Automação Industrial** da **ETEC Jorge Street**, localizada em **São Caetano do Sul - SP**.

O projeto integra hardware ESP32 com sensores biomédicos, uma API REST robusta e uma interface web moderna para monitoramento contínuo de parâmetros vitais durante o sono.

### 🎓 Contexto Acadêmico

Este projeto foi desenvolvido com o objetivo de aplicar os conhecimentos adquiridos durante o curso técnico em Automação Industrial, demonstrando a integração entre sistemas eletrônicos, programação e desenvolvimento web.

**Orientação e Desenvolvimento:**
- **Desenvolvedor**: Prof. André Tritiack (Old Timer IoT Labs)
- **Orientador**: Prof. André Tritiack - Orientação técnica e desenvolvimento do projeto
- **Organização**: Old Timer IoT Labs
- **Ferramenta de Desenvolvimento**: Este projeto foi desenvolvido com auxílio de **Inteligência Artificial** através da plataforma **Cursor AI**, uma IDE moderna que utiliza modelos avançados de IA (Claude Sonnet, GPT-4) para assistência em programação, refatoração de código e debugging.

### 🛠️ Ferramentas Utilizadas no Desenvolvimento

- **Cursor AI** - IDE com assistência de IA para desenvolvimento
  - Autocompletar inteligente
  - Refatoração automática de código
  - Debugging assistido
  - Suporte nativo para React, JavaScript, Node.js e Arduino
  - [Documentação Cursor AI](https://cursor.sh/docs)
- **Old Timer IoT Labs** - Organização de desenvolvimento e pesquisa em IoT

## 🎯 Objetivos do Projeto

Este projeto foi desenvolvido com o objetivo de criar um sistema completo de monitoramento de saúde que integra hardware e software, demonstrando competências técnicas em:

- ✅ **Monitoramento em Tempo Real**: Acompanhamento contínuo de SpO2 (Saturação de Oxigênio)
- ✅ **Interface Web Responsiva**: Dashboard moderno e acessível para visualização de dados
- ✅ **Integração Hardware-Software**: Comunicação entre ESP32 e aplicação web
- ✅ **API REST Completa**: Backend Node.js com WebSocket para comunicação em tempo real
- ✅ **Deploy Containerizado**: Docker para fácil instalação e portabilidade
- ✅ **Sistema de Alertas Inteligente**: Notificações automáticas para valores anormais
- ✅ **Integração com WhatsApp**: Alertas via mensagem quando valores críticos são detectados
- ✅ **Comunicação ESP-NOW**: Protocolo de comunicação peer-to-peer entre ESP32s

## 🚀 Funcionalidades

### 🌐 Interface Web
- **Dashboard Interativo**: Visualização de dados em tempo real
- **Histórico de Medições**: Consulta e análise de dados históricos
- **Sistema de Autenticação**: Controle de acesso seguro
- **Interface Responsiva**: Funciona em desktop, tablet e celular
- **Alertas Inteligentes**: Notificações para baixa saturação

### 🔌 Integração Hardware
- **Sensor MAX30105**: Medições precisas de SpO2
- **ESP32 DevKit V1**: Microcontrolador Wi-Fi/Bluetooth
- **Comunicação Dupla**: ESP-NOW + Wi-Fi simultaneamente
- **Display TFT**: Interface visual no dispositivo
- **Alertas Físicos**: LED e buzzer para notificações

### 🛠️ Backend API
- **API REST**: Endpoints para envio e consulta de dados
- **WebSocket**: Comunicação em tempo real
- **Validação de Dados**: Verificação automática de medições
- **Armazenamento**: Histórico de até 1000 medições
- **CORS**: Configurado para aceitar conexões externas
- **Integração WhatsApp**: Envio automático de alertas via CallMeBot API

## 🏗️ Arquitetura do Sistema

O sistema é composto por três componentes principais que trabalham em conjunto:

```
┌─────────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   ESP32 +           │    │   Backend API   │    │   Frontend      │
│   MAX30105          │───▶│   Node.js       │───▶│   React         │
│   (Transmissor)     │    │   + WebSocket   │    │   + Dashboard   │
└─────────────────────┘    └─────────────────┘    └─────────────────┘
         │                           │                       │
         │                           │                       │
         │                           │                       │
         ▼                           ▼                       ▼
┌─────────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   ESP32             │    │   Docker        │    │   Nginx         │
│   (Receptor)        │    │   Container     │    │   (Static)      │
│   ESP-NOW           │    │   + Compose     │    │   (Frontend)    │
└─────────────────────┘    └─────────────────┘    └─────────────────┘
```

### 📡 Fluxo de Dados

1. **Hardware (ESP32 Transmissor)**: Lê dados do sensor MAX30105 e envia via Wi-Fi para a API
2. **Comunicação Local (ESP-NOW)**: Transmissor envia alertas via ESP-NOW para o receptor
3. **Backend (Node.js)**: Recebe dados, armazena histórico e envia alertas via WhatsApp
4. **Frontend (React)**: Exibe dados em tempo real através de WebSocket
5. **Receptor (ESP32)**: Ativa buzzer e lâmpada quando recebe alertas via ESP-NOW

## 🛠️ Tecnologias Utilizadas

### Frontend
- **React 18** - Biblioteca para construção de interfaces
- **React Router DOM** - Roteamento da aplicação
- **Styled Components** - Estilização baseada em componentes
- **Socket.io Client** - Comunicação WebSocket
- **Create React App** - Ferramenta de build

### Backend
- **Node.js 18** - Runtime JavaScript
- **Express.js** - Framework web
- **Socket.io** - WebSocket para tempo real
- **CORS** - Cross-Origin Resource Sharing
- **Moment.js** - Manipulação de datas

### Hardware
- **ESP32 DevKit V1** - Microcontrolador Wi-Fi/Bluetooth
- **MAX30105** - Sensor de oxímetro de pulso (SpO2 e frequência cardíaca)
- **TFT Display** - Interface visual no dispositivo
- **Arduino IDE** - Ambiente de desenvolvimento do firmware
- **Componentes de Alerta**: Buzzer, LED, Lâmpada, Vibrador

### Deploy
- **Docker** - Containerização
- **Docker Compose** - Orquestração de containers
- **Nginx** - Servidor web para frontend
- **Alpine Linux** - Imagens otimizadas

### Deploy na Nuvem
- **Vercel + Railway** - Deploy gratuito e automático
- **Netlify + Render** - Alternativa gratuita
- **DigitalOcean** - VPS com Docker ($5/mês)
- **AWS** - Solução completa e escalável

## 📋 Pré-requisitos

### Para Desenvolvimento
- Node.js (versão 18 ou superior)
- npm ou yarn
- Git
- Navegador web moderno

### Para Hardware
- **2x ESP32 DevKit V1** (um para transmissor, um para receptor)
- **Sensor MAX30105** (para o transmissor)
- **Display TFT** (opcional, para visualização local)
- **Componentes de Alerta**:
  - Buzzer (pino 13 no receptor)
  - Lâmpada (pino 14 no receptor)
  - Vibrador (pino 4 no transmissor)
  - LEDs (pino 2 em ambos)
- **Arduino IDE** com suporte ESP32
- **Bibliotecas necessárias**:
  - MAX30105 (sensor de SpO2)
  - TFT_eSPI (display)
  - WiFi, HTTPClient, ArduinoJson (comunicação)
  - esp_now (comunicação peer-to-peer)

### Para Deploy
- Docker Desktop
- Docker Compose

## 🚀 Instalação e Configuração

> **📌 Para instalação rápida em nova máquina, consulte [SETUP.md](SETUP.md)**

### 1. Clone o Repositório

```bash
git clone https://github.com/prof-atritiack/sleep-help.git
cd sleep-help
```

### 2. Instalação Local (Desenvolvimento)

#### Frontend
```bash
# Instalar dependências
npm install

# Iniciar servidor de desenvolvimento
npm start
```

#### Backend
```bash
# Navegar para pasta do backend
cd backend

# Instalar dependências
npm install

# Iniciar servidor
node server.js
```

### 3. Deploy com Docker (Recomendado)

> **📌 Para setup rápido, consulte [SETUP.md](SETUP.md)**

#### Instalar Docker Desktop
1. Baixe e instale o Docker Desktop: https://www.docker.com/products/docker-desktop/
2. Inicie o Docker Desktop
3. Verifique se está rodando: `docker ps`

#### Executar Aplicação
```bash
# Construir e iniciar todos os serviços
docker-compose up --build -d

# Verificar status
docker-compose ps

# Ver logs
docker-compose logs -f
```

#### Configurar Variáveis de Ambiente (Opcional - WhatsApp)
```bash
# Copiar arquivo de exemplo
cd backend
cp .env.example .env

# Editar e preencher com suas credenciais
# WHATSAPP_PHONE=5511999999999
# WHATSAPP_API_KEY=sua_chave_aqui
# ALARM_THRESHOLD=95
```

### 4. Configuração do ESP32

> **📌 Para detalhes completos, consulte [CONFIGURACAO_ESP32.md](CONFIGURACAO_ESP32.md) e [ESP32_PROTOCOL.md](ESP32_PROTOCOL.md)**

#### Instalar Bibliotecas
1. Abra o Arduino IDE
2. Instale as bibliotecas necessárias:
   - MAX30105
   - TFT_eSPI
   - WiFi
   - HTTPClient
   - ArduinoJson
   - esp_now

#### Configurar Código
1. Abra os arquivos em: `codigos_ESP32_finais/`
   - **TRANSMISSOR_FINAL**: Código do transmissor (com sensor)
   - **RECEIVER_FINAL**: Código do receptor (com ESP-NOW)
2. Configure as credenciais Wi-Fi:
   ```cpp
   const char* ssid = "sua_rede_wifi";
   const char* password = "sua_senha";
   ```
3. Configure o IP do servidor:
   ```cpp
   const char* serverURL = "http://SEU_IP:3001/api/sensor-data";
   ```

#### Fazer Upload
1. Conecte o ESP32 ao computador
2. Selecione a placa: ESP32 Dev Module
3. Faça upload do código
4. Abra o Serial Monitor (115200 baud)

## 📱 URLs de Acesso

### Desenvolvimento Local
- **Frontend**: http://localhost:3000
- **Backend**: http://localhost:3001
- **API Status**: http://localhost:3001/api/status

### Docker
- **Frontend**: http://localhost:3000
- **Backend**: http://localhost:3001
- **API Status**: http://localhost:3001/api/status

### Rede Local
- **Frontend**: http://SEU_IP:3000
- **Backend**: http://SEU_IP:3001
- **API Status**: http://SEU_IP:3001/api/status

## ☁️ Deploy na Nuvem

### **Opção 1: Vercel + Railway (Recomendado)**
- ✅ **Gratuito** para projetos pessoais/TCC
- ✅ **Deploy automático** via Git
- ✅ **SSL automático** e domínio personalizado
- ✅ **Escalabilidade** para crescimento futuro

#### Deploy Rápido (15 minutos)
1. **Backend**: [Railway.app](https://railway.app) - Conecte GitHub → Deploy
2. **Frontend**: [Vercel.com](https://vercel.com) - Conecte GitHub → Deploy
3. **ESP32**: Atualize URL da API no código

### **Opção 2: Netlify + Render (Alternativa)**
- ✅ **Gratuito** com limitações
- ✅ **Fácil configuração**
- ✅ **Deploy automático**

### **Opção 3: DigitalOcean (VPS)**
- 💰 **$5/mês** - Droplet com Docker
- ✅ **Controle total** do servidor
- ✅ **Performance** dedicada

### **Opção 4: AWS (Produção)**
- 💰 **$20-50/mês** - Solução completa
- ✅ **Máxima escalabilidade**
- ✅ **Serviços gerenciados**

📚 **Documentação Adicional:**
- [Setup Rápido](SETUP.md) - Instalação em nova máquina
- [Configuração ESP32](CONFIGURACAO_ESP32.md) - Guia completo do hardware
- [Protocolo ESP32](ESP32_PROTOCOL.md) - Especificações de comunicação

## 🔌 API Endpoints

### Enviar Dados do Sensor
```http
POST /api/sensor-data
Content-Type: application/json

{
  "spo2": 98,
  "timestamp": "2025-09-27 12:30:45"
}
```

### Obter Dados Atuais
```http
GET /api/current-data
```

### Obter Dados Históricos
```http
GET /api/sensor-data?limit=100
```

### Status do Servidor
```http
GET /api/status
```

## 📊 Estrutura do Projeto

```
sleep-help/
├── src/                          # Frontend React
│   ├── components/               # Componentes reutilizáveis
│   │   ├── Header/              # Cabeçalho da aplicação
│   │   └── ...
│   ├── contexts/                # Contextos do React
│   │   └── AuthContext.js       # Contexto de autenticação
│   ├── pages/                   # Páginas da aplicação
│   │   ├── Dashboard/           # Dashboard principal
│   │   ├── History/             # Histórico de medições
│   │   ├── Login/               # Página de login
│   │   └── Contact/             # Página de contato
│   ├── App.js                   # Componente principal
│   └── index.js                 # Ponto de entrada
├── backend/                     # Backend Node.js
│   ├── server.js                # Servidor principal
│   ├── package.json             # Dependências do backend
│   └── Dockerfile               # Container do backend
├── codigos_ESP32_finais/        # Códigos finais para ESP32
│   ├── TRANSMISSOR_FINAL/       # Código do transmissor (com sensor)
│   └── RECEIVER_FINAL/          # Código do receptor (ESP-NOW)
├── espnow_funcionando/          # Códigos de referência ESP-NOW
├── public/                      # Arquivos estáticos
├── docker-compose.yml           # Configuração Docker
├── Dockerfile                   # Container do frontend
├── nginx.conf                   # Configuração Nginx
└── README.md                    # Este arquivo
```

## 🧪 Testando o Sistema

### 1. Teste da API
```bash
# Testar status da API
curl http://localhost:3001/api/status

# Enviar dados de teste
curl -X POST http://localhost:3001/api/sensor-data \
  -H "Content-Type: application/json" \
  -d '{"spo2":98,"timestamp":"2025-09-27 12:30:45"}'
```

### 2. Teste do Frontend
1. Acesse http://localhost:3000
2. Faça login com as credenciais padrão
3. Vá para o Dashboard
4. Verifique se os dados estão sendo exibidos

### 3. Teste do ESP32
1. Configure o ESP32 com o código fornecido
2. Conecte o sensor MAX30105
3. Coloque o dedo no sensor
4. Verifique os dados no Serial Monitor
5. Confirme se os dados chegam na aplicação web

## 🐳 Comandos Docker

### Gerenciamento de Containers
```bash
# Iniciar todos os serviços
docker-compose up -d

# Parar todos os serviços
docker-compose down

# Ver status dos containers
docker-compose ps

# Ver logs em tempo real
docker-compose logs -f

# Rebuild um serviço
docker-compose up --build backend
```

### Limpeza
```bash
# Remover containers parados
docker-compose rm

# Limpeza completa
docker system prune -a
```

## 🔧 Configurações Avançadas

### Variáveis de Ambiente
```bash
# Backend
NODE_ENV=production
PORT=3001

# WhatsApp (Opcional - para alertas)
# Para obter sua chave API do CallMeBot:
# 1. Adicione o número +34 644 97 54 14 aos seus contatos
# 2. Envie "Eu permito que o callmebot me envie mensagens" para esse contato
# 3. Você receberá uma chave de API pessoal
WHATSAPP_PHONE=5511999999999  # Formato: código do país + DDD + número
WHATSAPP_API_KEY=sua_chave_api_aqui
ALARM_THRESHOLD=95  # SpO2 <= 95% gera alarme

# Frontend
REACT_APP_API_URL=http://localhost:3001
```

### Personalização do ESP32
```cpp
// Intervalo de envio (ms)
#define INTERVALO_ENVIO_API 5000

// Sensibilidade do sensor
#define DEDO_THRESHOLD 30000

// Pinos do hardware
#define BUZZER_PIN 4
#define LED_PIN 2
```

## 🚨 Solução de Problemas

### Problemas Comuns

#### Docker não inicia
```bash
# Verificar se Docker Desktop está rodando
docker ps

# Reiniciar Docker Desktop
# Ou executar: docker-compose down && docker-compose up --build
```

#### ESP32 não conecta
- Verifique SSID e senha Wi-Fi
- Verifique sinal da rede
- Verifique o IP do servidor
- Reinicie o ESP32

#### API não responde
- Verifique se o backend está rodando
- Verifique logs: `docker-compose logs backend`
- Verifique se a porta 3001 está livre

#### Frontend não carrega
- Verifique se o frontend está rodando
- Verifique logs: `docker-compose logs frontend`
- Verifique se a porta 3000 está livre

### Logs Úteis
```bash
# Todos os serviços
docker-compose logs -f

# Apenas backend
docker-compose logs -f backend

# Apenas frontend
docker-compose logs -f frontend
```

## 📱 Configuração WhatsApp

O sistema possui integração com WhatsApp para envio de alertas quando valores críticos de SpO2 são detectados.

### Como Configurar

1. **Obter Chave da API CallMeBot**:
   - Adicione o número **+34 644 97 54 14** aos seus contatos do WhatsApp
   - Envie a mensagem: **"Eu permito que o callmebot me envie mensagens"**
   - Você receberá uma chave de API pessoal

2. **Configurar Variáveis de Ambiente**:
   ```bash
   # No arquivo .env ou nas variáveis de ambiente do sistema
   WHATSAPP_PHONE=5511999999999  # Seu número: código país + DDD + número
   WHATSAPP_API_KEY=sua_chave_api_recebida
   ALARM_THRESHOLD=95  # SpO2 <= 95% gera alarme (opcional, padrão: 95)
   ```

3. **Formato do Número**:
   - Exemplo Brasil: `5511999999999` (55 + 11 + 999999999)
   - Sem espaços, sem caracteres especiais, com código do país

### Funcionamento

- Quando o SpO2 for ≤ 95% (ou valor configurado), uma mensagem será enviada automaticamente
- O sistema possui um cooldown de 1 minuto entre alarmes para evitar spam
- As mensagens incluem o valor de SpO2 detectado e o horário da medição

### Exemplo de Mensagem

```
🚨 *ALERTA DE SATURAÇÃO DE OXIGÊNIO*

⚠️ Valores críticos detectados!

📊 SpO2: 92%
⏰ Horário: 2025-01-27 14:30:45

🔴 ATENÇÃO: A saturação está abaixo do normal (≤95%).
Por favor, verifique o paciente imediatamente.
```

## 📚 Documentação Adicional

### **Hardware e ESP32**
- [Configuração ESP32](CONFIGURACAO_ESP32.md) - Guia completo de configuração do hardware
- [Protocolo de Comunicação](ESP32_PROTOCOL.md) - Especificações técnicas de comunicação

### **Instalação e Setup**
- [Setup Rápido](SETUP.md) - Guia rápido para instalação em nova máquina

## 🔬 Aplicações e Benefícios

Este sistema pode ser aplicado em diversos contextos:

- **Monitoramento do Sono**: Acompanhamento contínuo da qualidade do sono e saturação de oxigênio
- **Prevenção de Problemas**: Identificação precoce de anormalidades respiratórias
- **Acompanhamento Médico**: Fornecimento de dados históricos para consultas e tratamentos
- **Pesquisa Científica**: Base de dados para estudos sobre padrões de sono e saúde
- **Automação Industrial**: Demonstração prática de integração entre sistemas eletrônicos e software
- **Telemedicina**: Monitoramento remoto de pacientes

## 🤝 Contribuição

1. Faça um fork do projeto
2. Crie uma branch para sua feature (`git checkout -b feature/AmazingFeature`)
3. Commit suas mudanças (`git commit -m 'Add some AmazingFeature'`)
4. Push para a branch (`git push origin feature/AmazingFeature`)
5. Abra um Pull Request

## 📚 Referências e Fontes

### 📖 Documentação Técnica Oficial

#### Microcontroladores e Hardware
- [ESP32 Technical Reference Manual](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf) - Manual oficial do ESP32
- [ESP32 DevKit V1 Pinout](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-devkitc.html) - Diagrama de pinos do ESP32
- [MAX30105 Datasheet](https://datasheets.maximintegrated.com/en/ds/MAX30105.pdf) - Especificações técnicas do sensor
- [Arduino ESP32 Core](https://github.com/espressif/arduino-esp32) - Repositório oficial do core ESP32 para Arduino

#### Desenvolvimento Web
- [React Documentation](https://react.dev/) - Documentação oficial do React
- [Node.js Documentation](https://nodejs.org/docs/) - Documentação oficial do Node.js
- [Express.js Guide](https://expressjs.com/en/guide/routing.html) - Guia oficial do Express
- [Socket.io Documentation](https://socket.io/docs/v4/) - Documentação do Socket.io

#### Containerização e Deploy
- [Docker Documentation](https://docs.docker.com/) - Documentação oficial do Docker
- [Docker Compose Documentation](https://docs.docker.com/compose/) - Guia do Docker Compose
- [Nginx Documentation](https://nginx.org/en/docs/) - Documentação do servidor Nginx

### 🔬 Padrões e Protocolos

- [IEEE 11073 - Medical Device Communication](https://standards.ieee.org/standard/11073-10407-2010.html) - Padrão IEEE para comunicação de dispositivos médicos
- [WebSocket Protocol (RFC 6455)](https://tools.ietf.org/html/rfc6455) - Especificação do protocolo WebSocket
- [ESP-NOW Protocol](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html) - Documentação do protocolo ESP-NOW

### 📊 Estudos e Pesquisas Científicas

- [Sleep Quality and Health Monitoring](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3632337/) - Estudo sobre monitoramento de qualidade do sono
- [Wearable Health Technology](https://ieeexplore.ieee.org/document/8259811) - Pesquisa sobre tecnologia vestível para saúde
- [Pulse Oximetry: Principles and Limitations](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC4504215/) - Princípios e limitações da oximetria de pulso

### 🛠️ Ferramentas de Desenvolvimento

- **Cursor AI** - IDE inteligente com assistência de IA
  - Website: [cursor.sh](https://cursor.sh)
  - Documentação: [cursor.sh/docs](https://cursor.sh/docs)
  - Modelos utilizados: Claude Sonnet 4, GPT-4
  - Recursos: Autocompletar inteligente, refatoração automática, debugging assistido, suporte multi-linguagem

### 🎓 Instituição e Orientação

- **ETEC Jorge Street** - Escola Técnica Estadual
  - Localização: São Caetano do Sul - SP
  - Curso: Técnico em Automação Industrial
  - Website: [etecjorgestreet.com.br](https://www.etecjorgestreet.com.br)
- **Desenvolvedor/Orientador**: Prof. André Tritiack
  - Organização: Old Timer IoT Labs
  - GitHub: [prof-atritiack](https://github.com/prof-atritiack)
  - Orientação técnica e desenvolvimento do projeto

## 🎓 Contexto Acadêmico e Competências Desenvolvidas

### Objetivos Educacionais

Este projeto foi desenvolvido como **Trabalho de Conclusão de Curso (TCC)** do curso técnico em **Automação Industrial** da **ETEC Jorge Street**, localizada em **São Caetano do Sul - SP**.

Os objetivos educacionais incluem:
- ✅ Aplicação prática dos conhecimentos em automação industrial
- ✅ Integração de sistemas eletrônicos com interfaces web modernas
- ✅ Desenvolvimento de soluções para problemas reais da indústria e saúde
- ✅ Demonstração de competências técnicas adquiridas durante o curso
- ✅ Utilização de tecnologias modernas (IA, Docker, WebSockets)

### Competências Técnicas Desenvolvidas

Durante o desenvolvimento deste projeto, foram aplicadas e desenvolvidas as seguintes competências:

#### Hardware e Eletrônica
- Programação de microcontroladores ESP32 em C/C++ (Arduino)
- Integração de sensores biomédicos (MAX30105)
- Comunicação sem fio (Wi-Fi e ESP-NOW)
- Controle de dispositivos de saída (LEDs, buzzer, vibrador)

#### Desenvolvimento de Software
- Desenvolvimento de interfaces web com React
- Criação de APIs RESTful com Node.js e Express
- Implementação de comunicação em tempo real com WebSocket
- Integração com APIs externas (WhatsApp via CallMeBot)

#### DevOps e Infraestrutura
- Containerização com Docker
- Orquestração com Docker Compose
- Configuração de servidor web (Nginx)
- Deploy e portabilidade de aplicações

#### Metodologias e Ferramentas
- Uso de Inteligência Artificial para desenvolvimento (Cursor AI)
- Versionamento de código (Git)
- Documentação técnica completa
- Gestão de projetos técnicos

## 📄 Licença

Este projeto está sob a licença MIT. Veja o arquivo [LICENSE](LICENSE) para mais detalhes.

## 📞 Informações do Projeto

- **Projeto**: Sleep Help - Sistema de Monitoramento de SpO2
- **Desenvolvedor**: Prof. André Tritiack
- **Organização**: Old Timer IoT Labs
- **GitHub**: [prof-atritiack](https://github.com/prof-atritiack)
- **Instituição**: ETEC Jorge Street
- **Localização**: São Caetano do Sul - SP, Brasil
- **Curso**: Técnico em Automação Industrial
- **Tipo**: Trabalho de Conclusão de Curso (TCC)
- **Orientador**: Prof. André Tritiack
- **Ferramenta de Desenvolvimento**: Cursor AI (com assistência de IA)

---

## 🙏 Agradecimentos

Agradecimentos especiais ao **Prof. André Tritiack** (Old Timer IoT Labs) pela orientação técnica e suporte durante o desenvolvimento deste projeto.

---

## 🔗 Links e Contato

- **Repositório GitHub**: [github.com/prof-atritiack/sleep-help](https://github.com/prof-atritiack/sleep-help)
- **Desenvolvedor**: Prof. André Tritiack
- **Organização**: Old Timer IoT Labs
- **Perfil GitHub**: [@prof-atritiack](https://github.com/prof-atritiack)

---

⭐ Se este projeto te ajudou, considere dar uma estrela!

*Desenvolvido com ❤️ e assistência de IA (Cursor) para o TCC em Automação Industrial da ETEC Jorge Street - São Caetano do Sul*