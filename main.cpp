#include <HX711.h>

// --- HX711 ---
#define DT D6   // GPIO12 no ESP12E
#define SCK D5  // GPIO14 no ESP12E
HX711 scale;

// --- Variáveis ---
float entrada_mv = 0;
String classificacao = "";

void setup() {
  Serial.begin(115200);

  // Inicializa o HX711
  scale.begin(DT, SCK);
  scale.set_gain(128);

  Serial.println("Sonda Lambda - Modo Serial");
  delay(1000);
}

void loop() {
  if (scale.is_ready()) {
    long leitura = scale.read();

    // Calibração 
    entrada_mv = (leitura / 8388607.0) * 1200.0 * 0.0123;

    // Classificação
    if (entrada_mv > 5.7) {
      classificacao = "Rica";
    } else if (entrada_mv >= 3.5) {
      classificacao = "Ideal";
    } else {
      classificacao = "Pobre";
    }

    //Serial padrão
    Serial.print("Tensao (mV): ");
    Serial.print(entrada_mv, 2);
    Serial.print(" | Mistura: ");
    Serial.println(classificacao);

    //Serial Plotter
    Serial.print("mV:");
    Serial.println(entrada_mv);

  } else {
    Serial.println("HX711 não está pronto");
  }

  delay(300); // ajustável
}
