#ifndef WEBSERVERESP_H
#define WEBSERVERESP_H

#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>

class WebServerESP {
public:
    String setup();
    void loop();

private:
    WiFiServer server = WiFiServer(80); // Objet serveur
    const char* ssid = "IDIoTsLab_Team";
    const char* password = "IDIoTsLab4members";

    // Variables pour gérer les ressources
    int humiditeSol = 0;
    int lumiere = 0;

    // Variables pour la gestion du temps
    unsigned long currentTime = 0;
    unsigned long previousTime = 0;
    const long timeoutTime = 2000;

    // Pour stocker la requête HTTP
    String header;
};

#endif
