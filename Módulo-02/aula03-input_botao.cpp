#include <Arduino.h>

const int pinoLED = 2;
const int pinoBotao = 4;

int estadoBotao = 0;

void setup() {
  pinMode(pinoLED, OUTPUT);
  pinMode(pinoBotao, INPUT_PULLUP);
}

void loop() {
  estadoBotao = digitalRead(pinoBotao);

  if(estadoBotao == LOW){
    digitalWrite(pinoLED, HIGH);
    delay(100);
  } else {
    digitalWrite(pinoLED, LOW);
  }
}