# Use a imagem oficial do Node.js como base
FROM node:18-alpine

# Definir o diretório de trabalho
WORKDIR /app

# Copiar package.json e package-lock.json
COPY package*.json ./

# Instalar dependências
RUN npm ci --only=production

# Copiar o código fonte
COPY . .

# Expor a porta 3001
EXPOSE 3001

# Comando para iniciar o servidor
CMD ["node", "server.js"]
