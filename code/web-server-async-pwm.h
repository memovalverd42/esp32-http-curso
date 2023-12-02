#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define LED 2

const char* ssid = "memovalverd_2.4G";
const char* password = "seguridadOUT#23";

void conectToWiFi( void );

const int output = 2;

String sliderValue = "0";

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

const char* PARAM_INPUT = "value";

// Create AsyncWebServer object on port 80
AsyncWebServer server(81);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>ESP Web Server</title>
    <style>
      html {
        font-family: Arial;
        display: inline-block;
        text-align: center;
      }
      h2 {
        font-size: 2.3rem;
      }
      p {
        font-size: 1.9rem;
      }
      body {
        max-width: 400px;
        margin: 0px auto;
        padding-bottom: 25px;
      }
      .slider {
        -webkit-appearance: none;
        margin: 14px;
        width: 360px;
        height: 25px;
        background: #ffd65c;
        outline: none;
        -webkit-transition: 0.2s;
        transition: opacity 0.2s;
      }
      .slider::-webkit-slider-thumb {
        -webkit-appearance: none;
        appearance: none;
        width: 35px;
        height: 35px;
        background: #003249;
        cursor: pointer;
      }
      .slider::-moz-range-thumb {
        width: 35px;
        height: 35px;
        background: #003249;
        cursor: pointer;
      }
    </style>
  </head>
  <body>
    <h2>ESP Web Server</h2>
    <p><span id="textSliderValue">%SLIDERVALUE%</span></p>
    <p>
      <input
        type="range"
        onchange="updateSliderPWM(this)"
        id="pwmSlider"
        min="0"
        max="255"
        value="%SLIDERVALUE%"
        step="1"
        class="slider"
      />
    </p>
    <script>
        const updateSliderPWM = async(element) => {
            const sliderValue = document.getElementById("pwmSlider").value;
            document.getElementById("textSliderValue").innerHTML = sliderValue;
            console.log(sliderValue);

            const url = `/slider?value=${sliderValue}`;

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

String processor(const String& var){
  if (var == "SLIDERVALUE"){
    return sliderValue;
  }
  return String();
}

void setup( void ){
  Serial.begin(9600);
  
  ledcSetup(ledChannel, freq, resolution);
  
  ledcAttachPin(output, ledChannel);
  
  ledcWrite(ledChannel, sliderValue.toInt());

  conectToWiFi();

  // Ruta index /
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Ruta <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
      ledcWrite(ledChannel, sliderValue.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  
  server.begin();
}
  
void loop() {
  
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