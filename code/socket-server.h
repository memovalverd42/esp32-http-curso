#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "memovalverd_2.4G";
const char* password = "seguridadOUT#23";

void conectToWiFi( void );

// Instancia de un server
WiFiServer server(1001);

void setup( void ) {

  Serial.begin(9600);
  conectToWiFi();
  server.begin();

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
