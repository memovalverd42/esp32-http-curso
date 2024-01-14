#include <Arduino.h>
#include <WiFi.h>
#include "WifiConexion.h"
// #include <AsyncTCP.h>
// #include <ESPAsyncWebServer.h>
#include "FS.h"
#include <SPIFFS.h>
// #include <Arduino_JSON.h>
#include <DHT.h>
#include <time.h>

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -21600;     // Desface Para México
const int   daylightOffset_sec = 0; 

#define DHTPIN  15
#define DHTTYPE DHT11
  
const char* ssid = "SALA 201";
const char* password = "CG.PAGES";

unsigned long lastMillis;

String getLocalTime( void );
void recordTemp(float temp, String date);

DHT dht(DHTPIN, DHTTYPE);

void setup( void ) {
  
  Serial.begin(9600);

  dht.begin();

  if(!SPIFFS.begin(true)){
    Serial.println("Fallo al abrir el sistema de archivos");
    return;
  }

  SPIFFS.remove("/temperatures.txt");

  conectToWiFi(ssid, password);

  // Configuracion y peticion de TimeStamp
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  lastMillis = millis();
}


void loop( void ) {

  if(millis() - lastMillis > 1000){
    recordTemp(dht.readTemperature(), getLocalTime());
    lastMillis = millis();
  }
  
}

void recordTemp(float temp, String date){

  File file = SPIFFS.open("/temperatures.txt", FILE_APPEND);
  if(!file){
    Serial.println("Error al abrir archivo");
    return;
  }

  String data = String(temp) + " | " + date;

  Serial.println(data);

  file.println(data);
  file.close();

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