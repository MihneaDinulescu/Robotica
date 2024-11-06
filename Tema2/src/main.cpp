#include <Arduino.h>

// Definirea pinii LED RGB și a butoanelor de start și dificultate
#define GREEN_RGB_PIN 5
#define BLUE_RGB_PIN 4
#define RED_RGB_PIN 6
#define START_PIN 2
#define DIFFICULTY_PIN 3

// Lista de cuvinte pentru joc, cu un total de 20 de cuvinte
String words[] = {
    "copac", "telefon", "zambet", "computer", "fruct",
    "casa", "pictura", "soare", "natura", "culoare",
    "cer", "joc", "masina", "carte", "muzica",
    "prieten", "floare", "apa", "stea", "luna"
};

// Vector pentru a monitoriza dacă un cuvânt a fost deja folosit în timpul rundei curente
int frequency[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Variabile globale pentru gestionarea logicii jocului
int userTime = 0;                       // Timpul total petrecut în runda curentă
int checkInterval = 0;                  // Interval de verificare pentru timpul de runda
int additionalTime = 0;                 // Adaos de timp pentru verificările frecvenței
bool isRoundActive = false;             // Starea jocului (dacă runda este activă)
bool isCountdownActive = false;         // Starea countdown-ului
int difficulty = 0;                     // Nivelul de dificultate (0=Easy, 1=Medium, 2=Hard)
unsigned long lastDebounceTimeStart = 0;
unsigned long lastDebounceTimeDifficulty = 0;
unsigned long debounceDelay = 50;       // Intervalul de debounce pentru butoane
bool lastStartButtonState = HIGH;       // Ultima stare a butonului de start
bool lastDifficultyButtonState = HIGH;  // Ultima stare a butonului de dificultate
const int roundDuration = 30000;        // Durata unei runde (30 secunde)
int correctWordsCount = 0;              // Numarul de cuvinte corecte introduse
const unsigned int easyInterval = 3000; // Interval pentru dificultate ușoară (3 secunde)
const unsigned int mediumInterval = 2000; // Interval pentru dificultate medie (2 secunde)
const unsigned int hardInterval = 1000; // Interval pentru dificultate dificilă (1 secundă)
unsigned int currentInterval = easyInterval; // Intervalul curent pentru dificultate
const char* difficulties[] = {"Easy", "Medium", "Hard"};
bool difficultyButtonPressed = false;   // Flag pentru a detecta apăsarea butonului de dificultate
int pointsCount = 0;                    // Puncte obținute în runda curentă

// Prototipuri de funcții pentru a fi utilizate în cod
void setLEDColor(int red, int green, int blue);
void handleDifficultyButton();
void stopRound();
void executeRound();
void countdown();
int generateWord();

// Setează culoarea LED-ului RGB
void setLEDColor(int red, int green, int blue) {
  analogWrite(RED_RGB_PIN, red);
  analogWrite(GREEN_RGB_PIN, green);
  analogWrite(BLUE_RGB_PIN, blue);
}

void setup() {
  // Configurarea pinilor și LED-ului RGB
  pinMode(START_PIN, INPUT_PULLUP);
  pinMode(DIFFICULTY_PIN, INPUT_PULLUP);
  pinMode(RED_RGB_PIN, OUTPUT);
  pinMode(GREEN_RGB_PIN, OUTPUT);
  pinMode(BLUE_RGB_PIN, OUTPUT);
  setLEDColor(255, 255, 255);           // Culoare albă inițială pentru LED
  Serial.begin(9600);
}

void loop() {
  // Citire starea butonului de dificultate
  bool reading = digitalRead(DIFFICULTY_PIN);
  if (reading != lastDifficultyButtonState) {
    lastDebounceTimeDifficulty = millis();
  }
  if ((millis() - lastDebounceTimeDifficulty) > debounceDelay) {
    // Dacă butonul a fost apăsat și nu este în runda sau countdown
    if (reading == LOW && !isRoundActive && !difficultyButtonPressed) {
      handleDifficultyButton();
      difficultyButtonPressed = true;
    }
  }
  if (reading == HIGH) {
    difficultyButtonPressed = false;
  }
  lastDifficultyButtonState = reading;

  // Citire starea butonului de start
  bool startReading = digitalRead(START_PIN);
  if (startReading != lastStartButtonState) {
    lastDebounceTimeStart = millis();
  }
  if ((millis() - lastDebounceTimeStart) > debounceDelay) {
    // Pornire runda dacă butonul de start este apăsat și nu suntem deja într-o rundă
    if (startReading == LOW) {
      if (!isRoundActive && !isCountdownActive) {
        countdown();
        executeRound();
      }
    }
  }
  lastStartButtonState = startReading;
}

// Funcție pentru a genera un cuvânt aleatoriu care nu a fost utilizat anterior
int generateWord() {
  int randomIndex;
  unsigned long startTime = millis();
  do {
    randomIndex = random(0, 20);
  } while (frequency[randomIndex] == 1);
  additionalTime += (millis() - startTime);
  frequency[randomIndex] = 1;
  return randomIndex;
}

// Funcție pentru countdown-ul înainte de începerea rundei
void countdown() {
  isCountdownActive = true;
  unsigned long currentMillis = millis();
  for (int i = 3; i > 0; i--) {
    setLEDColor(255, 255, 255);
    Serial.print(i);
    Serial.print("\n");
    while (millis() - currentMillis < 500) {}   // Așteaptă 500 ms
    setLEDColor(0, 0, 0);
    currentMillis = millis();
    while (millis() - currentMillis < 500) {}   // Așteaptă 500 ms
    currentMillis = millis();
    setLEDColor(255, 255, 255);
  }
  isCountdownActive = false;
}

// Funcție principală a jocului care gestionează introducerea cuvintelor și scorul
void executeRound() {
  Serial.println("Round started!");
  setLEDColor(0, 255, 0);
  isRoundActive = true;
  userTime = 0;
  pointsCount = 0;

  while (userTime < roundDuration + additionalTime) {
    setLEDColor(0,255,0);
    int wordIndex = generateWord();
    String currentWord = words[wordIndex];
    unsigned long wordStartTime = millis();
    String userInput = "";
    Serial.print("Target word: ");
    Serial.println(currentWord);
    checkInterval = 0;  
    while (millis() - wordStartTime < currentInterval) {
      checkInterval = millis() - wordStartTime;
      if (userTime + checkInterval > roundDuration + additionalTime) {
        stopRound();
        return;
      }

      if (Serial.available() > 0) {
        char inputChar = Serial.read();
        if (inputChar == '\b') {
          if (userInput.length() > 0) {
            userInput.remove(userInput.length() - 1);
          }
        } else {
          userInput += inputChar;
        }

        if (currentWord.startsWith(userInput)) {
          setLEDColor(0, 255, 0);
        } else {
          setLEDColor(255, 0, 0);
        }

        if (userInput == currentWord) {
          Serial.println("Correct word!");
          pointsCount++;
          break;
        }
      }
    }
    userTime += currentInterval;
  }
  stopRound();
}

// Funcție pentru a opri runda și a afișa scorul
void stopRound() {
  Serial.println("Round stopped!");
  Serial.print("You typed ");
  Serial.print(pointsCount);
  Serial.println(" words correctly!");
  isRoundActive = false;
  setLEDColor(255, 255, 255);
  for (int i = 0; i < 20; i++) {
    frequency[i] = 0;
  }
}

// Funcție pentru a gestiona apăsarea butonului de dificultate și setarea intervalului corespunzător
void handleDifficultyButton() {
  if (!isRoundActive && !isCountdownActive) {
    difficulty = (difficulty + 1) % 3;
    switch (difficulty) {
      case 0: currentInterval = easyInterval; break;
      case 1: currentInterval = mediumInterval; break;
      case 2: currentInterval = hardInterval; break;
    }
    Serial.print(difficulties[difficulty]);
    Serial.println(" mode on!");
  }
}
