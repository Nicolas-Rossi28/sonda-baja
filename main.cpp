const int adcPin = A0;
float voltage = 0;
bool sondaPronta = false;
unsigned long tempoEstavel = 0;
const float limiteInferior = 0.1;
const float limiteSuperior = 0.9;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int raw = analogRead(adcPin);
  voltage = raw * (1.0 / 1023.0); // ESP8266 ADC: 10 bits, máx 1.0V

  if (voltage >= limiteInferior && voltage <= limiteSuperior) {
    if (!sondaPronta && millis() - tempoEstavel > 3000) {
      sondaPronta = true;
      Serial.println("✅ Sonda lambda PRONTA para leitura!");
    }
  } else {
    tempoEstavel = millis();
    sondaPronta = false;
  }

  if (sondaPronta) {
    Serial.print("Tensão da Sonda: ");
    Serial.print(voltage, 3);
    Serial.print(" V -> Mistura: ");

    if (voltage > 0.75) {
      Serial.println("🔥 Muito Rica");
    } else if (voltage > 0.60) {
      Serial.println("🔥 Rica");
    } else if (voltage > 0.45) {
      Serial.println("⚖️ Moderada (Estequiométrica)");
    } else if (voltage > 0.30) {
      Serial.println("❄️ Pobre");
    } else {
      Serial.println("❄️ Muito Pobre");
    }
  } else {
    Serial.println("⏳ Aguardando sonda aquecer...");
  }

  delay(250);
}
