#include "SystemTime.h"

SystemTime::SystemTime()
{
}

void SystemTime::Setup(){
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}


String SystemTime::printLocalTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Impossible de récupérer l'heure !");
        return " ";
    }
    //Serial.println(&timeinfo, "%H:%M:%S");

    return String(timeinfo.tm_hour) + ":" + String(timeinfo.tm_min) + ":" + String(timeinfo.tm_sec);
}

int SystemTime::getCurrentHour() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Impossible de récupérer l'heure !");
        return -1; // Retourne -1 en cas d'erreur
    }
    return timeinfo.tm_hour; // Retourne uniquement l'heure
}


SystemTime::~SystemTime()
{

}