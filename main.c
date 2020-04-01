#include <stdio.h>
#include <omp.h>
#include <stdbool.h>
#include <string.h>
#include "libraries/c-vector/cvector.h"

#define CVECTOR_LOGARITHMIC_GROWTH

// Find all primes from 1 to lastNumber inclusive
// Returns cvector with found primes
int* eratosthenesSieve(const int lastNumber);

int main()
{
    int* primes = eratosthenesSieve(30);

    // Print found primes
    printf("\nFound primes:\n");
    for (int i = 0; i < cvector_size(primes); i++)
    {
        printf("%d\n", primes[i]);
    }

    cvector_free(primes);
    return 0;
}

int* eratosthenesSieve(const int lastNumber)
{
    const int n = lastNumber;
    cvector_vector_type(int) primes = NULL;

    // Number is potenitial prime if it is true
    // Numer is index+1
    bool sieve[n];
    memset(sieve, true, sizeof(sieve));

    // One is not prime
    sieve[0] = false;

    int startingPoint = 1;
    #pragma omp parallel shared(sieve, startingPoint)
    {
        while (startingPoint < n)
        {
            // Find first prime and take it
            int myPrime = 0;
            #pragma omp critical
            {
                #pragma omp flush
                if (startingPoint < n) // Safty check
                {
                    for (; startingPoint < n; startingPoint++)
                    {
                        if (sieve[startingPoint]) // It's prime
                        {
                            myPrime = ++startingPoint;
                            cvector_push_back(primes, myPrime);
#ifdef DEBUG
                            int id = omp_get_thread_num();
                            printf("Thead %d found prime: %d\n", id, myPrime);
#endif
                            break;
                        }

                    }
                }

            }

            if (myPrime == 0) // No prime found
                break;

            // Cross out multiples of selected prime
            for (int i = myPrime*2-1; i < n; i+=myPrime)
                sieve[i] = false;
        }
    }

    return primes;
}
