#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "FS.h"
#include <Arduino_JSON.h>
  
const char* ssid = "memovalverd_2.4G";
const char* password = "seguridadOUT#23";

void conectToWiFi( void );

AsyncWebServer server(81);

void setup( void ) {
  
  Serial.begin(9600);
  pinMode(2, OUTPUT);

  // delay(3000);

  conectToWiFi();

  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404, "text/plain", "Not found");
  });

  // Ruta index /
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    JSONVar response;

    response["status"] = "ok";
    response["micro"] = "ESP32";
    response["owner"] = "Guillermo Valverde";

    String json = JSON.stringify(response);

    request->send(200, "application/json", json);
  });

  // Ruta /led?toggle=<state>
  server.on("/led", HTTP_GET, [](AsyncWebServerRequest *request){
    JSONVar response;
    String state;
    if(request->hasParam("toggle")){
      state = request->getParam("toggle")->value();

      if(state == "ON"){
        digitalWrite(2, HIGH);
      } else {
        digitalWrite(2, LOW);
      }

      response["status"] = "ok";
      response["state"] = digitalRead(2);
    } else {
      response["status"] = "error";
      response["message"] = "Parametro toggle no encontrado";
      response["state"] = digitalRead(2);
    }

    String json = JSON.stringify(response);

    request->send(200, "application/json", json);
    
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