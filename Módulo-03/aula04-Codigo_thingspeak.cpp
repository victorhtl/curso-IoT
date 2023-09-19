#include <Arduino.h>
#include <WiFi.h>
//#include "password.cpp" // COLOQUE OS DADOS DA SUA REDE
#include "DHT.h"

// Configurações DHT
#define DHTPIN 4            // Coloque o pino onde você conectou o sensor
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Configurações da API
const char* host = "api.thingspeak.com";          // Este não altera
const int httpPort = 80;                          // Este não altera
const String channelID = "2273626";               // COLOCAR O SEU CHANNELID
const String writeApiKey = "GJLFHOAAM5AP47W4";    // COLOCAR A SUA KEY

float umidade = 0;
float temperatura = 0;

void readResponse(WiFiClient *client){
  unsigned long timeout = millis();
  while(client->available() == 0){
    if(millis() - timeout > 5000){
      Serial.println(">>> Client Timeout !");
      client->stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while(client->available()) {
    String line = client->readStringUntil('\r');
    Serial.print(line);
  }

  Serial.printf("\nClosing connection\n\n");
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(_SSID, _SENHA);
    Serial.println("Aguardando conexão...");
    while(WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi Conectado...");
    dht.begin();
}

void loop() {
    WiFiClient client;
    String footer = String(" HTTP/1.1\r\n") + "Host: " + String(host) + "\r\n" + "Connection: close\r\n\r\n";

    umidade = dht.readHumidity();
    temperatura = dht.readTemperature();

    if(isnan(umidade) || isnan(temperatura)){
        Serial.println("A leitura falhou...");
    }

    Serial.print("Umidade: ");
    Serial.print(umidade);
    Serial.print("% Temperatura: ");
    Serial.print(temperatura);
    Serial.println("°C");

    delay(16000);
    if (!client.connect(host, httpPort)){
        return;
    }
    client.print("GET /update?api_key=" + writeApiKey + "&field1=" + temperatura + footer);
    readResponse(&client);

    delay(16000);
    if (!client.connect(host, httpPort)){
        return;
    }
    client.print("GET /update?api_key=" + writeApiKey + "&field2=" + umidade + footer);
    readResponse(&client);
}