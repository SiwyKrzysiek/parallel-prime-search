#include <stdio.h>
#include <omp.h>
#include <stdbool.h>
#include <string.h>
#include "libraries/c-vector/cvector.h"

#define CVECTOR_LOGARITHMIC_GROWTH

int main()
{
    const int n = 30;
    // For storing found primes
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
            int myPrime = 0;

            #pragma omp critical
            {
                #pragma omp flush
                if (startingPoint < n) // Safty check
                {
                    // Find first prime and take it
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
            {
                sieve[i] = false;
            }

        }

    }

    // Print found primes
    printf("\nFound primes:\n");
    for (int i = 0; i < cvector_size(primes); i++)
    {
        printf("%d\n", primes[i]);
    }

    cvector_free(primes);
    return 0;
}
