# 📱 Integração WhatsApp - Documentação

## Visão Geral

O sistema possui integração com WhatsApp para envio automático de alertas quando valores críticos de SpO2 são detectados. A integração utiliza a **CallMeBot API**, que é gratuita e não requer WhatsApp Business.

## 🔧 Como Funciona

### Arquitetura

1. **ESP32** envia dados de SpO2 para o backend via POST `/api/sensor-data`
2. **Backend** verifica se o valor de SpO2 está abaixo do threshold configurado
3. Se detectar alarme, o sistema:
   - Verifica se WhatsApp está configurado
   - Verifica se passou o tempo de cooldown (1 minuto)
   - Envia mensagem via CallMeBot API
   - Registra o envio para evitar spam

### Fluxo de Detecção de Alarme

```javascript
if (SpO2 <= ALARM_THRESHOLD) {
  if (WHATSAPP_CONFIGURADO && PASSAR_COOLDOWN) {
    enviarWhatsApp(SpO2, timestamp);
  }
}
```

## 📋 Configuração

### Passo 1: Obter Chave da API CallMeBot

1. Adicione o número **+34 644 97 54 14** aos seus contatos do WhatsApp
2. Envie a mensagem exatamente assim: **"Eu permito que o callmebot me envie mensagens"**
3. Aguarde a resposta automática com sua chave de API pessoal
4. Anote a chave recebida

### Passo 2: Configurar Variáveis de Ambiente

Crie um arquivo `.env` na pasta `backend/` ou configure as variáveis de ambiente:

```bash
# Número de telefone para receber alertas
# Formato: código do país + DDD + número (sem espaços, sem caracteres especiais)
WHATSAPP_PHONE=5511999999999

# Chave da API do CallMeBot
WHATSAPP_API_KEY=sua_chave_aqui

# Threshold de alarme (opcional, padrão: 95)
# SpO2 <= este valor gera alerta
ALARM_THRESHOLD=95
```

### Passo 3: Formato do Número

O número deve estar no formato internacional, sem espaços ou caracteres especiais:

- **Brasil**: `5511999999999` (55 = código país, 11 = DDD, 999999999 = número)
- **Outros países**: `código_país + número_completo`

Exemplos:
- Brasil: `5511999999999`
- EUA: `15551234567`
- Portugal: `351912345678`

### Passo 4: Reiniciar o Servidor

Após configurar as variáveis de ambiente, reinicie o servidor:

```bash
# Desenvolvimento
npm run dev

# Produção
npm start

# Docker
docker-compose restart backend
```

## 🚨 Funcionalidades

### Detecção Automática de Alarme

O sistema detecta automaticamente quando:
- SpO2 ≤ 95% (ou valor configurado em `ALARM_THRESHOLD`)
- Dados válidos são recebidos do ESP32

### Cooldown (Prevenção de Spam)

Para evitar múltiplas mensagens seguidas, o sistema possui um **cooldown de 1 minuto**:
- Se um alarme foi enviado, aguarda 60 segundos antes de enviar outro
- Logs indicam quando está aguardando o cooldown

### Mensagem de Alerta

A mensagem enviada inclui:
- ⚠️ Indicador de alerta
- 📊 Valor de SpO2 detectado
- ⏰ Horário da medição
- 🔴 Instrução para verificar o paciente

Exemplo:
```
🚨 *ALERTA DE SATURAÇÃO DE OXIGÊNIO*

⚠️ Valores críticos detectados!

📊 SpO2: 92%
⏰ Horário: 2025-01-27 14:30:45

🔴 ATENÇÃO: A saturação está abaixo do normal (≤95%).
Por favor, verifique o paciente imediatamente.
```

## 🔍 Troubleshooting

### Mensagem não é enviada

**Verifique:**
1. ✅ Variáveis de ambiente configuradas corretamente
2. ✅ Número no formato correto (sem espaços, com código do país)
3. ✅ Chave da API válida
4. ✅ Valor de SpO2 realmente abaixo do threshold
5. ✅ Logs do servidor para erros

**Logs úteis:**
```bash
# Ver logs do backend
npm run dev

# Ou com Docker
docker-compose logs -f backend
```

### Erros Comuns

#### "Parâmetros obrigatórios não fornecidos"
- **Solução**: Verifique se `WHATSAPP_PHONE` e `WHATSAPP_API_KEY` estão configurados

#### "Alarme detectado, mas WhatsApp não configurado"
- **Solução**: Configure as variáveis de ambiente

#### "Aguardando cooldown"
- **Normal**: O sistema está aguardando 1 minuto entre alarmes (proteção contra spam)

#### "Erro HTTP XXX"
- **Solução**: Verifique se a chave da API está correta e se o número está no formato certo

### Teste Manual

Você pode testar o envio manualmente usando o módulo:

```javascript
const { sendWhatsAppMessage } = require('./whatsapp');

sendWhatsAppMessage('5511999999999', 'Teste de mensagem', 'sua_chave')
  .then(result => console.log('Sucesso:', result))
  .catch(error => console.error('Erro:', error));
```

## 📊 Monitoramento

### Logs do Sistema

O sistema registra todas as operações:
- ✅ Envios bem-sucedidos
- ❌ Erros de envio
- ⏸️ Alerta de cooldown
- ⚠️ Avisos de configuração

### Exemplo de Logs

```
📊 Dados recebidos: SpO2=92%
📱 Enviando mensagem WhatsApp para 5511999999999...
✅ Alerta WhatsApp enviado: Mensagem enviada com sucesso
```

ou

```
📊 Dados recebidos: SpO2=92%
⏸️ Alarme detectado, mas aguardando cooldown (último envio há 45s)
```

## 🔒 Segurança

- ⚠️ **Nunca compartilhe sua chave de API**
- ⚠️ **Nunca commite o arquivo .env no Git**
- ⚠️ **Use variáveis de ambiente em produção**
- ⚠️ **Limite o acesso ao servidor backend**

## 📚 Referências

- [CallMeBot API Documentation](https://www.callmebot.com/blog/free-api-whatsapp-messages/)
- [WhatsApp Business API](https://developers.facebook.com/docs/whatsapp) (alternativa paga)

## 🆘 Suporte

Se encontrar problemas:
1. Verifique os logs do servidor
2. Confirme a configuração das variáveis de ambiente
3. Teste a API CallMeBot manualmente
4. Verifique a documentação oficial do CallMeBot

---

**Desenvolvido para o projeto Sleep Help - Monitoramento de SpO2**

