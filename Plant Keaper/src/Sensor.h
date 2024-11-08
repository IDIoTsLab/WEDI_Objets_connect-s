#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#pragma once

class Sensor
{
public:
    Sensor();
    ~Sensor();

    int loop();

    bool WaterLevel1 = false; 

private:

    const int PinWaterLevel1 = 34;
    int SensorWaterLevel1 = 0;  


};

#endif