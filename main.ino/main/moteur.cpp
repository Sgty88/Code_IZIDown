#include "moteur.h"
#include "Config.h"

// Variables pour suivre la progression du moteur
long pasCibles = 0;
long pasActuels = 0;
int delaiMicrosecondes = 1000; 

void Moteur_Initialiser() {
  pinMode(PIN_EN, OUTPUT);
  pinMode(PIN_STEP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);
  
  digitalWrite(PIN_EN, LOW); // LOW = Active le driver TMC2209
}

void Moteur_PreparerDescente(int vitesseMode, bool modeDefini, int profondeurMm) {
  // 1. Choix du sens de rotation (LOW pour descendre, à inverser si la perceuse monte)
  digitalWrite(PIN_DIR, LOW); 
  
  pasActuels = 0;

  // 2. Calcul du nombre de pas total
  if (modeDefini) {
    pasCibles = 200000; // Un chiffre énorme, la machine s'arrêtera au capteur
  } else {
    // PAS_PAR_MM = 400 (selon ton Config.h)
    pasCibles = profondeurMm * PAS_PAR_MM; 
  }

  // 3. Calcul de la vitesse (Délai entre chaque pas)
  // 1 mm/s = 400 pas par seconde -> 1 pas toutes les 2500 µs
  if (vitesseMode == 1) delaiMicrosecondes = 2500; // Lent (1mm/s)
  if (vitesseMode == 2) delaiMicrosecondes = 1250; // Moyen (2mm/s)
  if (vitesseMode == 3) delaiMicrosecondes = 800;  // Rapide (3mm/s)
}

// Cette fonction est appelée en boucle par la machine à états
bool Moteur_FaireUnPas() {
  if (pasActuels >= pasCibles) {
    return true; // Le perçage est terminé !
  }

  // --- TON CODE QUI MARCHE ---
  digitalWrite(PIN_STEP, HIGH);
  delayMicroseconds(delaiMicrosecondes);
  digitalWrite(PIN_STEP, LOW);
  delayMicroseconds(delaiMicrosecondes);
  // ---------------------------

  pasActuels++;
  return false; // Le perçage n'est pas encore fini
}

bool Moteur_RemonterAuZero() {
  // On inverse le sens pour remonter
  digitalWrite(PIN_DIR, HIGH); 
  
  // Vitesse de remontée rapide fixe (ex: 800 µs)
  int delaiRemontee = 800; 

  // --- ATTENTION --- 
  // Ici, on remonte juste du nombre de pas qu'on a descendu.
  // Plus tard, tu remplaceras la condition du `for` par la lecture du capteur de fin de course.
  for (long i = 0; i < pasActuels; i++) {
    digitalWrite(PIN_STEP, HIGH);
    delayMicroseconds(delaiRemontee);
    digitalWrite(PIN_STEP, LOW);
    delayMicroseconds(delaiRemontee);
  }
  
  return true; // Remontée terminée
}

void Moteur_ArretImmediat() {
  // On désactive le driver en mettant EN_PIN à HIGH pour tout stopper brutalement
  digitalWrite(PIN_EN, HIGH); 
}