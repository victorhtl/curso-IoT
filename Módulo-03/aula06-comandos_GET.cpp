#include <Arduino.h>
#include <WiFi.h>
#include "password.cpp"

WiFiServer server(80);

String header;

#define LEDPIN 27
#define FANPIN 26

void setup() {
  Serial.begin(115200);

  pinMode(LEDPIN, OUTPUT);
  pinMode(FANPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);
  digitalWrite(FANPIN, HIGH);

  WiFi.begin(_SSID, _SENHA);
  Serial.println("Aguardando conexão...");
  while(WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print(".");
  }
  Serial.print("Wifi Conectado, Endereço IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  // Pega um cliente que está conectado e retorna
  WiFiClient client = server.available();

  // Se houver um cliente
  if(client){ 
    String controle;

    // Le todos os caracteres que o cliente enviou
    // Retorna uma resposta
    while(client.connected()){
      if(client.available()){
        char c = client.read();
        Serial.print(c);
        header += c;
        if(c=='\n'){
          if(controle.length()==0){
            
            // Resposta ao cliente
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            client.println("ola pessoal, esta e minha aplicacao iot");

            if(header.indexOf("GET /led/on") >= 0 ){
              digitalWrite(LEDPIN, LOW);
            } else if(header.indexOf("GET /led/off") >= 0 ){
              digitalWrite(LEDPIN, HIGH);
            } else if(header.indexOf("GET /fan/on") >= 0 ){
              digitalWrite(FANPIN, LOW);
            } else if(header.indexOf("GET /fan/off") >= 0 ){
              digitalWrite(FANPIN, HIGH);
            }

            header = "";

            break;
          } else {
            controle = "";
          } 
        } else if (c != '\r'){
          controle += c;
        } 
      }
    }
    // Encerra a conexão
    client.stop(); 
  }
}