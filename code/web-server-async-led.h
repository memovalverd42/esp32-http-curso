#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define LED 2

const char* ssid = "memovalverd_2.4G";
const char* password = "seguridadOUT#23";

void conectToWiFi( void );

const char* PARAM_INPUT_1 = "state";

const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML><html>
  <head>
    <title>ESP Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <style>
      html {font-family: Arial; display: inline-block; text-align: center;}
      h2 {font-size: 3.0rem;}
      p {font-size: 3.0rem;}
      body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
      .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
      .switch input {display: none}
      .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
      .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
      input:checked+.slider {background-color: #b30000}
      input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
    </style>
  </head>
  <body>
    <h4>Output - GPIO 2</h4>
    <label class="switch">
      <input type="checkbox" onchange="toggleCheckbox(this)" id="2">
      <span class="slider"></span>
    </label>
  <script>
      const toggleCheckbox = async(element) => {
        const url = `/update?state=${element.checked ? 1 : 0}`;
        
        const resp = await fetch(url, {
            method: 'GET',
        });
        if(!resp.ok){
            console.log("Error");
        }else{
            console.log(await resp.text());
        }
      }
  </script>
  </body>
  </html>
)rawliteral";

AsyncWebServer server(81);

void setup( void ){
  Serial.begin(9600);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  conectToWiFi();

  // Ruta index /
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Ruta <ESP_IP>/update?state=<state>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String state;
    if (request->hasParam(PARAM_INPUT_1)) {
      state = request->getParam(PARAM_INPUT_1)->value();
      digitalWrite(LED, state.toInt());
    }
    else {
      state = "-1";
    }
    Serial.print("LED: ");
    Serial.println(state);
    request->send(200, "text/plain", state);
  });

  server.begin();
}

void loop( void ) {

}

void conectToWiFi( void ) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Conectando a red ");
  Serial.println(ssid);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print('.');
    delay(500);
  }

  Serial.print("\nConectado a red ");
  Serial.println(ssid);
  Serial.print("Con IP: ");
  Serial.println(WiFi.localIP());
}