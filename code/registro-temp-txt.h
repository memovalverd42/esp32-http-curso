#include <Arduino.h>
#include <WiFi.h>
#include "FS.h"
#include <SPIFFS.h>
#include <time.h>
#include <DHT.h>

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -21600;     // Desface Para México
const int   daylightOffset_sec = 0; 
  
const char* ssid = "memovalverd_2.4G";
const char* password = "seguridadOUT#23";

unsigned long previousMillis = 0;

void conectToWiFi( void );
String getLocalTime( void );
void recordTemperature( float temperature, String date );
void readTemperatures( void );

DHT dht(15, DHT11);

void setup( void ) {
  
  Serial.begin(9600);

  dht.begin();

  if(!SPIFFS.begin(true)){
    Serial.println("Fallo al abrir el sistema de archivos");
    return;
  }

  SPIFFS.remove("/temperatures.txt");

  conectToWiFi();

  // Configuracion y peticion de TimeStamp
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  previousMillis = millis();
}

void loop( void ) {

  if(millis() - previousMillis > 2000){
    recordTemperature(dht.readTemperature(), getLocalTime());
    previousMillis = millis();
  }

}

void readTemperatures( void ) {
  File file = SPIFFS.open("/temperatures.txt");
  if(!file){
    Serial.println("No se encontró el archivo");
    return;
  }

  Serial.println("Contenido del archivo: ");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}

void recordTemperature( float temperature, String date ) {
  File file = SPIFFS.open("/temperatures.txt", FILE_APPEND);
  String tempFormated;
  char buffer[100];

  if(!file){
    Serial.println("No se encontró el archivo");
    return;
  }

  sprintf(buffer, "%0.2f°C | %s", temperature, date.c_str());
  Serial.println(buffer);

  file.println(buffer);
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