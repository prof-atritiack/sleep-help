# ☁️ Alternativas de Deploy na Nuvem

## 🎯 Opções por Categoria

### **1. 🆓 Gratuitas (Ideal para TCC)**

#### **Netlify + Render**
- **Frontend**: Netlify (estático)
- **Backend**: Render (Node.js)
- **Custo**: $0/mês
- **Vantagens**: Fácil configuração, SSL automático
- **Desvantagens**: Limitações de recursos

#### **GitHub Pages + Heroku**
- **Frontend**: GitHub Pages
- **Backend**: Heroku
- **Custo**: $0/mês (com limitações)
- **Vantagens**: Integração com GitHub
- **Desvantagens**: Heroku removeu plano gratuito

#### **Firebase + Cloud Run**
- **Frontend**: Firebase Hosting
- **Backend**: Google Cloud Run
- **Custo**: $0/mês (com limites)
- **Vantagens**: Integração Google, escalabilidade
- **Desvantagens**: Configuração mais complexa

### **2. 💰 Pagas (Produção)**

#### **AWS (Amazon Web Services)**
- **Frontend**: S3 + CloudFront
- **Backend**: EC2 + RDS
- **Custo**: ~$20-50/mês
- **Vantagens**: Máxima flexibilidade, escalabilidade
- **Desvantagens**: Complexidade, custo

#### **Google Cloud Platform**
- **Frontend**: Cloud Storage + CDN
- **Backend**: Compute Engine + Cloud SQL
- **Custo**: ~$15-40/mês
- **Vantagens**: Integração Google, machine learning
- **Desvantagens**: Curva de aprendizado

#### **Microsoft Azure**
- **Frontend**: Static Web Apps
- **Backend**: App Service + Database
- **Custo**: ~$10-30/mês
- **Vantagens**: Integração Microsoft, enterprise
- **Desvantagens**: Menos popular que AWS/GCP

### **3. 🐳 Docker (Containerizado)**

#### **DigitalOcean**
- **Plataforma**: Droplet com Docker
- **Custo**: $5-20/mês
- **Vantagens**: Simplicidade, preço fixo
- **Desvantagens**: Menos recursos que AWS

#### **Linode**
- **Plataforma**: VPS com Docker
- **Custo**: $5-20/mês
- **Vantagens**: Performance, suporte
- **Desvantagens**: Menos serviços gerenciados

#### **AWS ECS**
- **Plataforma**: Elastic Container Service
- **Custo**: $10-50/mês
- **Vantagens**: Escalabilidade, integração AWS
- **Desvantagens**: Complexidade, custo

## 🚀 Guias de Deploy Específicos

### **Netlify + Render**

#### Deploy Frontend (Netlify)
1. **Criar conta** no Netlify
2. **Conectar repositório** GitHub
3. **Configurar build**:
   - Build command: `npm run build`
   - Publish directory: `build/`
4. **Configurar variáveis**:
   ```
   REACT_APP_API_URL=https://seu-backend.onrender.com
   ```

#### Deploy Backend (Render)
1. **Criar conta** no Render
2. **Conectar repositório** GitHub
3. **Configurar serviço**:
   - Runtime: Node.js
   - Build command: `npm install`
   - Start command: `node server.js`
4. **Configurar variáveis**:
   ```
   NODE_ENV=production
   PORT=3001
   ```

### **DigitalOcean (Docker)**

#### Criar Droplet
1. **Criar conta** no DigitalOcean
2. **Criar Droplet**:
   - Imagem: Ubuntu 22.04
   - Tamanho: $5/mês (1GB RAM)
   - Região: Mais próxima
3. **Configurar SSH** e acessar

#### Instalar Docker
```bash
# Atualizar sistema
sudo apt update && sudo apt upgrade -y

# Instalar Docker
curl -fsSL https://get.docker.com -o get-docker.sh
sudo sh get-docker.sh

# Instalar Docker Compose
sudo curl -L "https://github.com/docker/compose/releases/latest/download/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
sudo chmod +x /usr/local/bin/docker-compose
```

#### Deploy da Aplicação
```bash
# Clonar repositório
git clone https://github.com/seu-usuario/sleep-help.git
cd sleep-help

# Configurar variáveis de ambiente
cp .env.example .env
nano .env

# Iniciar aplicação
docker-compose up -d

# Verificar status
docker-compose ps
```

