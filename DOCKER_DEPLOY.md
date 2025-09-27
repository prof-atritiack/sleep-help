# 🐳 Deploy Sleep Help com Docker

## 📋 Pré-requisitos

- Docker instalado
- Docker Compose instalado
- Portas 3000 e 3001 disponíveis

## 🚀 Deploy Rápido

### 1. Construir e Iniciar os Containers

```bash
# Construir e iniciar todos os serviços
docker-compose up --build

# Ou em modo detached (background)
docker-compose up --build -d
```

### 2. Verificar Status

```bash
# Ver status dos containers
docker-compose ps

# Ver logs
docker-compose logs

# Ver logs de um serviço específico
docker-compose logs backend
docker-compose logs frontend
```

### 3. Parar os Serviços

```bash
# Parar todos os serviços
docker-compose down

# Parar e remover volumes
docker-compose down -v
```

## 🔧 Configurações

### Portas
- **Frontend**: `http://localhost:3000`
- **Backend**: `http://localhost:3001`

### Variáveis de Ambiente
As variáveis podem ser configuradas no arquivo `docker-compose.yml`:

```yaml
environment:
  - NODE_ENV=production
  - PORT=3001
```

## 📊 Monitoramento

### Logs em Tempo Real
```bash
# Ver logs de todos os serviços
docker-compose logs -f

# Ver logs do backend
docker-compose logs -f backend

# Ver logs do frontend
docker-compose logs -f frontend
```

### Status dos Containers
```bash
# Listar containers
docker ps

# Ver informações detalhadas
docker-compose ps
```

## 🛠️ Comandos Úteis

### Rebuild
```bash
# Rebuild apenas um serviço
docker-compose up --build backend

# Rebuild todos os serviços
docker-compose up --build
```

### Limpeza
```bash
# Remover containers parados
docker-compose rm

# Remover imagens não utilizadas
docker image prune

# Limpeza completa
docker system prune -a
```

## 🔍 Troubleshooting

### Problemas Comuns

1. **Porta já em uso**
   ```bash
   # Verificar portas em uso
   netstat -tulpn | grep :3000
   netstat -tulpn | grep :3001
   ```

2. **Container não inicia**
   ```bash
   # Ver logs de erro
   docker-compose logs backend
   docker-compose logs frontend
   ```

3. **Problemas de rede**
   ```bash
   # Verificar rede Docker
   docker network ls
   docker network inspect sleep-help_sleep-help-network
   ```

### Logs de Debug

```bash
# Logs detalhados
docker-compose logs --tail=100 -f

# Entrar no container
docker-compose exec backend sh
docker-compose exec frontend sh
```

## 📱 Acesso

### URLs de Acesso
- **Aplicação Web**: `http://localhost:3000`
- **API Backend**: `http://localhost:3001`
- **Status da API**: `http://localhost:3001/api/status`

### Teste da API
```bash
# Testar API
curl http://localhost:3001/api/status

# Testar frontend
curl http://localhost:3000
```

## 🔄 Atualizações

### Atualizar Código
1. Faça as alterações no código
2. Execute: `docker-compose up --build`
3. Os containers serão reconstruídos automaticamente

### Atualizar Dependências
1. Atualize os `package.json`
2. Execute: `docker-compose up --build`
3. As dependências serão reinstaladas

## 📦 Backup

### Backup dos Containers
```bash
# Salvar imagem
docker save sleep-help-backend > backend.tar
docker save sleep-help-frontend > frontend.tar

# Carregar imagem
docker load < backend.tar
docker load < frontend.tar
```

## 🚀 Produção

### Configurações para Produção
1. Configure variáveis de ambiente
2. Use volumes para persistência
3. Configure SSL/TLS
4. Configure monitoramento

### Exemplo de docker-compose.prod.yml
```yaml
version: '3.8'

services:
  backend:
    build: ./backend
    ports:
      - "3001:3001"
    environment:
      - NODE_ENV=production
    volumes:
      - ./data:/app/data

  frontend:
    build: .
    ports:
      - "80:80"
    depends_on:
      - backend
```

---

**Desenvolvido para o TCC em Automação Industrial - ETEC Jorge Street**
