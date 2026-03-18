#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <Arduino.h>

/*
============================================================
  MACHINE A ETATS : "PERCEUSE AUTO"
============================================================

  BUT :
  - Organiser le programme en états simples
  - Eviter les "if" partout
  - Savoir EXACTEMENT "où on est" dans le cycle

  Ce module doit rester :
  - SIMPLE
  - LISIBLE
  - FACILE A DEBUG

  Dépendances (vos modules) :
  - IHM : lecture joystick/boutons + affichage
  - Moteur : descente "pas par pas" + remontée + arrêt immédiat

  IMPORTANT :
  - L'urgence est PRIORITAIRE (dès qu'on la détecte, on stoppe tout)
  - Le code doit permettre de corriger rapidement si un état marche mal
*/

// -------------------------------
// 1) ENUM DES ETATS
// -------------------------------
// Chaque valeur = une "étape" du fonctionnement.
// On colle à votre main actuel (CHOIX_VITESSE, etc.)
enum EtatSysteme {
  CHOIX_VITESSE,           // L'utilisateur choisit la vitesse au joystick
  CHOIX_MODE_PROFONDEUR,   // L'utilisateur choisit mode : DEFINI ou LIBRE
  REGLAGE_LIBRE_MM,        // Si LIBRE : l'utilisateur choisit la profondeur en mm
  ATTENTE_DEMARRAGE,       // Ecran "PRET" : attente bouton descente pour démarrer
  PERCAGE_DESCENTE,        // Descente du moteur (pas par pas)
  REMONTEE,                // Remontée au point zéro
  URGENCE                  // Etat d'arrêt d'urgence (bloquant)
};

// -------------------------------
// 2) CONTEXTE DE LA FSM
// -------------------------------
// Tout ce qui doit être mémorisé entre deux appels de loop().
// Exemple : vitesse choisie, mode, profondeur, etc.
struct FsmContext {
  // Etat courant
  EtatSysteme etatActuel = CHOIX_VITESSE;

  // Choix utilisateur
  int  vitesseChoisie = 1;   // 1..3
  bool modeDefini = true;    // true=DEFINI ; false=LIBRE
  int  profondeurMm = 0;     // utilisé si mode libre

  // Urgence : "flag" mis à true dès qu'on détecte l'arrêt urgence
  volatile bool flagUrgence = false;

  // Anti-rebond logiciel non bloquant pour VALIDER
  // (évite plusieurs transitions sur un seul appui)
  unsigned long dernierAppuiMs = 0;

  // Petit indicateur "onEntry" (action exécutée une seule fois à l'entrée d'un état)
  // Très utile pour ne pas ré-appeler des fonctions moteur à chaque tick.
  bool entreeEtat = true;
};

// -------------------------------
// 3) API DU MODULE
// -------------------------------

// Initialise la FSM + IHM + moteur (équivalent à votre setup logique)
void FSM_Init(FsmContext &ctx);

// Tick : à appeler dans loop() : exécute 1 "pas logique" de la FSM
void FSM_Tick(FsmContext &ctx);

// Déclenche l'urgence (appelable depuis ISR ou polling)
void FSM_DeclencherUrgence(FsmContext &ctx);

#endif