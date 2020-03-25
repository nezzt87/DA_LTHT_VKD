#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Wire.h>
#include <BH1750.h>
#include "DHT.h"
#include <ArduinoJson.h>
// Set these to run example.
#define FIREBASE_HOST "da-ltht-vdk-12.firebaseio.com"
#define FIREBASE_AUTH "token_or_secret"
#define WIFI_SSID "Quynh Anh"
#define WIFI_PASSWORD "15032009"
#define DHTTYPE DHT11   // DHT 11
#define DHTPIN D5

DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter(0x23);
StaticJsonBuffer<256> jb;
JsonObject& obj = jb.createObject();
void setup() {
  Serial.begin(9600);
  dht.begin();
  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin(D4, D3);
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  }
  else {
    Serial.println(F("Error initialising BH1750"));
  }
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST);
}

int n = 0;

void loop() {

  float lux = lightMeter.readLightLevel();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  //Check if any reads failed and exit early (to try again).
  if ( isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  n = millis()/10000 + 1;
  obj["Nhietdo"] = t;
  obj["DoSang"] = lux;
  Firebase.set(String("/log/" + String(n)), obj);
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(10000);
}
