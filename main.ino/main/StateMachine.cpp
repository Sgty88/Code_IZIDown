#include "StateMachine.h"
#include "IHM.h"
#include "moteur.h"
#include "Config.h"

/*
============================================================
  OUTIL DEBUG (optionnel)
============================================================
  Si vous voulez tracer l’état courant dans le moniteur série,
  décommentez DEBUG_FSM et utilisez Serial.begin dans main.
*/
// #define DEBUG_FSM

#ifdef DEBUG_FSM
static const char* nomEtat(EtatSysteme e) {
  switch(e) {
    case CHOIX_VITESSE: return "CHOIX_VITESSE";
    case CHOIX_MODE_PROFONDEUR: return "CHOIX_MODE_PROFONDEUR";
    case REGLAGE_LIBRE_MM: return "REGLAGE_LIBRE_MM";
    case ATTENTE_DEMARRAGE: return "ATTENTE_DEMARRAGE";
    case PERCAGE_DESCENTE: return "PERCAGE_DESCENTE";
    case REMONTEE: return "REMONTEE";
    case URGENCE: return "URGENCE";
    default: return "???";
  }
}
#endif

/*
============================================================
  Anti-rebond logiciel (VALIDEUR)
============================================================
  Pourquoi ?
  - Un bouton mécanique "rebondit"
  - Vous aviez des delay(300) dans le main (bloquant)
  - Ici on fait NON BLOQUANT : on ignore les appuis trop rapprochés.

  Si votre bouton est "trop sensible" :
  - augmentez deltaMs (ex: 300)
  Si le bouton est "lent" :
  - baissez deltaMs (ex: 150)
*/
static bool antiRebondOk(FsmContext &ctx, unsigned long maintenant, unsigned long deltaMs = 250)
{
  if (maintenant - ctx.dernierAppuiMs < deltaMs) return false;
  ctx.dernierAppuiMs = maintenant;
  return true;
}

/*
============================================================
  Changement d'état centralisé
============================================================
  Ce helper rend le code plus lisible :
  - on change l'état
  - on marque "entreeEtat=true" pour exécuter les actions d'entrée
*/
static void changerEtat(FsmContext &ctx, EtatSysteme nouvelEtat)
{
  ctx.etatActuel = nouvelEtat;
  ctx.entreeEtat = true;

#ifdef DEBUG_FSM
  Serial.print("[FSM] -> ");
  Serial.println(nomEtat(nouvelEtat));
#endif
}

/*
============================================================
  Urgence : fonction déclenchée par ISR ou polling
============================================================
  - On met un flag
  - On coupe le moteur immédiatement
  - La FSM basculera sur URGENCE au prochain tick
*/
void FSM_DeclencherUrgence(FsmContext &ctx)
{
  // On s'assure que c'est une NOUVELLE urgence
  if (!ctx.flagUrgence) {
    ctx.flagUrgence = true;
    ctx.dernierAppuiMs = millis(); // On mémorise le moment exact du crash
    Moteur_ArretImmediat();
  }
}

/*
============================================================
  Initialisation FSM
============================================================
  - Initialise IHM + moteur
  - Initialise les variables du contexte
  - Affiche l'accueil
*/
void FSM_Init(FsmContext &ctx)
{
  IHM_Initialiser();
  Moteur_Initialiser();

  ctx.etatActuel = CHOIX_VITESSE;

  ctx.vitesseChoisie = 1;
  ctx.modeDefini = true;
  ctx.profondeurMm = 0;

  ctx.flagUrgence = false;
  ctx.dernierAppuiMs = 0;
  ctx.entreeEtat = true;

  IHM_AfficherAccueil();

#ifdef DEBUG_FSM
  Serial.println("[FSM] Init OK");
#endif
}

