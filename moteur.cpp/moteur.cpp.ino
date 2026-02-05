#include "Moteur.h"
#include "Config.h"
#include <AccelStepper.h>

// On crée l'objet stepper ici, mais en "privé" (pas besoin de le voir ailleurs)
AccelStepper stepper(AccelStepper::DRIVER, PIN_STEP, PIN_DIR);

void initMoteur() {
    pinMode(PIN_EN, OUTPUT);
    digitalWrite(PIN_EN, LOW); // Activer
    stepper.setMaxSpeed(1000);
}

void lancerDescente(int vitesseMode) {
    // Conversion Vitesse (mm/s) -> Vitesse (pas/s)
    // Ex: Mode 1 (1mm/s) = 25 pas/s
    float speed = vitesseMode * PAS_PAR_MM; 
    stepper.setSpeed(speed);
    stepper.runSpeed(); // A appeler en boucle
}

// ... etc pour les autres fonctions

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
