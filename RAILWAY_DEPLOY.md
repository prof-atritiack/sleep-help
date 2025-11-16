# 🚂 Deploy no Railway - Guia Completo

## ✅ Passo a Passo para Corrigir o Erro

### **1. Verificar Branch no Railway**

1. Acesse: https://railway.app
2. Selecione seu projeto `sleep-help`
3. Vá em **Settings** → **Source**
4. Veja qual **branch** está configurada
   - Se for `main` → precisa fazer merge
   - Se for `feature/whatsapp-integration` → só fazer push

### **2. Fazer Push do Dockerfile Corrigido**

**Se Railway usa `feature/whatsapp-integration`:**
```bash
git push origin feature/whatsapp-integration
```

**Se Railway usa `main`:**
```bash
git checkout main
git merge feature/whatsapp-integration
git push origin main
```

### **3. Limpar Cache e Fazer Novo Deploy no Railway**

1. No Railway, vá em **Settings** → **Deploy**
2. Clique em **Clear Build Cache**
3. Vá em **Deployments** → **Redeploy** (ou aguarde auto-deploy)

---

## 🔧 Configuração do Railway

### **Backend (Node.js):**

1. **New Project** → **Deploy from GitHub repo**
2. Selecione: `prof-atritiack/sleep-help`
3. Railway detecta automaticamente o Docker
4. Configure:
   - **Root Directory**: `backend/`
   - **Dockerfile Path**: `backend/Dockerfile`
   - **Branch**: `feature/whatsapp-integration` (ou `main`)

### **Variáveis de Ambiente:**

No Railway, vá em **Variables** e adicione:
```
NODE_ENV=production
PORT=3001
WHATSAPP_PHONE=5511956093997
WHATSAPP_API_KEY=sua_chave
ALARM_THRESHOLD=95
```

### **Frontend (React):**

1. **New Service** → **GitHub Repo**
2. Selecione o mesmo repositório
3. Configure:
   - **Root Directory**: `/` (raiz)
   - **Build Command**: `npm run build`
   - **Start Command**: (deixe vazio, usa Dockerfile)
   - **Output Directory**: `build`
   - **Branch**: `feature/whatsapp-integration` (ou `main`)

### **Variáveis de Ambiente do Frontend:**

```
REACT_APP_API_URL=https://seu-backend.railway.app
```

---

## 🐳 Usando Docker no Railway

O Railway detecta automaticamente o `Dockerfile` na raiz.

**Para o Frontend:**
- Railway usa o `Dockerfile` da raiz
- Já está corrigido com `FROM node:18`

**Para o Backend:**
- Railway precisa usar `backend/Dockerfile`
- Configure **Root Directory**: `backend/`

---

## ✅ Checklist

- [ ] Verificar branch no Railway
- [ ] Fazer push do Dockerfile corrigido
- [ ] Limpar cache no Railway
- [ ] Fazer novo deploy
- [ ] Verificar logs do build
- [ ] Testar URL do backend

---

## 🚨 Se Ainda Der Erro

1. **Verificar logs no Railway:**
   - Vá em **Deployments** → Clique no último deploy → **View Logs**
   - Procure por erros específicos

2. **Verificar Dockerfile no GitHub:**
   - Acesse: https://github.com/prof-atritiack/sleep-help/blob/feature/whatsapp-integration/Dockerfile
   - Deve ter `FROM node:18` (NÃO alpine)

3. **Forçar rebuild:**
   - Settings → Deploy → **Clear Build Cache**
   - Deployments → **Redeploy**

---

## 📝 Comandos Úteis

```bash
# Ver branch atual
git branch

# Push para branch do Railway
git push origin feature/whatsapp-integration

# Ver commits recentes
git log --oneline -5
```

