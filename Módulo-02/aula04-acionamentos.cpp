#include <Arduino.h>

const int pinoRele = 4;
String serialInput;

void setup() {
  Serial.begin(115200);
  pinMode(pinoRele, OUTPUT);
  digitalWrite(pinoRele, HIGH);
}

void loop(){
  if(Serial.available() > 0){
    serialInput = Serial.readStringUntil('\n');

    if(serialInput == "liga rele"){
      digitalWrite(pinoRele, LOW);
      Serial.println("Ligado...");
    } else if(serialInput == "desliga rele"){
      digitalWrite(pinoRele, HIGH);
      Serial.println("Desligado...");
    } else {
      Serial.println("Nao reconhecido...");
    }
  }
}