# ✅ Checklist - Preparação para Enviar Projeto

## 📦 Arquivos que DEVEM ser enviados

### ✅ Código Fonte
- [x] `backend/server.js` - Servidor principal
- [x] `backend/whatsapp.js` - Módulo WhatsApp
- [x] `backend/package.json` - Dependências do backend
- [x] `package.json` - Dependências do frontend
- [x] `src/` - Código do frontend React
- [x] `public/` - Arquivos públicos do frontend

### ✅ Configuração
- [x] `docker-compose.yml` - Configuração Docker
- [x] `backend/Dockerfile` - Dockerfile do backend
- [x] `Dockerfile` - Dockerfile do frontend
- [x] `.gitignore` - Arquivos ignorados pelo Git

### ✅ Documentação
- [x] `README.md` - Documentação principal
- [x] `SETUP_NOVA_MAQUINA.md` - **NOVO** - Guia de instalação
- [x] `backend/WHATSAPP_INTEGRATION.md` - Guia WhatsApp
- [x] `backend/COMO_CONFIGURAR_ENV.md` - Guia .env
- [x] `DOCKER_WHATSAPP.md` - Guia Docker + WhatsApp
- [x] `DOCKER_UPDATE.md` - Atualização Docker
- [x] `backend/.env.example` - **NOVO** - Template de configuração

### ✅ Scripts Úteis
- [x] `backend/criar-env.ps1` - Script Windows para criar .env
- [x] `backend/criar-env.sh` - Script Linux/Mac para criar .env
- [x] `backend/test-whatsapp.js` - Script de teste WhatsApp
- [x] `backend/test-alarme-real.js` - Teste de alarme via API

## ❌ Arquivos que NÃO devem ser enviados

### 🔒 Sensíveis e Seguros
- [ ] `backend/.env` - **NUNCA enviar!** (contém credenciais)
- [ ] `node_modules/` - Instalar com `npm install`
- [ ] `.env` (na raiz) - Se existir
- [ ] Arquivos de log (`*.log`)

### 🗑️ Gerados/Temporários
- [ ] `build/` - Gerado durante build
- [ ] `.cache/` - Cache
- [ ] `coverage/` - Testes

## 📋 O que fazer na nova máquina

### 1. Instalar Pré-requisitos
```bash
# Verificar Node.js
node --version  # Deve ser 18+

# Verificar npm
npm --version
```

### 2. Instalar Dependências

```bash
# Backend
cd backend
npm install

# Frontend (na raiz)
cd ..
npm install
```

### 3. Configurar WhatsApp

```bash
# Criar arquivo .env
cd backend

# Windows
.\criar-env.ps1

# Linux/Mac
chmod +x criar-env.sh
./criar-env.sh

# Ou manualmente: copiar .env.example para .env e preencher
```

### 4. Iniciar

**Com Docker:**
```bash
docker-compose up --build -d
```

**Sem Docker:**
```bash
# Terminal 1 - Backend
cd backend
npm run dev

# Terminal 2 - Frontend
npm start
```

## 📝 Informações Importantes para Nova Máquina

### Variáveis de Ambiente Necessárias

O arquivo `backend/.env` precisa ser criado com:

```env
WHATSAPP_PHONE=5511999999999  # Número do destinatário
WHATSAPP_API_KEY=sua_chave    # Chave do CallMeBot
ALARM_THRESHOLD=95            # Threshold de alarme
PORT=3001                      # Porta do servidor
```

### Como Obter Chave WhatsApp

1. Adicione +34 644 97 54 14 aos contatos
2. Envie: "Eu permito que o callmebot me envie mensagens"
3. Receba a chave de API
4. Configure no arquivo `.env`

### URLs de Acesso

- Frontend: http://localhost:3000
- Backend: http://localhost:3001
- API Status: http://localhost:3001/api/status

## 🔍 Verificação Final

Antes de enviar, verifique:

- [ ] `.gitignore` está configurado corretamente
- [ ] Arquivo `.env` NÃO está na lista de arquivos
- [ ] `node_modules` está ignorado
- [ ] Todos os arquivos de documentação estão incluídos
- [ ] Scripts de configuração estão incluídos
- [ ] `SETUP_NOVA_MAQUINA.md` foi criado

## 📦 Formas de Enviar

### Opção 1: ZIP (Mais Simples)
1. Compactar todos os arquivos (exceto node_modules e .env)
2. Enviar ZIP
3. Na nova máquina: extrair e seguir `SETUP_NOVA_MAQUINA.md`

### Opção 2: Git (Recomendado)
1. Fazer commit de tudo
2. Push para repositório
3. Na nova máquina: `git clone` e seguir setup

### Opção 3: Pendrive/HD Externo
1. Copiar pasta completa (sem node_modules)
2. Transferir para nova máquina
3. Seguir `SETUP_NOVA_MAQUINA.md`

## ⚠️ Lembrete Final

**NUNCA inclua:**
- ❌ Arquivo `.env` com credenciais
- ❌ Pasta `node_modules` (muito pesada)
- ❌ Arquivos de log
- ❌ Cache e builds antigos

**SEMPRE inclua:**
- ✅ Código fonte completo
- ✅ Arquivos de configuração (docker-compose.yml, Dockerfile, etc.)
- ✅ Documentação
- ✅ Scripts úteis
- ✅ `.env.example` como template

---

**Projeto pronto para transferência! 🚀**

