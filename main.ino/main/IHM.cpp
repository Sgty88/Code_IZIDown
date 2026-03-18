#include "IHM.h"
#include "Config.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(LCD_ADRESSE, LCD_COLONNES, LCD_LIGNES);

void IHM_Initialiser() {
  lcd.init();
  lcd.backlight();
  
  pinMode(PIN_BP_VALIDER, INPUT);
  pinMode(PIN_BP_DESCENTE, INPUT);
  // Le joystick analogique n'a pas besoin de pinMode
}

// --- FONCTIONS D'AFFICHAGE (Avec Anti-Scintillement) ---

void IHM_AfficherAccueil() {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Perceuse Auto");
  lcd.setCursor(0, 1); lcd.print("Systeme Pret !");
}

void IHM_AfficherMenuVitesse(int choix) {
  static int dernierChoix = -1;
  if (choix != dernierChoix) {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Vitesse Coupe :");
    lcd.setCursor(0, 1);
    if (choix == 1) lcd.print("> Metaux Durs");
    if (choix == 2) lcd.print("> Metaux Doux");
    if (choix == 3) lcd.print("> Bois/Plast.");
    dernierChoix = choix;
  }
}

void IHM_AfficherMenuMode(bool modeDefini) {
  static bool dernierMode = !modeDefini;
  if (modeDefini != dernierMode) {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Mode Profondeur:");
    lcd.setCursor(0, 1);
    if (modeDefini) lcd.print("> Mode DEFINI");
    else            lcd.print("> Mode LIBRE");
    dernierMode = modeDefini;
  }
}

void IHM_AfficherReglageMm(int mm) {
  static int dernierMm = -1;
  if (mm != dernierMm) {
    lcd.setCursor(0, 0); lcd.print("Reglage Prof.:  ");
    lcd.setCursor(0, 1);
    lcd.print(mm); lcd.print(" mm       "); // Espaces pour effacer les vieux chiffres
    dernierMm = mm;
  }
}

void IHM_AfficherPret(int vitesse, int prof) {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("PRET! Btn=Desc.");
  lcd.setCursor(0, 1);
  lcd.print("V:"); lcd.print(vitesse);
  lcd.print(" P:"); 
  if (prof == -1) lcd.print("Auto"); else lcd.print(prof);
}

void IHM_AfficherEnCours(const char* message) {
  static String dernierMessage = "";
  if (dernierMessage != message) {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("MOTEUR ACTIF");
    lcd.setCursor(0, 1); lcd.print(message);
    dernierMessage = message;
  }
}

void IHM_AfficherUrgence() {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("!! URGENCE !!");
  lcd.setCursor(0, 1); lcd.print("Moteur Coupe.");
}

// --- FONCTIONS DE LECTURE (Joystick et Boutons) ---

// --- 1. CHOIX DE LA VITESSE ---
int IHM_LireJoystickMenuVitesse() {
  static int choix = 1; // La mémoire interne (garde sa valeur entre chaque tour)
  static unsigned long dernierMvt = 0;
  int valY = analogRead(A2); // (Ou la pin de ton axe Y)

  // Si on a bougé le joystick et que 300ms se sont écoulées (anti-mitraillette)
  if (millis() - dernierMvt > 300) {
    if (valY > 700) { 
      choix++; // On descend dans le menu
      if (choix > 3) choix = 1; // Boucle : après 3, on revient à 1
      dernierMvt = millis();
    } 
    else if (valY < 300) {
      choix--; // On monte dans le menu
      if (choix < 1) choix = 3; // Boucle : avant 1, on passe à 3
      dernierMvt = millis();
    }
  }
  return choix; // Retourne la valeur mémorisée, même si le joystick est relâché !
}

// --- 2. CHOIX DU MODE (Défini ou Libre) ---
bool IHM_LireJoystickMode() {
  static bool modeDefini = true; // Mémoire du mode
  static unsigned long dernierMvt = 0;
  int valY = analogRead(A2);

  if (millis() - dernierMvt > 300) {
    if (valY > 700 || valY < 300) { // Dès qu'on pousse en haut ou en bas
      modeDefini = !modeDefini;     // On inverse le choix (True devient False, et inversement)
      dernierMvt = millis();
    }
  }
  return modeDefini;
}

// --- 3. REGLAGE DES MILLIMETRES (Mode Libre) ---
int IHM_LireJoystickMm() {
  static int mm = 10; // On commence à 10mm par défaut
  static unsigned long dernierMvt = 0;
  int valY = analogRead(A2);

  // Délai plus court (150ms) pour que ça défile un peu plus vite quand on reste appuyé
  if (millis() - dernierMvt > 150) {
    if (valY > 700) {
      mm += 5; // Ajoute 5 mm
      if (mm > 200) mm = 200; // Limite à 200mm max
      dernierMvt = millis();
    } 
    else if (valY < 300) {
      mm -= 5; // Enlève 5 mm
      if (mm < 1) mm = 1; // Limite à 1mm min
      dernierMvt = millis();
    }
  }
  return mm;
}

bool IHM_BoutonValiderAppuye() {
  // LOW car INPUT_PULLUP
  return (digitalRead(PIN_BP_VALIDER) == HIGH);
}

bool IHM_BoutonDescenteAppuye() {
  return (digitalRead(PIN_BP_DESCENTE) == HIGH);
}

