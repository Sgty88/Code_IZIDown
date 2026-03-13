#ifndef IHM_H
#define IHM_H

#include <Arduino.h>

// Initialisation de l'écran et des pins boutons (sauf urgence)
void IHM_Initialiser();

// Fonctions d'affichage (Ce que l'écran montre)
void IHM_AfficherAccueil();
void IHM_Effacer();
void IHM_AfficherMenuVitesse(int choix);      // Affiche "Metaux Dur", etc.
void IHM_AfficherMenuMode(bool modeDefini);   // Affiche "Defini" ou "Libre"
void IHM_AfficherReglageMm(int mm);           // Affiche la valeur en mm
void IHM_AfficherPret(int vitesse, int prof); // Affiche "Pret a percer"
void IHM_AfficherUrgence();
void IHM_AfficherErreurBlocage();

// Fonctions de lecture (Ce que l'utilisateur fait)
int IHM_LireSelectionVitesse(); // Retourne 1, 2 ou 3 selon le potentiomètre
bool IHM_LireSelectionMode();   // Retourne true (Défini) ou false (Libre)
int IHM_LireReglageMm();        // Retourne entre 1 et 200 mm
bool IHM_BoutonValiderPresse(); // Retourne true si appuyé
bool IHM_BoutonStartPresse();   // Retourne true si appuyé

#endif