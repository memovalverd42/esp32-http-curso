#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "memovalverd_2.4G";
const char* password = "seguridadOUT#23";

void conectToWiFi( void );

void setup( void ) {

  Serial.begin(9600);
  conectToWiFi();

}

void loop( void ) {

  WiFiClient client;

  if(!client.connect(IPAddress(192,168,100,23), 1080)){      
    Serial.println("Conexion con el servord fallida");
    delay(1000);
    return;
  }
  Serial.println("Conectado, enviando data"); 
  client.print("Guillermo");

  delay(1000);

  if(client.available()){
    String respuesta = client.readString();
    Serial.println(respuesta);
  }

  client.stop();
  delay(1000);

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
