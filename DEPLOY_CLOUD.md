# ☁️ Deploy na Nuvem - Sleep Help

## 🎯 Opções de Deploy (Recomendadas para Demonstração)

### ⭐ **Opção 1: Railway + Vercel (GRATUITO - Melhor para Demo)**

#### **Backend no Railway:**
1. Acesse: https://railway.app
2. Faça login com GitHub
3. **New Project** → **Deploy from GitHub repo**
4. Selecione seu repositório
5. Railway detecta automaticamente o Docker
6. Configure variáveis de ambiente:
   ```
   NODE_ENV=production
   PORT=3001
   WHATSAPP_PHONE=5511956093997
   WHATSAPP_API_KEY=sua_chave
   ALARM_THRESHOLD=95
   ```
7. Railway gera URL automática: `https://seu-backend.railway.app`

#### **Frontend no Vercel:**
1. Acesse: https://vercel.com
2. Faça login com GitHub
3. **Add New Project** → Selecione repositório
4. Configure:
   - **Framework Preset**: Create React App
   - **Root Directory**: `./` (raiz)
   - **Build Command**: `npm run build`
   - **Output Directory**: `build`
   - **Environment Variables**:
     ```
     REACT_APP_API_URL=https://seu-backend.railway.app
     ```
5. Deploy automático!

---

### 🚀 **Opção 2: Render (Tudo em um lugar - GRATUITO)**

#### **Backend:**
1. Acesse: https://render.com
2. **New** → **Web Service**
3. Conecte GitHub
4. Configure:
   - **Name**: `sleep-help-backend`
   - **Environment**: `Docker`
   - **Dockerfile Path**: `backend/Dockerfile`
   - **Docker Context**: `backend/`
   - **Port**: `3001`
   - **Environment Variables**: (mesmas do Railway)

#### **Frontend:**
1. **New** → **Static Site**
2. Conecte GitHub
3. Configure:
   - **Name**: `sleep-help-frontend`
   - **Build Command**: `npm run build`
   - **Publish Directory**: `build`
   - **Environment Variables**:
     ```
     REACT_APP_API_URL=https://seu-backend.onrender.com
     ```

---

### 💰 **Opção 3: DigitalOcean App Platform ($5/mês)**

1. Acesse: https://cloud.digitalocean.com
2. **Create** → **App**
3. Conecte GitHub
4. Configure backend e frontend separadamente
5. Mais controle, melhor performance

---

## 📝 Passos para Deploy

### **1. Preparar Código para Nuvem**

#### Ajustar Frontend (já está configurado):
O frontend usa variável de ambiente `REACT_APP_API_URL`

#### Ajustar Backend:
O backend já está pronto para nuvem (porta via env)

### **2. Atualizar ESP32**

No código do ESP32, altere:
```cpp
// ANTES (local):
const char* serverURL = "http://localhost:3001/api/sensor-data";

// DEPOIS (nuvem):
const char* serverURL = "https://seu-backend.railway.app/api/sensor-data";
```

### **3. Configurar CORS no Backend**

O backend já tem CORS configurado para aceitar qualquer origem.

---

## 🔧 Arquivos de Configuração

### **vercel.json** (para Vercel)
```json
{
  "buildCommand": "npm run build",
  "outputDirectory": "build",
  "rewrites": [
    {
      "source": "/(.*)",
      "destination": "/index.html"
    }
  ]
}
```

### **render.yaml** (para Render)
```yaml
services:
  - type: web
    name: sleep-help-backend
    env: docker
    dockerfilePath: ./backend/Dockerfile
    dockerContext: ./backend
    envVars:
      - key: NODE_ENV
        value: production
      - key: PORT
        value: 3001

  - type: web
    name: sleep-help-frontend
    buildCommand: npm run build
    staticPublishPath: ./build
    envVars:
      - key: REACT_APP_API_URL
        fromService:
          name: sleep-help-backend
          type: web
          property: host
```

---

## ✅ Checklist de Deploy

- [ ] Backend deployado e URL obtida
- [ ] Frontend configurado com `REACT_APP_API_URL`
- [ ] Variáveis de ambiente configuradas
- [ ] CORS funcionando (testar API)
- [ ] ESP32 atualizado com nova URL
- [ ] Testar envio de dados do ESP32
- [ ] Testar recebimento no frontend

---

## 🧪 Testar Deploy

### **1. Testar Backend:**
```bash
curl https://seu-backend.railway.app/api/status
```

### **2. Testar Frontend:**
Acesse a URL do frontend e verifique se carrega

### **3. Testar ESP32:**
Envie dados do ESP32 e verifique se aparecem no dashboard

---

## 🚨 Problemas Comuns

### **CORS Error:**
- Verifique se backend tem CORS habilitado (já tem)
- Verifique se URL do frontend está correta

### **API não responde:**
- Verifique variáveis de ambiente
- Verifique logs no serviço de nuvem
- Verifique se porta está correta

### **Frontend não carrega:**
- Verifique `REACT_APP_API_URL`
- Verifique build logs
- Limpe cache do navegador

---

## 📱 URLs Finais

Após deploy, você terá:
- **Frontend**: `https://seu-frontend.vercel.app`
- **Backend**: `https://seu-backend.railway.app`
- **API**: `https://seu-backend.railway.app/api`

**Atualize o ESP32 com a URL do backend!**

