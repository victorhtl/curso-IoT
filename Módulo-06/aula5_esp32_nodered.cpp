#include <Arduino.h>
#include <WiFi.h>
#include <MQTT.h> // 256dpi/MQTT@^2.5.1

/* SSID e Password da rede WiFi */
const char* ssid = "";
const char* pass = "";

const char * host = "192.168.0.36";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

bool GPIO2_STATE = 0;

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

  client.subscribe("GPIO2");
  // client.unsubscribe("hello");
}

// Recebe automaticamente o tópico e o payload da mensagem
void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if(topic == "GPIO2"){
    if(payload == "change"){
      GPIO2_STATE = !GPIO2_STATE;
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  // GPIO
  pinMode(2, OUTPUT);

  // Conexão com o host
  Serial.print("Tentando conexao com o host: ");
  Serial.println(host);
  client.begin(host, net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();

  // Alterna o estado do LED da placa
  digitalWrite(2, GPIO2_STATE);

  if (!client.connected()) {
    connect();
  }

  // Publica uma mensagem a cada 1 segundo
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    client.publish("temperatura", "20");

    if(GPIO2_STATE) client.publish("GPIO2_STATE", "Ligado");
    else client.publish("GPIO2_STATE", "Desligado");
  }
} 