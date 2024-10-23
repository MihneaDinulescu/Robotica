#include <Arduino.h>

// Definirea pinilor pentru LED-uri și butoane
#define YELLOW1_PIN 10
#define YELLOW2_PIN 9
#define RED1_PIN 8
#define RED2_PIN 7
#define GREEN_RGB_PIN 5
#define BLUE_RGB_PIN 4
#define RED_RGB_PIN 6
#define START_PIN 2
#define STOP_PIN 3

// Variabile pentru gestionarea stării butonului Start (debouncing)
int buttonState = HIGH;          // Starea curentă a butonului
int lastButtonState = HIGH;      // Starea anterioară a butonului
unsigned long lastDebounceTime = 0;  // Timpul ultimei modificări a stării butonului
unsigned long debounceDelay = 50;    // Întârziere pentru debouncing (50ms)

// Variabile pentru gestionarea procesului de încărcare
bool charging = false;            // Indică dacă încărcarea este activă
unsigned long stopPressStartTime = 0; // Timpul când butonul Stop a fost apăsat
bool stopButtonHeld = false;       // Indică dacă butonul Stop a fost ținut apăsat
unsigned long lastBlinkTime = 0;   // Timpul pentru ultima acțiune de blink a LED-urilor

// Declarațiile funcțiilor folosite
void startCharging();
void stopCharging();
void checkStopButton();

void setup() {
  // Configurarea pinilor de input pentru butoane cu rezistență internă de pull-up
  pinMode(START_PIN, INPUT_PULLUP);
  pinMode(STOP_PIN, INPUT_PULLUP);
  
  // Configurarea pinilor de output pentru LED-uri
  pinMode(YELLOW1_PIN, OUTPUT);
  pinMode(YELLOW2_PIN, OUTPUT);
  pinMode(RED1_PIN, OUTPUT);
  pinMode(RED2_PIN, OUTPUT);
  pinMode(RED_RGB_PIN, OUTPUT);
  pinMode(GREEN_RGB_PIN, OUTPUT);
  pinMode(BLUE_RGB_PIN, OUTPUT);

  // Inițializarea LED-ului RGB în stare verde (sistem inactiv, standby)
  analogWrite(RED_RGB_PIN, 0);    // LED roșu oprit
  analogWrite(GREEN_RGB_PIN, 255); // LED verde aprins
  analogWrite(BLUE_RGB_PIN, 0);   // LED albastru oprit
}

void loop() {
  // Citirea stării curente a butonului Start
  int reading = digitalRead(START_PIN);

  // Verificare pentru debouncing: dacă starea butonului s-a schimbat
  if (reading != lastButtonState) {
    lastDebounceTime = millis(); // Actualizare timp ultima schimbare
  }
  
  // Dacă a trecut timpul de debounce
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Dacă starea butonului s-a schimbat
    if (reading != buttonState) {
      buttonState = reading;

      // Dacă butonul este apăsat (LOW), începe încărcarea
      if (buttonState == LOW) {
        startCharging(); 
      }
    }
  }

  // Actualizare starea butonului anterior pentru următoarea iterație
  lastButtonState = reading; 
}

