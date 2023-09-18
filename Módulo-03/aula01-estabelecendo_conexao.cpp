#include <Arduino.h>
#include <WiFi.h>

// Altere aqui os parâmetros de acordo com sua rede wifi
#define _SSID "Nome_da_sua_rede"
#define _SENHA "senha123"

void setup(){
    Serial.begin(115200);

    int redes = 0;

    redes = WiFi.scanNetworks();

    if(redes == 0){
        Serial.println("Nenhuma rede encontrada");
    } else {
        Serial.print(redes);
        Serial.println(" Redes Encontradas");

        for(int i = 0; i<redes; i++){
            Serial.print("SSID: ");
            Serial.print(WiFi.SSID(i));  // SSID é o nome da rede
            Serial.print(" RSSI: ");
            Serial.print(WiFi.RSSI(i));
            Serial.println("");
        }

        WiFi.scanDelete();

        // Faz a conexão 
        WiFi.begin(_SSID, _SENHA);

        // Aguardando a conexão ser bem sucedida
        while(WiFi.status() != WL_CONNECTED){
            Serial.print(".");
            delay(300);
        }
        Serial.println("");
        Serial.print("Wifi Conectado, Endereço IP: ");
        Serial.println(WiFi.localIP());
    }

}

void loop(){}
