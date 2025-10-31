# 📝 Como Configurar as Variáveis de Ambiente

## Passo a Passo

### 1. Criar o Arquivo .env

Na pasta `backend/`, crie um arquivo chamado `.env` (sem extensão).

**No Windows:**
```powershell
cd backend
New-Item -ItemType File -Name ".env"
```

**No Linux/Mac:**
```bash
cd backend
touch .env
```

### 2. Copiar o Template

Copie o conteúdo do arquivo `.env.example` e cole no arquivo `.env`, ou use o comando:

**Windows:**
```powershell
Copy-Item .env.example .env
```

**Linux/Mac:**
```bash
cp .env.example .env
```

### 3. Preencher os Dados

Abra o arquivo `.env` em um editor de texto e preencha:

#### a) WHATSAPP_PHONE

Seu número de telefone no formato internacional:
- **Sem espaços**
- **Sem caracteres especiais** (sem parênteses, hífens, etc.)
- **Com código do país**

**Exemplos:**
```
# Se você está no Brasil com número (11) 99999-9999:
WHATSAPP_PHONE=5511999999999

# Se você está nos EUA com número (555) 123-4567:
WHATSAPP_PHONE=15551234567

# Se você está em Portugal com número 912 345 678:
WHATSAPP_PHONE=351912345678
```

#### b) WHATSAPP_API_KEY

Cole a chave de API que você recebeu do CallMeBot.

Exemplo:
```
WHATSAPP_API_KEY=123456-ABC-DEF-789-GHI-JKL
```

#### c) ALARM_THRESHOLD (Opcional)

Por padrão é 95. Isso significa que SpO2 ≤ 95% gera alarme.

Se quiser mudar (exemplo: alarme apenas se SpO2 ≤ 90%):
```
ALARM_THRESHOLD=90
```

### 4. Exemplo Completo de Arquivo .env

```env
WHATSAPP_PHONE=5511999999999
WHATSAPP_API_KEY=sua_chave_aqui_123456
ALARM_THRESHOLD=95
PORT=3001
```

### 5. Verificar se Está Correto

O arquivo `.env` deve ter:
- ✅ Exatamente 3 linhas com valores (WHATSAPP_PHONE, WHATSAPP_API_KEY, ALARM_THRESHOLD)
- ✅ Nenhum espaço antes ou depois do `=`
- ✅ Número sem espaços ou caracteres especiais
- ✅ Chave de API sem aspas

### 6. Reiniciar o Servidor

Após salvar o arquivo `.env`, reinicie o servidor:

```bash
# Se estiver rodando, pare (Ctrl+C) e inicie novamente:
npm run dev
```

## ⚠️ Importante

- ❌ **NÃO** commite o arquivo `.env` no Git (ele já está no .gitignore)
- ❌ **NÃO** compartilhe sua chave de API
- ✅ Mantenha o arquivo `.env` apenas na sua máquina
- ✅ Use `.env.example` como template para outros desenvolvedores

## 🧪 Testar

Após configurar, você pode testar enviando dados de teste:

```bash
# Em outro terminal, teste com um valor de alarme (ex: SpO2 = 92)
curl -X POST http://localhost:3001/api/sensor-data \
  -H "Content-Type: application/json" \
  -d '{"spo2":92,"timestamp":"2025-01-27 14:30:45"}'
```

Se estiver tudo certo, você receberá uma mensagem no WhatsApp! 📱

## ❓ Problemas?

Se não funcionar, verifique:

1. ✅ Arquivo `.env` está na pasta `backend/`
2. ✅ Não há espaços extras no arquivo
3. ✅ Número está no formato correto (sem espaços, com código do país)
4. ✅ Chave de API está correta
5. ✅ Servidor foi reiniciado após criar o `.env`

Verifique os logs do servidor para mensagens de erro ou avisos.

