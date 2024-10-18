  // Controle de vitesse d'un moteur en pwm
  // avec un Arduino et un Mofset IRF540N
  // voir aussi http://bildr.org/2012/03/rfp30n06le-arduino/
  
  
  int motorPin = 3;     // Controle de vitesse PWM vers la gate du Mofset
  int potarPin=0; // on branche un potentiomètre sur la PIn A0
  int sensorValue=0; // la valeur entre 0 et 1023 déduite de la position du potentiomètre
  int outputValue=0; // la valeur entre 0 et 255 que l'on va utiliser pour régler la vitesse du moteur
  
  // Executee une seule fois
  
  void setup() {
     // initialiser la communication serie
     Serial.begin(19200);
     pinMode(motorPin, OUTPUT); // motorPin est utilisee en sortie
     
     analogWrite(motorPin,120 );  //  valeur de 0 à 255
  }
   
  // executee en boucle
  void loop() {
    /*
     // lire la valeur en entrée de la Pin A0
     sensorValue = analogRead(potarPin);
     // afficher sur le moniteur série la valeur lue, pas obligatoire mais toujours intéressant
     Serial.println(sensorValue);
     // conversion de la valeur sensorvalue (qui va de 0 à 1023) en une valeur entre 0 et 255, outputvalue
     outputValue = map(sensorValue, 0, 1023, 0, 255);
     // outputvalue permet de controler la vitesse du moteur
     */
     
     delay(100);
  }