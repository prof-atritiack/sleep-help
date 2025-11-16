# 🐳 Comandos Docker - Sleep Help

## 📋 Comandos Essenciais

### **Modo Produção** (docker-compose.yml)

#### 1. Parar containers existentes
```bash
docker-compose down
```

#### 2. Reconstruir e subir novos containers
```bash
# Reconstruir imagens e subir containers
docker-compose up --build -d

# Ou apenas reconstruir um serviço específico
docker-compose up --build -d backend
docker-compose up --build -d frontend
```

### **Modo Desenvolvimento** (docker-compose.dev.yml) - Com Hot Reload

#### 1. Iniciar com hot-reload
```bash
docker-compose -f docker-compose.dev.yml up --build -d
```

#### 2. Parar containers de desenvolvimento
```bash
docker-compose -f docker-compose.dev.yml down
```

#### 3. Ver logs de desenvolvimento
```bash
docker-compose -f docker-compose.dev.yml logs -f
```

#### 3. Ver status dos containers
```bash
# Produção
docker-compose ps

# Desenvolvimento
docker-compose -f docker-compose.dev.yml ps
```

#### 4. Ver logs
```bash
# Produção - Todos os serviços
docker-compose logs -f

# Produção - Apenas backend
docker-compose logs -f backend

# Produção - Apenas frontend
docker-compose logs -f frontend

# Desenvolvimento - Todos
docker-compose -f docker-compose.dev.yml logs -f
```

#### 5. Parar containers
```bash
# Produção
docker-compose stop

# Desenvolvimento
docker-compose -f docker-compose.dev.yml stop
```

#### 6. Reiniciar containers
```bash
# Produção
docker-compose restart

# Desenvolvimento
docker-compose -f docker-compose.dev.yml restart
```

#### 7. Remover tudo (containers, volumes, networks)
```bash
# Produção
docker-compose down -v

# Desenvolvimento
docker-compose -f docker-compose.dev.yml down -v
```

## 🔄 Processo Completo para Subir Containers

### **Produção:**
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

### **Desenvolvimento (com hot-reload):**
```bash
# 1. Parar containers atuais
docker-compose -f docker-compose.dev.yml down

# 2. Reconstruir e subir
docker-compose -f docker-compose.dev.yml up --build -d

# 3. Verificar status
docker-compose -f docker-compose.dev.yml ps

# 4. Ver logs
docker-compose -f docker-compose.dev.yml logs -f
```

## 🧹 Limpeza Completa

```bash
# Parar e remover tudo (produção)
docker-compose down -v

# Parar e remover tudo (desenvolvimento)
docker-compose -f docker-compose.dev.yml down -v

# Limpar imagens não utilizadas
docker image prune -a

# Limpar sistema completo (cuidado!)
docker system prune -a
```

## 📚 Documentação Adicional

- **`DOCKER_RESUMO.md`** - Resumo completo do Docker
- **`DOCKER_HOT_RELOAD.md`** - Guia detalhado de hot-reload
- **`DOCKER_DESKTOP_GUIA.md`** - Usar no Docker Desktop
- **`DEPLOY_CLOUD.md`** - Deploy na nuvem

