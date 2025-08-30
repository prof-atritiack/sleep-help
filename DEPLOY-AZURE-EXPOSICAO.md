# 🚀 Deploy na Azure e Preparação para Exposição

Este documento explica como fazer o deploy do projeto Sleep Help na Azure e preparar um QR code para os visitantes da exposição acessarem o site.

## 📋 Índice

1. [Deploy na Azure](#deploy-na-azure)
2. [Configuração do Projeto](#configuração-do-projeto)
3. [Criação do QR Code](#criação-do-qr-code)
4. [Checklist Completo](#checklist-completo)
5. [Custos e Alternativas](#custos-e-alternativas)
6. [Dicas para a Exposição](#dicas-para-a-exposição)
7. [Solução de Problemas](#solução-de-problemas)

---

## 🎯 Deploy na Azure

### Opção 1: Azure Static Web Apps (Recomendado)

Esta é a opção mais simples e gratuita para sites estáticos como o nosso projeto:

#### Passo 1: Preparar o Projeto
```bash
# Certifique-se de que o projeto está funcionando localmente
npm start

# Gere a versão de produção
npm run build
```

#### Passo 2: Criar Static Web App na Azure
1. Acesse o [Azure Portal](https://portal.azure.com)
2. Clique em "Criar um recurso"
3. Pesquise por "Static Web App"
4. Clique em "Criar"
5. Preencha os campos:
   - **Assinatura**: Sua assinatura Azure
   - **Grupo de recursos**: Crie um novo ou use existente
   - **Nome**: `sleep-help-app` (ou outro nome único)
   - **Região**: Brasil Sul (ou mais próxima)
   - **Plano**: Gratuito
   - **Build details**: 
     - Build Preset: Custom
     - App location: `/`
     - Output location: `/build`
     - API location: (deixe vazio)

#### Passo 3: Deploy
- **Se conectou com GitHub**: O deploy será automático
- **Se não**: Faça upload manual da pasta `build`

### Opção 2: Azure App Service

Para mais controle sobre o ambiente:

#### Passo 1: Criar App Service
1. Escolha "Web App" no Azure Portal
2. Selecione Node.js como runtime
3. Configure o plano de hospedagem (F1 é gratuito)

#### Passo 2: Deploy via Azure CLI
```bash
# Instalar Azure CLI (se não tiver)
# Windows: https://docs.microsoft.com/en-us/cli/azure/install-azure-cli-windows

# Login na Azure
az login

# Deploy do projeto
az webapp up --name seu-app-name --resource-group seu-rg --plan seu-plan
```

---

## ⚙️ Configuração do Projeto

### Verificar Configurações

O projeto já está configurado corretamente com:
- ✅ Script `build` no `package.json`
- ✅ Configuração de produção no `public/index.html`
- ✅ Dependências necessárias instaladas

### Build de Produção

```bash
# Gerar versão de produção
npm run build

# Testar a versão build localmente
# Abra a pasta 'build' e teste o index.html
```

---

## 🔗 Criação do QR Code

### Opção 1: Gerador Online Gratuito
- [QR Code Generator](https://www.qr-code-generator.com/)
- [QRCode Monkey](https://www.qrcode-monkey.com/)

### Opção 2: Gerador Personalizado (Incluído)

Use o arquivo `generate-qr.html` que criamos para gerar QR codes personalizados:

1. **Abra o arquivo**: `generate-qr.html` no navegador
2. **Insira a URL**: Cole a URL do Azure (ex: `https://sleep-help-app.azurestaticapps.net`)
3. **Gere o QR**: Clique em "Gerar QR Code"
4. **Baixe**: Clique em "Baixar QR Code"
5. **Imprima**: Imprima em alta qualidade (300 DPI)

### Especificações do QR Code
- **Tamanho recomendado**: 10x10 cm para fácil leitura
- **Resolução**: 300 DPI para impressão
- **Formato**: PNG para melhor qualidade
- **Cores**: Preto sobre branco para máximo contraste

---

## ✅ Checklist Completo

### Pré-deploy
- [ ] Projeto funcionando localmente (`npm start`)
- [ ] Build gerado com sucesso (`npm run build`)
- [ ] Versão build testada localmente
- [ ] Conta Azure criada e ativa

### Azure Setup
- [ ] Static Web App criado no Azure Portal
- [ ] Configurações de build configuradas
- [ ] Deploy realizado com sucesso
- [ ] URL anotada e testada
- [ ] Site acessível publicamente

### QR Code
- [ ] QR code gerado com a URL correta
- [ ] Imagem baixada em alta qualidade
- [ ] QR code impresso em tamanho adequado
- [ ] QR code testado com diferentes dispositivos
- [ ] Instruções impressas junto ao QR code

### Exposição
- [ ] QR code colado em local visível
- [ ] Dispositivo de demonstração funcionando
- [ ] Backup offline preparado
- [ ] Wi-Fi configurado e testado
- [ ] Equipe treinada para suporte

---

## 💰 Custos e Limitações

### Azure Static Web Apps (Gratuito)
- **2GB de armazenamento**
- **100GB de transferência/mês**
- **1 build por dia**
- **Perfeito para projetos de exposição**

### Alternativas Gratuitas
- **Netlify**: Deploy direto do GitHub
- **Vercel**: Deploy automático
- **GitHub Pages**: Hospedagem gratuita

### Custos Adicionais (Opcional)
- **Domínio personalizado**: ~R$ 30/ano
- **SSL personalizado**: Gratuito com Let's Encrypt
- **CDN**: Incluído no plano gratuito

---

## 💡 Dicas para a Exposição

### Preparação
1. **Teste tudo antes**: Verifique se o QR code funciona
2. **Backup offline**: Tenha uma versão local funcionando
3. **Wi-Fi**: Certifique-se de que há internet na exposição
4. **Demonstração**: Tenha um dispositivo funcionando para mostrar
5. **Instruções**: Adicione texto explicativo junto ao QR code

### Durante a Exposição
1. **Monitoramento**: Verifique se o site está funcionando
2. **Suporte**: Tenha alguém disponível para ajudar visitantes
3. **Feedback**: Colete opiniões dos usuários
4. **Backup**: Tenha um plano B caso algo dê errado

### Apresentação
1. **Demonstração ao vivo**: Mostre o projeto funcionando
2. **Explicação técnica**: Conte como foi desenvolvido
3. **Impacto social**: Explique a importância do projeto
4. **Futuro**: Fale sobre próximos passos

---

## 🔧 Solução de Problemas

### Problemas Comuns

#### Site não carrega
- Verifique se o deploy foi concluído
- Teste a URL no navegador
- Verifique logs no Azure Portal

#### QR code não funciona
- Teste com diferentes aplicativos
- Verifique se a URL está correta
- Imprima em melhor qualidade

#### Problemas de performance
- Verifique o tamanho dos arquivos
- Otimize imagens se necessário
- Use CDN se disponível

#### Problemas de rede
- Tenha backup offline
- Configure hotspot no celular
- Use rede da instituição

### Contatos de Suporte
- **Azure**: [Documentação oficial](https://docs.microsoft.com/azure/static-web-apps/)
- **React**: [Documentação oficial](https://reactjs.org/docs/)
- **Comunidade**: Stack Overflow, GitHub Issues

---

## 📚 Recursos Adicionais

### Documentação
- [Azure Static Web Apps](https://docs.microsoft.com/azure/static-web-apps/)
- [React Deployment](https://create-react-app.dev/docs/deployment/)
- [QR Code Best Practices](https://www.qr-code-generator.com/qr-code-marketing/qr-codes-best-practices/)

### Ferramentas
- [Azure Portal](https://portal.azure.com)
- [Azure CLI](https://docs.microsoft.com/en-us/cli/azure/)
- [QR Code Scanner](https://play.google.com/store/apps/details?id=com.google.zxing.client.android)

### Vídeos Tutoriais
- [Deploy React na Azure](https://www.youtube.com/watch?v=example)
- [Criar QR Codes](https://www.youtube.com/watch?v=example)

---

## 🎉 Conclusão

Com este guia, você tem tudo necessário para:
1. ✅ Fazer deploy do projeto na Azure
2. ✅ Criar QR codes personalizados
3. ✅ Preparar a apresentação na exposição
4. ✅ Oferecer uma experiência profissional aos visitantes

**Boa sorte na exposição! 🚀**

---

*Última atualização: $(Get-Date)*
*Versão do documento: 1.0*
