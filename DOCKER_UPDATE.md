# 🐳 Atualização Docker - Integração WhatsApp

## ✅ Mudanças Realizadas

### 1. docker-compose.yml
- ✅ Adicionado suporte para variáveis WhatsApp via `env_file`
- ✅ Configuração de variáveis de ambiente para WhatsApp
- ✅ Leitura automática do arquivo `backend/.env`

### 2. backend/Dockerfile
- ✅ Atualizado com comentário sobre dotenv
- ✅ Dotenv já está nas dependências do package.json

### 3. Documentação
- ✅ Criado `DOCKER_WHATSAPP.md` com instruções completas
- ✅ Este arquivo com guia rápido

## 🚀 Como Atualizar

### Passo 1: Verificar Arquivo .env

Certifique-se que o arquivo `backend/.env` existe e está configurado:

```bash
# Verificar se existe
ls backend/.env

# Ver conteúdo (primeiras linhas)
cat backend/.env | head -5
```

Deve conter:
```env
WHATSAPP_PHONE=5511999999999
WHATSAPP_API_KEY=sua_chave_aqui
ALARM_THRESHOLD=95
```

### Passo 2: Parar Containers Atuais

```bash
# Parar containers (se estiverem rodando)
docker-compose down
```

### Passo 3: Rebuild dos Containers

```bash
# Rebuild forçado (sem cache)
docker-compose build --no-cache

# Ou rebuild normal
docker-compose build

# Build e iniciar
docker-compose up --build -d
```

### Passo 4: Verificar Logs

```bash
# Ver logs do backend
docker-compose logs -f backend

# Você deve ver mensagens como:
# ✅ Configuração WhatsApp carregada
# 📱 WhatsApp configurado: telefone configurado
```

### Passo 5: Testar Integração

```bash
# Teste dentro do container
docker-compose exec backend node test-whatsapp.js

# Ou teste via API
curl -X POST http://localhost:3001/api/sensor-data \
  -H "Content-Type: application/json" \
  -d '{"spo2":92,"timestamp":"2025-01-27 14:30:45"}'
```

## 🔍 Verificação

### Verificar se variáveis estão carregadas

```bash
# Verificar variáveis no container
docker-compose exec backend env | grep WHATSAPP

# Deve mostrar:
# WHATSAPP_PHONE=5511999999999
# WHATSAPP_API_KEY=sua_chave
# ALARM_THRESHOLD=95
```

### Verificar status do container

```bash
# Status dos containers
docker-compose ps

# Logs em tempo real
docker-compose logs -f backend
```

## 🐛 Troubleshooting

### Problema: Variáveis não estão sendo carregadas

**Solução:**
1. Verifique se `backend/.env` existe
2. Verifique se o arquivo está no formato correto (sem espaços extras)
3. Reinicie o container: `docker-compose restart backend`
4. Se não funcionar, tente rebuild: `docker-compose up --build -d backend`

### Problema: Container não inicia

**Solução:**
```bash
# Ver logs detalhados
docker-compose logs backend

# Rebuild completo
docker-compose down
docker-compose build --no-cache backend
docker-compose up -d backend
```

### Problema: Erro ao conectar com API

**Solução:**
1. Verifique se as credenciais no `.env` estão corretas
2. Verifique logs: `docker-compose logs backend | grep -i whatsapp`
3. Teste manualmente: `docker-compose exec backend node test-whatsapp.js`

## 📝 Notas Importantes

1. **Segurança**: O arquivo `.env` está no `.dockerignore`, então não será copiado para a imagem
2. **Variáveis**: As variáveis são carregadas via `env_file` no docker-compose.yml
3. **Produção**: Em produção, use secrets do Docker ou variáveis de ambiente do host
4. **Backup**: Sempre mantenha backup do arquivo `.env` (mas NÃO no Git!)

## 📚 Referências

- [Configuração WhatsApp no Docker](./DOCKER_WHATSAPP.md)
- [Integração WhatsApp](./backend/WHATSAPP_INTEGRATION.md)
- [Como Configurar .env](./backend/COMO_CONFIGURAR_ENV.md)

