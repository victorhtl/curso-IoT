#include <Arduino.h>
#include <WiFi.h>
#include <MQTT.h> // 256dpi/MQTT@^2.5.1

/* SSID e Password da rede WiFi */
const char* ssid = "";
const char* pass = "";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

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

  client.subscribe("hello");
  // client.unsubscribe("hello");
}

// Recebe automaticamente o tópico e o payload da mensagem
void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  // Conexão com o host
  client.begin("192.168.248.13", net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  // Avisa ao broker que o cliente ainda existe
  client.loop();

  if (!client.connected()) {
    connect();
  }

  // Publica uma mensagem a cada 1 segundo
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    client.publish("temperatura", "20");
  }
}