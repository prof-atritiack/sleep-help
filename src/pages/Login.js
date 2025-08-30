import React, { useState, useEffect } from 'react';
import { useNavigate } from 'react-router-dom';
import { useAuth } from '../contexts/AuthContext';
import './Login.css';

const Login = () => {
  const [formData, setFormData] = useState({
    username: '',
    password: ''
  });
  const [error, setError] = useState('');
  const [loading, setLoading] = useState(false);
  const { login } = useAuth();
  const navigate = useNavigate();

  useEffect(() => {
    // Simular dados de usuário para demonstração
    const demoUsers = [
      { id: 1, username: 'admin', password: 'admin123', name: 'Administrador' },
      { id: 2, username: 'user', password: 'user123', name: 'Usuário Teste' }
    ];
    localStorage.setItem('demoUsers', JSON.stringify(demoUsers));
  }, []);

  const handleInputChange = (e) => {
    setFormData({
      ...formData,
      [e.target.name]: e.target.value
    });
    setError('');
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    setLoading(true);
    setError('');

    try {
      // Simular verificação de credenciais
      await new Promise(resolve => setTimeout(resolve, 1000));
      
      const demoUsers = JSON.parse(localStorage.getItem('demoUsers') || '[]');
      const user = demoUsers.find(u => 
        u.username === formData.username && u.password === formData.password
      );

      if (user) {
        login({
          id: user.id,
          username: user.username,
          name: user.name
        });
        navigate('/dashboard');
      } else {
        setError('Usuário ou senha incorretos');
      }
    } catch (err) {
      setError('Erro ao fazer login. Tente novamente.');
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="login-container">
      <div className="login-card">
        <div className="login-header">
          <div className="login-logo">
            <span className="login-logo-icon">💤</span>
            <h2>Sleep Help</h2>
          </div>
          <p className="login-subtitle">Acesso ao Sistema de Monitoramento</p>
        </div>

        <form onSubmit={handleSubmit} className="login-form">
          <div className="form-group">
            <label htmlFor="username">Usuário</label>
            <input
              type="text"
              id="username"
              name="username"
              value={formData.username}
              onChange={handleInputChange}
              placeholder="Digite seu usuário"
              required
              className="form-input"
            />
          </div>

          <div className="form-group">
            <label htmlFor="password">Senha</label>
            <input
              type="password"
              id="password"
              name="password"
              value={formData.password}
              onChange={handleInputChange}
              placeholder="Digite sua senha"
              required
              className="form-input"
            />
          </div>

          {error && (
            <div className="error-message">
              {error}
            </div>
          )}

          <button 
            type="submit" 
            className="login-btn"
            disabled={loading}
          >
            {loading ? 'Entrando...' : 'Entrar'}
          </button>
        </form>

        <div className="demo-credentials">
          <h4>Credenciais de Demonstração:</h4>
          <div className="credential-item">
            <strong>Admin:</strong> admin / admin123
          </div>
          <div className="credential-item">
            <strong>Usuário:</strong> user / user123
          </div>
        </div>

        <div className="healthcare-info">
          <div className="healthcare-icon">🏥</div>
          <p>Sistema de Monitoramento de Saúde</p>
          <p>SpO2 • BPM • Alertas Inteligentes</p>
        </div>
      </div>
    </div>
  );
};

export default Login;

