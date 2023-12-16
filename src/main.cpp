#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

const char* ssid = "memovalverd_ESP32";
const char* password = "seguridadOUT#23";

WiFiServer server(1001);

void setup( void ){
  Serial.begin(9600);

  delay(3000);

  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());
}

void loop( void ) {
  WiFiClient client = server.available();

  if( client ){
    Serial.println("Nuevo cliente!");
    while( client.connected() ){
      String data = client.readString();
      Serial.print("Dato recibido: ");
      Serial.println(data);

      client.print("Hola desde ESP32");
    }
    client.stop();
    Serial.println("Cliente desconectado");
  }
}