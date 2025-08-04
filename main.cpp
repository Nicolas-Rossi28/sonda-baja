#include <HX711.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WebServer.h>

// --- Display OLED ---
#define SCREEN_WIDTH 128
#define SDA_PIN 21 // OLED
#define SCL_PIN 22 // OLED
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- HX711 ---
#define DT 18   // GPIO18
#define SCK 19  // GPIO19
HX711 scale;

// --- Wi-Fi (modo Access Point) ---
const char* ssid = "Sonda_Imperador";
const char* password = "saebaja";
WebServer server(80);

// --- Variáveis ---
float entrada_mv = 0;
String classificacao = "";

void setup() {
  Serial.begin(115200);

  // Display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Erro no display OLED");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 20);
  display.println("Sonda");
  display.setCursor(10, 40);
  display.println("Case Jusepe");
  display.display();
  delay(2000);

  // HX711
  scale.begin(DT, SCK);
  scale.set_gain(128);

  // Wi-Fi Access Point
  WiFi.softAP(ssid, password);
  Serial.println("Access Point iniciado");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  // Página HTML
  server.on("/", []() {
    String html = "<!DOCTYPE html><html><head><meta http-equiv='refresh' content='1'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<style>body{font-family:sans-serif;text-align:center;}h1{font-size:24px;}</style>";
    html += "</head><body>";
    html += "<h1><b>Sonda Lambda</b></h1>";
    html += "<p><b>Tensao:</b> " + String(entrada_mv, 2) + " mV</p>";
    html += "<p><b>Mistura:</b> " + classificacao + "</p>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });

  server.begin();
}

void loop() {
  server.handleClient();

  if (scale.is_ready()) {
    long leitura = scale.read();

    // Calibração empírica baseada na leitura real: ~4.5 mV → 1200 mV brutos
    entrada_mv = (leitura / 8388607.0) * 1200.0 * 0.0123; // Corrigido

    // Classificação com novos limites
    if (entrada_mv > 5.7) {
      classificacao = " Rica";
    } else if (entrada_mv >= 3.5) {
      classificacao = " Ideal";
    } else {
      classificacao = " Pobre";
    }

    // Serial Monitor
    Serial.print("Tensão: ");
    Serial.print(entrada_mv, 2);
    Serial.print(" mV -> ");
    Serial.println(classificacao);

  } else {
    Serial.println(" HX711 não está pronto");
    entrada_mv = 0;
    classificacao = "Erro";
  }

  // Display OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Tensao: ");
  display.print(entrada_mv, 2);
  display.println(" mV");

  display.setTextSize(2);
  display.setCursor(0, 20);
  display.println(classificacao);
  display.display();

  delay(300);
}
