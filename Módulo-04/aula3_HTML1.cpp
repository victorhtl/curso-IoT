/**
 * Confirme o endereço IP pelo canal serial
 * 
 * Fazer um GET no endereço IP retorna uma página
 * html com links para acionamento dos GPIO 27 e 26
*/

#include <Arduino.h>
#include <WiFi.h>

// Coloque as credenciais da sua rede
#define _SSID " "
#define _SENHA " "

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

            if(header.indexOf("GET /led/on") >= 0 ){
              digitalWrite(LEDPIN, LOW);
            } else if(header.indexOf("GET /led/off") >= 0 ){
              digitalWrite(LEDPIN, HIGH);
            } else if(header.indexOf("GET /fan/on") >= 0 ){
              digitalWrite(FANPIN, LOW);
            } else if(header.indexOf("GET /fan/off") >= 0 ){
              digitalWrite(FANPIN, HIGH);
            }

            client.println("<!DOCTYPE html><html lang=\"pt-BR\">");
            client.println("<head><meta charset=\"UTF-8\">");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
            client.println("<title>IOT</title></head><body>");
            client.println("<h1>Aplicação IOT</h1>");
            
            client.println("<p><a href=\"/led/on\">Liga Lampada</a></p>");
            client.println("<p><a href=\"/led/off\">Desliga Lampada</a></p>");
            client.println("<p><a href=\"/fan/on\">Liga Fan</a></p>");
            client.println("<p><a href=\"/fan/off\">Desliga Fan</a></p>");
            
            client.println("</body></html>");

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