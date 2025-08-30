import React, { useState } from 'react';
import { useAuth } from '../contexts/AuthContext';
import { useNavigate } from 'react-router-dom';
import './Contact.css';

const Contact = () => {
  const { user } = useAuth();
  const navigate = useNavigate();
  const [formData, setFormData] = useState({
    name: user?.name || '',
    email: '',
    subject: '',
    message: '',
    priority: 'normal'
  });
  const [submitted, setSubmitted] = useState(false);

  const handleInputChange = (e) => {
    const { name, value } = e.target;
    setFormData(prev => ({
      ...prev,
      [name]: value
    }));
  };

  const handleSubmit = (e) => {
    e.preventDefault();
    // Simular envio do formulário
    setTimeout(() => {
      setSubmitted(true);
      setFormData({
        name: '',
        email: '',
        subject: '',
        message: '',
        priority: 'normal'
      });
    }, 1000);
  };

  const handleReset = () => {
    setSubmitted(false);
    setFormData({
      name: user?.name || '',
      email: '',
      subject: '',
      message: '',
      priority: 'normal'
    });
  };

  const emergencyContacts = [
    {
      name: 'SAMU',
      number: '192',
      description: 'Serviço de Atendimento Móvel de Urgência',
      icon: '🚑'
    },
    {
      name: 'Bombeiros',
      number: '193',
      description: 'Corpo de Bombeiros',
      icon: '🚒'
    },
    {
      name: 'Polícia',
      number: '190',
      description: 'Polícia Militar',
      icon: '👮'
    }
  ];

  const supportInfo = [
    {
      title: 'Suporte Técnico',
      description: 'Assistência com problemas técnicos do sistema',
      email: 'suporte@sleephelp.com.br',
      phone: '(11) 9999-9999',
      icon: '🔧'
    },
    {
      title: 'Suporte Médico',
      description: 'Dúvidas sobre interpretação de dados médicos',
      email: 'medico@sleephelp.com.br',
      phone: '(11) 8888-8888',
      icon: '👨‍⚕️'
    },
    {
      title: 'Administrativo',
      description: 'Questões sobre conta e configurações',
      email: 'admin@sleephelp.com.br',
      phone: '(11) 7777-7777',
      icon: '📋'
    }
  ];

  return (
    <div className="contact-container">
      <div className="contact-header">
        <h1>Contato e Suporte</h1>
        <p>Entre em contato conosco para suporte e emergências</p>
      </div>

      <div className="contact-grid">
        {/* Formulário de Contato */}
        <div className="contact-form-section">
          <h3>Enviar Mensagem</h3>
          {submitted ? (
            <div className="success-message">
              <div className="success-icon">✅</div>
              <h4>Mensagem Enviada!</h4>
              <p>Sua mensagem foi enviada com sucesso. Entraremos em contato em breve.</p>
              <button className="reset-btn" onClick={handleReset}>
                Enviar Nova Mensagem
              </button>
            </div>
          ) : (
            <form onSubmit={handleSubmit} className="contact-form">
              <div className="form-row">
                <div className="form-group">
                  <label htmlFor="name">Nome *</label>
                  <input
                    type="text"
                    id="name"
                    name="name"
                    value={formData.name}
                    onChange={handleInputChange}
                    required
                    className="form-input"
                  />
                </div>
                <div className="form-group">
                  <label htmlFor="email">Email *</label>
                  <input
                    type="email"
                    id="email"
                    name="email"
                    value={formData.email}
                    onChange={handleInputChange}
                    required
                    className="form-input"
                  />
                </div>
              </div>

              <div className="form-row">
                <div className="form-group">
                  <label htmlFor="subject">Assunto *</label>
                  <input
                    type="text"
                    id="subject"
                    name="subject"
                    value={formData.subject}
                    onChange={handleInputChange}
                    required
                    className="form-input"
                  />
                </div>
                <div className="form-group">
                  <label htmlFor="priority">Prioridade</label>
                  <select
                    id="priority"
                    name="priority"
                    value={formData.priority}
                    onChange={handleInputChange}
                    className="form-input"
                  >
                    <option value="normal">Normal</option>
                    <option value="urgent">Urgente</option>
                    <option value="critical">Crítico</option>
                  </select>
                </div>
              </div>

              <div className="form-group">
                <label htmlFor="message">Mensagem *</label>
                <textarea
                  id="message"
                  name="message"
                  value={formData.message}
                  onChange={handleInputChange}
                  required
                  rows="6"
                  className="form-textarea"
                  placeholder="Descreva sua dúvida ou solicitação..."
                ></textarea>
              </div>

              <button type="submit" className="submit-btn">
                Enviar Mensagem
              </button>
            </form>
          )}
        </div>

        {/* Contatos de Emergência */}
        <div className="emergency-section">
          <h3>🚨 Contatos de Emergência</h3>
          <div className="emergency-contacts">
            {emergencyContacts.map((contact, index) => (
              <div key={index} className="emergency-contact">
                <div className="emergency-icon">{contact.icon}</div>
                <div className="emergency-info">
                  <h4>{contact.name}</h4>
                  <p className="emergency-number">{contact.number}</p>
                  <p className="emergency-description">{contact.description}</p>
                </div>
                <button className="call-btn" onClick={() => window.open(`tel:${contact.number}`)}>
                  📞 Ligar
                </button>
              </div>
            ))}
          </div>
        </div>

        {/* Informações de Suporte */}
        <div className="support-section">
          <h3>📞 Canais de Suporte</h3>
          <div className="support-channels">
            {supportInfo.map((channel, index) => (
              <div key={index} className="support-channel">
                <div className="channel-icon">{channel.icon}</div>
                <div className="channel-info">
                  <h4>{channel.title}</h4>
                  <p>{channel.description}</p>
                  <div className="channel-contacts">
                    <div className="contact-item">
                      <span className="contact-label">Email:</span>
                      <a href={`mailto:${channel.email}`} className="contact-link">
                        {channel.email}
                      </a>
                    </div>
                    <div className="contact-item">
                      <span className="contact-label">Telefone:</span>
                      <a href={`tel:${channel.phone}`} className="contact-link">
                        {channel.phone}
                      </a>
                    </div>
                  </div>
                </div>
              </div>
            ))}
          </div>
        </div>

        {/* Informações Adicionais */}
        <div className="info-section">
          <h3>ℹ️ Informações Adicionais</h3>
          <div className="info-grid">
            <div className="info-item">
              <h4>Horário de Atendimento</h4>
              <p><strong>Segunda a Sexta:</strong> 8h às 18h</p>
              <p><strong>Sábados:</strong> 8h às 12h</p>
              <p><strong>Emergências:</strong> 24h por dia</p>
            </div>
            <div className="info-item">
              <h4>Localização</h4>
              <p><strong>Endereço:</strong> Rua da Saúde, 123</p>
              <p><strong>Bairro:</strong> Centro Médico</p>
              <p><strong>Cidade:</strong> São Paulo - SP</p>
            </div>
            <div className="info-item">
              <h4>Documentação</h4>
              <p><strong>Manual do Usuário:</strong> <a href="#" className="info-link">Download PDF</a></p>
              <p><strong>FAQ:</strong> <a href="#" className="info-link">Perguntas Frequentes</a></p>
              <p><strong>Vídeos Tutoriais:</strong> <a href="#" className="info-link">Canal YouTube</a></p>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};

export default Contact;

