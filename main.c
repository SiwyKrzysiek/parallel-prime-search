#include <stdio.h>
#include <omp.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "libraries/c-vector/cvector.h"

#define CVECTOR_LOGARITHMIC_GROWTH

// Find all primes from 1 to lastNumber inclusive
// Returns cvector with found primes
int* eratosthenesSieve(const int lastNumber);

int main()
{
    const int n = 10000000;

    // Firstly find all primes in subset [1, ⌊√n⌋]
    const int subsetSize = sqrt(n);
    // printf("Subset size: %d\n", subsetSize);
    int* primes = eratosthenesSieve(subsetSize);
    const int subsetPrimesCount = cvector_size(primes);

    int subsetPrimes[subsetPrimesCount]; // Can't use vector because address will change. Maybe should not use it at all
    memcpy(subsetPrimes, cvector_begin(primes), subsetPrimesCount * sizeof(*primes));

    int potentialPrime = 0;
    #pragma omp parallel for shared(subsetSize, primes, subsetPrimesCount) private(potentialPrime)
    for (potentialPrime = subsetSize+1; potentialPrime <= n; potentialPrime++)
    {
        bool isPrime = true;
        for (int i = 0; i < subsetPrimesCount; i++)
        {
            if (potentialPrime % subsetPrimes[i] == 0)
            {
                isPrime = false;
                break;
            }
        }

        if (isPrime)
        {
            #pragma omp critical
            cvector_push_back(primes, potentialPrime);
        }
    }

    // puts("Found primes:");
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
                if (startingPoint < n) // Safety check
                {
                    for (; startingPoint < n; startingPoint++)
                    {
                        if (sieve[startingPoint]) // It's prime
                        {
                            myPrime = ++startingPoint;
                            cvector_push_back(primes, myPrime);
#ifdef DEBUG
                            // int id = omp_get_thread_num();
                            // printf("Thread %d found prime: %d\n", id, myPrime);
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
