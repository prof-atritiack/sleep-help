# 🐳 Comandos Docker - Sleep Help

## 📋 Comandos Essenciais

### 1. Parar containers existentes
```bash
docker-compose down
```

### 2. Reconstruir e subir novos containers
```bash
# Reconstruir imagens e subir containers
docker-compose up --build -d

# Ou apenas reconstruir um serviço específico
docker-compose up --build -d backend
docker-compose up --build -d frontend
```

### 3. Ver status dos containers
```bash
docker-compose ps
```

### 4. Ver logs
```bash
# Todos os serviços
docker-compose logs -f

# Apenas backend
docker-compose logs -f backend

# Apenas frontend
docker-compose logs -f frontend
```

### 5. Parar containers
```bash
docker-compose stop
```

### 6. Reiniciar containers
```bash
docker-compose restart
```

### 7. Remover tudo (containers, volumes, networks)
```bash
docker-compose down -v
```

## 🔄 Processo Completo para Subir Novos Containers

```bash
# 1. Parar containers atuais
docker-compose down

# 2. Limpar imagens antigas (opcional)
docker rmi sleep-help-backend sleep-help-frontend

# 3. Reconstruir e subir
docker-compose up --build -d

# 4. Verificar status
docker-compose ps

# 5. Ver logs
docker-compose logs -f
```

## 🧹 Limpeza Completa

```bash
# Parar e remover tudo
docker-compose down -v

# Limpar imagens não utilizadas
docker image prune -a

# Limpar sistema completo (cuidado!)
docker system prune -a
```

