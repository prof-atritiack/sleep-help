# 🐳 Guia Docker Desktop - Sleep Help

## ✅ Status Atual

Os containers foram criados e estão rodando com sucesso!

- ✅ **Backend**: `sleep-help-backend` na porta 3001
- ✅ **Frontend**: `sleep-help-frontend` na porta 3000

## 📱 Acessar a Aplicação

- **Frontend**: http://localhost:3000
- **Backend API**: http://localhost:3001
- **Status API**: http://localhost:3001/api/status

## 🖥️ Como Usar no Docker Desktop

### 1. Abrir Docker Desktop

Abra o Docker Desktop no Windows. Você verá os containers na aba **Containers**.

### 2. Ver Containers

Na interface do Docker Desktop:
- Procure por `sleep-help-backend` e `sleep-help-frontend`
- Ambos devem estar com status **Running** (verde)

### 3. Ver Logs

**No Docker Desktop:**
1. Clique no container `sleep-help-backend`
2. Vá na aba **Logs** para ver os logs em tempo real
3. Repita para `sleep-help-frontend`

**Via Terminal:**
```bash
# Ver logs de todos
docker-compose logs -f

# Ver logs do backend
docker-compose logs -f backend

# Ver logs do frontend
docker-compose logs -f frontend
```

### 4. Parar Containers

**No Docker Desktop:**
1. Selecione os containers
2. Clique em **Stop** (ou botão de pausa)

**Via Terminal:**
```bash
docker-compose stop
```

### 5. Reiniciar Containers

**No Docker Desktop:**
1. Selecione os containers parados
2. Clique em **Start** (ou botão de play)

**Via Terminal:**
```bash
docker-compose restart
```

### 6. Reconstruir Containers

**No Docker Desktop:**
1. Pare os containers
2. Remova as imagens antigas (opcional)
3. Use o terminal para reconstruir:
```bash
docker-compose up --build -d
```

### 7. Remover Tudo

**No Docker Desktop:**
1. Pare os containers
2. Selecione e clique em **Delete**

**Via Terminal:**
```bash
docker-compose down
```

## 🔍 Verificar Status

### No Docker Desktop:
- Aba **Containers**: Veja todos os containers e seus status
- Aba **Images**: Veja as imagens construídas
- Aba **Volumes**: Veja volumes (se houver)
- Aba **Networks**: Veja a rede `sleep-help_sleep-help-network`

### Via Terminal:
```bash
# Status dos containers
docker-compose ps

# Informações detalhadas
docker ps -a

# Ver uso de recursos
docker stats
```

## 🐛 Solução de Problemas

### Container não inicia
1. Verifique os logs: `docker-compose logs backend`
2. Verifique se as portas 3000 e 3001 estão livres
3. Tente reconstruir: `docker-compose up --build -d`

### Porta já em uso
```bash
# Windows - Verificar processo na porta
netstat -ano | findstr :3001

# Parar processo ou mudar porta no docker-compose.yml
```

### Reconstruir do zero
```bash
# Parar tudo
docker-compose down

# Remover imagens
docker rmi sleep-help-backend sleep-help-frontend

# Reconstruir
docker-compose up --build -d
```

## 📊 Monitoramento

### Ver uso de recursos:
```bash
docker stats
```

### Ver informações do container:
```bash
docker inspect sleep-help-backend
docker inspect sleep-help-frontend
```

## 🔄 Comandos Rápidos

```bash
# Subir containers
docker-compose up -d

# Parar containers
docker-compose stop

# Reiniciar containers
docker-compose restart

# Ver logs
docker-compose logs -f

# Reconstruir e subir
docker-compose up --build -d

# Parar e remover
docker-compose down
```

---

**Pronto!** Seus containers estão rodando e a aplicação está disponível em http://localhost:3000

