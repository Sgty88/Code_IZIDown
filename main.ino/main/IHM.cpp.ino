#include "IHM.h"
#include "Config.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Création de l'objet LCD (Adresse 0x27 standard, parfois 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- INITIALISATION ---
void IHM_Initialiser() {
  lcd.init();
  lcd.backlight();
  
  // Configuration des boutons (INPUT_PULLUP = Actif quand mis à la masse/LOW)
  pinMode(PIN_BP_VALIDER, INPUT_PULLUP);
  pinMode(PIN_BP_DESCENTE, INPUT_PULLUP); 
  // Note : Le bouton d'urgence est géré dans le main via interruption
  
  // Le potentiomètre est une entrée analogique, pas besoin de pinMode explicite sur Uno
}

void IHM_Effacer() {
  lcd.clear();
}

// --- FONCTIONS D'AFFICHAGE ---

void IHM_AfficherAccueil() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Perceuse Auto");
  lcd.setCursor(0, 1);
  lcd.print("Groupe 4 - Init");
}

void IHM_AfficherMenuVitesse(int choix) {
  // Astuce anti-scintillement : on utilise une variable statique
  static int dernierChoix = -1;
  
  if (choix != dernierChoix) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Choix Vitesse:");
    lcd.setCursor(0, 1);
    
    switch(choix) {
      case 1: lcd.print("1. Metaux Durs"); break;
      case 2: lcd.print("2. Metaux Doux"); break;
      case 3: lcd.print("3. Bois/Plast"); break;
    }
    dernierChoix = choix; // On mémorise pour ne pas ré-afficher inutilement
  }
}

void IHM_AfficherMenuMode(bool modeDefini) {
  static bool dernierMode = !modeDefini; // Force l'affichage au 1er passage
  
  if (modeDefini != dernierMode) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mode Profondeur:");
    lcd.setCursor(0, 1);
    
    if (modeDefini) {
      lcd.print("< Mode DEFINI >"); // Fin de course
    } else {
      lcd.print("< Mode LIBRE >");  // Choix mm
    }
    dernierMode = modeDefini;
  }
}

void IHM_AfficherReglageMm(int mm) {
  static int dernierMm = -1;
  
  if (mm != dernierMm) {
    // Ici on n'efface pas tout l'écran, juste la ligne du bas pour aller vite
    lcd.setCursor(0, 0);
    lcd.print("Reglage (mm):");
    
    // On efface juste la zone du nombre (espaces vides)
    lcd.setCursor(0, 1);
    lcd.print("     "); 
    lcd.setCursor(0, 1);
    lcd.print(mm);
    lcd.print(" mm");
    
    dernierMm = mm;
  }
}

void IHM_AfficherPret(int vitesse, int prof) {
  // Pas de static ici car appelé une seule fois avant le perçage
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PRET! Btn1=GO");
  lcd.setCursor(0, 1);
  lcd.print("V:");
  lcd.print(vitesse);
  lcd.print(" P:");
  if (prof == -1) lcd.print("Auto");
  else lcd.print(prof);
}

void IHM_AfficherUrgence() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("!!! ARRET !!!");
  lcd.setCursor(0, 1);
  lcd.print("!! URGENCE !!");
}

void IHM_AfficherErreurBlocage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ERREUR MOTEUR");
  lcd.setCursor(0, 1);
  lcd.print("Force Trop Elev.");
}

// --- FONCTIONS DE LECTURE ---

int IHM_LireSelectionVitesse() {
  int val = analogRead(PIN_POT_PERCE);
  // Map 0-1023 vers 1-3
  // On met 1024 et 4 pour s'assurer que les tranches sont égales
  int choix = map(val, 0, 1024, 1, 4); 
  if (choix > 3) choix = 3; // Sécurité
  return choix;
}

bool IHM_LireSelectionMode() {
  int val = analogRead(PIN_POT_PERCE);
  // Si potentiomètre à gauche (< 512) -> Mode Libre (false)
  // Si potentiomètre à droite (> 512) -> Mode Défini (true)
  return (val > 512);
}

int IHM_LireReglageMm() {
  int val = analogRead(PIN_POT_PERCE);
  // Map vers 1mm à 200mm
  return map(val, 0, 1023, 1, 200);
}

bool IHM_BoutonValiderPresse() {
  // Lecture simple (INPUT_PULLUP inverse la logique : LOW = Appuyé)
  if (digitalRead(PIN_BP_VALIDER) == LOW) {
    delay(50); // Petit délai anti-rebond matériel
    return true; 
  }
  return false;
}

bool IHM_BoutonStartPresse() {
  if (digitalRead(PIN_BP_DESCENTE) == LOW) {
    delay(50);
    return true;
  }
  return false;
}