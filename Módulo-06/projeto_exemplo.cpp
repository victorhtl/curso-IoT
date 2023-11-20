#include <Arduino.h>
#include <iostream>
#include <WiFi.h>
#include <MQTT.h> // 256dpi/MQTT@^2.5.1
#include "DHT.h" //hwspeedy/DHT-Sensor@^1.4.3

/* SSID e Password da rede WiFi */
const char* ssid = "";
const char* pass = "";

/* Endereço IP do broker MQTT */
const char * host = " ";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis_1_sec = 0;
unsigned long lastMillis_2_sec = 0;

// Configuração DHT11
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define LED 2
#define RELE1 18
#define RELE2 19

bool LED_STATE = LOW;
bool RELE1_STATE = HIGH;
bool RELE2_STATE = HIGH;

// Faz a conexão com o broker
void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  // Informa o ID (que precsisa ser unico), usuário e senha do broker
  while (!client.connect("ESP32", "admin", "123")) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nconnected!");

  client.subscribe("LED");
  client.subscribe("RELE1");
  client.subscribe("RELE2");
}

// Recebe automaticamente o tópico e o payload da mensagem
void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  if(payload == "on"){
    if(topic == "LED") LED_STATE = HIGH;
    if(topic == "RELE1") RELE1_STATE = LOW;
    if(topic == "RELE2") RELE2_STATE = LOW;
  } else if(payload == "off"){
    if(topic == "LED") LED_STATE = LOW;
    if(topic == "RELE1") RELE1_STATE = HIGH;
    if(topic == "RELE2") RELE2_STATE = HIGH;
  } 
}

void updateState(){
  digitalWrite(LED, LED_STATE);
  digitalWrite(RELE1, RELE1_STATE);
  digitalWrite(RELE2, RELE2_STATE);

  if(LED_STATE) client.publish("LED_S", "on");
  else client.publish("LED_S", "off");

  if(!RELE1_STATE) client.publish("RELE1_S", "on");
  else client.publish("RELE1_S", "off");

  if(!RELE2_STATE) client.publish("RELE2_S", "on");
  else client.publish("RELE2_S", "off");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  pinMode(LED, OUTPUT);
  pinMode(RELE1, OUTPUT);
  pinMode(RELE2, OUTPUT);
  digitalWrite(LED, LED_STATE);
  digitalWrite(RELE1, RELE1_STATE);
  digitalWrite(RELE2, RELE2_STATE);

  // Conexão com o host
  Serial.print("Tentando conexao com o host: ");
  Serial.println(host);
  client.begin(host, net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();

  if (!client.connected()) {
    connect();
  }

  if (millis() - lastMillis_1_sec > 1000){
    lastMillis_1_sec = millis();
    updateState();
  }

  if(millis() - lastMillis_2_sec > 2000){
    lastMillis_2_sec = millis();

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    std::string umidade = std::to_string(h);
    std::string temperatura = std::to_string(t);

    client.publish("umidade", umidade.c_str());
    client.publish("temperatura", temperatura.c_str());
  }
} 