/*
============================================================
  Tick FSM : appelé dans loop()
============================================================
  Règle d'or :
  - PAS de delay() long ici
  - Chaque appel fait un petit "morceau" de logique
*/
void FSM_Tick(FsmContext &ctx)
{
  // 1) URGENCE prioritaire : si flagUrgence est mis, on force l'état URGENCE
  if (ctx.flagUrgence) {
    ctx.etatActuel = URGENCE;
  }

  // Timestamp courant utilisé pour anti-rebond
  unsigned long now = millis();

  // 2) Machine à états
  switch (ctx.etatActuel) {

    // =========================================================
    // ETAT 1 : CHOIX_VITESSE
    // =========================================================
    case CHOIX_VITESSE:
    {
      /*
        - On lit le joystick -> renvoie 1..3
        - On affiche le menu associé
        - Sur VALIDER (avec anti-rebond) -> état suivant
      */
      ctx.vitesseChoisie = IHM_LireJoystickMenuVitesse();
      IHM_AfficherMenuVitesse(ctx.vitesseChoisie);

      if (IHM_BoutonValiderAppuye() && antiRebondOk(ctx, now)) {
        changerEtat(ctx, CHOIX_MODE_PROFONDEUR);
      }
    }
    break;

    // =========================================================
    // ETAT 2 : CHOIX_MODE_PROFONDEUR
    // =========================================================
    case CHOIX_MODE_PROFONDEUR:
    {
      /*
        - On lit le joystick -> vrai/faux (DEFINI/LIBRE)
        - On affiche le menu mode
        - Sur VALIDER :
            * si DEFINI  -> on saute le réglage mm
            * si LIBRE   -> on va régler la profondeur
      */
      ctx.modeDefini = IHM_LireJoystickMode();
      IHM_AfficherMenuMode(ctx.modeDefini);

      if (IHM_BoutonValiderAppuye() && antiRebondOk(ctx, now)) {
        if (ctx.modeDefini) {
          changerEtat(ctx, ATTENTE_DEMARRAGE);
        } else {
          changerEtat(ctx, REGLAGE_LIBRE_MM);
        }
      }
    }
    break;

    // =========================================================
    // ETAT 3 : REGLAGE_LIBRE_MM
    // =========================================================
    case REGLAGE_LIBRE_MM:
    {
      /*
        - On lit le joystick -> profondeur 1..200 mm
        - On affiche la profondeur
        - Sur VALIDER -> ATTENTE_DEMARRAGE
      */
      ctx.profondeurMm = IHM_LireJoystickMm();
      IHM_AfficherReglageMm(ctx.profondeurMm);

      if (IHM_BoutonValiderAppuye() && antiRebondOk(ctx, now)) {
        changerEtat(ctx, ATTENTE_DEMARRAGE);
      }
    }
    break;

    // =========================================================
    // ETAT 4 : ATTENTE_DEMARRAGE
    // =========================================================
    case ATTENTE_DEMARRAGE:
    {
      /*
        - Affiche PRET
        - Attend appui sur bouton "Descente" pour démarrer
        - Prépare ensuite le moteur avec:
            Moteur_PreparerDescente(vitesse, mode, profondeur)
        - Puis passe à PERCAGE_DESCENTE
      */
      IHM_AfficherPret(ctx.vitesseChoisie, ctx.modeDefini ? -1 : ctx.profondeurMm);

      if (IHM_BoutonDescenteAppuye()) {
        // Préparation moteur AVANT de passer en descente
        Moteur_PreparerDescente(ctx.vitesseChoisie, ctx.modeDefini, ctx.profondeurMm);

        changerEtat(ctx, PERCAGE_DESCENTE);
      }
    }
    break;

    // =========================================================
    // ETAT 5 : PERCAGE_DESCENTE
    // =========================================================
    case PERCAGE_DESCENTE:
    {
      /*
        - Affiche "Descente..."
        - Appelle Moteur_FaireUnPas() à chaque tick
        - Quand Moteur_FaireUnPas() renvoie true -> descente terminée -> REMONTEE
      */
      IHM_AfficherEnCours("Descente...");

      if (Moteur_FaireUnPas()) {
        changerEtat(ctx, REMONTEE);
      }
    }
    break;

    // =========================================================
    // ETAT 6 : REMONTEE
    // =========================================================
    case REMONTEE:
    {
      /*
        - Affiche "Remontee..."
        - Appelle Moteur_RemonterAuZero()

        ATTENTION IMPORTANT :
        Votre Moteur_RemonterAuZero() actuel contient une boucle for + delayMicroseconds,
        donc elle est BLOQUANTE.
        => pendant cette remontée, la loop ne tourne plus, et un arrêt d'urgence en polling
           ne sera pas lu.
        => si vous voulez AU ultra réactif, il faudra rendre cette fonction non-bloquante.

        Ici on garde la logique actuelle :
        - si la fonction renvoie true -> on revient au début du cycle
      */
      IHM_AfficherEnCours("Remontee...");

      if (Moteur_RemonterAuZero()) {
        changerEtat(ctx, CHOIX_VITESSE);
      }
    }
    break;

    // =========================================================
    // ETAT 7 : URGENCE
    // =========================================================
    case URGENCE:
    {
      IHM_AfficherUrgence();

      // Mémoire pour forcer l'utilisateur à relâcher le bouton au moins une fois
      static bool boutonRelache = false;

      // 1. Si l'utilisateur relâche le bouton (le signal retombe à LOW)
      if (digitalRead(PIN_BP_AU) == LOW) {
        boutonRelache = true;
      }

      // 2. Si le bouton a bien été relâché, qu'on RAPpui dessus (HIGH),
      // et qu'il s'est passé au moins 1 seconde depuis le crash (anti-rebond de sécurité)
      if (boutonRelache && digitalRead(PIN_BP_AU) == HIGH && (now - ctx.dernierAppuiMs > 1000)) {
        
        boutonRelache = false;   // On réinitialise pour la prochaine fois
        ctx.flagUrgence = false; // On lève le drapeau d'urgence
        ctx.dernierAppuiMs = now; // Anti-rebond pour les autres menus
        
        // On réactive le driver du moteur (qui avait été coupé brutalement)
        // Moteur_Initialiser remet la broche EN_PIN à LOW
        Moteur_Initialiser(); 
        
        // Retour au menu de départ !
        changerEtat(ctx, CHOIX_VITESSE);
      }
    }
    break;
  }
}