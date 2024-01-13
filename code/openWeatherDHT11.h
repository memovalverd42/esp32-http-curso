#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "FS.h"
#include <Arduino_JSON.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define BTN_PIN 4
#define DHTPIN 15
#define DHTTYPE DHT11

byte divisorChar[] = {
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110
};

byte gradeChar[] = {
  B01110,
  B01010,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

bool isTemp = true;
unsigned long lastMillis;
bool lastButtonState = true;

float localTemp;
float localHum;
String cityTemp;
String cityHum;
  
const char* ssid = "memovalverd_2.4G";
const char* password = "seguridadOUT#23";

// String baseURL = "https://pokeapi.co/api/v2/pokemon?limit=1&offset=";
String baseURL = "https://api.openweathermap.org/data/2.5/weather?lat=19.1913154&lon=-96.1419394&units=metric&appid=2bf5873e176fab2e510754fae8ffc595";

void conectToWiFi( void );
JSONVar httpRequest(String url);
void showInfo( bool isTemp );
void printUnits(bool isTemp);

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);
  
void setup() {
  
  Serial.begin(9600);
  pinMode(BTN_PIN, INPUT_PULLUP);

  dht.begin();

  lcd.init();
  lcd.backlight();
  lcd.createChar(0, divisorChar);  // Linea divisora
  lcd.createChar(1, gradeChar);    // Circulito de grados
  lcd.home();
  lcd.print("Conectando...");

  // delay(3000);

  conectToWiFi();

  lcd.clear();
  lcd.print("Conectado!");
  
  lastMillis = millis();
}


void loop() {

  if(!digitalRead(BTN_PIN) && lastButtonState) {
    delay(100);
    isTemp = !isTemp;
    showInfo(isTemp);
  } lastButtonState = digitalRead(BTN_PIN);

  if(millis() - lastMillis >= 5000) {
    JSONVar response;

    if ((WiFi.status() == WL_CONNECTED)) {
      response = httpRequest(baseURL);
    }

    localTemp = dht.readTemperature();
    localHum = dht.readHumidity();
    cityTemp = JSON.stringify(response["main"]["temp"]);
    cityHum = JSON.stringify(response["main"]["humidity"]);

    showInfo(isTemp);
    lastMillis = millis();
  }
  
}

void showInfo( bool isTemp ) {
  String labelIn = isTemp ? "Temp IN" : "Hum IN";
  String labelEx = isTemp ? "Temp EX" : "Hum EX";

  // Cabeceras
  lcd.clear();
  lcd.print(labelIn);
  lcd.setCursor(7, 0);
  lcd.write(0);
  lcd.print(labelEx);

  // Valores
  lcd.setCursor(0, 1);
  lcd.print(isTemp ? localTemp : localHum);
  printUnits(isTemp);
  lcd.setCursor(7, 1);
  lcd.write(0);
  lcd.print(isTemp ? cityTemp : cityHum);
  printUnits(isTemp);
}

void printUnits(bool isTemp) {
  if(isTemp){
    lcd.write(1);
    lcd.print("C");
  } else {
    lcd.print("%");
  }
}

JSONVar httpRequest(String url) {
  int httpCode;
  HTTPClient http;
  JSONVar jsonObject;

  http.begin(url);
  httpCode = http.GET();

  if (httpCode > 0){

    String jsonBuffer = http.getString();
    Serial.print("Codigo de respuesta: ");
    Serial.println(httpCode);
    // Serial.print("Respuesta JSON: ");
    // Serial.println(jsonBuffer);

    jsonObject = JSON.parse(jsonBuffer);

  } else {
    Serial.println("Error en la peticion HTTP");
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