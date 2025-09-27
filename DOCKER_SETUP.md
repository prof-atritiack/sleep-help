# 🐳 Configuração Docker para Sleep Help

## 📋 Pré-requisitos

### 1. Instalar Docker Desktop
- Baixe e instale o Docker Desktop para Windows
- Link: https://www.docker.com/products/docker-desktop/

### 2. Verificar Instalação
```bash
# Verificar versão do Docker
docker --version

# Verificar versão do Docker Compose
docker-compose --version
```

## 🚀 Iniciar Docker Desktop

### Windows
1. **Abra o Docker Desktop** no menu Iniciar
2. **Aguarde** até aparecer "Docker Desktop is running"
3. **Verifique** se o ícone do Docker está verde na bandeja do sistema

### Verificar Status
```bash
# Verificar se Docker está rodando
docker ps

# Deve retornar uma lista vazia (sem erro)
```

## 🔧 Configuração da Aplicação

### 1. Navegar para o Diretório
```bash
cd C:\Users\atrit\OneDrive\Documentos\_SLEEP_HELP\sleep-help
```

### 2. Construir e Executar
```bash
# Construir e iniciar todos os serviços
docker-compose up --build

# Ou em modo background
docker-compose up --build -d
```

### 3. Verificar Status
```bash
# Ver containers rodando
docker-compose ps

# Ver logs
docker-compose logs
```

## 📱 Acesso à Aplicação

### URLs
- **Frontend**: http://localhost:3000
- **Backend**: http://localhost:3001
- **API Status**: http://localhost:3001/api/status

### Teste Rápido
```bash
# Testar backend
curl http://localhost:3001/api/status

# Testar frontend
curl http://localhost:3000
```

## 🛠️ Comandos Úteis

### Gerenciamento de Containers
```bash
# Parar todos os serviços
docker-compose down

# Parar e remover volumes
docker-compose down -v

# Rebuild apenas um serviço
docker-compose up --build backend

# Ver logs em tempo real
docker-compose logs -f
```

### Limpeza
```bash
# Remover containers parados
docker-compose rm

# Limpeza completa
docker system prune -a
```

## 🚨 Solução de Problemas

### Docker Desktop não inicia
1. **Reinicie o computador**
2. **Execute como administrador**
3. **Verifique se a virtualização está habilitada** no BIOS

### Erro de permissão
1. **Execute o PowerShell como administrador**
2. **Verifique se o Docker Desktop está rodando**

### Porta já em uso
```bash
# Verificar portas em uso
netstat -tulpn | grep :3000
netstat -tulpn | grep :3001

# Parar processos que usam as portas
# Ou alterar as portas no docker-compose.yml
```

### Problemas de rede
```bash
# Verificar rede Docker
docker network ls

# Reiniciar Docker Desktop
# Ou executar: docker-compose down && docker-compose up --build
```

## 📊 Monitoramento

### Logs em Tempo Real
```bash
# Todos os serviços
docker-compose logs -f

# Apenas backend
docker-compose logs -f backend

# Apenas frontend
docker-compose logs -f frontend
```

### Status dos Containers
```bash
# Listar containers
docker ps

# Informações detalhadas
docker-compose ps

# Estatísticas de uso
docker stats
```

## 🔄 Atualizações

### Atualizar Código
1. **Faça as alterações** no código
2. **Execute**: `docker-compose up --build`
3. **Os containers serão reconstruídos** automaticamente

### Atualizar Dependências
1. **Atualize** os `package.json`
2. **Execute**: `docker-compose up --build`
3. **As dependências serão reinstaladas**

## 📦 Estrutura dos Containers

### Backend Container
- **Imagem**: Node.js 18 Alpine
- **Porta**: 3001
- **Comando**: `node server.js`

### Frontend Container
- **Imagem**: Nginx Alpine
- **Porta**: 3000 (mapeada para 80)
- **Comando**: `nginx -g daemon off;`

## 🎯 Próximos Passos

1. **Inicie o Docker Desktop**
2. **Execute**: `docker-compose up --build`
3. **Acesse**: http://localhost:3000
4. **Teste a aplicação** com dados do ESP32

---

**Desenvolvido para o TCC em Automação Industrial - ETEC Jorge Street**
