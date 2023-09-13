#include <Arduino.h>

const int ledPin = 4; // Pino no qual o PWM será gerado

void setup() {
  Serial.begin(115200);

  // Inicializa o pino como saída
  pinMode(ledPin, OUTPUT);

  // Inicializa o canal PWM com frequência de 25000Hz e resolução de 8 bits
  ledcSetup(0, 25000, 8);

  // Vincula o canal PWM ao pino
  ledcAttachPin(ledPin, 0);
}

void loop(){
  // Gere PWM variando o valor de duty cycle de 0 a 255
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    Serial.println(dutyCycle);
    ledcWrite(0, dutyCycle); // Escreve o valor do duty cycle no canal PWM
    delay(20); // Aguarda um curto período de tempo
  }

  // Gere PWM variando o valor de duty cycle de 255 a 0
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    Serial.println(dutyCycle);
    ledcWrite(0, dutyCycle); // Escreve o valor do duty cycle no canal PWM
    delay(20); // Aguarda um curto período de tempo
  }
}