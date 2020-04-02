# Równoległe wyszukiwanie liczb pierwszych

Wykorzystanie standardu OpenMP w celu wydajnego poszukiwania wszystkich liczb pierwszych w danym zakresie.

## Działanie programu

Program stosuje podejście złożone z 2 etapów do wyznaczenia liczb pierwszych z przedziału [1…n].
Pierwszy etap polega na zastosowaniu równoległego sita Eratostenesa do wyznaczenia liczb pierwszych z przedziału S = [1…⌊√n⌋].

Następnie korzystając z zależności, że jeżeli liczba ma dzielniki większy od 1 to będzie on mniejszy niż jej pierwiastek, dla każdej liczby od ⌊√n⌋ do n sprawdzane jest czy w przedziale S znajduje się jej dzielnik.
Jeśli taki dzielnik nie zostanie znaleziony to liczba jest liczbą pierwszą.

### Równoległe sito Eratostenesa

Sekcją krytyczną objęty jest wybór przez dany wątek kolejnej liczby pierwszej. Następnie już poza sekcją krytyczną każdy wątek wykreśla z sita wielokrotności swojej liczby.

Znalezione liczy pierwsze zapisywane są w **wektorze**.
Prawdopodobnie wydajniej byłoby utworzyć tablicę wynikową, ponieważ znalezione liczby muszą być przepisane do tablicy o stałym adresie przed przystąpieniem do drugiego kroku. Jednak bardzo chciałem wypróbować bibliotekę udostępniającą tablicę dynamiczną w C.

### Równoległe sprawdzenie pierwszości kolejnych liczb

Do sprawdzenia pierwszości kolejnych liczb zastosowana jest równoległa pętla for. Wątki sprawdzają kolejne liczby i jeśli są one liczbą pierwszą to w sekcji krytycznej wstawiają je do wektora liczb pierwszych.

Dla zwiększenia wydajności pętla nie jest typu `ordered` co powoduje, że wyniki **nie są posortowane**.  
Bariera na końcu pętli for została usunięta, ponieważ nie ma potrzeby, żeby wątki czekały na siebie.

## Uruchomienie programu

Do kompilacji programu wymagana jest biblioteka _openmp_. Program był testowany na systemie OS X.

Program można zbudować przy pomocy narzędzia makefile pleceniem `make`.

Po zbudowaniu program można uruchomić poleceniem `./findPrimes range_boundary`.  
Gdzie _range\_boundary_ jest największą liczbą, która zostanie uwzględniona w algorytmie (n).

## Sprawdzenie poprawności działania

Do sprawdzenia poprawności wyników został napisany prosty skrypt `testPrimes.py`. Zakłada on, że wynik głównego programu został zapisany w pliku.

## Przydatne materiały

- [Jak kompilować programy z OpenMP na OS X](https://iscinumpy.gitlab.io/post/omp-on-high-sierra/)
