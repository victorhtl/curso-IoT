#include <Arduino.h>

#define PIN 2

// Setup -> configuração inicial
void setup(){
  pinMode(2, OUTPUT);
}

void loop(){ // while()
  digitalWrite(2, HIGH); // 1
  delay(500);
  digitalWrite(2, LOW);  // 0
  delay(500);
}