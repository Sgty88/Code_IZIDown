#ifndef MOTEUR_H
#define MOTEUR_H

#include <Arduino.h>

void Moteur_Initialiser();
void Moteur_PreparerDescente(int vitesseMode, bool modeDefini, int profondeurMm);
bool Moteur_FaireUnPas();
bool Moteur_RemonterAuZero();
void Moteur_ArretImmediat();

#endif