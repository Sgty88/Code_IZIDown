#ifndef CONFIG_H
#define CONFIG_H

// PINS (D'après ton tableau corrigé)
#define PIN_BP_DESCENTE 11
#define PIN_BP_VALIDER  13
#define PIN_BP_AU       2
#define PIN_POT_PERCE   A0
#define PIN_STEP        8
#define PIN_DIR         9
#define PIN_EN          12 

// Paramètres Mécaniques
#define PAS_PAR_TOUR 200
#define PAS_VIS      8.0 // 8mm par tour
// Calcul : 200 pas / 8mm = 25 pas pour 1mm
#define PAS_PAR_MM   25.0 

#endif


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
