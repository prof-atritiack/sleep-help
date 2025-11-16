# 📝 Explicação: O que é o arquivo .env?

## 🤔 O que é o arquivo .env?

O arquivo `.env` é um arquivo de **configuração** que armazena informações **sensíveis** e **específicas** do seu ambiente (sua máquina).

Pense nele como um "caderninho de anotações" onde você guarda:
- Senhas
- Chaves de API
- Números de telefone
- Configurações pessoais

## 🎯 Para que serve no projeto Sleep Help?

No projeto Sleep Help, o arquivo `.env` é usado para configurar o **WhatsApp** (opcional).

### O que acontece SEM o arquivo .env:
- ✅ A aplicação funciona normalmente
- ✅ O ESP32 envia dados para a API
- ✅ O frontend mostra os dados
- ❌ **MAS** não envia alertas via WhatsApp quando SpO2 <= 95%

### O que acontece COM o arquivo .env:
- ✅ Tudo acima funciona
- ✅ **PLUS**: Envia alertas automáticos via WhatsApp quando detecta SpO2 <= 95%

## 📋 O que vai dentro do arquivo .env?

O arquivo `.env` fica na pasta `backend/` e tem este formato:

```env
WHATSAPP_PHONE=5511999999999
WHATSAPP_API_KEY=sua_chave_api_aqui
ALARM_THRESHOLD=95
PORT=3001
```

### Explicação de cada linha:

1. **WHATSAPP_PHONE**: Seu número de telefone
   - Formato: código do país + DDD + número
   - Exemplo Brasil: `5511999999999` (55 + 11 + 999999999)
   - Sem espaços, sem hífens, sem parênteses

2. **WHATSAPP_API_KEY**: Chave de API do CallMeBot
   - Você recebe essa chave após configurar o CallMeBot
   - É uma string longa tipo: `123456-ABC-DEF-789-GHI-JKL`

3. **ALARM_THRESHOLD**: Limite para gerar alarme
   - Padrão: `95` (SpO2 <= 95% gera alarme)
   - Você pode mudar para `90` se quiser

4. **PORT**: Porta do servidor
   - Padrão: `3001`
   - Geralmente não precisa mudar

## 🔒 Por que usar arquivo .env?

### Segurança:
- ❌ **NÃO** coloque senhas/chaves diretamente no código
- ✅ **SIM** coloque no arquivo `.env` (que não vai para o Git)

### Organização:
- Configurações separadas do código
- Fácil de mudar sem editar código
- Cada pessoa pode ter seu próprio `.env`

## 📝 Como criar o arquivo .env?

### Opção 1: Manual (Mais Simples)

1. Vá para a pasta `backend/`
2. Crie um arquivo chamado exatamente `.env` (com o ponto na frente)
3. Cole este conteúdo:

```env
WHATSAPP_PHONE=5511999999999
WHATSAPP_API_KEY=sua_chave_api_aqui
ALARM_THRESHOLD=95
PORT=3001
```

4. Substitua os valores pelos seus dados reais
5. Salve o arquivo

### Opção 2: Usando Script (Windows PowerShell)

```powershell
cd backend
.\criar-env.ps1
```

O script vai perguntar cada valor e criar o arquivo automaticamente.

### Opção 3: Copiar do exemplo

```bash
cd backend
cp .env.example .env
# Depois edite o arquivo .env e preencha seus dados
```

## ⚠️ IMPORTANTE: O arquivo .env é OPCIONAL!

### Você PRECISA do .env se:
- ✅ Quer receber alertas via WhatsApp quando SpO2 <= 95%

### Você NÃO PRECISA do .env se:
- ✅ Só quer que a aplicação funcione normalmente
- ✅ Não precisa de alertas via WhatsApp
- ✅ Só quer ver os dados no dashboard web

## 🐳 E no Docker?

Quando você usa Docker, o arquivo `.env` funciona da mesma forma:

1. Crie o arquivo `backend/.env` na sua máquina
2. O Docker Compose lê automaticamente esse arquivo
3. As variáveis são passadas para o container

**No docker-compose.yml:**
```yaml
env_file:
  - ./backend/.env  # Docker lê este arquivo automaticamente
```

## 📊 Exemplo Prático

### Sem .env:
```
ESP32 → API → Frontend ✅
ESP32 → API → WhatsApp ❌ (não configurado)
```

### Com .env configurado:
```
ESP32 → API → Frontend ✅
ESP32 → API → WhatsApp ✅ (envia alertas!)
```

## 🔍 Como verificar se está funcionando?

### 1. Verificar se o arquivo existe:
```bash
# Windows PowerShell
Test-Path backend\.env

# Deve retornar: True
```

### 2. Verificar se o Docker está lendo:
```bash
docker-compose logs backend | grep "WHATSAPP"
```

### 3. Testar WhatsApp:
```bash
cd backend
node test-whatsapp.js
```

Se estiver configurado, você receberá uma mensagem de teste no WhatsApp!

## ❓ Resumo Rápido

| Pergunta | Resposta |
|----------|----------|
| **O que é?** | Arquivo de configuração com dados sensíveis |
| **Onde fica?** | Na pasta `backend/` |
| **É obrigatório?** | ❌ Não! Só se quiser WhatsApp |
| **O que tem dentro?** | Número de telefone, chave API, configurações |
| **Precisa no Docker?** | Sim, se quiser WhatsApp funcionando |
| **Vai para o Git?** | ❌ Não! Está no .gitignore |

## 🎯 Conclusão

- **Arquivo .env = Configurações pessoais**
- **Opcional** para o projeto funcionar
- **Necessário** apenas se quiser alertas via WhatsApp
- **Seguro** (não vai para o Git)
- **Fácil** de criar e configurar

---

**Dúvida?** O projeto funciona perfeitamente sem o `.env`! Ele só adiciona a funcionalidade de alertas via WhatsApp.

