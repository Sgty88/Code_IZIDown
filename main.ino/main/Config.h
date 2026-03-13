#ifndef CONFIG_H
#define CONFIG_H

// --- 1. CABLAGE UTILISATEUR (Boutons & Potentiomètre) ---
#define PIN_BP_DESCENTE 11
#define PIN_BP_VALIDER  13
#define PIN_BP_AU       12   // Interruption (Sécurité)
#define PIN_POT_PERCE   A0

// --- 2. CABLAGE ECRAN (I2C) ---
// Sur UNO, ces broches sont fixes (A4/A5), mais on les note pour le plan de câblage
#define PIN_I2C_SDA     SDA
#define PIN_I2C_SCL     SCL
#define LCD_ADRESSE     0x27 // Change ici si ton écran ne s'allume pas (essaie 0x3F)
#define LCD_COLONNES    16
#define LCD_LIGNES      4

// --- 3. CABLAGE MOTEUR (Driver TMC2209) ---
#define PIN_STEP        8
#define PIN_DIR         9
#define PIN_EN          2 
#define PIN_MS1         5
#define PIN_MS2         6

// --- 4. COMMUNICATION UART (Pour StallGuard) ---
#define PIN_RX_DRIVER   RX  // A connecter au TX du driver
#define PIN_TX_DRIVER   TX  // A connecter au RX du driver

// --- 5. PARAMETRES MECANIQUES ---
#define PAS_MOTEUR      200   // Nema 17 standard
#define MICROSTEPS      16    // Réglage driver (MS1/MS2)
#define PAS_VIS         8.0   // 8mm par tour (Tige T8)

// Calcul automatique : (200 * 16) / 8 = 400 pas pour 1mm
#define PAS_PAR_MM      ((PAS_MOTEUR * MICROSTEPS) / PAS_VIS) 

#endif

/*
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}*/
