#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Sensor.h"
#include "Screen.h"
#include "Device.h"



Sensor sensor;
Screen screen;
Device device;







void setup() {
  // Initialisation du moniteur série pour le débogage
  Serial.begin(115200);
  screen.setup();
  device.setup();



}





void loop() {
  screen.Clear();

  sensor.loop();
  screen.loop();
  device.loop();


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



