#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int main()
{
    int nthreads, tid;
    
#pragma omp parallel private(nthreads, tid)
    {
        tid = omp_get_thread_num();
        printf("Hello %d\n", tid);
        if (tid == 0) {
            nthreads = omp_get_num_threads();
            
            printf("Number is : %d\n", nthreads);
        }
    }
}