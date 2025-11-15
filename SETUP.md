# 🚀 Setup Rápido - Sleep Help

## 📋 Pré-requisitos

- **Docker Desktop** instalado e rodando
- **Git** (opcional, para clonar repositório)

## ⚡ Instalação em 3 Passos

### 1. Clone ou baixe o projeto
```bash
git clone <url-do-repositorio>
cd sleep-help
```

### 2. Configure variáveis de ambiente (Opcional - para WhatsApp)

**Windows (PowerShell):**
```powershell
cd backend
Copy-Item .env.example .env
# Edite o arquivo .env e preencha suas credenciais
```

**Linux/Mac:**
```bash
cd backend
cp .env.example .env
# Edite o arquivo .env e preencha suas credenciais
```

**Conteúdo do `.env`:**
```env
WHATSAPP_PHONE=5511999999999
WHATSAPP_API_KEY=sua_chave_api_aqui
ALARM_THRESHOLD=95
PORT=3001
```

> **Nota:** Se não configurar o WhatsApp, o sistema funcionará normalmente, apenas sem alertas via WhatsApp.

### 3. Inicie com Docker
```bash
# Na raiz do projeto
docker-compose up --build -d
```

## ✅ Verificar Instalação

### 1. Verificar containers
```bash
docker-compose ps
```

Deve mostrar:
- `sleep-help-backend` (porta 3001)
- `sleep-help-frontend` (porta 3000)

### 2. Testar API
```bash
curl http://localhost:3001/api/status
```

Ou acesse no navegador: http://localhost:3001/api/status

### 3. Acessar Interface Web
Abra no navegador: **http://localhost:3000**

## 📱 Configuração WhatsApp (Opcional)

1. Adicione o número **+34 644 97 54 14** aos seus contatos
2. Envie: **"Eu permito que o callmebot me envie mensagens"**
3. Você receberá uma chave de API
4. Configure no arquivo `backend/.env`

## 🔧 Configuração ESP32

1. Abra os códigos em `codigos_ESP32_finais/`
2. Configure o IP do servidor no código:
   ```cpp
   const char* serverURL = "http://SEU_IP:3001/api/sensor-data";
   ```
3. Configure as credenciais Wi-Fi
4. Faça upload para o ESP32

## 🐳 Comandos Docker Úteis

```bash
# Ver logs
docker-compose logs -f

# Parar containers
docker-compose down

# Reiniciar após mudanças
docker-compose up --build -d

# Ver logs de um serviço específico
docker-compose logs -f backend
docker-compose logs -f frontend
```

## 🚨 Solução de Problemas

### Porta já em uso
```bash
# Windows
netstat -ano | findstr :3001

# Linux/Mac
lsof -i :3001
```

### Docker não inicia
- Verifique se Docker Desktop está rodando
- Reinicie Docker Desktop
- Execute: `docker ps` para verificar

### Containers não sobem
```bash
docker-compose down
docker-compose up --build -d
```

## 📚 Documentação Completa

Consulte o [README.md](README.md) para informações detalhadas sobre:
- Arquitetura do sistema
- API endpoints
- Configurações avançadas
- Deploy na nuvem

---

**Pronto para usar! 🎉**

