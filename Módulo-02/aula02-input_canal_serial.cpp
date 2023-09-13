#include <Arduino.h>

#define LED 2

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
}

void loop() {
  if (Serial.available() > 0){
    String input = Serial.readStringUntil('\n');

    if(input == "liga led"){
      digitalWrite(LED, HIGH);
      Serial.print(input);    // RESPOSTA DO DESAFIO
    } else if (input == "desliga led"){
      digitalWrite(LED, LOW);
      Serial.print(input);    // RESPOSTA DO DESAFIO
    } else {
      Serial.println("Comando nao reconhecido...");
    }
  }
}