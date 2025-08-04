# 🚗 Sonda Lambda Reader - Baja SAE (ESP12E + HX711 + OLED + Wi-Fi)

Projeto desenvolvido para o desafio de eletrônica do Baja SAE, UTFPR Curitiba – Equipe Imperador.  
Este sistema permite a leitura da sonda lambda narrowband com saída de 1–9 mV, utilizando um amplificador HX711, visualização local em um display OLED 128x64 e monitoramento remoto via servidor Wi-Fi embutido no ESP12E.

---

## 📦 Requisitos

### ✅ Hardware
- ESP8266 ESP-12E (NodeMCU)
- Módulo HX711 (24-bit ADC)
- Display OLED 128x64 (I2C)
- Sonda lambda Gauss GO9460 (ou equivalente narrowband)
- Fonte 12V (bateria) + regulador 5V (ex: L298N)

### ✅ Driver USB
> Seu ESP12E provavelmente usa o chip **CP2102** para comunicação USB. Instale o driver abaixo:

- [CP2102 USB to UART Driver (Silicon Labs)](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)

---

## 🔧 Arduino IDE – Configuração Inicial

### 1. Instale o suporte ao ESP8266

- Acesse **Arquivo > Preferências**
- Em "URLs adicionais para Gerenciadores de Placas", adicione:
http://arduino.esp8266.com/stable/package_esp8266com_index.json
- Vá em **Ferramentas > Placa > Gerenciador de Placas**
- Busque por `esp8266`
- Instale **esp8266 by ESP8266 Community**

### 2. Selecione a placa
- Placa: `NodeMCU 1.0 (ESP-12E Module)`
- Flash Size: `4MB (FS:none)`
- Upload Speed: `115200`
- Porta: selecione a porta COM correta (ex: `COM3`)

---

## 📚 Bibliotecas Arduino Necessárias

Instale as seguintes bibliotecas em **Sketch > Incluir Biblioteca > Gerenciar Bibliotecas...**:

- **HX711** (por Bogde)
- **Adafruit GFX Library**
- **Adafruit SSD1306**
- **ESP8266WiFi**
- **ESP8266WebServer**

---

## 📡 Funcionalidade

- O ESP cria um ponto de acesso Wi-Fi:  
**SSID:** `Sonda_Jusepe`  
**Senha:** `12345678`

- Acesse via navegador no celular conectado ao Wi-Fi:  
[http://192.168.4.1](http://192.168.4.1)

- A página exibirá:
- Tensão da sonda (em mV)
- Classificação da mistura: **Pobre / Ideal / Rica**
- Atualiza automaticamente a cada 1 segundo

- O display OLED mostra os mesmos dados localmente no veículo.

---

## 🛠️ Calibração

Este projeto usa um fator de calibração empírico com base na leitura real da sonda comparada ao multímetro.  
Ajustes futuros podem ser feitos com base em novos testes, ou utilizando fontes de referência conhecidas.

---

