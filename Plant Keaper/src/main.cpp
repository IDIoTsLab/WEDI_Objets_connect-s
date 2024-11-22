#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>

#include "Sensor.h"
#include "Screen.h"
#include "Device.h"
#include "WebServerESP.h"



Sensor sensor;
Screen screen;
Device device;
WebServerESP webServer;







void setup() {
  // Initialisation du moniteur série pour le débogage
  Serial.begin(115200);
  screen.setup();
  device.setup();
  webServer.setup();
  Serial.println("");
  String fullAddress = webServer.GetAddress();
  screen.QRcode(fullAddress.c_str());
  Serial.println();
  Serial.println(fullAddress);
  webServer.loop();
  delay(60000);



}





void loop() {
  screen.Clear();

  sensor.loop();
  screen.loop();
  device.loop();
  webServer.loop();


  screen.write(0,F("Humidite(P)"), sensor.ReadMoisture(),F("%"),false);
  screen.write(1,F("Luminosite"), sensor.ReadLuminosity(),F("%"),false);
  screen.write(2,F("temperature"), sensor.ReadTemperature(),F("C"),false);
  screen.write(3,F("humidite"), sensor.ReadHumidity(),F("%"),false);


  if (sensor.ReadWaterlevel() == false ){
    screen.write(4,F("--Niveau d'eaux bas--"),-1, " ", true);
  }
  else {
    screen.write(4,F("Niveau d'eaux OK"),-1, " ", true);
  }
  

  screen.Display();
  

  if (sensor.ReadWaterlevel() == true ){

  device.pump(150);
  }
  else 
  {
    delay(1000);
    device.pump(0);
  }




delay(100);


}



