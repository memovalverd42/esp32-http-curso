#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

void setup( void ){
  Serial.begin(9600);
  delay(3000);

  if(!SPIFFS.begin(true)){
    Serial.println("Fallo al abrir el sistema de archivos");
    return;
  }

  File file = SPIFFS.open("/test.txt");
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

void loop( void ) {

}