#include "Sensor.h"


Sensor::Sensor() 
    : dht(DHTPIN, DHTTYPE)  // Initialisation de l'objet DHT avec la broche et le type de capteur
{
    dht.begin();  // Initialiser le capteur DHT
}
int Sensor::loop() {
return 0;
}


int Sensor::ReadMoisture() {

  //lecture de la valeur du capteur et transformation en pourcentage
  //100 correspond à la valeur maximale de humidité du sol en pourcentage
  //0 correspond à la valeur minimale de humidité du sol en pourcentage
  sensorMoisture = analogRead(sensorMoisturePin);
  PurcentMoisture = map (sensorMoisture, ValueMoistureSensorMin , ValueMoistureSensorMax, 100, 0);

  if (PurcentMoisture < 0) {
    PurcentMoisture = 0;
  }

  if (PurcentMoisture > 100) {
    PurcentMoisture = 100;
  }

  //affichage des données sur le moniteur série
  Serial.print("humidité du sol : ");
  Serial.print("Valeur analogique : ");
  Serial.print(sensorMoisture);
  Serial.print("  Valeur en % : ");
  Serial.print(PurcentMoisture);
  Serial.println("%");
  return PurcentMoisture;  
}

int Sensor::ReadLuminosity() {
  sensorLuminosity = analogRead(sensorLuminosityPin);
  PurcentLuminosity = map (sensorLuminosity, ValueMoistureSensorMin , ValueLuminositySensorMax, 0, 100);

  Serial.print ("Luminosity : ");
    Serial.print("Valeur analogique : ");
  Serial.print(sensorLuminosity);       
  Serial.print("  Valeur en % : ");
  Serial.print(PurcentLuminosity);
  Serial.println("%");



return PurcentLuminosity;
}

bool Sensor::ReadWaterlevel() {
    SensorWaterLevel1 = analogRead(PinWaterLevel1);
    Serial.print("Valeur capteur 1 : ");
    Serial.println(SensorWaterLevel1);

    if(SensorWaterLevel1 < 4000) {
        WaterLevel1 = true;

    } else {
        WaterLevel1 = false;
        Serial.println("////Niveau d'eau insuffisant////");
    }
    return WaterLevel1;
}

float Sensor::ReadTemperature() {
    // Lecture de la température en degrés Celsius
    float temperature = dht.readTemperature();
    return temperature;
}


float Sensor::ReadHumidity() {
    // Lecture de l'humidité en pourcentage
    float humidity = dht.readHumidity();
    return humidity;
}









Sensor::~Sensor()
{

}