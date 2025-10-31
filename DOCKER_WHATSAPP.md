# 🐳 Configuração WhatsApp no Docker

## 📋 Pré-requisitos

Antes de executar o Docker com integração WhatsApp, você precisa:

1. Ter criado o arquivo `.env` na pasta `backend/` com suas credenciais
2. Ou configurar as variáveis de ambiente no `docker-compose.yml`

## 🔧 Opção 1: Usando arquivo .env (Recomendado)

### Passo 1: Criar arquivo .env

Na pasta `backend/`, crie um arquivo `.env`:

```bash
cd backend
# Use o script de criação ou crie manualmente
```

Conteúdo do arquivo `.env`:
```env
WHATSAPP_PHONE=5511999999999
WHATSAPP_API_KEY=sua_chave_aqui
ALARM_THRESHOLD=95
PORT=3001
```

### Passo 2: Executar Docker

O `docker-compose.yml` já está configurado para ler automaticamente o arquivo `.env`:

```bash
# Build e iniciar
docker-compose up --build -d

# Ver logs
docker-compose logs -f backend
```

## 🔧 Opção 2: Variáveis no docker-compose.yml

Você também pode definir as variáveis diretamente no `docker-compose.yml`:

```yaml
environment:
  - WHATSAPP_PHONE=5511999999999
  - WHATSAPP_API_KEY=sua_chave_aqui
  - ALARM_THRESHOLD=95
```

## 🔧 Opção 3: Arquivo .env na raiz do projeto

Crie um arquivo `.env` na raiz do projeto (mesma pasta do `docker-compose.yml`):

```env
WHATSAPP_PHONE=5511999999999
WHATSAPP_API_KEY=sua_chave_aqui
ALARM_THRESHOLD=95
```

O Docker Compose lerá automaticamente essas variáveis.

## 🚀 Build e Deploy

### Build completo

```bash
# Build com cache
docker-compose build

# Build sem cache (força rebuild)
docker-compose build --no-cache

# Build e iniciar
docker-compose up --build -d
```

### Verificar se está funcionando

```bash
# Ver logs do backend
docker-compose logs -f backend

# Executar teste dentro do container
docker-compose exec backend node test-whatsapp.js
```

### Parar e reiniciar

```bash
# Parar containers
docker-compose down

# Parar e remover volumes (se necessário)
docker-compose down -v

# Reiniciar
docker-compose up -d
```

## ✅ Verificar Configuração

Para verificar se as variáveis estão configuradas corretamente no container:

```bash
# Verificar variáveis de ambiente no container
docker-compose exec backend env | grep WHATSAPP

# Ou verificar logs na inicialização
docker-compose logs backend | grep -i whatsapp
```

## 🧪 Testar Integração

### Teste dentro do container

```bash
docker-compose exec backend node test-whatsapp.js
```

### Teste via API externa

```bash
# Enviar dados de teste que geram alarme
curl -X POST http://localhost:3001/api/sensor-data \
  -H "Content-Type: application/json" \
  -d '{"spo2":92,"timestamp":"2025-01-27 14:30:45"}'
```

Você deve receber uma mensagem no WhatsApp se SpO2 ≤ 95!

## 📝 Notas Importantes

1. **Segurança**: Nunca commite o arquivo `.env` no Git
2. **Variáveis**: As variáveis de ambiente têm precedência na seguinte ordem:
   - Variáveis definidas diretamente no `docker-compose.yml`
   - Arquivo `.env` na pasta `backend/`
   - Arquivo `.env` na raiz do projeto
3. **Atualização**: Após alterar o `.env`, reinicie o container:
   ```bash
   docker-compose restart backend
   ```

## 🐛 Troubleshooting

### Variáveis não estão sendo carregadas

```bash
# Verificar se o arquivo .env existe
ls -la backend/.env

# Verificar conteúdo (CUIDADO: pode expor credenciais)
cat backend/.env

# Verificar logs de inicialização
docker-compose logs backend | head -20
```

### Container não inicia

```bash
# Ver logs detalhados
docker-compose logs backend

# Verificar se a porta está livre
netstat -ano | findstr :3001
```

### Rebuild completo

```bash
# Parar tudo
docker-compose down

# Remover imagens antigas
docker rmi sleep-help-backend:latest

# Rebuild do zero
docker-compose build --no-cache backend
docker-compose up -d
```

## 📚 Documentação Adicional

- [Configuração WhatsApp](./backend/WHATSAPP_INTEGRATION.md)
- [Como Configurar .env](./backend/COMO_CONFIGURAR_ENV.md)
- [Deploy Docker](./DOCKER_DEPLOY.md)

