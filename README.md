# Sleep Help - Sistema de Monitoramento de SpO2

![Sleep Help Logo](https://img.shields.io/badge/Sleep%20Help-Monitoramento%20de%20SpO2-blue)
![React](https://img.shields.io/badge/React-18.2.0-blue)
![Node.js](https://img.shields.io/badge/Node.js-18.x-green)
![ESP32](https://img.shields.io/badge/ESP32-DevKit%20V1-red)
![Docker](https://img.shields.io/badge/Docker-Containerized-blue)

Interface web completa para monitoramento de SpO2 (Saturação de Oxigênio) durante o sono, com integração ESP32 e deploy em Docker.

## 📋 Sobre o Projeto

**Sleep Help** é um sistema completo de monitoramento de saúde desenvolvido como trabalho de conclusão de curso técnico em **Automação Industrial** da **ETEC Jorge Street**.

O projeto integra hardware ESP32 com sensores biomédicos, uma API REST robusta e uma interface web moderna para monitoramento contínuo de parâmetros vitais durante o sono.

## 🎯 Objetivos

- ✅ **Monitoramento em Tempo Real**: Acompanhamento contínuo de SpO2
- ✅ **Interface Web Responsiva**: Dashboard moderno e acessível
- ✅ **Integração Hardware**: ESP32 com sensor MAX30105
- ✅ **API REST Completa**: Backend Node.js com WebSocket
- ✅ **Deploy Containerizado**: Docker para fácil instalação
- ✅ **Sistema de Alertas**: Notificações automáticas para valores anormais
- ✅ **Integração WhatsApp**: Alertas via mensagem quando valores críticos são detectados

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

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   ESP32 +       │    │   Backend API   │    │   Frontend      │
│   MAX30105      │───▶│   Node.js       │───▶│   React         │
│   (Hardware)    │    │   + WebSocket   │    │   + Dashboard   │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         │                       │                       │
         ▼                       ▼                       ▼
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   ESP-NOW       │    │   Docker        │    │   Nginx         │
│   (Local)       │    │   Container     │    │   (Static)      │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

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
- **MAX30105** - Sensor de oxímetro de pulso
- **TFT Display** - Interface visual
- **Arduino IDE** - Desenvolvimento do firmware

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
- ESP32 DevKit V1
- Sensor MAX30105
- Display TFT (opcional)
- Buzzer (opcional)
- Arduino IDE
- Bibliotecas: MAX30105, TFT_eSPI, WiFi, HTTPClient, ArduinoJson

### Para Deploy
- Docker Desktop
- Docker Compose

## 🚀 Instalação e Configuração

### 1. Clone o Repositório

```bash
git clone https://github.com/seu-usuario/sleep-help.git
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

#### Instalar Docker Desktop
1. Baixe e instale o Docker Desktop
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

### 4. Configuração do ESP32

#### Instalar Bibliotecas
1. Abra o Arduino IDE
2. Instale as bibliotecas necessárias:
   - MAX30105
   - TFT_eSPI
   - WiFi
   - HTTPClient
   - ArduinoJson

#### Configurar Código
1. Abra o arquivo: `codigos_arduino/TRANSMISSOR_ESP32_V3/TRANSMISSOR_ESP32_V3_API.ino`
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

📚 **Guias Detalhados:**
- [Deploy Rápido (15 min)](DEPLOY_RAPIDO.md)
- [Deploy na Nuvem](DEPLOY_CLOUD.md)
- [Alternativas de Deploy](DEPLOY_ALTERNATIVAS.md)

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
├── codigos_arduino/             # Códigos para ESP32
│   ├── ESP32_SPO2_RANDOM/       # Código com dados aleatórios
│   ├── TRANSMISSOR_ESP32_V3/    # Código com sensor real
│   └── RECEIVER_ESPNOW_V0/      # Código do receptor
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
- [Configuração ESP32](codigos_arduino/TRANSMISSOR_ESP32_V3/README_API.md)
- [Protocolo de Comunicação](ESP32_PROTOCOL.md)

### **Deploy e Infraestrutura**
- [Deploy Rápido (15 min)](DEPLOY_RAPIDO.md)
- [Deploy na Nuvem](DEPLOY_CLOUD.md)
- [Alternativas de Deploy](DEPLOY_ALTERNATIVAS.md)
- [Deploy Docker](DOCKER_DEPLOY.md)
- [Configuração Docker](DOCKER_SETUP.md)

## 🔬 Aplicações e Benefícios

- **Monitoramento do Sono**: Acompanhamento da qualidade do sono
- **Prevenção de Problemas**: Identificação precoce de anormalidades
- **Acompanhamento Médico**: Dados para consultas e tratamentos
- **Pesquisa**: Base de dados para estudos sobre padrões de sono
- **Automação Industrial**: Aplicação prática dos conhecimentos técnicos

## 🤝 Contribuição

1. Faça um fork do projeto
2. Crie uma branch para sua feature (`git checkout -b feature/AmazingFeature`)
3. Commit suas mudanças (`git commit -m 'Add some AmazingFeature'`)
4. Push para a branch (`git push origin feature/AmazingFeature`)
5. Abra um Pull Request

## 📚 Fontes e Referências

### Documentação Técnica
- [ESP32 Technical Reference Manual](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)
- [React Documentation](https://react.dev/)
- [Node.js Documentation](https://nodejs.org/docs/)
- [Docker Documentation](https://docs.docker.com/)

### Sensores e Hardware
- [MAX30105 Pulse Oximeter Sensor](https://www.analog.com/en/products/max30105.html)
- [ESP32 DevKit V1 Pinout](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-devkitc.html)

### Padrões e Protocolos
- [IEEE 11073 - Medical Device Communication](https://standards.ieee.org/standard/11073-10407-2010.html)
- [WebSocket Protocol](https://tools.ietf.org/html/rfc6455)

### Estudos e Pesquisas
- [Sleep Quality and Health Monitoring](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3632337/)
- [Wearable Health Technology](https://ieeexplore.ieee.org/document/8259811)

### Ferramentas de Desenvolvimento
- **Cursor AI** - IDE inteligente com assistência de IA
  - **Modelos**: Claude Sonnet 4, GPT-4, e outros modelos avançados
  - **Recursos**: Autocompletar inteligente, refatoração automática, debugging assistido
  - **Integração**: Suporte nativo para React, JavaScript, e desenvolvimento web moderno
- [Cursor AI Documentation](https://cursor.sh/docs)

## 🎓 Contexto Acadêmico

Este projeto foi desenvolvido como **Trabalho de Conclusão de Curso (TCC)** do curso técnico em **Automação Industrial** da **ETEC Jorge Street**.

### Objetivos Educacionais
- Aplicação prática dos conhecimentos em automação industrial
- Integração de sistemas eletrônicos com interfaces web
- Desenvolvimento de soluções para problemas reais da indústria
- Demonstração de competências técnicas adquiridas durante o curso

### Competências Desenvolvidas
- Programação de microcontroladores (ESP32)
- Desenvolvimento de interfaces web (React)
- Desenvolvimento de APIs (Node.js)
- Containerização (Docker)
- Integração de sistemas
- Gestão de projetos técnicos
- Documentação técnica

## 📄 Licença

Este projeto está sob a licença MIT. Veja o arquivo [LICENSE](LICENSE) para mais detalhes.

## 📞 Contato

- **Projeto**: [Sleep Help](https://github.com/seu-usuario/sleep-help)
- **Instituição**: ETEC Jorge Street
- **Curso**: Técnico em Automação Industrial
- **Desenvolvedor**: [Seu Nome]

---

⭐ Se este projeto te ajudou, considere dar uma estrela!

*Desenvolvido com ❤️ para o TCC em Automação Industrial da ETEC Jorge Street*