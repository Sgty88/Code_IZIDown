#ifndef MOTEUR_H
#define MOTEUR_H

#include <Arduino.h>


void Moteur_Initialiser();
void Moteur_ActiverRelais(bool etat);
void Moteur_PreparerDescente(int vitesse, int distanceMax);
bool Moteur_GererDescente(); // Renvoie TRUE quand fini
bool Moteur_DetecterBlocage();
bool Moteur_LireFinDeCourseBas();
void Moteur_GererRemontee();
bool Moteur_LireFinDeCourseHaut(); // Renvoie TRUE quand arrivé en haut
void Moteur_Stop();
void Moteur_ArretImmediat();

#endif
// On déclare les fonctions qu'on pourra utiliser dans le main
/*void initMoteur();
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

}*/
