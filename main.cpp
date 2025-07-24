#include <Wire.h>                 // NOVO: Biblioteca para comunicação I2C
#include <Adafruit_GFX.h>         // NOVO: Biblioteca principal para gráficos
#include <Adafruit_SSD1306.h>     // NOVO: Biblioteca para o driver do display OLED

// --- Configurações do Display OLED ---
#define SCREEN_WIDTH 128          // NOVO: Largura do display em pixels
#define SCREEN_HEIGHT 64          // NOVO: Altura do display em pixels
#define OLED_RESET -1             // NOVO: Pino de reset (-1 se compartilhar o pino de reset do Arduino)
#define SCREEN_ADDRESS 0x3C       // NOVO: Endereço I2C do display (pode ser 0x3D em alguns modelos)

// NOVO: Cria o objeto 'display' usando as configurações acima
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- Variáveis da Sonda Lambda (seu código original) ---
const int adcPin = A0;
float voltage = 0;
bool sondaPronta = false;
unsigned long tempoEstavel = 0;
const float limiteInferior = 0.1;
const float limiteSuperior = 0.9;

void setup() {
  Serial.begin(115200);

  // NOVO: Inicializa o display OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Falha ao iniciar o display OLED"));
    for (;;); // Loop infinito em caso de falha
  }
  display.clearDisplay(); // Limpa o buffer do display
  display.display();      // Envia o buffer limpo para a tela
}

void loop() {
  int raw = analogRead(adcPin);
  voltage = raw * (1.0 / 1023.0); // ESP8266 ADC: 10 bits, máx 1.0V

  // Lógica para verificar se a sonda está pronta (seu código original)
  if (voltage >= limiteInferior && voltage <= limiteSuperior) {
    if (!sondaPronta && millis() - tempoEstavel > 3000) {
      sondaPronta = true;
      Serial.println("✅ Sonda lambda PRONTA para leitura!");
    }
  } else {
    tempoEstavel = millis();
    sondaPronta = false;
  }

  // NOVO: Limpa a tela a cada atualização para não sobrepor o texto
  display.clearDisplay();
  
  // NOVO: Define o tamanho e a cor do texto
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // NOVO: Posiciona o cursor para a primeira linha
  display.setCursor(0, 0);

  if (sondaPronta) {
    // ---- Imprime na Serial (seu código original) ----
    Serial.print("Tensão da Sonda: ");
    Serial.print(voltage, 3);
    Serial.print(" V -> Mistura: ");

    // ---- Desenha no Display OLED ----
    // NOVO: Imprime a tensão na primeira linha do display
    display.print("Tensao: ");
    display.print(voltage, 3);
    display.println(" V");
    display.println(""); // Pula uma linha

    // NOVO: Define um texto maior para o status da mistura
    display.setTextSize(2);
    display.setCursor(0, 20); // Posiciona o cursor mais para baixo

    // Lógica para definir a mensagem de mistura
    if (voltage > 0.75) {
      Serial.println("Muito Rica");
      display.println("MUITO RICA");
    } else if (voltage > 0.60) {
      Serial.println("Rica");
      display.println("RICA");
    } else if (voltage > 0.45) {
      Serial.println("Moderada (Estequiométrica)");
      display.println("IDEAL");
    } else if (voltage > 0.30) {
      Serial.println("Pobre");
      display.println("POBRE");
    } else {
      Serial.println("Muito Pobre");
      display.println("MUITO POBRE");
    }
  } else {
    Serial.println("⏳ Aguardando sonda aquecer...");

    // NOVO: Exibe mensagem de "aquecendo" no display
    display.setTextSize(2);
    display.setCursor(10, 20);
    display.println("AQUECENDO");
  }

  // NOVO: Envia tudo o que foi desenhado no buffer para a tela do display. ESSENCIAL!
  display.display();

  delay(250);
}
