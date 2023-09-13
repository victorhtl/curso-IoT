#include <Arduino.h>

#include "DHT.h"

#define DHTPIN 4

// Definir o modelo
#define DHTTYPE DHT11
//#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  // Porque o sensor é lento
  delay(2000); 

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if(isnan(h) || isnan(t)){
    Serial.println("A leitura falhou...");
  }

  Serial.print("Humidade: ");
  Serial.print(h);
  Serial.print("% Temperaruta: ");
  Serial.print(t);
  Serial.println("°C");
}