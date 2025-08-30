import React from 'react';
import { Link, useLocation, useNavigate } from 'react-router-dom';
import { useAuth } from '../contexts/AuthContext';
import './Header.css';

const Header = () => {
  const { user, logout } = useAuth();
  const location = useLocation();
  const navigate = useNavigate();

  const handleLogout = () => {
    logout();
    navigate('/');
  };

  const isActive = (path) => {
    return location.pathname === path;
  };

  return (
    <header className="header">
      <div className="header-container">
        <div className="logo-section">
          <div className="logo">
            <span className="logo-icon">💤</span>
            <h1 className="logo-text">Sleep Help</h1>
          </div>
          <p className="logo-subtitle">Monitoramento Inteligente de Saúde</p>
        </div>

        {user && (
          <nav className="navigation">
            <Link 
              to="/dashboard" 
              className={`nav-link ${isActive('/dashboard') ? 'active' : ''}`}
            >
              Dashboard
            </Link>
            <Link 
              to="/history" 
              className={`nav-link ${isActive('/history') ? 'active' : ''}`}
            >
              Histórico
            </Link>
            <Link 
              to="/contact" 
              className={`nav-link ${isActive('/contact') ? 'active' : ''}`}
            >
              Contato
            </Link>
          </nav>
        )}

        <div className="user-section">
          {user ? (
            <div className="user-info">
              <span className="user-name">Olá, {user.name}</span>
              <button className="logout-btn" onClick={handleLogout}>
                Sair
              </button>
            </div>
          ) : (
            <div className="welcome-text">
              Sistema de Monitoramento
            </div>
          )}
        </div>
      </div>
    </header>
  );
};

export default Header;

