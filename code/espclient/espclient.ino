#include <ESP8266WiFi.h>
#include <SocketIOClient.h>

SocketIOClient client;
const char* ssid = "Quynh Anh";
const char* password = "15032009";

char host[] = "192.168.1.6";
int port = 3485;

extern String RID;
extern String Rfull;

unsigned long previousMillis = 0;
long interval = 2000;

void setup(){
  Serial.begin(115200);
  delay(10);

  Serial.print("Ket noi vao mang ");
  Serial.print(ssid);

  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println(F("Da ket noi wifi"));
  Serial.println(F("Dia chi IP cua ESP8266 (Socket Client ESP8266 : "));
  Serial.println(WiFi.localIP());

  if(!client.connect(host,port)){
    Serial.println("Ket noi den socket server that bai !!");
    return;
  }

  if(client.connected()){
    client.send("connection", "message", "Connected !!!!");
  }
}

void loop(){

  if(millis() - previousMillis > interval){
    previousMillis = millis();

    client.send("atime", "message", "Time Please!!!");
  }

  if(client.monitor()){
    Serial.println(RID);
    Serial.println(Rfull);
  }

  if (!client.connected()){
    client.reconnect(host,port);
  }
}
