#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(81);

const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-rbsA2VBKQhggwzxH7pPCaAqO46MgnOM80zW1RWuH61DGLwZJEdK2Kadq2F9CUG65" crossorigin="anonymous">
    <title>Control LEDS</title>
</head>
<body class="m-5">
    <h1>LED CONTROL</h1>
    <div class="form-check form-switch">
        <input class="form-check-input" type="checkbox" role="switch" id="2" onchange="toggleCheckbox(this)">
        <label class="form-check-label">LED 2</label>
    </div>

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

const char* ssid = "SALA 201";
const char* password = "CG.PAGES";

const char* PARAM_MESSAGE = "state";

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup() {

    Serial.begin(9600);

    pinMode(2, OUTPUT);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    delay(3000);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "text/html", html);
    });

    // Send a GET request to <IP>/update?state=<message>
    server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String state;
        if (request->hasParam(PARAM_MESSAGE)) {
            state = request->getParam(PARAM_MESSAGE)->value();

            digitalWrite(2, state.toInt());
        } else {
            state = "-1";
        }
        request->send(200, "text/plain", state);
    });

    server.onNotFound(notFound);

    server.begin();
}

void loop() {
}