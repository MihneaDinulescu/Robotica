# TypeRacer Game

Acest proiect implementează un joc asemănător cu TypeRacer, unde utilizatorul trebuie să tasteze corect cuvintele afișate în terminal într-un interval de timp stabilit în funcție de dificultatea aleasă. Sistemul include semnalizare LED pentru feedback în timp real, două butoane pentru controlul jocului și selectarea dificultății, și un cronometru pentru limitarea timpului de joc la 30 de secunde.

## Funcționalitate

Jocul are trei stări principale și funcționează după cum urmează:

1. **Modul de repaus**: LED-ul RGB este alb, indicând că jocul nu este activ.
2. **Numărătoarea inversă**: LED-ul RGB clipește de 3 ori înainte de începerea rundei, indicând numărătoarea inversă.
3. **Runda activă**: LED-ul este verde când textul introdus este corect și devine roșu în caz de greșeală. Runda durează 30 de secunde, iar la final, scorul este afișat în terminal.

### Componente Utilizate

- **Arduino UNO** 
- **1x LED RGB**: Folosit pentru a indica starea jocului.
- **2x Butoane**: Un buton pentru controlul start/stop al rundei și un altul pentru selectarea dificultății.
- **5x Rezistoare**: (3x 330 ohm, 2x 1000 ohm)
- **Breadboard și fire de legătură**

## Variabile Cheie

- **`words[]`**: Lista de cuvinte afișate în terminal pe care utilizatorul trebuie să le tasteze.
- **`frequency[]`**: Folosit pentru a ține evidența cuvintelor afișate într-o rundă, evitând repetarea.
- **`difficulty`**: Nivelul de dificultate selectat (0 - Easy, 1 - Medium, 2 - Hard).
- **`currentInterval`**: Intervalul de timp la care un nou cuvânt este afișat, în funcție de dificultate.
- **`pointsCount`**: Numărul de cuvinte scrise corect într-o rundă.

## Descrierea Funcțiilor

### `setup()`
Configurează pinii pentru LED RGB și butoane, setează LED-ul în starea de repaus (alb) și inițializează comunicația serială.

### `loop()`
Citește starea butoanelor pentru controlul dificultății și inițierea rundei. Dacă butonul de start este apăsat, pornește numărătoarea inversă și apoi inițiază runda.

### `setLEDColor(int red, int green, int blue)`
Controlează culoarea LED-ului RGB, semnalizând diferite stări (repaus, corect, greșit, numărătoare inversă).

### `handleDifficultyButton()`
Permite ciclarea între nivelele de dificultate: **Easy**, **Medium** și **Hard**. La schimbarea dificultății, nivelul curent este afișat în terminal.

### `countdown()`
Inițiază o numărătoare inversă de 3 secunde înainte de începutul rundei, cu LED-ul RGB clipind la fiecare secundă.

### `executeRound()`
Pornește runda și afișează cuvintele în terminal. LED-ul devine verde pentru răspunsurile corecte și roșu pentru greșeli. După 30 de secunde, runda se încheie și scorul este afișat.

### `stopRound()`
Oprește runda și afișează scorul final. Resetează LED-ul la starea de repaus și resetează variabilele pentru o nouă rundă.

### `generateWord()`
Selectează un cuvânt aleatoriu din lista de cuvinte care nu a fost afișat deja în runda curentă.

## Mod de Funcționare

1. **Starea de repaus**: LED-ul RGB este alb și jocul este oprit. Utilizatorul poate selecta dificultatea folosind butonul de dificultate. În terminal, este afișat nivelul curent ("Easy", "Medium", "Hard").

2. **Inițierea jocului**: La apăsarea butonului de start, LED-ul clipește timp de 3 secunde, indicând numărătoarea inversă. În terminal, este afișată secvența de numărătoare: 3, 2, 1.

3. **Runda activă**:
   - LED-ul este verde pentru răspunsurile corecte și roșu pentru greșeli.
   - După fiecare cuvânt scris corect, un nou cuvânt apare în terminal.
   - Dacă timpul pentru un cuvânt expiră, un alt cuvânt este afișat automat.

4. **Finalizarea rundei**: După 30 de secunde, runda se termină automat și terminalul afișează numărul de cuvinte scrise corect.

## Secvențe de Blink

- **Numărătoare inversă**: LED-ul RGB clipește alb timp de 3 secunde.
- **În timpul jocului**: LED-ul RGB devine verde pentru cuvinte corecte și roșu pentru greșeli.
- **La oprirea jocului**: LED-ul RGB revine la culoarea albă (stare de repaus).

## Comportament de Debounce

Debouncing-ul este implementat pentru butoanele de control pentru a preveni înregistrarea multiplă a unei singure apăsări din cauza zgomotului electric. Fiecare buton este verificat pentru a se asigura că apăsările sunt înregistrate corect.

## Schemă Electrică

(https://wokwi.com/projects/413816665500170241).

## Video Funcționalitate

- [Video](https://www.youtube.com/shorts/LxDtUrere-U)

## Poze

- [Imagini]
(https://drive.google.com/drive/folders/1bgrv2ijq_XERh3xoOmXJaFIQANDYogtp)
