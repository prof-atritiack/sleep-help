// Script de teste para WhatsApp
require('dotenv').config();
const { sendAlarmAlert, sendWhatsAppMessage } = require('./whatsapp');

async function testarWhatsApp() {
  console.log('🧪 Testando integração WhatsApp...\n');
  
  // Verificar se as variáveis estão configuradas
  const phone = process.env.WHATSAPP_PHONE;
  const apiKey = process.env.WHATSAPP_API_KEY;
  const threshold = process.env.ALARM_THRESHOLD || '95';
  
  console.log('📋 Configuração:');
  console.log(`   Telefone: ${phone ? '✅ Configurado' : '❌ Não configurado'}`);
  console.log(`   API Key: ${apiKey ? '✅ Configurada (' + apiKey.substring(0, 4) + '...)' : '❌ Não configurada'}`);
  console.log(`   Threshold: ${threshold}%\n`);
  
  if (!phone || !apiKey) {
    console.error('❌ Erro: WHATSAPP_PHONE ou WHATSAPP_API_KEY não configurados!');
    console.error('   Configure essas variáveis no arquivo .env\n');
    process.exit(1);
  }
  
  // Teste 1: Enviar mensagem de teste simples
  console.log('📱 Teste 1: Enviando mensagem de teste simples...');
  try {
    const testMessage = `🧪 *TESTE DO SISTEMA SLEEP HELP*\n\nEsta é uma mensagem de teste para verificar se a integração WhatsApp está funcionando.\n\nSe você recebeu esta mensagem, está tudo funcionando! ✅`;
    
    const result1 = await sendWhatsAppMessage(phone, testMessage, apiKey);
    console.log('   ✅ Sucesso!', result1.message);
    console.log('   Resposta:', result1.response || 'Sem resposta');
  } catch (error) {
    console.error('   ❌ Erro:', error.message);
    if (error.response) {
      console.error('   Resposta:', error.response);
    }
  }
  
  console.log('\n⏳ Aguardando 3 segundos antes do próximo teste...\n');
  await new Promise(resolve => setTimeout(resolve, 3000));
  
  // Teste 2: Enviar alerta de alarme simulado
  console.log('🚨 Teste 2: Enviando alerta de alarme simulado...');
  try {
    const spo2Teste = 92; // Valor abaixo do threshold para testar alarme
    const timestamp = new Date().toISOString().replace('T', ' ').substring(0, 19);
    
    const result2 = await sendAlarmAlert(spo2Teste, timestamp, phone, apiKey);
    console.log('   ✅ Alerta enviado!', result2.message);
    console.log('   Resposta:', result2.response || 'Sem resposta');
  } catch (error) {
    console.error('   ❌ Erro:', error.message);
    if (error.response) {
      console.error('   Resposta:', error.response);
    }
  }
  
  console.log('\n✅ Teste concluído!');
  console.log('📱 Verifique seu WhatsApp para confirmar o recebimento das mensagens.\n');
}

// Executar teste
testarWhatsApp().catch(error => {
  console.error('\n❌ Erro fatal:', error);
  process.exit(1);
});

