#include "Device.h"


Sensor sensor2;

Device::Device()
{

}


void Device::setup(){

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


void Device::pump(int powerLevel){
    ledcWrite(pwmChannelPump, powerLevel);
}

void Device::light(int powerLevel){
    ledcWrite(pwmChannelLight, powerLevel);
}

void Device::loop() {

/*
      if (sensor2.ReadLuminosity() < 50 ) {
    delay(500);
    ledcWrite(pwmChannelLight, 255); 
  }
  else {
    ledcWrite(pwmChannelLight, 0); 
  }




  if (sensor2.ReadMoisture() < 50 ){
    ledcWrite(pwmChannelPump, 150); 

  }
  else {
    ledcWrite(pwmChannelPump, 0);
  }
  
*/
}

Device::~Device()
{

}