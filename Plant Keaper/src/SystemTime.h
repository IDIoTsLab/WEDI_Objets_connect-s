#ifndef SYSTEMTIME_H
#define SYSTEMTIME_H

#include <Arduino.h>
#include <WiFi.h>
#include <Time.h>


#pragma once

class SystemTime
{
public:
    SystemTime();
    ~SystemTime();

    void Setup();
    String printLocalTime();
    int getCurrentHour();


private:
// Configuration NTP
const char* ntpServer = "pool.ntp.org";  // Serveur NTP public
const long gmtOffset_sec = 3600;        // Offset en secondes pour GMT+1 (France)
const int daylightOffset_sec = 3600;    // Offset pour l'heure d'été (1 heure)
};

#endif