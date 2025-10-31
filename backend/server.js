// Carregar variáveis de ambiente do arquivo .env
require('dotenv').config();

const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');
const http = require('http');
const socketIo = require('socket.io');
const moment = require('moment');
const { sendAlarmAlert } = require('./whatsapp');

const app = express();
const server = http.createServer(app);
const io = socketIo(server, {
  cors: {
    origin: "*",
    methods: ["GET", "POST"]
  }
});

// Middleware
app.use(cors());
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

// Armazenamento em memória (em produção, usar banco de dados)
let sensorData = {
  spo2: [],
  lastUpdate: null
};

// Configuração WhatsApp (variáveis de ambiente)
const WHATSAPP_PHONE = process.env.WHATSAPP_PHONE || '';
const WHATSAPP_API_KEY = process.env.WHATSAPP_API_KEY || '';
const ALARM_THRESHOLD = parseInt(process.env.ALARM_THRESHOLD || '95'); // SpO2 <= 95% gera alarme
let lastAlarmSent = null; // Controla para não enviar múltiplos alarmes seguidos
const ALARM_COOLDOWN = 60000; // 1 minuto entre alarmes (evita spam)

// Dados de exemplo para demonstração
const generateSampleData = () => {
  const now = moment();
  return {
    spo2: Math.floor(Math.random() * 10) + 90, // 90-100%
    timestamp: now.format('YYYY-MM-DD HH:mm:ss'),
    unix: now.valueOf()
  };
};

// Endpoint para receber dados do ESP32
app.post('/api/sensor-data', (req, res) => {
  try {
    const { spo2, timestamp } = req.body;
    
    // Validar dados
    if (spo2 === undefined) {
      return res.status(400).json({ 
        error: 'Dados inválidos. SpO2 é obrigatório.' 
      });
    }

    // Criar registro de dados
    const dataPoint = {
      spo2: parseInt(spo2),
      timestamp: timestamp || moment().format('YYYY-MM-DD HH:mm:ss'),
      unix: moment().valueOf()
    };

    // Armazenar dados (manter apenas últimos 1000 registros)
    sensorData.spo2.push(dataPoint.spo2);
    sensorData.lastUpdate = dataPoint;

    // Limitar histórico
    if (sensorData.spo2.length > 1000) {
      sensorData.spo2 = sensorData.spo2.slice(-1000);
    }

    // Enviar dados em tempo real via WebSocket
    io.emit('sensorData', dataPoint);

    console.log(`📊 Dados recebidos: SpO2=${dataPoint.spo2}%`);

    // Verificar se há alarme e enviar WhatsApp
    const isAlarm = dataPoint.spo2 <= ALARM_THRESHOLD;
    const now = Date.now();
    
    if (isAlarm && WHATSAPP_PHONE && WHATSAPP_API_KEY) {
      // Verificar se passou o tempo de cooldown desde o último alarme
      if (!lastAlarmSent || (now - lastAlarmSent) >= ALARM_COOLDOWN) {
        sendAlarmAlert(dataPoint.spo2, dataPoint.timestamp, WHATSAPP_PHONE, WHATSAPP_API_KEY)
          .then((result) => {
            console.log('✅ Alerta WhatsApp enviado:', result.message);
            lastAlarmSent = now;
          })
          .catch((error) => {
            console.error('❌ Erro ao enviar alerta WhatsApp:', error.message);
          });
      } else {
        console.log(`⏸️ Alarme detectado, mas aguardando cooldown (último envio há ${Math.floor((now - lastAlarmSent) / 1000)}s)`);
      }
    } else if (isAlarm && (!WHATSAPP_PHONE || !WHATSAPP_API_KEY)) {
      console.warn('⚠️ Alarme detectado, mas WhatsApp não configurado. Configure WHATSAPP_PHONE e WHATSAPP_API_KEY');
    }

    res.json({ 
      success: true, 
      message: 'Dados recebidos com sucesso',
      data: dataPoint,
      alarm: isAlarm
    });

  } catch (error) {
    console.error('❌ Erro ao processar dados:', error);
    res.status(500).json({ 
      error: 'Erro interno do servidor' 
    });
  }
});

// Endpoint para obter dados históricos
app.get('/api/sensor-data', (req, res) => {
  const { limit = 100 } = req.query;
  
  const historicalData = {
    spo2: sensorData.spo2.slice(-limit),
    lastUpdate: sensorData.lastUpdate,
    count: sensorData.spo2.length
  };

  res.json(historicalData);
});

// Endpoint para obter dados atuais
app.get('/api/current-data', (req, res) => {
  res.json({
    current: sensorData.lastUpdate,
    status: 'online',
    timestamp: moment().format('YYYY-MM-DD HH:mm:ss')
  });
});

// Endpoint de status
app.get('/api/status', (req, res) => {
  res.json({
    status: 'online',
    uptime: process.uptime(),
    timestamp: moment().format('YYYY-MM-DD HH:mm:ss'),
    dataPoints: sensorData.spo2.length
  });
});

// WebSocket para dados em tempo real
io.on('connection', (socket) => {
  console.log('🔌 Cliente conectado via WebSocket');
  
  // Enviar dados atuais ao conectar
  if (sensorData.lastUpdate) {
    socket.emit('sensorData', sensorData.lastUpdate);
  }

  socket.on('disconnect', () => {
    console.log('🔌 Cliente desconectado');
  });
});

// Gerar dados de exemplo a cada 5 segundos (para teste)
setInterval(() => {
  if (sensorData.spo2.length === 0) {
    const sampleData = generateSampleData();
    sensorData.spo2.push(sampleData.spo2);
    sensorData.lastUpdate = sampleData;
    
    io.emit('sensorData', sampleData);
    console.log(`📊 Dados de exemplo: SpO2=${sampleData.spo2}%`);
  }
}, 5000);

const PORT = process.env.PORT || 3001;

server.listen(PORT, '0.0.0.0', () => {
  console.log(`🚀 Servidor backend rodando na porta ${PORT}`);
  console.log(`📡 API disponível em: http://localhost:${PORT}/api`);
  console.log(`🔌 WebSocket disponível em: ws://localhost:${PORT}`);
  console.log(`📊 Endpoints:`);
  console.log(`   POST /api/sensor-data - Receber dados do ESP32`);
  console.log(`   GET  /api/sensor-data - Obter dados históricos`);
  console.log(`   GET  /api/current-data - Obter dados atuais`);
  console.log(`   GET  /api/status - Status do servidor`);
});
