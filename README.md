# Równoległe wyszukiwanie liczb pierwszych

<!-- Link do repozytorium: https://github.com/SiwyKrzysiek/parallel-prime-search -->

Wykorzystanie standardu OpenMP w celu wydajnego poszukiwania wszystkich liczb pierwszych w danym zakresie.

## Spis treści

- [Równoległe wyszukiwanie liczb pierwszych](#równoległe-wyszukiwanie-liczb-pierwszych)
  - [Spis treści](#spis-treści)
  - [Działanie programu](#działanie-programu)
    - [Równoległe sito Eratostenesa](#równoległe-sito-eratostenesa)
    - [Równoległe sprawdzenie pierwszości kolejnych liczb](#równoległe-sprawdzenie-pierwszości-kolejnych-liczb)
  - [Uruchomienie programu](#uruchomienie-programu)
  - [Sprawdzenie poprawności działania](#sprawdzenie-poprawności-działania)
  - [Zadania](#zadania)
    - [Zadanie 1](#zadanie-1)
    - [Zadanie 2](#zadanie-2)
      - [Wykonanie sekwencyjne](#wykonanie-sekwencyjne)
      - [Wykonanie równoległe](#wykonanie-równoległe)
    - [Zadanie 3](#zadanie-3)
      - [Przyśpieszenie względem programu iteracyjnego](#przyśpieszenie-względem-programu-iteracyjnego)
      - [Efektywność wykorzystania wątków](#efektywnośćwykorzystania-wątków)
  - [Przydatne materiały](#przydatne-materiały)

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

Do sprawdzenia poprawności wyników został napisany prosty skrypt [testPrimes.py](testPrimes.py). Zakłada on, że wynik głównego programu został zapisany w pliku.

## Zadania

Zadania do zrealizowania w celu porównania algorytmu równoległego z sekwencyjnym.

### Zadanie 1

Proszę zrówoleglić wykonanie programu z wykorzystaniem OpenMP (na początku zastanówcie się Państwo które sekcje programu można zrównoleglić, co powinno zostać wykonane sekwencyjnie, gdzie powinna zostać użyta sekcja krytyczna)

Kod programu znajduje się w pliku [main.c](./main.c).
Opis algorytmu oraz sekcji krytycznych znajduje się w akapicie [Działanie programu](#Działanie-programu).

### Zadanie 2

Proszę obliczyć czas wykonania programu sekwencyjnego i zrównoleglonengo (dla l. wątków 2-8) dla rozmiaru zadania n=10⁵, n=10⁶, n=10⁷.

Pomiary zostały przeprowadzone na 4 rdzeniowym procesorze.

#### Wykonanie sekwencyjne

| n   | czas \[µs\] |
| --- | ----------- |
| 10⁵ | 7988        |
| 10⁶ | 127219      |
| 10⁷ | 2529676     |

#### Wykonanie równoległe

Czas wykonania jest przedstawiony w µs.

| n \\ wątki | 2      | 3      | 4      | 5      | 6      | 7      | 8      |
| ---------- | ------ | ------ | ------ | ------ | ------ | ------ | ------ |
| 10⁵        | 4126   | 4182   | 5376   | 5504   | 6449   | 6975   | 6213   |
| 10⁶        | 37487  | 34090  | 31521  | 33520  | 35476  | 46280  | 49261  |
| 10⁷        | 495117 | 367541 | 320260 | 317706 | 302994 | 311747 | 331973 |

Z pomiarów jasno wynika, że dla tego problemu warto zastosować algorytm równoległy.
Co ciekawe czas wykonania nie jest najkrótszy dla 8 wątków. Może to wynikać z 4 rdzeniowej architektury testowanego procesora.

### Zadanie 3

Dla zrównoleglonego programu dla rozmiaru zadania n=10⁵, n=10⁶, n=10⁷ (dla l. wątków 2-8) proszę obliczyć przyspieszenie programu i jego efektywność (E(p,n) = S(p,n)/p).

#### Przyśpieszenie względem programu iteracyjnego

Przyśpieszenie jest liczone jako stosunek czasu wykonania sekwencyjnego do czasu wykonania równoległego.

| n \\ wątki | 2    | 3    | 4    | 5    | 6    | 7    | 8    |
| ---------- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| 10⁵        | 194% | 191% | 149% | 145% | 124% | 115% | 129% |
| 10⁶        | 339% | 373% | 404% | 380% | 359% | 275% | 258% |
| 10⁷        | 511% | 688% | 790% | 796% | 835% | 811% | 762% |

Przyśpieszenie pozwala określić jaką liczbę wątków należy przeznaczyć by otrzymać wynik w możliwie krótkim czasie.

#### Efektywność wykorzystania wątków

Efektywność jest liczona jako stosunek przyśpieszenia do liczby wątków.

| n \\ wątki | 2    | 3    | 4    | 5    | 6    | 7    | 8   |
| ---------- | ---- | ---- | ---- | ---- | ---- | ---- | --- |
| 10⁵        | 97%  | 64%  | 37%  | 29%  | 21%  | 16%  | 16% |
| 10⁶        | 170% | 124% | 101% | 76%  | 60%  | 39%  | 32% |
| 10⁷        | 255% | 229% | 197% | 159% | 139% | 116% | 95% |

Efektywność pozwala określić jaką liczbę wątków należy przydzielić by uzyskać optymalne wykorzystanie procesora.

## Przydatne materiały

- [Jak kompilować programy z OpenMP na OS X](https://iscinumpy.gitlab.io/post/omp-on-high-sierra/)
- [Prosta implementacja wektora](https://github.com/eteran/c-vector)
- [Dokumentacja OpenMP](https://docs.microsoft.com/en-us/cpp/parallel/openmp/2-directives)
