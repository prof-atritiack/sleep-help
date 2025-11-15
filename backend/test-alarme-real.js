// Teste simulado de alarme real via API do servidor
const http = require('http');

async function testarAlarmeReal() {
  console.log('🧪 Testando alarme real através da API do servidor...\n');
  
  const dadosAlarme = {
    spo2: 92, // Valor abaixo de 95 para gerar alarme
    timestamp: new Date().toISOString().replace('T', ' ').substring(0, 19)
  };
  
  const dadosNormais = {
    spo2: 98, // Valor normal (não deve gerar alarme)
    timestamp: new Date().toISOString().replace('T', ' ').substring(0, 19)
  };
  
  const options = {
    hostname: 'localhost',
    port: 3001,
    path: '/api/sensor-data',
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    }
  };
  
  console.log('📊 Teste 1: Enviando dados NORMAIS (SpO2 = 98%)...');
  await enviarDados(options, dadosNormais);
  
  console.log('\n⏳ Aguardando 2 segundos...\n');
  await new Promise(resolve => setTimeout(resolve, 2000));
  
  console.log('🚨 Teste 2: Enviando dados de ALARME (SpO2 = 92%)...');
  await enviarDados(options, dadosAlarme);
  
  console.log('\n✅ Teste concluído!');
  console.log('📱 Verifique seu WhatsApp - você deve ter recebido apenas uma mensagem de alerta.\n');
}

function enviarDados(options, dados) {
  return new Promise((resolve, reject) => {
    const req = http.request(options, (res) => {
      let responseData = '';
      
      res.on('data', (chunk) => {
        responseData += chunk;
      });
      
      res.on('end', () => {
        try {
          const json = JSON.parse(responseData);
          console.log(`   Status: ${res.statusCode}`);
          console.log(`   SpO2: ${dados.spo2}%`);
          console.log(`   Alarme detectado: ${json.alarm ? '✅ SIM' : '❌ NÃO'}`);
          console.log(`   Mensagem: ${json.message}`);
          resolve(json);
        } catch (error) {
          console.error('   ❌ Erro ao processar resposta:', error.message);
          reject(error);
        }
      });
    });
    
    req.on('error', (error) => {
      console.error('   ❌ Erro na requisição:', error.message);
      console.error('   ⚠️  Certifique-se de que o servidor está rodando: npm run dev');
      reject(error);
    });
    
    req.write(JSON.stringify(dados));
    req.end();
  });
}

testarAlarmeReal().catch(error => {
  console.error('\n❌ Erro:', error.message);
  process.exit(1);
});

