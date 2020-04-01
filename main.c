#include <stdio.h>
#include <omp.h>

int main()
{
    #pragma omp parallel
    {
        int ID = omp_get_thread_num();
        printf("Hello %d\n", ID);
    }
    return 0;
}