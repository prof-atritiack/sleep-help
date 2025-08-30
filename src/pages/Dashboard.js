import React, { useState, useEffect } from 'react';
import { useAuth } from '../contexts/AuthContext';
import { useNavigate } from 'react-router-dom';
import './Dashboard.css';

const Dashboard = () => {
  const { user } = useAuth();
  const navigate = useNavigate();
  const [readings, setReadings] = useState({
    spo2: 98,
    bpm: 72,
    timestamp: new Date()
  });
  const [monitoringStatus, setMonitoringStatus] = useState('normal');
  const [isMonitoring, setIsMonitoring] = useState(true);

  useEffect(() => {
    if (!user) {
      navigate('/');
      return;
    }

    // Simular leituras em tempo real
    const interval = setInterval(() => {
      if (isMonitoring) {
        const newSpo2 = Math.floor(Math.random() * 6) + 95; // 95-100
        const newBpm = Math.floor(Math.random() * 20) + 65; // 65-85
        
        setReadings({
          spo2: newSpo2,
          bpm: newBpm,
          timestamp: new Date()
        });

        // Atualizar status baseado na saturação
        if (newSpo2 < 95) {
          setMonitoringStatus('baixa-saturacao');
        } else {
          setMonitoringStatus('normal');
        }
      }
    }, 3000);

    return () => clearInterval(interval);
  }, [user, navigate, isMonitoring]);

  const handleEmergencyCall = () => {
    if (confirm('Confirmar chamada para o SAMU (192)?')) {
      alert('Chamada para o SAMU iniciada. Aguarde atendimento.');
      // Aqui seria implementada a integração real com sistema de emergência
    }
  };

  const handleSMSSend = () => {
    if (confirm('Enviar SMS de alerta para contatos de emergência?')) {
      alert('SMS de alerta enviado para contatos cadastrados.');
      // Aqui seria implementado o envio real de SMS
    }
  };

  const toggleMonitoring = () => {
    setIsMonitoring(!isMonitoring);
    if (isMonitoring) {
      setMonitoringStatus('pausado');
    } else {
      setMonitoringStatus('normal');
    }
  };

  const getStatusColor = () => {
    switch (monitoringStatus) {
      case 'baixa-saturacao':
        return 'var(--danger-red)';
      case 'normal':
        return 'var(--success-green)';
      case 'pausado':
        return 'var(--warning-orange)';
      default:
        return 'var(--text-gray)';
    }
  };

  const getStatusText = () => {
    switch (monitoringStatus) {
      case 'baixa-saturacao':
        return 'Baixa Saturação';
      case 'normal':
        return 'Normal';
      case 'pausado':
        return 'Pausado';
      default:
        return 'Desconhecido';
    }
  };

  return (
    <div className="dashboard-container">
      <div className="dashboard-header">
        <h1>Dashboard de Monitoramento</h1>
        <p className="user-welcome">Bem-vindo, {user?.name}</p>
        <p className="user-id">ID: {user?.id}</p>
      </div>

      <div className="dashboard-grid">
        {/* Status do Monitoramento */}
        <div className="status-card">
          <h3>Status do Monitoramento</h3>
          <div className="status-indicator">
            <div 
              className="status-dot"
              style={{ backgroundColor: getStatusColor() }}
            ></div>
            <span className="status-text">{getStatusText()}</span>
          </div>
          <button 
            className={`monitoring-toggle ${isMonitoring ? 'active' : ''}`}
            onClick={toggleMonitoring}
          >
            {isMonitoring ? 'Pausar Monitoramento' : 'Retomar Monitoramento'}
          </button>
        </div>

        {/* Leituras Principais */}
        <div className="readings-card">
          <h3>Leituras em Tempo Real</h3>
          <div className="readings-grid">
            <div className="reading-item">
              <div className="reading-icon">💓</div>
              <div className="reading-value">{readings.bpm}</div>
          <div className="reading-label">BPM</div>
            </div>
            <div className="reading-item">
              <div className="reading-icon">🩸</div>
              <div className="reading-value">{readings.spo2}%</div>
              <div className="reading-label">SpO2</div>
            </div>
          </div>
          <div className="timestamp">
            Última atualização: {readings.timestamp.toLocaleTimeString()}
          </div>
        </div>

        {/* Botões de Emergência */}
        <div className="emergency-card">
          <h3>Ações de Emergência</h3>
          <div className="emergency-buttons">
            <button 
              className="emergency-btn samu-btn"
              onClick={handleEmergencyCall}
            >
              🚑 Chamar SAMU (192)
            </button>
            <button 
              className="emergency-btn sms-btn"
              onClick={handleSMSSend}
            >
              📱 Enviar SMS de Alerta
            </button>
          </div>
        </div>

        {/* Informações do Usuário */}
        <div className="user-info-card">
          <h3>Informações do Usuário</h3>
          <div className="user-details">
            <div className="detail-item">
              <span className="detail-label">Nome:</span>
              <span className="detail-value">{user?.name}</span>
            </div>
            <div className="detail-item">
              <span className="detail-label">ID:</span>
              <span className="detail-value">{user?.id}</span>
            </div>
            <div className="detail-item">
              <span className="detail-label">Usuário:</span>
              <span className="detail-value">{user?.username}</span>
            </div>
          </div>
        </div>

        {/* Gráfico de Tendências */}
        <div className="trends-card">
          <h3>Tendências</h3>
          <div className="trend-chart">
            <div className="chart-placeholder">
              📊 Gráfico de Tendências
              <p>Visualização das variações de SpO2 e BPM ao longo do tempo</p>
            </div>
          </div>
        </div>

        {/* Alertas e Notificações */}
        <div className="alerts-card">
          <h3>Alertas e Notificações</h3>
          <div className="alerts-list">
            {monitoringStatus === 'baixa-saturacao' && (
              <div className="alert-item critical">
                ⚠️ Alerta: Baixa saturação de oxigênio detectada
              </div>
            )}
            <div className="alert-item info">
              ℹ️ Monitoramento ativo e funcionando normalmente
            </div>
            <div className="alert-item success">
              ✅ Sistema de alertas configurado e ativo
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};

export default Dashboard;

