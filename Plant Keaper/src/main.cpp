#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Sensor.h"
#include "Screen.h"

// Configuration de la communication I2C

// Configuration de l'écran OLED
/*
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // GPIO non utilisé pour le reset sur cet écran
#define OLED_ADDRESS  0x3C  // Adresse I2C de l'écran
#define WHITE SSD1306_WHITE
*/

Sensor sensor;
Screen screen;


//configuration capteur d'humidité 
const int sensorMoisturePin = 32;  
const int sensorLuminosityPin = 33;

int sensorMoisture = 0;
int PurcentMoisture = 0;
int sensorLuminosity = 0;
int PurcentLuminosity = 0;

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

  //1200(3,3V), 230(5V) correspond à la valeur minimal mesurée par le capteur (imergée)
  //3100(3,3V), 550(5V) correspond à la valeur maximal mesurée par le capteur (sec)
const int ValueMoistureSensorMin = 1200;
const int ValueMoistureSensorMax = 3100;
const int ValueLuminositySensorMin = 0;
const int ValueLuminositySensorMax = 4095;

//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // Initialisation du moniteur série pour le débogage
  Serial.begin(115200);

  screen.setup();
  // Initialisation de l'écran OLED
/*
      if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        Serial.println(F("Échec de l'initialisation de l'écran OLED"));
        while (true); // Boucle infinie si l'initialisation échoue
    }
*/
    // Configuration du canal 0 avec la fréquence et la résolution choisie
    ledcSetup(pwmChannelLight, frequenceLight, resolutionLight);
    // Assigne le canal PWM au pin 23
    ledcAttachPin(pwmPinLight, pwmChannelLight);

    // Créer la tension en sortie choisi
    ledcWrite(pwmChannelLight, 0); 




    ledcSetup(pwmChannelPump, frequencePump, resolutionPump);
    ledcAttachPin(pwmPinPump, pwmChannelPump);
    ledcWrite(pwmChannelPump, 0); 



}

int ReadLuminosity(){
  sensorLuminosity = analogRead(sensorLuminosityPin);
  Serial.print ("Luminosity : ");
  Serial.println(sensorLuminosity);


  PurcentLuminosity = map (sensorLuminosity, ValueMoistureSensorMin , ValueLuminositySensorMax, 0, 100);


return PurcentLuminosity;
}

int ReadMoisture(){

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
  Serial.println("humidité du sol :");
  Serial.print("Valeur analogique : ");
  Serial.println(sensorMoisture);
  Serial.print("Valeur en % :");
  Serial.print(PurcentMoisture);
  Serial.println("%");
  return PurcentMoisture;  
}




void loop() {
  
  //ReadMoisture();
  //ReadLuminosity();

  sensor.loop();
  screen.loop();
  //display.clearDisplay();  // Effacer l'écran à chaque boucle

  //test dev
  //display.drawRect(0, 17, 128, 44, SSD1306_WHITE); //-> ecran du bas 
  // hauteur de la police = 6






  //patie supértieur de l'écran (jaune)
  



  //partie inférieur de l'écran (bleu)
  /*
  display.setTextSize(1);  
  display.setCursor(0, 17);
  display.print(F("Humidite : "));
  display.print(PurcentMoisture);
  display.println("%");

  display.setCursor(0, 25);
  display.print(F("Luminosite : "));
  display.print(PurcentLuminosity);
  display.println("%");


  display.display(); // Affiche le contenu à l'écran
  */
  
  if (PurcentLuminosity < 50 ) {
    delay(500);
    ledcWrite(pwmChannelLight, 255); 
  }
  else {
    ledcWrite(pwmChannelLight, 0); 
  }




  if (PurcentMoisture < 50 ){
    ledcWrite(pwmChannelPump, 150); 

  }
  else {
    ledcWrite(pwmChannelPump, 0);
  }
  




delay(50);


}



