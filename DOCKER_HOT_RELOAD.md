# 🔥 Docker com Hot Reload - Atualizações Automáticas

## ✅ Configuração Criada

Agora você tem **2 modos** de usar o Docker:

### 1. **Produção** (sem hot-reload)
```bash
docker-compose up --build -d
```
- Código copiado na imagem
- Precisa rebuild para mudanças
- Otimizado para produção

### 2. **Desenvolvimento** (com hot-reload) ⭐ NOVO
```bash
docker-compose -f docker-compose.dev.yml up --build -d
```
- Código sincronizado via volumes
- **Atualizações automáticas** ao salvar arquivos
- Backend reinicia automaticamente (nodemon)
- Frontend recarrega automaticamente (React)

## 🚀 Como Usar o Modo Desenvolvimento

### Iniciar com Hot Reload:
```bash
docker-compose -f docker-compose.dev.yml up --build -d
```

### Ver logs:
```bash
docker-compose -f docker-compose.dev.yml logs -f
```

### Parar:
```bash
docker-compose -f docker-compose.dev.yml down
```

## 📝 O que acontece automaticamente:

✅ **Backend**: Qualquer mudança em `backend/*.js` → nodemon reinicia automaticamente  
✅ **Frontend**: Qualquer mudança em `src/*` ou `public/*` → React recarrega automaticamente  
✅ **Arquivo .env**: Mudanças são lidas automaticamente (só reiniciar backend)

## ⚠️ Importante:

- **Modo Dev**: Porta 3000 (React dev server)
- **Modo Prod**: Porta 3000 (nginx com build estático)
- Não use os dois modos ao mesmo tempo (conflito de portas)

## 🔄 Trocar entre Modos:

**Parar tudo:**
```bash
docker-compose down
docker-compose -f docker-compose.dev.yml down
```

**Iniciar modo desejado:**
```bash
# Desenvolvimento (hot-reload)
docker-compose -f docker-compose.dev.yml up -d

# Produção (sem hot-reload)
docker-compose up -d
```

