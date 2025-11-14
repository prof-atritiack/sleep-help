# 🐳 Portabilidade Docker - Sleep Help

## ✅ Sim, o projeto está pronto para replicação fácil!

O projeto **Sleep Help** está totalmente configurado para ser facilmente replicado em qualquer computador com Docker Desktop instalado. Toda a infraestrutura está containerizada e pronta para uso.

## 📋 O que está incluído

### ✅ Arquivos Docker Configurados

1. **`docker-compose.yml`** - Orquestração completa dos serviços
   - Backend (Node.js)
   - Frontend (React + Nginx)
   - Rede Docker configurada
   - Variáveis de ambiente suportadas

2. **`Dockerfile`** (raiz) - Frontend React
   - Build otimizado com multi-stage
   - Nginx para servir arquivos estáticos

3. **`backend/Dockerfile`** - Backend Node.js
   - Node.js 18 Alpine (leve)
   - Dependências instaladas automaticamente

### ✅ Documentação Completa

- `DOCKER_SETUP.md` - Guia de instalação
- `DOCKER_DEPLOY.md` - Guia de deploy
- `SETUP_NOVA_MAQUINA.md` - Setup em nova máquina
- `DOCKER_WHATSAPP.md` - Configuração WhatsApp + Docker

## 🚀 Como Replicar em Outro Computador

### Passo 1: Pré-requisitos

No computador de destino, instale apenas:
- **Docker Desktop** (Windows/Mac/Linux)
  - Download: https://www.docker.com/products/docker-desktop/

### Passo 2: Transferir o Projeto

**Opção A: Git (Recomendado)**
```bash
git clone <url-do-repositorio>
cd sleep-help
```

**Opção B: Copiar Arquivos**
- Copie toda a pasta do projeto
- Certifique-se de incluir todos os arquivos

### Passo 3: Configurar Variáveis de Ambiente

Crie o arquivo `backend/.env`:
```bash
cd backend
# Windows PowerShell
.\criar-env.ps1

# Linux/Mac
chmod +x criar-env.sh
./criar-env.sh
```

Ou crie manualmente `backend/.env`:
```env
WHATSAPP_PHONE=5511999999999
WHATSAPP_API_KEY=sua_chave_aqui
ALARM_THRESHOLD=95
PORT=3001
```

### Passo 4: Iniciar com Docker

```bash
# Na raiz do projeto
docker-compose up --build -d
```

### Passo 5: Verificar

```bash
# Verificar containers rodando
docker-compose ps

# Ver logs
docker-compose logs -f

# Testar API
curl http://localhost:3001/api/status
```

**Acesse:**
- Frontend: http://localhost:3000
- Backend: http://localhost:3001

## ✅ Vantagens da Configuração Atual

### 1. **Isolamento Completo**
- Todas as dependências dentro dos containers
- Não precisa instalar Node.js, npm, etc. no host
- Versões fixas garantem consistência

### 2. **Configuração Simples**
- Um único comando: `docker-compose up --build`
- Variáveis de ambiente centralizadas
- Sem conflitos de portas ou dependências

### 3. **Multiplataforma**
- Funciona em Windows, Mac e Linux
- Mesmo comportamento em qualquer sistema
- Sem necessidade de ajustes específicos do SO

### 4. **Fácil Manutenção**
- Atualizações: `docker-compose up --build`
- Logs centralizados: `docker-compose logs`
- Limpeza fácil: `docker-compose down`

## 📦 O que NÃO precisa instalar no computador de destino

❌ **Node.js** - Não precisa (está no container)
❌ **npm** - Não precisa (está no container)
❌ **Python** - Não precisa
❌ **Nginx** - Não precisa (está no container)
❌ **Qualquer dependência do projeto** - Tudo está no Docker

## ✅ O que precisa apenas

✅ **Docker Desktop** - Único requisito
✅ **Arquivos do projeto** - Código fonte
✅ **Arquivo .env** - Configurações (criado localmente)

## 🔄 Atualizações e Sincronização

### Atualizar Código
```bash
# 1. Atualizar código (git pull ou copiar arquivos)
# 2. Rebuild
docker-compose up --build -d
```

### Sincronizar Entre Máquinas
1. **Código**: Use Git ou copie os arquivos
2. **Configurações**: Recrie o `.env` em cada máquina (não commite!)
3. **Docker**: Mesma configuração funciona em todas as máquinas

## 🎯 Checklist de Replicação

- [ ] Docker Desktop instalado
- [ ] Projeto copiado/clonado
- [ ] Arquivo `backend/.env` criado
- [ ] `docker-compose up --build` executado
- [ ] Containers rodando (`docker-compose ps`)
- [ ] Frontend acessível (http://localhost:3000)
- [ ] Backend acessível (http://localhost:3001)
- [ ] WhatsApp configurado (se necessário)

## 🚨 Troubleshooting

### Docker não inicia
- Verifique se Docker Desktop está rodando
- Reinicie Docker Desktop
- Verifique: `docker ps`

### Porta já em uso
- Altere as portas no `docker-compose.yml`:
  ```yaml
  ports:
    - "3002:3001"  # Mude 3000 para outra porta
  ```

### Erro ao build
- Verifique logs: `docker-compose logs`
- Limpe cache: `docker system prune`
- Rebuild: `docker-compose up --build --no-cache`

## 📊 Resumo

| Item | Status | Observação |
|------|--------|------------|
| Docker Compose | ✅ Configurado | `docker-compose.yml` completo |
| Dockerfiles | ✅ Configurados | Backend e Frontend |
| Variáveis de Ambiente | ✅ Suportadas | Via `.env` ou `docker-compose.yml` |
| Documentação | ✅ Completa | Múltiplos guias disponíveis |
| Portabilidade | ✅ 100% | Funciona em qualquer OS com Docker |
| Dependências | ✅ Containerizadas | Nada precisa ser instalado no host |

## 🎉 Conclusão

**SIM, o projeto está totalmente pronto para replicação fácil!**

Com apenas Docker Desktop instalado, você pode:
1. Copiar o projeto
2. Criar o `.env`
3. Executar `docker-compose up --build`
4. Pronto! Sistema funcionando

Não é necessário instalar Node.js, npm, ou qualquer outra dependência no computador de destino. Tudo está containerizado e pronto para uso.

---

**Desenvolvido para o TCC em Automação Industrial - ETEC Jorge Street**

