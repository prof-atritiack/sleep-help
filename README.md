# Sleep Help - Monitoramento de Saúde

Interface web para monitoramento de SpO2 (Saturação de Oxigênio) durante o sono.

## 📋 Sobre o Projeto

**Sleep Help** é um sistema de monitoramento de saúde desenvolvido como trabalho de conclusão de curso técnico em **Automação Industrial** da **ETEC Jorge Street**.

O projeto visa fornecer uma interface web intuitiva para acompanhar parâmetros vitais durante o sono, contribuindo para o monitoramento da qualidade do sono e saúde geral do usuário.

## 🎯 Objetivos

- Monitorar SpO2 em tempo real durante o sono
- Fornecer interface web responsiva e acessível
- Armazenar histórico de medições para análise
- Alertar sobre possíveis anormalidades nos parâmetros vitais
- Contribuir para a qualidade do sono e saúde do usuário

## 🚀 Funcionalidades

- **Monitoramento em Tempo Real**: Acompanhamento contínuo de SpO2
- **Dashboard Interativo**: Visualização de dados através de gráficos e indicadores
- **Histórico de Medições**: Armazenamento e consulta de dados históricos
- **Sistema de Autenticação**: Controle de acesso e segurança dos dados
- **Interface Responsiva**: Design moderno e adaptável a diferentes dispositivos
- **Alertas Inteligentes**: Notificações sobre valores fora da faixa normal

## 🔌 Arquitetura do Sistema

### Implementação Atual
- Interface web desenvolvida em React
- Sistema de autenticação e gerenciamento de usuários
- Dashboard para visualização de dados

### Implementação Futura
- **Hardware**: Módulo **ESP32 DevKit V1** como controlador principal
- **Sensores**: 
  - Sensor de SpO2 (oxímetro de pulso)
- **Comunicação**: Wi-Fi para envio de dados para a interface web
- **Processamento**: Análise local dos dados antes do envio

## 🛠️ Tecnologias Utilizadas

### Frontend
- **React 18** - Biblioteca para construção de interfaces
- **React Router DOM** - Roteamento da aplicação
- **Styled Components** - Estilização baseada em componentes
- **Create React App** - Ferramenta de build e desenvolvimento

### Hardware (Futuro)
- **ESP32 DevKit V1** - Microcontrolador Wi-Fi/Bluetooth
- **Sensores biomédicos** - Para captura de dados vitais
- **Protocolos de comunicação** - HTTP/MQTT para transmissão de dados

## 📋 Pré-requisitos

- Node.js (versão 14 ou superior)
- npm ou yarn
- Navegador web moderno

## 🔧 Instalação e Execução

1. **Clone o repositório:**
```bash
git clone https://github.com/seu-usuario/sleep-help.git
cd sleep-help
```

2. **Instale as dependências:**
```bash
npm install
```

3. **Execute o projeto:**
```bash
npm start
```

O aplicativo será aberto automaticamente em `http://localhost:3000`

## 📦 Scripts Disponíveis

- `npm start` - Inicia o servidor de desenvolvimento
- `npm run build` - Cria a versão de produção
- `npm test` - Executa os testes automatizados
- `npm run eject` - Ejetar do Create React App (irreversível)

## 🌐 Deploy

Para criar uma versão de produção:

```bash
npm run build
```

Os arquivos otimizados serão gerados na pasta `build/`.

## 📱 Estrutura do Projeto

```
src/
├── components/     # Componentes reutilizáveis
│   ├── Header/    # Cabeçalho da aplicação
│   └── ...        # Outros componentes
├── contexts/      # Contextos do React (Auth, etc.)
├── pages/         # Páginas da aplicação
│   ├── Dashboard/ # Página principal
│   ├── History/   # Histórico de medições
│   ├── Login/     # Autenticação
│   └── Contact/   # Contato
├── App.js         # Componente principal
└── index.js       # Ponto de entrada
```

## 🔬 Aplicações e Benefícios

- **Monitoramento do Sono**: Acompanhamento da qualidade do sono
- **Prevenção de Problemas**: Identificação precoce de anormalidades
- **Acompanhamento Médico**: Dados para consultas e tratamentos
- **Pesquisa**: Base de dados para estudos sobre padrões de sono
- **Automação Industrial**: Aplicação prática dos conhecimentos técnicos

## 🤝 Contribuição

1. Faça um fork do projeto
2. Crie uma branch para sua feature (`git checkout -b feature/AmazingFeature`)
3. Commit suas mudanças (`git commit -m 'Add some AmazingFeature'`)
4. Push para a branch (`git push origin feature/AmazingFeature`)
5. Abra um Pull Request

## 📚 Fontes e Referências

### Documentação Técnica
- [ESP32 Technical Reference Manual](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)
- [React Documentation](https://react.dev/)
- [Create React App Documentation](https://create-react-app.dev/)

### Sensores e Hardware
- [MAX30100 Pulse Oximeter and Heart-Rate Sensor](https://www.analog.com/en/products/max30100.html)
- [ESP32 DevKit V1 Pinout](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-devkitc.html)

### Padrões e Protocolos
- [IEEE 11073 - Medical Device Communication](https://standards.ieee.org/standard/11073-10407-2010.html)
- [MQTT Protocol Specification](http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html)

### Estudos e Pesquisas
- [Sleep Quality and Health Monitoring](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3632337/)
- [Wearable Health Technology](https://ieeexplore.ieee.org/document/8259811)

### Ferramentas de Desenvolvimento
- **Cursor AI** - IDE inteligente com assistência de IA para desenvolvimento
  - **Modelos Utilizados**: Claude Sonnet 4, GPT-4, e outros modelos avançados de linguagem
  - **Recursos**: Autocompletar inteligente, refatoração automática, debugging assistido por IA
  - **Integração**: Suporte nativo para React, JavaScript, e desenvolvimento web moderno
- [Cursor AI Documentation](https://cursor.sh/docs)
- [Cursor AI Features](https://cursor.sh/features)

## 🎓 Contexto Acadêmico

Este projeto foi desenvolvido como **Trabalho de Conclusão de Curso (TCC)** do curso técnico em **Automação Industrial** da **ETEC Jorge Street**.

### Objetivos Educacionais
- Aplicação prática dos conhecimentos em automação industrial
- Integração de sistemas eletrônicos com interfaces web
- Desenvolvimento de soluções para problemas reais da indústria
- Demonstração de competências técnicas adquiridas durante o curso

### Competências Desenvolvidas
- Programação de microcontroladores
- Desenvolvimento de interfaces web
- Integração de sistemas
- Gestão de projetos técnicos
- Documentação técnica

## 📄 Licença

Este projeto está sob a licença MIT. Veja o arquivo [LICENSE](LICENSE) para mais detalhes.

## 📞 Contato

- **Projeto**: [Sleep Help](https://github.com/seu-usuario/sleep-help)
- **Instituição**: ETEC Jorge Street
- **Curso**: Técnico em Automação Industrial

---

⭐ Se este projeto te ajudou, considere dar uma estrela!

*Desenvolvido com ❤️ para o TCC em Automação Industrial da ETEC Jorge Street*

