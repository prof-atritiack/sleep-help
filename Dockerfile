# Use a imagem oficial do Node.js como base (não-alpine para evitar problemas de permissão)
FROM node:18

# Definir o diretório de trabalho
WORKDIR /app

# Copiar package.json e package-lock.json
COPY package*.json ./

# Instalar dependências (incluindo devDependencies para build)
RUN npm ci

# Copiar o código fonte
COPY . .

# Construir a aplicação para produção
RUN npm run build

# Usar nginx para servir os arquivos estáticos
FROM nginx:alpine

# Copiar os arquivos construídos do React
COPY --from=0 /app/build /usr/share/nginx/html

# Copiar configuração customizada do nginx
COPY nginx.conf /etc/nginx/nginx.conf

# Expor a porta 80
EXPOSE 80

# Comando para iniciar o nginx
CMD ["nginx", "-g", "daemon off;"]
