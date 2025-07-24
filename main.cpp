// ✅ Projeto: Sonda Lambda - ESP12E (ESP8266) como Servidor Web (Access Point)
// ✅ Função: Cria uma rede Wi-Fi local para mostrar tensão da sonda e classificação da mistura

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Sonda_Jusepe";
const char* password = "bajasae2025";

ESP8266WebServer server(80);

const int adcPin = A0;
float voltage = 0.0;
String mistura = "";

void setup() {
  Serial.begin(115200);

  // Cria Access Point
  WiFi.softAP(ssid, password);
  Serial.println("\n[AP Iniciado]" );
  Serial.print("IP do ESP: ");
  Serial.println(WiFi.softAPIP());

  // Rota principal
  server.on("/", []() {
    voltage = analogRead(adcPin) * (1.0 / 1023.0);

    if (voltage > 0.75) mistura = "🔥 Muito Rica";
    else if (voltage > 0.60) mistura = "🔥 Rica";
    else if (voltage > 0.45) mistura = "⚖️ Ideal (Estequiométrica)";
    else if (voltage > 0.30) mistura = "❄️ Pobre";
    else mistura = "❄️ Muito Pobre";

    String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<style>body{font-family:sans-serif;text-align:center;} .caixa{border:1px solid #ccc; padding:20px; margin-top:50px; border-radius:10px; box-shadow:0px 0px 10px #888;} h1{color:#333}</style></head><body>";
    html += "<div class='caixa'><h1>Sonda Lambda - Baja SAE</h1>";
    html += "<p><strong>Tensão:</strong> " + String(voltage, 3) + " V</p>";
    html += "<p><strong>Mistura:</strong> " + mistura + "</p></div></body></html>";

    server.send(200, "text/html", html);
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
