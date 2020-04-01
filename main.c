#include <stdio.h>
#include <omp.h>
#include <stdbool.h>
#include <string.h>

int main()
{
    const int n = 30;

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
    for (int i = 0; i < n; i++)
    {
        if (sieve[i])
        {
            printf("%d\n", i+1);
        }
    }


    return 0;
}
