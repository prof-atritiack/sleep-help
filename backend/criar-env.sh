#!/bin/bash
# Script bash para criar arquivo .env
# Execute: chmod +x criar-env.sh && ./criar-env.sh

echo "========================================"
echo "  Configuração WhatsApp - CallMeBot"
echo "========================================"
echo ""

# Verificar se .env já existe
if [ -f ".env" ]; then
    read -p "O arquivo .env já existe. Deseja sobrescrever? (s/n): " resposta
    if [ "$resposta" != "s" ] && [ "$resposta" != "S" ]; then
        echo "Operação cancelada."
        exit
    fi
fi

echo "Por favor, preencha as informações:"
echo ""

# Solicitar número de telefone
echo "Número de telefone para receber alertas"
echo "Formato: código do país + DDD + número (sem espaços)"
echo "Exemplo Brasil: 5511999999999 (55 + 11 + 999999999)"
read -p "WHATSAPP_PHONE: " phone

# Solicitar chave da API
echo ""
echo "Chave da API do CallMeBot"
echo "Cole a chave que você recebeu:"
read -p "WHATSAPP_API_KEY: " apiKey

# Solicitar threshold (opcional)
echo ""
echo "Threshold de alarme (SpO2 <= valor gera alarme)"
echo "Pressione Enter para usar o padrão (95)"
read -p "ALARM_THRESHOLD: " threshold
if [ -z "$threshold" ]; then
    threshold="95"
fi

# Solicitar porta (opcional)
echo ""
echo "Porta do servidor"
echo "Pressione Enter para usar o padrão (3001)"
read -p "PORT: " port
if [ -z "$port" ]; then
    port="3001"
fi

# Criar arquivo .env
cat > .env << EOF
# Configuração WhatsApp - CallMeBot API
WHATSAPP_PHONE=$phone
WHATSAPP_API_KEY=$apiKey
ALARM_THRESHOLD=$threshold

# Configurações do Servidor
PORT=$port
EOF

echo ""
echo "✅ Arquivo .env criado com sucesso!"
echo ""
echo "Conteúdo criado:"
cat .env
echo ""
echo "Próximos passos:"
echo "1. Verifique se os dados estão corretos acima"
echo "2. Reinicie o servidor: npm run dev"
echo "3. Teste enviando dados com SpO2 <= $threshold"

