#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "FS.h"
#include <Arduino_JSON.h>
  
const char* ssid = "memovalverd_2.4G";
const char* password = "seguridadOUT#23";

String jsonBuffer;
// String baseURL = "https://pokeapi.co/api/v2/pokemon?limit=1&offset=";
// String baseURL = "https://stranger-things-api.fly.dev/api/v1/characters/random/?count=1";
// String baseURL = "https://api.openweathermap.org/data/2.5/weather?lat=19.1924201&lon=-96.1421051&units=metric&lang=es&appid=2bf5873e176fab2e510754fae8ffc595";

void conectToWiFi( void );

JSONVar httpGetRequest( String url );
  
void setup() {
  
  Serial.begin(9600);
  delay(3000);

  conectToWiFi();
  
}
  
void loop() {
  
  if ((WiFi.status() == WL_CONNECTED)) {

  }

  delay(5000);
  
}

JSONVar httpGetRequest( String url ) {

  int httpCode;
  JSONVar jsonObject;
  HTTPClient http;
  
  http.begin(url);
  httpCode = http.GET();                                    
  
  if (httpCode > 0) {
  
    String jsonBuffer = http.getString();
    Serial.println(httpCode);
    // Serial.println(jsonBuffer);

    jsonObject = JSON.parse(jsonBuffer);
  
    // if (JSON.typeof(jsonObject) == "undefined") {
    //   Serial.println("Fallo de conversion de JSON");
    //   return;
    // }

  } else {
    Serial.println("Error en la peticion");
  }
  
  http.end();

  return jsonObject;

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