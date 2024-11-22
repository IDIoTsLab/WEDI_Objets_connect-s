#ifndef WEBSERVERESP_H
#define WEBSERVERESP_H

#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>

#include "Sensor.h"

class WebServerESP {
public:
    void setup(); // Initialisation du Wi-Fi et du serveur
    void loop();  // Boucle principale pour gérer les clients

    String GetAddress();

private:
    WiFiServer server = WiFiServer(80); // Objet serveur
    WiFiClient client;                  // Client en cours de traitement
    String currentLine = "";            // Ligne courante lue

    const char* ssid = "IDIoTsLab_Team";
    const char* password = "IDIoTsLab4members";

    int humiditeSol = 0;
    int lumiere = 0;

    unsigned long currentTime = 0;
    unsigned long previousTime = 0;
    const unsigned long timeoutTime = 2000;

    String header;

    bool isNewClient = false; // Indique si un client est nouveau

    void sendResponse(String luminosite, String humidite, String humiditeSol, String temperature, String waterLevel);
    void handleClientRequest(const String& header); // Méthode pour traiter les requêtes
};

#endif
