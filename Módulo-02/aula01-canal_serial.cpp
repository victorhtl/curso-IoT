#include <Arduino.h>

#define LED 2

void setup(){
  // Lembre de configurar monitor_speed = 115200 em platformio.ini
  Serial.begin(115200); // baudrate
  pinMode(LED, OUTPUT);
}

void loop(){ 
  digitalWrite(LED, HIGH);
  Serial.println("Led Ligado");
  delay(500);
  digitalWrite(LED, LOW);
  Serial.println("Led Desligado");
  delay(500);
}