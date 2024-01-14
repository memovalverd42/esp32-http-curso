#include <Arduino.h>
#include <WiFi.h>
// #include <AsyncTCP.h>
// #include <ESPAsyncWebServer.h>
#include "FS.h"
// #include <Arduino_JSON.h>
#include <SPIFFS.h>
#include <time.h>

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -21600;     // Desface Para México
const int   daylightOffset_sec = 0; 
  
const char* ssid = "SALA 201";
const char* password = "CG.PAGES";

void conectToWiFi( void );
String getLocalTime( void );

void setup( void ) {
  
  Serial.begin(9600);

  conectToWiFi();

  // Configuracion y peticion de TimeStamp
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

}

void loop( void ) {

  Serial.println(getLocalTime());
  delay(1000);

}

String getLocalTime( void ) {
  struct tm timeinfo;
  char buffer[64];

  if(!getLocalTime(&timeinfo)){
    return "Failed to obtain time";
  }

  strftime(buffer, sizeof(buffer), "%A, %B %d %Y %H:%M:%S", &timeinfo);

  String date = buffer;

  return date;  
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