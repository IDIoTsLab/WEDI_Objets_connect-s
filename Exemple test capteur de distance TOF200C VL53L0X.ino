#include <Wire.h>        // Inclut la bibliothèque Wire pour la communication I2C
#include <VL53L0X.h>     // Inclut la bibliothèque pour le capteur de distance VL53L0X

VL53L0X sensor;          // Crée un objet "sensor" pour contrôler le capteur VL53L0X
int data = 0;            // Variable pour stocker la distance mesurée

// La fonction "setup" est exécutée une seule fois au démarrage
void setup()
{
  Serial.begin(9600);    // Démarre la communication série à 9600 bauds (pour afficher des données sur le moniteur série)
  Wire.begin();          // Initialise la communication I2C

  // Définit le temps alloué pour chaque mesure (en microsecondes), ici 200ms
  sensor.setMeasurementTimingBudget(200000);

  sensor.setTimeout(500);  // Définit un temps d'attente maximum de 500ms avant de considérer une mesure échouée
    if (!sensor.init())      // Initialise le capteur, et vérifie si l'initialisation est réussie
  {
    Serial.println("Failed to detect and initialize sensor!");  // Affiche un message d'erreur si le capteur n'est pas détecté
    while (1) {}           // Boucle infinie si le capteur ne fonctionne pas, pour bloquer le programme
  }

  sensor.startContinuous();  // Démarre la mesure continue des distances
}

// La fonction "loop" s'exécute en boucle tant que l'Arduino est sous tension
void loop()
{
  // Lit la distance mesurée par le capteur en millimètres
  data = sensor.readRangeContinuousMillimeters();

  // Vérifie si la valeur retournée est valide (8190 signifie "hors de portée")
  // donc si data est diférent (!=) de 8190 
  if (data != 8190){
    // Affiche la distance mesurée dans le moniteur série si la mesure est valide
    Serial.print(sensor.readRangeContinuousMillimeters());
  }
  else {
    // Affiche "out of range" si la mesure est hors de portée
    Serial.println("out of range");
  }

  // Vérifie si un dépassement du temps d'attente s'est produit
  if (sensor.timeoutOccurred()) {
    // Affiche "TIMEOUT" si le capteur a pris trop de temps pour effectuer la mesure
    Serial.print(" TIMEOUT");
  }

  Serial.println();  // Passe à la ligne suivante dans le moniteur série
}
