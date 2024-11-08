#include "Sensor.h"

Sensor::Sensor()
{
    

}

int Sensor::loop() {
    SensorWaterLevel1 = analogRead(PinWaterLevel1);
    Serial.print("Valeur capteur 1 : ");
    Serial.println(SensorWaterLevel1);

    if(SensorWaterLevel1 < 4000) {
        WaterLevel1 = true;

    } else {
        WaterLevel1 = false;
        Serial.println("////Niveau d'eau insuffisant////");
    }

return 0;
}


Sensor::~Sensor()
{

}