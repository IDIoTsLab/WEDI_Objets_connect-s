#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "qrcode.h"


class Screen
{
public:
    Screen();
    ~Screen();

    void setup();
    void loop();


    void write(int space, String text,float value, String unite, bool TextOrNot);

    void Clear();
    void Display();

    void QRcode(const char *text);

private:
    // Constantes pour la configuration de l'écran
    #define SCREEN_WIDTH 128
    #define SCREEN_HEIGHT 64
    #define OLED_RESET    -1  // GPIO non utilisé pour le reset sur cet écran
    #define OLED_ADDRESS  0x3C  // Adresse I2C de l'écran
    #define WHITE SSD1306_WHITE

    Adafruit_SSD1306 display;  // Déclaration correcte de l'instance `display` comme membre de la classe
};

#endif