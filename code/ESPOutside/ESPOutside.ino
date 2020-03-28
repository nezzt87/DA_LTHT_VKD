#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Wire.h>
#include <BH1750.h>
#include "DHT.h"
#include <ArduinoJson.h>
#include <Servo.h>
#include <time.h>
/*
   ESP with Servo



   BH1750
   SCL <--> D3
   SDA <--> D4

   DHT11
   DHTPIN <--> D5

   Servo

   attach <--> D2 (GPIO4)

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

Servo myservo;  // create servo object to control a servo

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
  //Test servo
  myservo.attach(4);  // attaches the servo on GIO4 to the servo object ~ D4
  int pos;

  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

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
int lastPos = 0;
void loop() {

  float lux = lightMeter.readLightLevel();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  //Check if any reads failed and exit early (to try again).
  if ( isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  int pos = Firebase.getInt("/setServo");
  int i;
  if (pos > lastPos) {
    for (i = lastPos; i <= pos; i += 1) { // goes from 180 degrees to 0 degrees
      myservo.write(i);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  } else {
    for (i = lastPos; i >= pos; i -= 1) {
      myservo.write(i);
      delay(15);
    }
  }

  lastPos = pos;


  obj["Servo"] = pos;
  obj["Nhietdo"] = t;
  obj["DoSang"] = lux;
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
  Firebase.set(String("/log/" + String(buffer)), obj);
  if (Firebase.failed()) {
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
    return;
  }

  //Cach 1 phut thi thu thap du lieu 1 lan
  delay(60000);
}
