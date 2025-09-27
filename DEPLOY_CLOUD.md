# ☁️ Deploy Sleep Help na Nuvem

## 🎯 Estratégia Recomendada: Vercel + Railway

### **Por que esta combinação?**
- ✅ **Gratuito** para projetos pessoais/TCC
- ✅ **Fácil configuração** com Docker
- ✅ **Deploy automático** via Git
- ✅ **SSL automático** e domínio personalizado
- ✅ **Escalabilidade** para crescimento futuro

## 🚀 Deploy Passo a Passo

### **1. Preparação do Projeto**

#### Separar Frontend e Backend
```bash
# Estrutura recomendada para deploy
sleep-help/
├── frontend/          # Aplicação React
├── backend/           # API Node.js
├── docker-compose.yml # Para desenvolvimento local
└── README.md
```

### **2. Deploy do Backend (Railway)**

#### Criar conta no Railway
1. Acesse: https://railway.app
2. Faça login com GitHub
3. Conecte seu repositório

#### Configurar Backend
1. **Criar novo projeto** no Railway
2. **Conectar repositório** GitHub
3. **Selecionar pasta** `backend/`
4. **Configurar variáveis** de ambiente:
   ```
   NODE_ENV=production
   PORT=3001
   ```

#### Deploy automático
- Railway detecta automaticamente o `package.json`
- Faz build e deploy do Node.js
- Gera URL pública: `https://seu-projeto.railway.app`

### **3. Deploy do Frontend (Vercel)**

#### Criar conta no Vercel
1. Acesse: https://vercel.com
2. Faça login com GitHub
3. Conecte seu repositório

#### Configurar Frontend
1. **Importar projeto** do GitHub
2. **Configurar build**:
   - Framework: Create React App
   - Root Directory: `frontend/`
3. **Configurar variáveis** de ambiente:
   ```
   REACT_APP_API_URL=https://seu-projeto.railway.app
   ```

#### Deploy automático
- Vercel detecta automaticamente o React
- Faz build e deploy
- Gera URL pública: `https://seu-projeto.vercel.app`

### **4. Configuração do ESP32**

#### Atualizar URL da API
```cpp
// No código do ESP32
const char* serverURL = "https://seu-projeto.railway.app/api/sensor-data";
```

## 🔧 Configurações Avançadas

### **Railway (Backend)**

#### Configurar domínio personalizado
1. **Railway Dashboard** → **Settings** → **Domains**
2. **Adicionar domínio** personalizado
3. **Configurar DNS** no seu provedor

#### Configurar variáveis de ambiente
```bash
# No Railway Dashboard
NODE_ENV=production
PORT=3001
CORS_ORIGIN=https://seu-projeto.vercel.app
```

#### Configurar banco de dados (opcional)
1. **Adicionar serviço** PostgreSQL
2. **Configurar conexão** no código
3. **Migrar dados** para produção

### **Vercel (Frontend)**

#### Configurar domínio personalizado
1. **Vercel Dashboard** → **Settings** → **Domains**
2. **Adicionar domínio** personalizado
3. **Configurar DNS** no seu provedor

#### Configurar variáveis de ambiente
```bash
# No Vercel Dashboard
REACT_APP_API_URL=https://seu-projeto.railway.app
REACT_APP_ENV=production
```

#### Configurar build personalizado
```json
// vercel.json
{
  "builds": [
    {
      "src": "package.json",
      "use": "@vercel/static-build",
      "config": {
        "distDir": "build"
      }
    }
  ],
  "routes": [
    {
      "src": "/(.*)",
      "dest": "/index.html"
    }
  ]
}
```

## 📊 Monitoramento e Logs

### **Railway (Backend)**
```bash
# Ver logs em tempo real
railway logs

# Ver métricas
railway metrics

# Acessar terminal
railway shell
```

### **Vercel (Frontend)**
```bash
# Ver logs de build
vercel logs

# Ver métricas
vercel analytics

# Acessar terminal
vercel dev
```

## 🔄 Deploy Automático

### **Configurar GitHub Actions**

#### Workflow para Backend (Railway)
```yaml
# .github/workflows/railway.yml
name: Deploy to Railway
on:
  push:
    branches: [main]
    paths: ['backend/**']
jobs:
  deploy:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - uses: railway-app/railway-deploy@v1
        with:
          railway-token: ${{ secrets.RAILWAY_TOKEN }}
```

#### Workflow para Frontend (Vercel)
```yaml
# .github/workflows/vercel.yml
name: Deploy to Vercel
on:
  push:
    branches: [main]
    paths: ['frontend/**']
jobs:
  deploy:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - uses: amondnet/vercel-action@v20
        with:
          vercel-token: ${{ secrets.VERCEL_TOKEN }}
          vercel-org-id: ${{ secrets.ORG_ID }}
          vercel-project-id: ${{ secrets.PROJECT_ID }}
```

## 🚨 Solução de Problemas

### **Problemas Comuns**

#### Backend não inicia
```bash
# Verificar logs
railway logs

# Verificar variáveis de ambiente
railway variables

# Reiniciar serviço
railway restart
```

#### Frontend não carrega
```bash
# Verificar logs de build
vercel logs

# Verificar variáveis de ambiente
vercel env

# Rebuild manual
vercel --prod
```

#### ESP32 não conecta
- Verificar URL da API
- Verificar certificados SSL
- Verificar CORS no backend

### **Logs Úteis**
```bash
# Railway
railway logs --follow

# Vercel
vercel logs --follow

# GitHub Actions
gh run list
gh run view [run-id]
```

## 💰 Custos

### **Gratuito (Recomendado para TCC)**
- **Railway**: $5/mês de crédito gratuito
- **Vercel**: 100GB de bandwidth gratuito
- **Total**: $0/mês

### **Pago (Produção)**
- **Railway**: $5/mês + uso
- **Vercel**: $20/mês (Pro)
- **Total**: ~$25/mês

## 🔐 Segurança

### **Configurar HTTPS**
- ✅ **Railway**: HTTPS automático
- ✅ **Vercel**: HTTPS automático
- ✅ **ESP32**: Suporte a HTTPS

### **Configurar CORS**
```javascript
// backend/server.js
app.use(cors({
  origin: ['https://seu-projeto.vercel.app'],
  credentials: true
}));
```

### **Configurar Rate Limiting**
```javascript
// backend/server.js
const rateLimit = require('express-rate-limit');

const limiter = rateLimit({
  windowMs: 15 * 60 * 1000, // 15 minutos
  max: 100 // máximo 100 requests por IP
});

app.use('/api/', limiter);
```

## 📱 URLs Finais

### **Produção**
- **Frontend**: https://seu-projeto.vercel.app
- **Backend**: https://seu-projeto.railway.app
- **API**: https://seu-projeto.railway.app/api/status

### **Desenvolvimento**
- **Frontend**: http://localhost:3000
- **Backend**: http://localhost:3001

## 🎯 Próximos Passos

1. **Criar contas** no Railway e Vercel
2. **Configurar repositório** GitHub
3. **Fazer deploy** do backend
4. **Fazer deploy** do frontend
5. **Atualizar ESP32** com nova URL
6. **Testar** sistema completo
7. **Configurar domínio** personalizado (opcional)

---

**Desenvolvido para o TCC em Automação Industrial - ETEC Jorge Street**
