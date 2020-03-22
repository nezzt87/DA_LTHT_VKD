
/*
  This is a simple code to test BH1750FVI Light senosr
  communicate using I2C Protocol 
  this library enable 2 slave device address
  Main address  0x23 
  secondary address 0x5C 
  connect this sensor as following :
  VCC >>> 3.3V
  SDA >>> A4 
  SCL >>> A5
  addr >> A3
  Gnd >>>Gnd

  Written By : Mohannad Rawashdeh
  
 */
 
 // First define the library :

#include <Wire.h>
#include <BH1750FVI.h>


BH1750FVI LightSensor;

#include "DHT.h"
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(2, DHTTYPE);

#define DHTPIN 9 
void setup() {   // put your setup code here, to run once:
   Serial.begin(9600);
  LightSensor.begin();
  Serial.println("DHTxx test!");

  dht.begin();
  /*
 Set the address for this sensor 
 you can use 2 different address
 Device_Address_H "0x5C"
 Device_Address_L "0x23"
 you must connect Addr pin to A3 .
 */
  LightSensor.SetAddress(Device_Address_H);//Address 0x5C
 // To adjust the slave on other address , uncomment this line
 // lightMeter.SetAddress(Device_Address_L); //Address 0x5C
 //-----------------------------------------------
  /*
   set the Working Mode for this sensor 
   Select the following Mode:
    Continuous_H_resolution_Mode
    Continuous_H_resolution_Mode2
    Continuous_L_resolution_Mode
    OneTime_H_resolution_Mode
    OneTime_H_resolution_Mode2
    OneTime_L_resolution_Mode
    
    The data sheet recommanded To use Continuous_H_resolution_Mode
  */

  LightSensor.SetMode(Continuous_H_resolution_Mode);
  
  Serial.println("Running...");
}


void loop() {
  // put your main code here, to run repeatedly: 
  uint16_t lux = LightSensor.GetLightIntensity();// Get Lux value
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lux");
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
  delay(5000);
}
