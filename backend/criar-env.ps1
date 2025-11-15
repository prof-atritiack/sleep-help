# Script PowerShell para criar arquivo .env
# Execute: .\criar-env.ps1

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Configuração WhatsApp - CallMeBot" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Verificar se .env já existe
if (Test-Path ".env") {
    $resposta = Read-Host "O arquivo .env já existe. Deseja sobrescrever? (s/n)"
    if ($resposta -ne "s" -and $resposta -ne "S") {
        Write-Host "Operação cancelada." -ForegroundColor Yellow
        exit
    }
}

Write-Host "Por favor, preencha as informações:" -ForegroundColor Green
Write-Host ""

# Solicitar número de telefone
Write-Host "Número de telefone para receber alertas" -ForegroundColor Yellow
Write-Host "Formato: código do país + DDD + número (sem espaços)" -ForegroundColor Gray
Write-Host "Exemplo Brasil: 5511999999999 (55 + 11 + 999999999)" -ForegroundColor Gray
$phone = Read-Host "WHATSAPP_PHONE"

# Solicitar chave da API
Write-Host ""
Write-Host "Chave da API do CallMeBot" -ForegroundColor Yellow
Write-Host "Cole a chave que você recebeu:" -ForegroundColor Gray
$apiKey = Read-Host "WHATSAPP_API_KEY"

# Solicitar threshold (opcional)
Write-Host ""
Write-Host "Threshold de alarme (SpO2 <= valor gera alarme)" -ForegroundColor Yellow
Write-Host "Pressione Enter para usar o padrão (95)" -ForegroundColor Gray
$threshold = Read-Host "ALARM_THRESHOLD"
if ([string]::IsNullOrWhiteSpace($threshold)) {
    $threshold = "95"
}

# Solicitar porta (opcional)
Write-Host ""
Write-Host "Porta do servidor" -ForegroundColor Yellow
Write-Host "Pressione Enter para usar o padrão (3001)" -ForegroundColor Gray
$port = Read-Host "PORT"
if ([string]::IsNullOrWhiteSpace($port)) {
    $port = "3001"
}

# Criar conteúdo do .env
$envContent = @"
# Configuração WhatsApp - CallMeBot API
WHATSAPP_PHONE=$phone
WHATSAPP_API_KEY=$apiKey
ALARM_THRESHOLD=$threshold

# Configurações do Servidor
PORT=$port
"@

# Salvar arquivo
$envContent | Out-File -FilePath ".env" -Encoding UTF8 -NoNewline

Write-Host ""
Write-Host "✅ Arquivo .env criado com sucesso!" -ForegroundColor Green
Write-Host ""
Write-Host "Conteúdo criado:" -ForegroundColor Cyan
Write-Host $envContent -ForegroundColor White
Write-Host ""
Write-Host "Próximos passos:" -ForegroundColor Yellow
Write-Host "1. Verifique se os dados estão corretos acima" -ForegroundColor White
Write-Host "2. Reinicie o servidor: npm run dev" -ForegroundColor White
Write-Host "3. Teste enviando dados com SpO2 <= $threshold" -ForegroundColor White

