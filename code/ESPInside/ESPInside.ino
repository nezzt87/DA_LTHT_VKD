#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Wire.h>
#include <BH1750.h>
#include "DHT.h"
#include <ArduinoJson.h>
#include <time.h>
/*
   ESP without Servo



   BH1750
   SCL <--> D3
   SDA <--> D4

   DHT11
   DHTPIN <--> D5


   con lai thi 3.3V voi GND
*/

// Set these to define some constant.
#define FIREBASE_HOST "da-ltht-vdk-12.firebaseio.com"
#define FIREBASE_AUTH "token_or_secret"
#define WIFI_SSID "Quynh Anh"
#define WIFI_PASSWORD "15032009"
#define DHTTYPE DHT11   // DHT 11
#define DHTPIN D5
#define TIMEZONE 7

DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter(0x23);

StaticJsonBuffer<256> jb;
JsonObject& obj = jb.createObject();


int dst = 0;

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
  //connect to Firebase
  Firebase.begin(FIREBASE_HOST);
  
  // config time
  configTime(TIMEZONE * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
}
//bien chua thoi gian dang ddmmyy/HHMM
char buffer[80];

void loop() {

  float lux = lightMeter.readLightLevel();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  //Check if any reads failed and exit early (to try again).
  if ( isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  obj["InsideTemp"] = t;
  obj["InsideLux"] = lux;
  /*
     Get date and time
     Example /code/GetTime/GetTime.ino

     strftime example
     http://www.cplusplus.com/reference/ctime/strftime/
  */
  struct tm * timeinfo;
  time_t now = time(nullptr);
  Serial.println(ctime(&now));
  time (&now);
  timeinfo = localtime (&now);
  strftime (buffer, 80, "%d%m%y/%H%M", timeinfo);

  // Gui du lieu len firebase
  Firebase.set(String("/log/" + String(buffer) + "/inside"), obj);
  if (Firebase.failed()) {
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
    return;
  }

  //Cach 1 phut thi thu thap du lieu 1 lan
  delay(60000);
}
