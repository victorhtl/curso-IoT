/*
  Confirme no canal serial o endereço IP

  Ao fazer um GET no endereço IP da placa, é
  retornado uma página web que faz o acionamento
  dos GPIO 27 e 26 para um RELÉ
*/

#include <Arduino.h>
#include <WiFi.h>

// Coloque as credenciais da sua rede
#define _SSID " "
#define _SENHA " "

WiFiServer server(80);

String header;

String ledState = "off";
String fanState = "off";

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

            if(header.indexOf("GET /led/on") >= 0 ){
              digitalWrite(LEDPIN, LOW);
              ledState = "on";
            } else if(header.indexOf("GET /led/off") >= 0 ){
              digitalWrite(LEDPIN, HIGH);
              ledState = "off";
            } else if(header.indexOf("GET /fan/on") >= 0 ){
              digitalWrite(FANPIN, LOW);
              fanState = "on";
            } else if(header.indexOf("GET /fan/off") >= 0 ){
              digitalWrite(FANPIN, HIGH);
              fanState = "off";
            }

            client.println("<!DOCTYPE html><html lang=\"pt-BR\"><head><meta charset=\"UTF-8\">");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
            client.println("<title>IOT</title><style>html{");
            client.println("font-family: Helvetica;text-align: center;");
            client.println("margin: 0px auto;font-size: 25px;}.button{");
            client.println("background-color: green;color: white;");
            client.println("font-size: 40px;border: none;padding: 16px 40px;");
            client.println("cursor: pointer;}.button2{background-color: gray;}");
            client.println("</style></head>");
            
            client.println("<body><h1>Aplicação IOT</h1>");

            // Botao da luz
            client.println("<p>LUZ - "+ ledState +" </p>");
            if(ledState == "off"){
              client.println("<p><a href=\"/led/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/led/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            
            // Botao do Fan
            client.println("<p>FAN - "+ fanState +"</p>");
            if(fanState == "off"){
              client.println("<p><a href=\"/fan/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/fan/off\"><button class=\"button button2\">OFF</button>");
            }
            
            client.println("</a></p></body></html>");

            client.println("");

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