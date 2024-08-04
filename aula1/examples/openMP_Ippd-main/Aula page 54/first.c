#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10

int main() {
int sum = 0;
int a[N], b[N];

omp_set_num_threads(4);
// Initialize arrays a and b
for (int i = 0; i < N; i++) {
a[i] = i + 1; // Just an example initialization
b[i] = 2 * i; // Just an example initialization
}

#pragma omp parallel for reduction(+:sum)
for (int i = 0; i < N; i++) {
printf("Thread %d is processing iteration %d - value: %d\n", omp_get_thread_num(), i, a[i] * b[i]);
sum += a[i] * b[i];
}

printf("The sum is: %d\n", sum);

return 1;
}