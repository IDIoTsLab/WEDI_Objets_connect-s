#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>

#include "Sensor.h"
#include "Screen.h"
#include "Device.h"
#include "WebServerESP.h"
#include "WifiConfig.h"

WifiConfig wifiConfig("Plant Keaper", "12345678");


Sensor sensor;
Screen screen;
Device device;
WebServerESP webServer;






void setup() {
  Serial.begin(115200);
  screen.setup();
  device.setup();
  screen.TopScreen(F("Web Config, go to:"),13, false); // texte + decallage horizontal
  screen.QRcode("http://192.168.1.1"); 
  screen.Display();
  wifiConfig.begin();
  
String NomWifi = (wifiConfig.GetSSID());

String PasswordWifi = (wifiConfig.GetPassword());

Serial.println("Nom Wifi : " + NomWifi);
Serial.println("Mot de passe Wifi : " + PasswordWifi);

wifiConfig.stopServer();
delay(1000);
 webServer.setup(NomWifi,PasswordWifi);
 webServer.loop();

  String fullAddress = webServer.GetAddress();
  screen.Clear();
  screen.TopScreen(F("Server Web :"),32,false); 
  screen.QRcode(fullAddress.c_str());
  Serial.println(fullAddress);
  delay(5000);

}





void loop() {
  screen.Clear();

  sensor.loop();
  screen.TopScreen(F("Plant Keaper"),32,true);
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



