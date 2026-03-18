#include "IHM.h"
#include "Config.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(LCD_ADRESSE, LCD_COLONNES, LCD_LIGNES);

void IHM_Initialiser() {
  lcd.init();
  lcd.backlight();
  
  pinMode(PIN_BP_VALIDER, INPUT_PULLDOWN);
  pinMode(PIN_BP_DESCENTE, INPUT_PULLDOWN);
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

int IHM_LireJoystickMenuVitesse() {
  int valY = analogRead(A2); // On utilise l'axe Y du joystick (A2)
  if (valY < 300) return 3;  // Poussé vers le haut (ou bas selon montage)
  if (valY > 700) return 1;  // Poussé de l'autre sens
  return 2;                  // Au centre
}

bool IHM_LireJoystickMode() {
  int valY = analogRead(A2);
  return (valY > 512); // Moitié d'un côté = Défini, l'autre = Libre
}

int IHM_LireJoystickMm() {
  int valY = analogRead(A2);
  return map(valY, 0, 1023, 1, 200); // Convertit la position en millimètres
}

bool IHM_BoutonValiderAppuye() {
  // LOW car INPUT_PULLUP
  return (digitalRead(PIN_BP_VALIDER) == LOW);
}

bool IHM_BoutonDescenteAppuye() {
  return (digitalRead(PIN_BP_DESCENTE) == LOW);
}