void startCharging() {
  charging = true; // Setează starea de încărcare activă
 
  // Schimbă LED-ul RGB la roșu pentru a indica încărcarea activă
  analogWrite(RED_RGB_PIN, 255);   // LED roșu aprins
  analogWrite(GREEN_RGB_PIN, 0);   // LED verde oprit
  analogWrite(BLUE_RGB_PIN, 0);    // LED albastru oprit

  unsigned long lastBlinkTime = millis(); // Momentul inițial pentru blink
  unsigned long blinkInterval = 750; // Interval de 750ms pentru blink

  // Secvență de blink pentru LED-uri galbene și roșii (4 trepte)
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 2; i++) {
      // Aprinde LED-ul corespunzător
      digitalWrite(YELLOW1_PIN - j, HIGH);
      
      // Așteaptă intervalul specificat cu verificare pentru butonul Stop
      while (millis() - lastBlinkTime < blinkInterval) {
        checkStopButton(); 
        if (stopButtonHeld) return; // Oprește dacă butonul Stop este apăsat
      }

      lastBlinkTime = millis(); // Actualizează timpul de blink
      digitalWrite(YELLOW1_PIN - j, LOW); // Stinge LED-ul

      // Așteaptă din nou intervalul de blink cu verificare pentru butonul Stop
      while (millis() - lastBlinkTime < blinkInterval) {
        checkStopButton(); 
        if (stopButtonHeld) return; // Oprește dacă butonul Stop este apăsat
      }

      lastBlinkTime = millis(); // Actualizează timpul de blink
      digitalWrite(YELLOW1_PIN - j, HIGH); // Reaprinde LED-ul
    }
  }

  // Oprește toate LED-urile galbene și roșii
  digitalWrite(YELLOW1_PIN, LOW);
  digitalWrite(YELLOW2_PIN, LOW);
  digitalWrite(RED1_PIN, LOW);
  digitalWrite(RED2_PIN, LOW);

  // Secvență finală de blink pentru toate LED-urile galbene și roșii
  for (int i = 0; i < 3; i++) {
    while (millis() - lastBlinkTime < blinkInterval) {
      checkStopButton(); 
      if (stopButtonHeld) return; // Oprește dacă butonul Stop este apăsat
    }

    lastBlinkTime = millis(); // Actualizează timpul de blink
    digitalWrite(YELLOW1_PIN, HIGH); // Aprinde toate LED-urile
    digitalWrite(YELLOW2_PIN, HIGH);
    digitalWrite(RED1_PIN, HIGH);
    digitalWrite(RED2_PIN, HIGH);

    while (millis() - lastBlinkTime < blinkInterval) {
      checkStopButton(); 
      if (stopButtonHeld) return; // Oprește dacă butonul Stop este apăsat
    }

    lastBlinkTime = millis(); // Actualizează timpul de blink
    digitalWrite(YELLOW1_PIN, LOW); // Stinge toate LED-urile
    digitalWrite(YELLOW2_PIN, LOW);
    digitalWrite(RED1_PIN, LOW);
    digitalWrite(RED2_PIN, LOW);
  }

  // Revine la starea de standby: LED-ul RGB revine la verde
  analogWrite(RED_RGB_PIN, 0);    // LED roșu oprit
  analogWrite(GREEN_RGB_PIN, 255); // LED verde aprins
  analogWrite(BLUE_RGB_PIN, 0);   // LED albastru oprit
}

void checkStopButton() {
  // Citirea stării butonului Stop
  int stopReading = digitalRead(STOP_PIN);

  // Dacă butonul Stop este apăsat (LOW)
  if (stopReading == LOW) {
    if (stopPressStartTime == 0) {
      // Înregistrează timpul când butonul a fost apăsat
      stopPressStartTime = millis();
    } else {
      // Dacă butonul a fost ținut apăsat mai mult de 1 secundă
      if (millis() - stopPressStartTime >= 1000) {
        stopButtonHeld = true; // Setează că butonul a fost ținut apăsat
        stopCharging(); // Oprește încărcarea
      }
    }
  } else {
    // Dacă butonul este eliberat, resetează valorile
    stopPressStartTime = 0;
    stopButtonHeld = false;
  }
}

void stopCharging() {
  charging = false; // Setează starea de încărcare inactivă

  // Oprește toate LED-urile galbene și roșii
  digitalWrite(YELLOW1_PIN, LOW);
  digitalWrite(YELLOW2_PIN, LOW);
  digitalWrite(RED1_PIN, LOW);
  digitalWrite(RED2_PIN, LOW);

  // Secvență de blink pentru a indica oprirea încărcării
  for (int i = 0; i < 3; i++) {
    delay(750); // Așteaptă 750ms
    digitalWrite(YELLOW1_PIN, HIGH); // Aprinde toate LED-urile
    digitalWrite(YELLOW2_PIN, HIGH);
    digitalWrite(RED1_PIN, HIGH);
    digitalWrite(RED2_PIN, HIGH);
    delay(750); // Așteaptă 750ms
    digitalWrite(YELLOW1_PIN, LOW); // Stinge toate LED-urile
    digitalWrite(YELLOW2_PIN, LOW);
    digitalWrite(RED1_PIN, LOW);
    digitalWrite(RED2_PIN, LOW);
  }

  // Revine la starea de standby: LED-ul RGB revine la verde
  analogWrite(RED_RGB_PIN, 0);    // LED roșu oprit
  analogWrite(GREEN_RGB_PIN, 255); // LED
  analogWrite(BLUE_RGB_PIN, 0);   
}