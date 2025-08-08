#include <HX711.h>
#include <SPI.h>
#include <SD.h>

// --- HX711 ---
#define DT D6   // GPIO12 no ESP12E
#define SCK D5  // GPIO14 no ESP12E

// --- Cartao SD ---
#define SD_CS   D2    // Chip Select do SD
#define SD_MOSI D7    // MOSI do SD (GPIO13)
#define SD_MISO D8    // MISO do SD (GPIO15)
#define SD_SCK  D4    // SCK do SD (GPIO2)

HX711 scale;

// --- Variáveis ---
float entrada_mv = 0;
String classificacao = "";
File dataFile;
char filename[32]; // espaço para o nome do arquivo

void setup() {
  Serial.begin(115200);

  // Inicializa o HX711
  scale.begin(DT, SCK);
  scale.set_gain(128);

  SPI.begin();
  if (!SD.begin(SD_CS)) {
    Serial.println("Falha ao iniciar cartão SD!");
    while (1);
  }

  // Criação automática do arquivo
  int file_number = 0;
  do {
    sprintf(filename, "/datalog_%d.csv", file_number++);
    if (file_number > 1000) {
      Serial.println("Limite de arquivos atingido!");
      while (1);
    }
  } while (SD.exists(filename));

  dataFile = SD.open(filename, FILE_WRITE);
  if (!dataFile) {
    Serial.println("Erro ao criar arquivo no SD!");
    while (1);
  }
  Serial.print("Arquivo criado: ");
  Serial.println(filename);

  // Cabeçalho do arquivo
  dataFile.println("mv;classificacao");
  dataFile.flush();
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
    if (entrada_mv > 0.9) {
      Serial.print("Tensao (mV): ");
      Serial.print(entrada_mv, 2);
      Serial.print(" | Mistura: ");
      Serial.println(classificacao);

      // Salvando no SD card
      dataFile.print(entrada_mv, 2);
      dataFile.print(";");
      dataFile.println(classificacao);
      dataFile.flush(); // garante que os dados sejam gravados imediatamente
    }
  }

  delay(300); // ajustável
}
