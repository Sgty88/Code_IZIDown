#include "Moteur.h"
#include "Config.h"
#include <AccelStepper.h>

// On crée l'objet stepper ici, mais en "privé" (pas besoin de le voir ailleurs)
#include "Moteur.h"
#include "Config.h"

// Variables pour simuler le temps qui passe (juste pour le test IHM)
unsigned long tempsDebutAction = 0;

void Moteur_Initialiser() {
  Serial.println("[MOTEUR] Initialisation OK (Simulation)");
  // On met quand même les pins en sortie pour éviter les soucis électriques
  pinMode(PIN_EN, OUTPUT);
  digitalWrite(PIN_EN, HIGH); // Désactivé par défaut
}

void Moteur_ActiverRelais(bool etat) {
  if(etat) Serial.println("[RELAIS] Arrosage ON");
  else Serial.println("[RELAIS] Arrosage OFF");
}

void Moteur_PreparerDescente(int vitesse, int distanceMax) {
  Serial.print("[MOTEUR] Configuration Descente -> Vitesse: ");
  Serial.print(vitesse);
  Serial.print(" | Distance: ");
  Serial.println(distanceMax);
  tempsDebutAction = millis(); // On lance le chrono
}

bool Moteur_GererDescente() {
  // SIMULATION : On fait semblant que la descente dure 3 secondes
  if (millis() - tempsDebutAction > 3000) {
    Serial.println("[MOTEUR] Descente terminée (Simulé)");
    return true; // C'est fini !
  }
  return false; // On continue de descendre
}

bool Moteur_DetecterBlocage() {
  return false; // Pour l'instant, on dit qu'il n'y a jamais de blocage
}

bool Moteur_LireFinDeCourseBas() {
  return false; // On simule qu'on ne touche pas le capteur bas
}

void Moteur_GererRemontee() {
  // Pas besoin de code ici pour le test, le main attend juste FinDeCourseHaut
}

bool Moteur_LireFinDeCourseHaut() {
  // SIMULATION : On fait semblant que la remontée prend 2 secondes
  // On utilise un modulo pour faire clignoter ou attendre un peu
  static unsigned long debutRemontee = 0;
  if (debutRemontee == 0) debutRemontee = millis();

  if (millis() - debutRemontee > 2000) {
    debutRemontee = 0; // Reset pour la prochaine fois
    Serial.println("[MOTEUR] Arrivé au point zéro (Simulé)");
    return true;
  }
  return false;
}

void Moteur_Stop() {
  Serial.println("[MOTEUR] Stop Normal");
}

void Moteur_ArretImmediat() {
  Serial.println("[URGENCE] MOTEUR COUPÉ !");
}



/*AccelStepper stepper(AccelStepper::DRIVER, PIN_STEP, PIN_DIR);

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

}*/
