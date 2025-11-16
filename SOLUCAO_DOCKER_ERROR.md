# 🔧 Solução para Erro de Build Docker

## ❌ Erro Atual:
```
sh: react-scripts: Permission denied
```

## ✅ Soluções (tente nesta ordem):

### **1. Verificar Branch do Deploy**

**No Railway/Vercel/Render:**
- Vá em **Settings** → **Build & Deploy**
- Verifique qual **branch** está configurada
- Se estiver em `main`, mas você está em `feature/whatsapp-integration`:
  - **Opção A**: Mude a branch do deploy para `feature/whatsapp-integration`
  - **Opção B**: Faça merge para `main` e faça push

### **2. Fazer Push das Mudanças**

```bash
# Verificar branch atual
git branch

# Adicionar Dockerfile corrigido
git add Dockerfile

# Commit
git commit -m "fix: Dockerfile corrigido para node:18"

# Push para a branch do deploy
git push origin feature/whatsapp-integration

# OU se o deploy usa main:
git checkout main
git merge feature/whatsapp-integration
git push origin main
```

### **3. Limpar Cache do Build**

**No Railway:**
- Settings → Clear Build Cache → Deploy novamente

**No Render:**
- Settings → Clear Build Cache → Manual Deploy

**No Vercel:**
- Settings → Clear Build Cache → Redeploy

### **4. Verificar Dockerfile no Repositório**

Acesse seu repositório no GitHub e verifique se o Dockerfile tem:
```dockerfile
FROM node:18
```
(NÃO `node:18-alpine`)

---

## 🚀 Solução Rápida (Recomendada):

1. **Verificar qual branch o deploy usa**
2. **Fazer push do Dockerfile corrigido para essa branch**
3. **Limpar cache e fazer novo deploy**

---

## 📝 Dockerfile Correto (já está no código):

```dockerfile
FROM node:18
WORKDIR /app
COPY package*.json ./
RUN npm ci
COPY . .
RUN npm run build
FROM nginx:alpine
COPY --from=0 /app/build /usr/share/nginx/html
COPY nginx.conf /etc/nginx/nginx.conf
EXPOSE 80
CMD ["nginx", "-g", "daemon off;"]
```

