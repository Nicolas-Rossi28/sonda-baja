#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- Configurações do Display OLED ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- Variáveis da Sonda Lambda ---
const int adcPin = A0;
float voltage = 0;
bool sondaPronta = false;
unsigned long tempoEstavel = 0;
const float limiteInferior = 0.1;
const float limiteSuperior = 0.9;

void setup() {
  Serial.begin(115200);

  // Inicializa o display OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Falha ao iniciar o display OLED"));
    for (;;); // Loop infinito em caso de falha
  }

  // Exibe mensagem de boas-vindas
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 20);
  display.println("Sonda");
  display.setCursor(10, 40);
  display.println("Case Jusepe");
  display.display();
  delay(2000);  // Mostra a tela por 2 segundos

  display.clearDisplay();
  display.display();
}

void loop() {
  int raw = analogRead(adcPin);
  voltage = raw * (1.0 / 1023.0); // ESP8266 ADC: 10 bits, faixa de 0 a 1.0V

      sondaPronta = true;
      Serial.println("✅ Sonda lambda PRONTA para leitura!");


  // Limpa display para nova atualização
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  if (sondaPronta) {
    // Serial
    Serial.print("Tensao: ");
    Serial.print(voltage, 3);
    Serial.print(" V -> Mistura: ");

    // Display: mostra tensão
    display.print("Tensao: ");
    display.print(voltage, 3);
    display.println(" V");

    // Display: mostra classificação da mistura
    display.setTextSize(2);
    display.setCursor(0, 20);

    if (voltage > 0.75) {
      Serial.println(" Muito Rica");
      display.println("MUITO RICA");
    } else if (voltage > 0.60) {
      Serial.println(" Rica");
      display.println("RICA");
    } else if (voltage > 0.45) {
      Serial.println(" Estequiométrica");
      display.println("IDEAL");
    } else if (voltage > 0.30) {
      Serial.println(" Pobre");
      display.println("POBRE");
    } else {
      Serial.println(" Muito Pobre");
      display.println("MUITO POBRE");
    }

  } 

  display.display(); // Atualiza o display com o conteúdo do buffer
  delay(250);
}

