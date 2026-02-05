#ifndef MOTEUR_H
#define MOTEUR_H

#include <Arduino.h>

// On déclare les fonctions qu'on pourra utiliser dans le main
void initMoteur();
void lancerDescente(int vitesseMode); // 1, 2 ou 3
void lancerRemontee();
void arretMoteur();
bool verifierBlocage(); // Pour le StallGuard

#endif


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
