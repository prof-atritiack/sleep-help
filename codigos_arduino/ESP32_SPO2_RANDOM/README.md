# 📡 ESP32 SpO2 Random - Envio de Dados Aleatórios

## 🎯 Descrição

Este código para ESP32 gera valores aleatórios de SpO2 (Saturação de Oxigênio) e os envia para a API da aplicação Sleep Help. É ideal para testes e demonstrações do sistema.

## 🚀 Funcionalidades

- ✅ **Geração de dados aleatórios** de SpO2 (90-100%)
- ✅ **Envio automático** para API via HTTP POST
- ✅ **Reconexão automática** de Wi-Fi
- ✅ **Indicadores visuais** com LED
- ✅ **Monitoramento via Serial** com logs detalhados
- ✅ **Detecção de alertas** para baixa saturação
- ✅ **Timestamp automático** para cada medição

## 📁 Arquivos

- `ESP32_SPO2_RANDOM.ino` - Código principal do ESP32
- `CONFIGURACAO.txt` - Instruções de configuração
- `README.md` - Este arquivo

## 🔧 Configuração Rápida

1. **Configure as credenciais Wi-Fi** no código
2. **Verifique o IP do servidor** (172.20.10.7:3001)
3. **Faça upload** para o ESP32
4. **Abra o Serial Monitor** (115200 baud)
5. **Acesse a aplicação web** para ver os dados!

## 📊 Exemplo de Saída

```
========================================
    SLEEP HELP - ESP32 SpO2 Random
    Enviando dados aleatórios para API
========================================
✅ Wi-Fi conectado!
IP: 10.10.14.224
📊 Enviando dados #1
   SpO2: 98%
   Timestamp: 2025-09-27 12:30:45
✅ Dados enviados com sucesso!
✅ Saturação normal
```

## 🎛️ Controles

- **LED interno**: Indica status das operações
- **Serial Monitor**: Mostra logs detalhados
- **Intervalo configurável**: Padrão 5 segundos

## 🔗 Integração

Este código se integra perfeitamente com:
- **Backend Sleep Help** (Node.js)
- **Frontend Sleep Help** (React)
- **Dashboard em tempo real**
- **Sistema de alertas**

---

**Desenvolvido para o TCC em Automação Industrial - ETEC Jorge Street**
