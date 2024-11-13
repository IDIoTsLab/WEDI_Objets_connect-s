#ifndef DEVICE_H
#define DEVICE_H

#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "Sensor.h"

extern Sensor sensor2;


class Device
{
public:
    Device();
    ~Device();
    void setup();
    void loop();

    void pump(int PowerLevel);//0 to 255

    void light(int PowerLevel); //0 to 255


private:

//config PWN lampe
int pwmChannelLight = 0; //Choisit le canal 0
int frequenceLight = 1000; //Fréquence PWM de 1 KHz
int resolutionLight = 8; // Résolution de 8 bits, 256 valeurs possibles
int pwmPinLight = 2;

// config PWN pompe
int pwmChannelPump = 1; //Choisit le canal 0
int frequencePump = 1000; //Fréquence PWM de 1 KHz
int resolutionPump = 8; // Résolution de 8 bits, 256 valeurs possibles
int pwmPinPump = 4;






};

#endif