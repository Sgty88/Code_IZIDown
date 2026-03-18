#ifndef IHM_H
#define IHM_H

#include <Arduino.h>

// Initialisation
void IHM_Initialiser();

// Affichage LCD
void IHM_AfficherAccueil();
void IHM_AfficherMenuVitesse(int choix);
void IHM_AfficherMenuMode(bool modeDefini);
void IHM_AfficherReglageMm(int mm);
void IHM_AfficherPret(int vitesse, int prof);
void IHM_AfficherEnCours(const char* message);
void IHM_AfficherUrgence();

// Lecture des entrées (Joystick et Boutons)
int IHM_LireJoystickMenuVitesse();
bool IHM_LireJoystickMode();
int IHM_LireJoystickMm();
bool IHM_BoutonValiderAppuye();
bool IHM_BoutonDescenteAppuye();

#endif