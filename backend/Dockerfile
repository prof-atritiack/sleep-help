# Use a imagem oficial do Node.js como base
FROM node:18-alpine

# Definir o diretório de trabalho
WORKDIR /app

# Copiar package.json e package-lock.json
COPY package*.json ./

# Instalar dependências (incluindo dotenv para variáveis de ambiente)
RUN npm ci --only=production

# Copiar o código fonte
COPY . .

# Expor a porta 3001
EXPOSE 3001

# Comando para iniciar o servidor
# Nota: Variáveis de ambiente devem ser passadas via docker-compose.yml ou env_file
CMD ["node", "server.js"]
