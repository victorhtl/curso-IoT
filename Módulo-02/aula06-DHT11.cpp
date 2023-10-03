#include <Arduino.h>

#include "DHT.h" //hwspeedy/DHT-Sensor@^1.4.3

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

  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.print("% Temperatura: ");
  Serial.print(t);
  Serial.println("°C");
}