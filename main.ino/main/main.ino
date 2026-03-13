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

/*
#include <Arduino.h>
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
} */

#include <Arduino.h>
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
  Serial.print(potValue);*/
  
  Serial.print(" | Descente: ");
  Serial.println(btnDescente ? "APPUI" : "relache");
  
  //Serial.print(" | Valider: ");
  //Serial.print(btnValider ? "APPUI" : "relache");
  
  /*Serial.print(" | Urgence: ");
  Serial.println(btnUrgence ? "APPUI" : "relache");*/

  delay(200); // Pause pour ne pas spammer l'écran
}