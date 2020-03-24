#include <ArduinoJson.h>

#include <ESP8266WiFi.h>

#include <FirebaseArduino.h>
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.disconnect();
  WiFi.begin("Quynh Anh", "15032009");
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    delay(222);
    Serial.print(".");
  }
  Firebase.begin("da-ltht-vdk-12.firebaseio.com");

}

void loop() {
  // put your main code here, to run repeatedly:

}
