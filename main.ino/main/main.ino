#include <Arduino.h>
#include "Config.h"
#include "StateMachine.h"

#ifndef NOT_AN_INTERRUPT
#define NOT_AN_INTERRUPT -1
#endif

FsmContext g_fsm;

// ISR si interruption disponible
void declencherUrgenceISR()
{
  FSM_DeclencherUrgence(g_fsm);
}

void setup()
{
  Serial.begin(9600);

  // Config AU en pullup (LOW = appuyé)
  pinMode(PIN_BP_AU, INPUT);

  // Init FSM (IHM + moteur)
  FSM_Init(g_fsm);

  delay(2000); // laisse le temps de lire l'accueil (optionnel)

  // Attache interruption seulement si la broche le permet
  int irq = digitalPinToInterrupt(PIN_BP_AU);
  if (irq != NOT_AN_INTERRUPT) {
    attachInterrupt(irq, declencherUrgenceISR, FALLING);
    Serial.println("[AU] Interruption active");
  } else {
    Serial.println("[AU] Pas d'interruption sur cette pin -> polling");
  }
}

void loop()
{
  // Si pas d'interruption possible, on lit AU en polling
  if (digitalPinToInterrupt(PIN_BP_AU) == NOT_AN_INTERRUPT) {
    if (digitalRead(PIN_BP_AU) == HIGH) {
      FSM_DeclencherUrgence(g_fsm);
    }
  }

  // Tick FSM
  FSM_Tick(g_fsm);

  // Tempo légère (évite boucle trop rapide)
  delay(1);
}





/*#define EN_PIN 2    // LOW: Driver enabled, HIGH: Driver disabled
#define STEP_PIN 8  // Step on the rising edge
#define DIR_PIN 9   // Set stepping direction

// 200 pas/tour * 16 microsteps * 5 tours = 16000 pas
int noOfSteps = 16000;           

// Délai réduit pour aller plus vite (400 microsecondes au lieu de 1000)
int microSecondsDelay = 100;  

void setup() {
  pinMode(EN_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  
  digitalWrite(EN_PIN, LOW);   // Activer le driver
}

void loop() {
  // Aller : 5 tours dans un sens
  digitalWrite(DIR_PIN, LOW);  
  moveSteps(noOfSteps);
  
  delay(500); // Petite pause d'une demi-seconde pour ne pas brusquer la mécanique
  
  // Retour : 5 tours dans le sens inverse
  digitalWrite(DIR_PIN, HIGH); 
  moveSteps(noOfSteps);
  
  delay(500); // Petite pause avant de recommencer
}

void moveSteps(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(microSecondsDelay);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(microSecondsDelay);
  }
}


/*#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>
#include "Config.h"

// --- CRÉATION DES OBJETS ---
// L'écran (avec les infos de Config.h)
LiquidCrystal_I2C lcd(LCD_ADRESSE, LCD_COLONNES, LCD_LIGNES);

// Le moteur
AccelStepper stepper(AccelStepper::DRIVER, PIN_STEP, PIN_DIR);

void setup() {
  Serial.begin(9600);
  
  // --- INITIALISATION ÉCRAN ---
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Init Moteur...");

  // --- INITIALISATION MOTEUR ---
  pinMode(PIN_EN, OUTPUT);
  digitalWrite(PIN_EN, LOW); // LOW pour activer le driver TMC2209

  stepper.setMaxSpeed(800);     // Vitesse max (pas/sec)
  stepper.setAcceleration(400); // Accélération

  delay(1500); // Laisse le temps de lire le message de démarrage
}

void loop() {
  // --- 1. SÉQUENCE DE DESCENTE ---
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">> Descente...");
  lcd.setCursor(0, 1);
  lcd.print("Vers: 1600 pas");
  Serial.println("Descente en cours...");

  stepper.moveTo(1600);
  stepper.runToPosition(); // Bloque ici jusqu'à la fin du mouvement
  
  // --- 2. PAUSE EN BAS ---
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("|| PAUSE EN BAS");
  Serial.println("Pause...");
  delay(1000); 
  
  // --- 3. SÉQUENCE DE REMONTÉE ---
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("<< Remontee...");
  lcd.setCursor(0, 1);
  lcd.print("Vers: 0 pas");
  Serial.println("Remontee en cours...");

  stepper.moveTo(0);
  stepper.runToPosition(); // Bloque ici jusqu'au retour à 0
  
  // --- 4. PAUSE EN HAUT ---
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("|| PAUSE EN HAUT");
  Serial.println("Pause...");
  delay(1000); 
}/*







/*#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Config.h"

LiquidCrystal_I2C lcd(LCD_ADRESSE, LCD_COLONNES, LCD_LIGNES);

void setup() {
  Serial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  
  // Le bouton du joystick a besoin de la résistance de tirage interne
  pinMode(PIN_JOYSTICK_BTN, INPUT_PULLUP);
  
  lcd.setCursor(0, 0);
  lcd.print("Test Joystick...");
  delay(1500);
  lcd.clear();
}

void loop() {
  // 1. Lecture des valeurs
  int valeurX = analogRead(PIN_JOYSTICK_X);
  int valeurY = analogRead(PIN_JOYSTICK_Y);
  bool clic = !digitalRead(PIN_JOYSTICK_BTN); // Inversé car INPUT_PULLUP

  // 2. Affichage sur le Moniteur Série (plus détaillé)
  Serial.print("X: "); Serial.print(valeurX);
  Serial.print(" | Y: "); Serial.print(valeurY);
  Serial.print(" | Bouton: "); Serial.println(clic ? "CLIC !" : "Relache");

  // 3. Affichage sur l'écran LCD
  lcd.setCursor(0, 0);
  lcd.print("X:");
  lcd.print(valeurX);
  lcd.print("   "); // Des espaces pour effacer les anciens chiffres

  lcd.setCursor(8, 0);
  lcd.print("Y:");
  lcd.print(valeurY);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  if (clic) {
    lcd.print(">> CLIC VALIDE <<");
  } else {
    lcd.print("                 "); // Efface la ligne
  }

  delay(100); // Petite pause pour que l'écran soit lisible
}*/






