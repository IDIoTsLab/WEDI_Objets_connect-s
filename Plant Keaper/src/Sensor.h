#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#pragma once

class Sensor
{
public:
    Sensor();
    ~Sensor();

    int loop();
    
    int ReadMoisture();
    int ReadLuminosity();
    bool ReadWaterlevel();

    bool WaterLevel1 = false; 
    float ReadTemperature();
    float ReadHumidity();

private:
    DHT dht; 

    int SensorWaterLevel1 = 0;  

  //1200(3,3V), 230(5V) correspond à la valeur minimal mesurée par le capteur (imergée)
  //3100(3,3V), 550(5V) correspond à la valeur maximal mesurée par le capteur (sec)
    const int ValueMoistureSensorMin = 1200;
    const int ValueMoistureSensorMax = 3100;

    //
    const int ValueLuminositySensorMin = 0;
    const int ValueLuminositySensorMax = 4095;

    //cofiguration des pins
    const int sensorMoisturePin = 32;  
    const int sensorLuminosityPin = 33;
    const int PinWaterLevel1 = 34;
    const int DHTPin = 21; 

    int sensorMoisture = 0;
    int PurcentMoisture = 0;
    int sensorLuminosity = 0;
    int PurcentLuminosity = 0;


};

#endif