#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// --- Configurações do WiFi Access Point ---
const char* ssid = "Sonda_Jusepe";
const char* password = "bajasae2025";

ESP8266WebServer server(80);

// --- Configurações do Display OLED ---
#define SCREEN_WIDTH 128    // Largura do display OLED (pixels)
#define SCREEN_HEIGHT 64    // Altura do display OLED (pixels)
#define OLED_RESET -1       // Pino de reset do OLED (ou -1 se for compartilhado com o ESP)
#define SCREEN_ADDRESS 0x3C // Endereço I2C para a maioria dos displays SSD1306
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- Variáveis da Sonda Lambda ---
const int adcPin = A0;      // Pino ADC do ESP12E
float voltage = 0;          // Variável para armazenar a tensão lida
String mistura = "";        // Variável para armazenar a classificação da mistura

// --- FUNÇÃO DE SETUP: Inicializações e Configurações ---
void setup() {
  Serial.begin(115200); // Inicia a comunicação serial para depuração

  // --- Configurações do Access Point WiFi ---
  WiFi.softAP(ssid, password);
  Serial.println("\n[AP Iniciado]");
  Serial.print("IP do ESP: ");
  Serial.println(WiFi.softAPIP());

  // --- Inicializa o Display OLED ---
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Falha ao iniciar o display OLED"));
    for (;;); // Loop infinito em caso de falha, se o display não iniciar
  }

  // --- Mensagem de Boas-Vindas no Display ---
  display.clearDisplay();
  display.setTextSize(2); // Tamanho da fonte
  display.setTextColor(WHITE); // Cor do texto
  display.setCursor(10, 20); // Posição do cursor
  display.println("Sonda");
  display.setCursor(10, 40);
  display.println("Case Jusepe");
  display.display(); // Mostra o conteúdo do buffer no display
  delay(2000);       // Mantém a mensagem por 2 segundos

  display.clearDisplay(); // Limpa o display
  display.display();      // Atualiza o display (agora limpo)

  // --- Configuração da Rota do Servidor Web ---
  // Esta função é chamada SOMENTE quando alguém acessa o IP do ESP no navegador.
  server.on("/", []() {
    // A leitura do ADC é feita no momento da requisição para ter o valor mais atualizado
    int raw = analogRead(adcPin);
    voltage = raw * (1.0 / 1023.0); // Calibração padrão para 0-1V sem divisor ou divisor calibrado para 1V final

    // --- Lógica para classificar a mistura ---
    if (voltage > 0.75) {
      mistura = "Muito Rica";
    } else if (voltage > 0.60) {
      mistura = "Rica";
    } else if (voltage > 0.45) { // Ponto estequiométrico nominal
      mistura = "Ideal (Estequiometrica)";
    } else if (voltage > 0.30) {
      mistura = "Pobre";
    } else {
      mistura = "Muito Pobre";
    }

    // --- Monta o HTML da página web ---
    String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
    // --- META REFRESH: Atualiza a página a cada 3 segundos ---
    html += "<meta http-equiv='refresh' content='3'>";
    html += "<style>";
    html += "body{font-family:sans-serif;text-align:center;background-color:#f0f0f0;margin:0;padding:20px;}";
    html += ".caixa{border:1px solid #ccc; padding:20px; margin-top:50px; border-radius:10px; box-shadow:0px 0px 10px rgba(0,0,0,0.1); background-color:#fff; max-width:400px; margin-left:auto; margin-right:auto;}";
    html += "h1{color:#333; font-size:1.8em;}";
    html += "p{font-size:1.2em; color:#555;}";
    html += "strong{color:#000;}";
    html += "</style></head><body>";
    html += "<div class='caixa'><h1>Case Sonda Lambda<br>Jusepe - Baja Imperador</h1>";
    html += "<p><strong>Tensao:</strong> " + String(voltage, 3) + " V</p>"; // String(voltage, 3) para 3 casas decimais
    html += "<p><strong>Mistura:</strong> " + mistura + "</p></div></body></html>";

    // Envia a página HTML para o navegador
    server.send(200, "text/html", html);
  });

  server.begin(); // Inicia o servidor HTTP, pronto para receber requisições
  Serial.println("Servidor HTTP iniciado.");
}

// --- FUNÇÃO DE LOOP: Executa repetidamente ---
void loop() {
  server.handleClient(); // Permite que o servidor web processe as requisições dos clientes

  // --- Leitura e Classificação para o Display OLED e Serial ---
  // A leitura do ADC é feita aqui também para o display e serial
  int raw = analogRead(adcPin);
  voltage = raw * (1.0 / 1023.0); // Calibração padrão para 0-1V sem divisor ou divisor calibrado para 1V final

  // --- Saída Serial para Depuração ---
  Serial.print("Tensao: ");
  Serial.print(voltage, 3);
  Serial.print(" V -> Mistura: ");

  // --- Atualiza o Display OLED ---
  display.clearDisplay(); // Limpa o buffer do display para a próxima atualização
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Tensao: ");
  display.print(voltage, 3);
  display.println(" V");

  display.setTextSize(2);
  display.setCursor(0, 20);

  // Lógica de classificação para o display e serial
  if (voltage > 0.75) {
    Serial.println(" Muito Rica");
    display.println("MUITO RICA");
  } else if (voltage > 0.60) {
    Serial.println(" Rica");
    display.println("RICA");
  } else if (voltage > 0.45) {
    Serial.println(" Estequiometrica");
    display.println("IDEAL");
  } else if (voltage > 0.30) {
    Serial.println(" Pobre");
    display.println("POBRE");
  } else {
    Serial.println(" Muito Pobre");
    display.println("MUITO POBRE");
  }

  display.display(); // Envia o conteúdo do buffer para o display físico
  delay(250);        // Pequeno atraso para não sobrecarregar
}
