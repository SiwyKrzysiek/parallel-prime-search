# Równoległe wyszukiwanie liczb pierwszych

Wykorzystanie standardu OpenMP w celu wydajnego poszukiwania wszystkich liczb pierwszych w danym zakresie.

## Działanie programu

Program stosuje podejście złożone z 2 etapów do wyznaczenia liczb pierwszych z przedziału [1…n].
Pierwszy etap polega na zastosowaniu równoległego sita Eratostenesa do wyznaczenia liczb pierwszych z przedziału S = [1…⌊√n⌋].

Następnie korzystając z zależności, że jeżeli liczba ma dzielniki większy od 1 to będzie on mniejszy niż jej pierwiastek, dla każdej liczby od ⌊√n⌋ do n sprawdzane jest czy w przedziale S znajduje się jej dzielnik.
Jeśli taki dzielnik nie zostanie znaleziony to liczba jest liczbą pierwszą.

## Przydatne materiały

- [Jak kompilować programy z OpenMP na OS X](https://iscinumpy.gitlab.io/post/omp-on-high-sierra/)