/*#include <Wire.h>
 
 
void setup()
{
  Wire.begin();
 
  Serial.begin(9600);
  while (!Serial);             // Leonardo: wait for serial monitor
  Serial.println("\nI2C Scanner");
}
 
 
void loop()
{
  byte error, address;
  int nDevices;
 
  Serial.println("Scanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
 
  delay(5000);           // wait 5 seconds for next scan
}*/


/*#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Config.h"

// Création de l'objet écran avec les infos de Config.h
LiquidCrystal_I2C lcd(LCD_ADRESSE, LCD_COLONNES, LCD_LIGNES);

void setup() {
  lcd.clear();
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Test Ecran OK !");
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print("Chrono : ");
  lcd.print(millis() / 1000); // Affiche les secondes qui passent
  delay(1000);
}*/

/*#include <Arduino.h>
#include "Config.h"

void setup() {
  Serial.begin(9600); // Ouvre le moniteur série à 9600 bauds
  
  pinMode(PIN_BP_DESCENTE, INPUT);
  pinMode(PIN_BP_VALIDER, INPUT);
  pinMode(PIN_BP_AU, INPUT);
  
  Serial.println("--- DEMARRAGE TEST ENTREES ---");
}

void loop() {
  int potValue = analogRead(PIN_POT_PERCE);
  
  // Lecture des boutons (Inversé car INPUT_PULLUP : LOW = Appuyé)
  bool btnDescente = !digitalRead(PIN_BP_DESCENTE);
  bool btnValider  = !digitalRead(PIN_BP_VALIDER);
  bool btnUrgence  = !digitalRead(PIN_BP_AU);

  /*Serial.print("Potentiometre: ");
  Serial.print(potValue);
  
  Serial.print(" | Descente: ");
  Serial.println(btnDescente ? "APPUI" : "relache");
  
  //Serial.print(" | Valider: ");
  //Serial.print(btnValider ? "APPUI" : "relache");
  
  /*Serial.print(" | Urgence: ");
  Serial.println(btnUrgence ? "APPUI" : "relache");

  delay(200); // Pause pour ne pas spammer l'écran
}*/