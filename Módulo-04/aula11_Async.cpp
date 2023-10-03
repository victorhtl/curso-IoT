#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
// ottowinter/ESPAsyncWebServer-esphome@^3.1.0

//const char* ssid;     // COLOQUE O SSID DA SUA REDE
//const char* password; // COLOQUE A SENHA DA SUA REDE

#define LED 2

String sliderValue = "0";

// Cria o WebServer Assincrono na porta 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"=( 
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP Web Server</title>
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 2.3rem;}
    p {font-size: 1.9rem;}
    body {max-width: 400px; margin:0px auto; padding-bottom: 25px;}
    .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #FFD65C;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #003249; cursor: pointer;}
    .slider::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; } 
  </style>
</head>
<body>
  <h2>ESP Web Server</h2>
  <p><span id="textSliderValue">%SLIDERVALUE%</span></p>
  <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider" min="0" max="255" value="%SLIDERVALUE%" step="1" class="slider"></p>
<script>
function updateSliderPWM(element) {
  var sliderValue = document.getElementById("pwmSlider").value;
  document.getElementById("textSliderValue").innerHTML = sliderValue;
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+sliderValue, true);
  xhr.send();
}
</script>
</body>
</html>
)=";

// Substitui o PlaceHolder do HTML
String processor(const String& var){
  if(var == "SLIDERVALUE"){
    return sliderValue;
  }
  return String();
}

void setup(){
  Serial.begin(115200);

  // Definição do PWM
  pinMode(LED, OUTPUT);
  ledcSetup(0, 25000, 8);
  ledcAttachPin(LED, 0);
  ledcWrite(0, 0);
  
  // Conecta ao wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando..");
  }
  Serial.println(WiFi.localIP());

  // Definir a URL raiz
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Recebe a query do cliente no formato /slider?value=<valor>
  // /slider -> endpoint
  // value -> parâmetro da query
  // <valor> -> valor do parâmetro
  // Você pode criar quantos endpoint achar necessário
  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request){
    if(request->hasParam("value")){
      sliderValue = request->getParam("value")->value();
      ledcWrite(0, sliderValue.toInt());
    } else {
      sliderValue = "error";
    }
    Serial.print("Valor PWM: ");
    Serial.println(sliderValue);
    request->send(200, "text/plain", "OK");
  });

  // Inicializar o servidor
  server.begin();
}

void loop() {

}