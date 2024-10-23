# Sistem de Control al Procesului de Încărcare cu Semnalizare LED

Acest proiect implementează un sistem de control pentru un proces de încărcare, cu semnalizare prin LED-uri. Sistemul este controlat de două butoane: unul pentru pornirea procesului (Start) și unul pentru oprire (Stop). LED-urile indică stările procesului de încărcare printr-o secvență luminoasă.

## Funcționalitate

Sistemul este alcătuit din mai multe LED-uri și două butoane care controlează procesul:

- **Start (START_PIN)**: Inițiază procesul de încărcare și aprinde LED-urile într-o secvență definită.
- **Stop (STOP_PIN)**: Oprește procesul de încărcare dacă este apăsat și ținut apăsat timp de 1 secundă.

## Componente Utilizate

- **LED-uri**:
  - **YELLOW1_PIN (pin 10)** și **YELLOW2_PIN (pin 9)**: LED-uri galbene.
  - **RED1_PIN (pin 8)** și **RED2_PIN (pin 7)**: LED-uri roșii.
  - **RED_RGB_PIN (pin 6)**, **GREEN_RGB_PIN (pin 5)** și **BLUE_RGB_PIN (pin 4)**: LED RGB cu trei canale (roșu, verde, albastru) care indică starea sistemului.

- **Butoane**:
  - **START_PIN (pin 2)**: Buton pentru inițierea procesului de încărcare (configurat ca `INPUT_PULLUP`).
  - **STOP_PIN (pin 3)**: Buton pentru oprirea procesului de încărcare (configurat ca `INPUT_PULLUP`).

## Variabile Cheie

- **`buttonState`** și **`lastButtonState`**: Păstrează starea curentă și anterioară a butonului Start.
- **`lastDebounceTime`**: Momentul ultimei modificări a stării butonului Start (utilizat pentru debouncing).
- **`debounceDelay`**: Întârzierea pentru debouncing (50 ms).
- **`charging`**: Variabilă booleană care indică dacă procesul de încărcare este activ.
- **`stopPressStartTime`**: Momentul în care butonul Stop este apăsat.
- **`stopButtonHeld`**: Indică dacă butonul Stop a fost ținut apăsat suficient pentru a opri procesul.

## Descrierea Funcțiilor

### `setup()`

- Configurează pinii pentru butoane ca `INPUT` și pentru LED-uri ca `OUTPUT`.
- Inițializează LED-ul RGB în starea verde (standby, fără încărcare activă).

### `loop()`

- Verifică starea butonului Start, implementând debouncing. Dacă butonul Start este apăsat, începe procesul de încărcare prin apelul funcției `startCharging()`.
- Verifică în fiecare iterație dacă s-a schimbat starea butonului și dacă a trecut timpul de debounce.

### `startCharging()`

- Inițiază procesul de încărcare, aprinzând LED-ul RGB pe roșu (indicând încărcare activă).
- LED-urile galbene și roșii clipesc într-o secvență specifică. Procesul de încărcare este întrerupt dacă se detectează apăsarea și menținerea butonului Stop prin funcția `checkStopButton()`.

### `checkStopButton()`

- Verifică dacă butonul Stop este apăsat și menținut timp de 1 secundă. Dacă această condiție este îndeplinită, se apelează funcția `stopCharging()`.

### `stopCharging()`

- Oprește procesul de încărcare. LED-urile execută o secvență de clipiri pentru a indica oprirea și LED-ul RGB revine la culoarea verde (standby).

## Mod de Funcționare

1. La pornirea sistemului, LED-ul RGB este verde, indicând că nu se desfășoară nicio încărcare.
2. Dacă butonul Start este apăsat:
   - LED-ul RGB devine roșu, indicând că încărcarea a început.
   - LED-urile galbene și roșii clipesc într-o secvență definită.
3. În timpul procesului de încărcare:
   - Dacă butonul Stop este apăsat și ținut apăsat timp de 1 secundă, procesul se oprește.
   - LED-urile galbene și roșii clipesc simultan de trei ori pentru a indica oprirea.
   - LED-ul RGB revine la culoarea verde (standby).

## Secvențe de Blink

- **În timpul încărcării**: LED-urile galbene și roșii se aprind și se sting alternativ la un interval de 750 ms.
- **La oprirea încărcării**: Toate LED-urile clipesc de trei ori simultan pentru a semnaliza oprirea procesului.

## Comportament de Debounce

Debouncing-ul este implementat pentru butoane pentru a preveni înregistrarea multiplă a unei singure apăsări din cauza zgomotului electric. Codul verifică dacă butonul a fost apăsat suficient timp pentru a înregistra o comandă validă.