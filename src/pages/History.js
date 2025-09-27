import React, { useState, useEffect } from 'react';
import { useAuth } from '../contexts/AuthContext';
import { useNavigate } from 'react-router-dom';
import './History.css';

const History = () => {
  const { user } = useAuth();
  const navigate = useNavigate();
  const [readings, setReadings] = useState([]);
  const [filteredReadings, setFilteredReadings] = useState([]);
  const [filters, setFilters] = useState({
    dateFrom: '',
    dateTo: '',
    spo2Min: '',
    spo2Max: '',
    status: 'all'
  });

  useEffect(() => {
    if (!user) {
      navigate('/');
      return;
    }

    // Gerar dados de histórico simulados
    generateMockData();
  }, [user, navigate]);

  const generateMockData = () => {
    const mockData = [];
    const now = new Date();
    
    for (let i = 0; i < 50; i++) {
      const date = new Date(now.getTime() - (i * 2 * 60 * 60 * 1000)); // 2 horas atrás
      const spo2 = Math.floor(Math.random() * 6) + 95; // 95-100
      
      mockData.push({
        id: i + 1,
        timestamp: date,
        spo2,
        status: spo2 < 95 ? 'baixa-saturacao' : 'normal',
        notes: spo2 < 95 ? 'Atenção: Saturação baixa' : 'Valores normais'
      });
    }
    
    setReadings(mockData);
    setFilteredReadings(mockData);
  };

  const handleFilterChange = (e) => {
    const { name, value } = e.target;
    setFilters(prev => ({
      ...prev,
      [name]: value
    }));
  };

  const applyFilters = () => {
    let filtered = [...readings];

    if (filters.dateFrom) {
      const fromDate = new Date(filters.dateFrom);
      filtered = filtered.filter(reading => reading.timestamp >= fromDate);
    }

    if (filters.dateTo) {
      const toDate = new Date(filters.dateTo + 'T23:59:59');
      filtered = filtered.filter(reading => reading.timestamp <= toDate);
    }

    if (filters.spo2Min) {
      filtered = filtered.filter(reading => reading.spo2 >= parseInt(filters.spo2Min));
    }

    if (filters.spo2Max) {
      filtered = filtered.filter(reading => reading.spo2 <= parseInt(filters.spo2Max));
    }


    if (filters.status !== 'all') {
      filtered = filtered.filter(reading => reading.status === filters.status);
    }

    setFilteredReadings(filtered);
  };

  const clearFilters = () => {
    setFilters({
      dateFrom: '',
      dateTo: '',
      spo2Min: '',
      spo2Max: '',
      status: 'all'
    });
    setFilteredReadings(readings);
  };

  const exportData = () => {
    const csvContent = [
      ['ID', 'Data/Hora', 'SpO2 (%)', 'Status', 'Observações'],
      ...filteredReadings.map(reading => [
        reading.id,
        reading.timestamp.toLocaleString('pt-BR'),
        reading.spo2,
        reading.status === 'baixa-saturacao' ? 'Baixa Saturação' : 'Normal',
        reading.notes
      ])
    ].map(row => row.join(',')).join('\n');

    const blob = new Blob([csvContent], { type: 'text/csv;charset=utf-8;' });
    const link = document.createElement('a');
    const url = URL.createObjectURL(blob);
    link.setAttribute('href', url);
    link.setAttribute('download', `historico_sleep_help_${new Date().toISOString().split('T')[0]}.csv`);
    link.style.visibility = 'hidden';
    document.body.appendChild(link);
    link.click();
    document.body.removeChild(link);
  };

  const getStatusBadge = (status) => {
    const statusConfig = {
      'normal': { text: 'Normal', class: 'status-normal' },
      'baixa-saturacao': { text: 'Baixa Saturação', class: 'status-warning' }
    };
    
    const config = statusConfig[status] || { text: 'Desconhecido', class: 'status-unknown' };
    
    return <span className={`status-badge ${config.class}`}>{config.text}</span>;
  };

  return (
    <div className="history-container">
      <div className="history-header">
        <h1>Histórico de Leituras</h1>
        <p>Visualize e analise todas as leituras registradas</p>
      </div>

      {/* Filtros */}
      <div className="filters-section">
        <h3>Filtros</h3>
        <div className="filters-grid">
          <div className="filter-group">
            <label>Data Inicial:</label>
            <input
              type="date"
              name="dateFrom"
              value={filters.dateFrom}
              onChange={handleFilterChange}
              className="filter-input"
            />
          </div>

          <div className="filter-group">
            <label>Data Final:</label>
            <input
              type="date"
              name="dateTo"
              value={filters.dateTo}
              onChange={handleFilterChange}
              className="filter-input"
            />
          </div>

          <div className="filter-group">
            <label>SpO2 Mínimo (%):</label>
            <input
              type="number"
              name="spo2Min"
              value={filters.spo2Min}
              onChange={handleFilterChange}
              placeholder="95"
              min="0"
              max="100"
              className="filter-input"
            />
          </div>

          <div className="filter-group">
            <label>SpO2 Máximo (%):</label>
            <input
              type="number"
              name="spo2Max"
              value={filters.spo2Max}
              onChange={handleFilterChange}
              placeholder="100"
              min="0"
              max="100"
              className="filter-input"
            />
          </div>


          <div className="filter-group">
            <label>Status:</label>
            <select
              name="status"
              value={filters.status}
              onChange={handleFilterChange}
              className="filter-input"
            >
              <option value="all">Todos</option>
              <option value="normal">Normal</option>
              <option value="baixa-saturacao">Baixa Saturação</option>
            </select>
          </div>
        </div>

        <div className="filter-actions">
          <button className="filter-btn apply-btn" onClick={applyFilters}>
            Aplicar Filtros
          </button>
          <button className="filter-btn clear-btn" onClick={clearFilters}>
            Limpar Filtros
          </button>
          <button className="filter-btn export-btn" onClick={exportData}>
            📊 Exportar CSV
          </button>
        </div>
      </div>

      {/* Estatísticas */}
      <div className="stats-section">
        <div className="stat-card">
          <h4>Total de Leituras</h4>
          <span className="stat-value">{filteredReadings.length}</span>
        </div>
        <div className="stat-card">
          <h4>SpO2 Médio</h4>
          <span className="stat-value">
            {filteredReadings.length > 0 
              ? (filteredReadings.reduce((sum, r) => sum + r.spo2, 0) / filteredReadings.length).toFixed(1)
              : '0'
            }%
          </span>
        </div>
        <div className="stat-card">
          <h4>Alertas</h4>
          <span className="stat-value warning">
            {filteredReadings.filter(r => r.status === 'baixa-saturacao').length}
          </span>
        </div>
      </div>

      {/* Tabela de Leituras */}
      <div className="table-section">
        <h3>Leituras ({filteredReadings.length} registros)</h3>
        <div className="table-container">
          <table className="readings-table">
            <thead>
              <tr>
                <th>ID</th>
                <th>Data/Hora</th>
                <th>SpO2 (%)</th>
                <th>Status</th>
                <th>Observações</th>
              </tr>
            </thead>
            <tbody>
              {filteredReadings.map(reading => (
                <tr key={reading.id} className={reading.status === 'baixa-saturacao' ? 'warning-row' : ''}>
                  <td>{reading.id}</td>
                  <td>{reading.timestamp.toLocaleString('pt-BR')}</td>
                  <td className={reading.spo2 < 95 ? 'warning-value' : ''}>
                    {reading.spo2}%
                  </td>
                  <td>{getStatusBadge(reading.status)}</td>
                  <td>{reading.notes}</td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      </div>
    </div>
  );
};

export default History;

