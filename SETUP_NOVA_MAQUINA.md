# 🖥️ Setup em Nova Máquina - Sleep Help

## 📋 Pré-requisitos

### O que precisa instalar:

1. **Node.js** (versão 18 ou superior)
   - Download: https://nodejs.org/
   - Verificar instalação: `node --version`

2. **npm** (vem com Node.js)
   - Verificar: `npm --version`

3. **Docker Desktop** (opcional, mas recomendado)
   - Download: https://www.docker.com/products/docker-desktop/
   - Verificar: `docker --version`

4. **Git** (opcional, para clonar repositório)
   - Download: https://git-scm.com/

## 🚀 Instalação Rápida

### Opção 1: Com Docker (Recomendado)

```bash
# 1. Navegar para a pasta do projeto
cd sleep-help

# 2. Criar arquivo .env na pasta backend/
cd backend
# Copie o arquivo .env.example e renomeie para .env
# Ou use o script: ./criar-env.ps1 (Windows) ou ./criar-env.sh (Linux/Mac)

# 3. Voltar para a raiz e iniciar Docker
cd ..
docker-compose up --build -d

# 4. Verificar se está rodando
docker-compose ps
```

### Opção 2: Instalação Local (Sem Docker)

#### Backend:

```bash
# 1. Navegar para pasta backend
cd backend

# 2. Instalar dependências
npm install

# 3. Criar arquivo .env
# Copie .env.example para .env e preencha:
# WHATSAPP_PHONE=5511999999999
# WHATSAPP_API_KEY=sua_chave_aqui
# ALARM_THRESHOLD=95
# PORT=3001

# 4. Iniciar servidor
npm run dev
# ou
npm start
```

#### Frontend:

```bash
# 1. Na raiz do projeto
npm install

# 2. Iniciar servidor de desenvolvimento
npm start
```

## 📝 Configuração WhatsApp

### Passo 1: Obter Chave da API CallMeBot

1. Adicione o número **+34 644 97 54 14** aos seus contatos do WhatsApp
2. Envie a mensagem: **"Eu permito que o callmebot me envie mensagens"**
3. Você receberá uma chave de API pessoal
4. Anote a chave

### Passo 2: Criar Arquivo .env

**Na pasta `backend/`**, crie um arquivo chamado `.env`:

#### Windows (PowerShell):
```powershell
cd backend
.\criar-env.ps1
```

#### Linux/Mac:
```bash
cd backend
chmod +x criar-env.sh
./criar-env.sh
```

#### Manual:
Crie um arquivo `backend/.env` com:
```env
WHATSAPP_PHONE=5511999999999
WHATSAPP_API_KEY=sua_chave_aqui
ALARM_THRESHOLD=95
PORT=3001
```

**Importante:**
- Substitua `5511999999999` pelo seu número (formato: código país + DDD + número)
- Substitua `sua_chave_aqui` pela chave recebida do CallMeBot
- **NUNCA compartilhe ou commite o arquivo .env no Git!**

## 🔧 Configuração do ESP32

### Atualizar URL da API

No código do ESP32, atualize a URL do servidor:

```cpp
// Se rodando localmente
const char* serverURL = "http://SEU_IP_LOCAL:3001/api/sensor-data";

// Se usando Docker na mesma rede
const char* serverURL = "http://IP_DO_SERVIDOR:3001/api/sensor-data";
```

### Descobrir IP da Máquina

**Windows:**
```powershell
ipconfig
# Procure por "IPv4 Address" na sua placa de rede
```

**Linux/Mac:**
```bash
ip addr show
# ou
ifconfig
```

## ✅ Verificar Instalação

### 1. Testar Backend

```bash
# Verificar se API está respondendo
curl http://localhost:3001/api/status

# Ou no navegador
# http://localhost:3001/api/status
```

### 2. Testar WhatsApp

```bash
# Dentro da pasta backend
cd backend
node test-whatsapp.js
```

Você deve receber mensagens de teste no WhatsApp!

### 3. Testar Frontend

Abra no navegador:
```
http://localhost:3000
```

## 🐳 Comandos Docker Úteis

```bash
# Iniciar containers
docker-compose up -d

# Parar containers
docker-compose down

# Ver logs
docker-compose logs -f backend
docker-compose logs -f frontend

# Rebuild (após mudanças)
docker-compose up --build -d

# Parar tudo
docker-compose down
```

## 🔍 Solução de Problemas

### Problema: Porta já em uso

**Solução:**
```bash
# Windows - Encontrar processo na porta 3001
netstat -ano | findstr :3001

# Linux/Mac
lsof -i :3001

# Parar o processo ou mudar a porta no .env
```

### Problema: Dependências não instalam

**Solução:**
```bash
# Limpar cache e reinstalar
npm cache clean --force
rm -rf node_modules package-lock.json
npm install
```

### Problema: Docker não inicia

**Solução:**
- Verifique se Docker Desktop está rodando
- Reinicie Docker Desktop
- Verifique: `docker ps`

### Problema: WhatsApp não funciona

**Verifique:**
1. Arquivo `.env` existe na pasta `backend/`
2. Credenciais estão corretas
3. Servidor foi reiniciado após criar `.env`
4. Teste manual: `node backend/test-whatsapp.js`

## 📚 Arquivos Importantes

- `README.md` - Documentação principal
- `backend/WHATSAPP_INTEGRATION.md` - Guia completo do WhatsApp
- `backend/COMO_CONFIGURAR_ENV.md` - Como configurar variáveis
- `DOCKER_WHATSAPP.md` - Configuração Docker + WhatsApp
- `DOCKER_UPDATE.md` - Atualizações Docker

## 🎯 Checklist de Instalação

- [ ] Node.js instalado
- [ ] npm instalado
- [ ] Docker Desktop instalado (se usar Docker)
- [ ] Arquivo `.env` criado na pasta `backend/`
- [ ] Variáveis WhatsApp configuradas no `.env`
- [ ] Dependências instaladas (`npm install`)
- [ ] Servidor backend iniciado
- [ ] Frontend iniciado (ou Docker rodando)
- [ ] Teste WhatsApp funcionando
- [ ] ESP32 configurado com IP correto

## 📞 URLs de Acesso

### Desenvolvimento Local
- **Frontend**: http://localhost:3000
- **Backend API**: http://localhost:3001
- **API Status**: http://localhost:3001/api/status

### Docker
- **Frontend**: http://localhost:3000
- **Backend API**: http://localhost:3001

### Rede Local
- **Frontend**: http://SEU_IP:3000
- **Backend API**: http://SEU_IP:3001

---

**Pronto para começar! 🚀**

Se encontrar problemas, consulte a documentação adicional ou verifique os logs do servidor.

