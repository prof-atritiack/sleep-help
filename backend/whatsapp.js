const https = require('https');

/**
 * Módulo de integração com WhatsApp usando CallMeBot API
 * API gratuita que permite envio de mensagens via WhatsApp
 * 
 * Para obter sua chave API:
 * 1. Adicione o número +34 644 97 54 14 aos seus contatos
 * 2. Envie a mensagem "Eu permito que o callmebot me envie mensagens" para esse contato
 * 3. Você receberá uma chave de API pessoal
 */

/**
 * Envia uma mensagem via WhatsApp usando CallMeBot API
 * @param {string} phoneNumber - Número de telefone do destinatário (formato: 5511999999999)
 * @param {string} message - Mensagem a ser enviada
 * @param {string} apiKey - Chave da API do CallMeBot
 * @returns {Promise<{success: boolean, message: string}>}
 */
async function sendWhatsAppMessage(phoneNumber, message, apiKey) {
  return new Promise((resolve, reject) => {
    // Validar parâmetros
    if (!phoneNumber || !message || !apiKey) {
      return reject({
        success: false,
        message: 'Parâmetros obrigatórios não fornecidos: phoneNumber, message e apiKey são necessários'
      });
    }

    // Formatar URL da API CallMeBot
    // Documentação: https://www.callmebot.com/blog/free-api-whatsapp-messages/
    const encodedMessage = encodeURIComponent(message);
    const url = `https://api.callmebot.com/whatsapp.php?phone=${phoneNumber}&text=${encodedMessage}&apikey=${apiKey}`;

    console.log(`📱 Enviando mensagem WhatsApp para ${phoneNumber}...`);

    https.get(url, (res) => {
      let data = '';

      res.on('data', (chunk) => {
        data += chunk;
      });

      res.on('end', () => {
        // Verificar se a resposta contém erros mesmo com status 200
        const responseLower = data.toLowerCase();
        const hasError = responseLower.includes('error') || 
                        responseLower.includes('invalid') || 
                        responseLower.includes('unauthorized') ||
                        responseLower.includes('not authorized') ||
                        responseLower.includes('api key') ||
                        (responseLower.includes('209') && !responseLower.includes('queued'));
        
        if (res.statusCode === 200 && !hasError) {
          // Verificar se a mensagem foi realmente enfileirada
          if (responseLower.includes('queued') || responseLower.includes('message to:')) {
            console.log('✅ Mensagem WhatsApp enviada com sucesso!');
            console.log(`📋 Resposta da API: ${data.substring(0, 200)}...`);
            resolve({
              success: true,
              message: 'Mensagem enviada com sucesso',
              response: data
            });
          } else {
            // Status 200 mas resposta não indica sucesso
            console.error(`⚠️ Resposta inesperada da API (Status ${res.statusCode}):`);
            console.error(`📋 Resposta: ${data}`);
            reject({
              success: false,
              message: 'API retornou sucesso mas resposta não indica envio bem-sucedido',
              statusCode: res.statusCode,
              response: data
            });
          }
        } else {
          // Log detalhado do erro
          console.error(`❌ Erro ao enviar WhatsApp: Status ${res.statusCode}`);
          console.error(`📋 Resposta da API: ${data}`);
          
          // Mensagens de erro específicas para códigos comuns
          let errorMessage = `Erro HTTP ${res.statusCode}`;
          if (res.statusCode === 209) {
            // Erro 209 geralmente significa limite de mensagens excedido (50 msg/4h)
            if (data.includes('limit') || data.includes('50 messages')) {
              errorMessage = 'Erro 209: Limite de mensagens excedido! CallMeBot permite apenas 50 mensagens a cada 4 horas. Aguarde antes de enviar mais mensagens.';
            } else {
              errorMessage = 'Erro 209: Chave API inválida, expirada ou número não autorizado. Verifique se: 1) A chave API está correta, 2) O número está autorizado no CallMeBot';
            }
          } else if (res.statusCode === 400) {
            errorMessage = 'Erro 400: Parâmetros inválidos (número ou mensagem)';
          } else if (res.statusCode === 429) {
            errorMessage = 'Erro 429: Limite de mensagens excedido. Aguarde alguns minutos.';
          }
          
          reject({
            success: false,
            message: errorMessage,
            statusCode: res.statusCode,
            response: data
          });
        }
      });
    }).on('error', (error) => {
      console.error('❌ Erro na requisição WhatsApp:', error.message);
      reject({
        success: false,
        message: `Erro na requisição: ${error.message}`
      });
    });
  });
}

/**
 * Envia alerta de alarme quando SpO2 está abaixo do normal
 * @param {number} spo2 - Valor de SpO2 detectado
 * @param {string} timestamp - Timestamp da medição
 * @param {string} phoneNumber - Número de telefone para receber o alerta
 * @param {string} apiKey - Chave da API do CallMeBot
 * @returns {Promise<{success: boolean, message: string}>}
 */
async function sendAlarmAlert(spo2, timestamp, phoneNumber, apiKey) {
  const message = `🚨 *ALERTA DE SATURAÇÃO DE OXIGÊNIO*\n\n` +
    `⚠️ Valores críticos detectados!\n\n` +
    `📊 SpO2: ${spo2}%\n` +
    `⏰ Horário: ${timestamp}\n\n` +
    `🔴 ATENÇÃO: A saturação está abaixo do normal (≤95%).\n` +
    `Por favor, verifique o paciente imediatamente.`;

  return sendWhatsAppMessage(phoneNumber, message, apiKey);
}

module.exports = {
  sendWhatsAppMessage,
  sendAlarmAlert
};

