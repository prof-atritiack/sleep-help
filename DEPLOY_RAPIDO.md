# ⚡ Deploy Rápido - Sleep Help na Nuvem

## 🎯 Deploy em 15 Minutos

### **Opção 1: Vercel + Railway (Recomendado)**

#### **Passo 1: Preparar Repositório (2 min)**
```bash
# Criar repositório no GitHub
git init
git add .
git commit -m "Initial commit"
git branch -M main
git remote add origin https://github.com/seu-usuario/sleep-help.git
git push -u origin main
```

#### **Passo 2: Deploy Backend - Railway (5 min)**
1. **Acesse**: https://railway.app
2. **Login** com GitHub
3. **New Project** → **Deploy from GitHub repo**
4. **Selecione** seu repositório
5. **Configure**:
   - Root Directory: `backend/`
   - Build Command: `npm install`
   - Start Command: `node server.js`
6. **Deploy** automático
7. **Copie** a URL gerada: `https://seu-projeto.railway.app`

#### **Passo 3: Deploy Frontend - Vercel (5 min)**
1. **Acesse**: https://vercel.com
2. **Login** com GitHub
3. **Import Project** → **Selecione** seu repositório
4. **Configure**:
   - Framework: Create React App
   - Root Directory: `./` (raiz)
   - Build Command: `npm run build`
   - Output Directory: `build`
5. **Environment Variables**:
   ```
   REACT_APP_API_URL=https://seu-projeto.railway.app
   ```
6. **Deploy** automático
7. **Copie** a URL gerada: `https://seu-projeto.vercel.app`

#### **Passo 4: Atualizar ESP32 (3 min)**
```cpp
// No arquivo TRANSMISSOR_ESP32_V3_API.ino
const char* serverURL = "https://seu-projeto.railway.app/api/sensor-data";
```

### **Opção 2: Netlify + Render (Alternativa)**

#### **Passo 1: Deploy Backend - Render (5 min)**
1. **Acesse**: https://render.com
2. **Login** com GitHub
3. **New** → **Web Service**
4. **Selecione** seu repositório
5. **Configure**:
   - Root Directory: `backend/`
   - Build Command: `npm install`
   - Start Command: `node server.js`
6. **Deploy** automático

#### **Passo 2: Deploy Frontend - Netlify (5 min)**
1. **Acesse**: https://netlify.com
2. **Login** com GitHub
3. **New site from Git** → **GitHub**
4. **Selecione** seu repositório
5. **Configure**:
   - Build command: `npm run build`
   - Publish directory: `build/`
6. **Environment Variables**:
   ```
   REACT_APP_API_URL=https://seu-backend.onrender.com
   ```
7. **Deploy** automático

## 🔧 Configurações Essenciais

### **Backend (Railway/Render)**
```javascript
// Adicionar no server.js
const cors = require('cors');

app.use(cors({
  origin: [
    'https://seu-frontend.vercel.app',
    'https://seu-frontend.netlify.app',
    'http://localhost:3000'
  ],
  credentials: true
}));
```

### **Frontend (Vercel/Netlify)**
```javascript
// Verificar se está usando a variável de ambiente
const API_URL = process.env.REACT_APP_API_URL || 'http://localhost:3001';
```

### **ESP32**
```cpp
// Configurar Wi-Fi
const char* ssid = "sua_rede_wifi";
const char* password = "sua_senha";

// Configurar URL da API
const char* serverURL = "https://sua-api.railway.app/api/sensor-data";
```

## 🚨 Solução de Problemas Rápida

### **Backend não inicia**
```bash
# Verificar logs
railway logs

# Verificar variáveis
railway variables

# Reiniciar
railway restart
```

### **Frontend não carrega**
```bash
# Verificar logs
vercel logs

# Rebuild
vercel --prod
```

### **ESP32 não conecta**
- Verificar URL da API
- Verificar Wi-Fi
- Verificar certificados SSL

## 📱 URLs Finais

### **Desenvolvimento**
- Frontend: http://localhost:3000
- Backend: http://localhost:3001

### **Produção**
- Frontend: https://seu-projeto.vercel.app
- Backend: https://seu-projeto.railway.app
- API: https://seu-projeto.railway.app/api/status

## 🎯 Próximos Passos

1. **Testar** aplicação web
2. **Testar** API com ESP32
3. **Configurar** domínio personalizado (opcional)
4. **Configurar** monitoramento (opcional)
5. **Documentar** URLs para apresentação

## 💡 Dicas Importantes

### **Para Apresentação do TCC**
- Use URLs curtas e memoráveis
- Teste tudo antes da apresentação
- Tenha backup local funcionando
- Documente as URLs no README

### **Para Desenvolvimento Contínuo**
- Configure deploy automático
- Use branches para features
- Monitore logs regularmente
- Mantenha backups

---

**Deploy em 15 minutos - Sleep Help na Nuvem! 🚀**
