// First define the library :
#include <BH1750FVI.h>
#include <ArduinoJson.h>
//#include <ESP8266WiFi.h>
//#include <FirebaseArduino.h>
#include "DHT.h"
#include <Wire.h>
// Set these to run example.
#define FIREBASE_HOST "da-ltht-vdk-12.firebaseio.com"
#define FIREBASE_AUTH "token_or_secret"
#define WIFI_SSID "Quynh Anh"
#define WIFI_PASSWORD "15032009"
#define DHTTYPE DHT11   // DHT 11
#define DHTPIN D5
DHT dht(DHTPIN, DHTTYPE);
BH1750FVI LightSensor;
StaticJsonBuffer<256> jb;
JsonObject& obj = jb.createObject();


void setup() {
  Serial.begin(115200);
  Wire.begin(D3, D4);
//  dht.begin();
  LightSensor.begin();
  LightSensor.SetAddress(Device_Address_H);
  LightSensor.SetMode(Continuous_H_resolution_Mode);

//  Serial.println("Running...");
//  // connect to wifi.
//  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//  Serial.print("connecting");
//  while (WiFi.status() != WL_CONNECTED) {
//    Serial.print(".");
//    delay(500);
//  }
//  Serial.println();
//  Serial.print("connected: ");
//  Serial.println(WiFi.localIP());
//
//  Firebase.begin(FIREBASE_HOST);

}
void loop() {
  uint16_t lux = LightSensor.GetLightIntensity();// Get Lux value

//  float h = dht.readHumidity();
//  // Read temperature as Celsius (the default)
//  float t = dht.readTemperature();
//  // Read temperature as Fahrenheit (isFahrenheit = true)
//  float f = dht.readTemperature(true);
//
//  // Check if any reads failed and exit early (to try again).
//  if (isnan(h) || isnan(t) || isnan(f)) {
//    Serial.println("Failed to read from DHT sensor!");
//    return;
//  }
//  Serial.print("Temperature: ");
//  Serial.print(t);
//  Serial.print("*C");
  Serial.println(lux);
//  obj["Nhiet:"] = t;
//  obj["Do sang:"] = lux;
//  Firebase.push("/Ben trong", obj);
  delay(5000);
}
