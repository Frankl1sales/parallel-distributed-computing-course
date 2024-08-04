#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int fib(int n){
    int r1, r2;
    if (n < 2) return n;
    #pragma omp task shared(r1)
        r2 = fib(n - 2);
    r1 = fib(n - 1);
    #pragma omp taskwait
    return r1 + r2;
}

int main() {
    int r, n=10;
    
    #pragma omp parallel
    {
        #pragma omp single
        r = fib(n);
    }

    printf("- %d -\n", r);
    
    return 0;
}
