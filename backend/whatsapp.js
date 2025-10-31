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
        if (res.statusCode === 200) {
          console.log('✅ Mensagem WhatsApp enviada com sucesso!');
          resolve({
            success: true,
            message: 'Mensagem enviada com sucesso',
            response: data
          });
        } else {
          console.error(`❌ Erro ao enviar WhatsApp: Status ${res.statusCode}`);
          reject({
            success: false,
            message: `Erro HTTP ${res.statusCode}`,
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