#### Configurar Nginx (Proxy Reverso)
```bash
# Instalar Nginx
sudo apt install nginx -y

# Configurar proxy
sudo nano /etc/nginx/sites-available/sleep-help

# Conteúdo do arquivo
server {
    listen 80;
    server_name seu-dominio.com;

    location / {
        proxy_pass http://localhost:3000;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
    }

    location /api {
        proxy_pass http://localhost:3001;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
    }
}

# Ativar configuração
sudo ln -s /etc/nginx/sites-available/sleep-help /etc/nginx/sites-enabled/
sudo nginx -t
sudo systemctl reload nginx
```

#### Configurar SSL (Let's Encrypt)
```bash
# Instalar Certbot
sudo apt install certbot python3-certbot-nginx -y

# Obter certificado SSL
sudo certbot --nginx -d seu-dominio.com

# Verificar renovação automática
sudo certbot renew --dry-run
```

### **AWS (Completo)**

#### Frontend (S3 + CloudFront)
1. **Criar bucket S3**:
   - Nome: `sleep-help-frontend`
   - Região: Mais próxima
   - Configurar como site estático

2. **Configurar CloudFront**:
   - Origin: Bucket S3
   - Comportamentos: Cache otimizado
   - Domínio personalizado (opcional)

3. **Deploy**:
   ```bash
   # Build da aplicação
   npm run build
   
   # Upload para S3
   aws s3 sync build/ s3://sleep-help-frontend
   ```

#### Backend (EC2 + RDS)
1. **Criar instância EC2**:
   - AMI: Ubuntu 22.04
   - Tipo: t2.micro (gratuito)
   - Security Group: Portas 22, 80, 443, 3001

2. **Configurar RDS**:
   - Engine: PostgreSQL
   - Instance: db.t3.micro (gratuito)
   - Configurar security group

3. **Deploy**:
   ```bash
   # Conectar via SSH
   ssh -i sua-chave.pem ubuntu@seu-ip

   # Instalar Docker
   curl -fsSL https://get.docker.com -o get-docker.sh
   sudo sh get-docker.sh

   # Deploy da aplicação
   git clone https://github.com/seu-usuario/sleep-help.git
   cd sleep-help
   docker-compose up -d
   ```

## 🔧 Configurações Específicas

### **Variáveis de Ambiente por Plataforma**

#### Railway
```bash
NODE_ENV=production
PORT=3001
CORS_ORIGIN=https://seu-frontend.vercel.app
```

#### Render
```bash
NODE_ENV=production
PORT=3001
CORS_ORIGIN=https://seu-frontend.netlify.app
```

#### DigitalOcean
```bash
NODE_ENV=production
PORT=3001
CORS_ORIGIN=https://seu-dominio.com
```

#### AWS
```bash
NODE_ENV=production
PORT=3001
CORS_ORIGIN=https://seu-dominio.com
DATABASE_URL=postgresql://user:pass@rds-endpoint:5432/sleephelp
```

### **Configurações de CORS**

#### Desenvolvimento
```javascript
app.use(cors({
  origin: ['http://localhost:3000'],
  credentials: true
}));
```

#### Produção
```javascript
app.use(cors({
  origin: [
    'https://seu-frontend.vercel.app',
    'https://seu-frontend.netlify.app',
    'https://seu-dominio.com'
  ],
  credentials: true
}));
```

## 📊 Comparação de Custos

| Plataforma | Frontend | Backend | Custo/Mês | Dificuldade |
|------------|----------|---------|-----------|-------------|
| **Vercel + Railway** | Gratuito | $5 crédito | $0 | Fácil |
| **Netlify + Render** | Gratuito | Gratuito | $0 | Fácil |
| **DigitalOcean** | Docker | Docker | $5-20 | Média |
| **AWS** | S3 + CloudFront | EC2 + RDS | $20-50 | Difícil |
| **Google Cloud** | Storage + CDN | Compute + SQL | $15-40 | Difícil |

## 🎯 Recomendação Final

### **Para TCC (Gratuito)**
1. **Vercel + Railway** (Recomendado)
2. **Netlify + Render** (Alternativa)
3. **GitHub Pages + Heroku** (Limitado)

### **Para Produção (Pago)**
1. **DigitalOcean** (Simplicidade)
2. **AWS** (Escalabilidade)
3. **Google Cloud** (Integração)

### **Para Aprendizado**
1. **DigitalOcean** (Docker)
2. **AWS** (Cloud completo)
3. **Vercel + Railway** (Serverless)

---

**Desenvolvido para o TCC em Automação Industrial - ETEC Jorge Street**
