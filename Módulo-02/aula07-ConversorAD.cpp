#include <Arduino.h>

// Trimpot conectado ao pino 34
#define potPin 34

#define ledPin 2

int dutyCycle;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Configura o PWM a uma resolução de 12 bits, desta forma
  // o dutyCycle vai variar de 0 a 4095, assim como o conversor AD
  ledcSetup(0, 1000, 12);
  ledcAttachPin(ledPin, 0);
}

void loop() {
  dutyCycle = analogRead(potPin);
  // Serial.println(dutyCycle); // Descomente esta linha para ver o dutyCycle

  // Conforme o dutyCycle, lido pelo canal analógico, aumenta
  // o led brilhará mais forte
  ledcWrite(0, dutyCycle);
  delay(500);
}