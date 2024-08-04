#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100  // Aumentar o número de iterações

int main() {
    int sum = 0;
    int a[N], b[N];
    int num_threads = 4; // Configurar o número de threads
    int chunk_size = N / num_threads;

    omp_set_num_threads(num_threads);

    // Inicializa os arrays a e b
    for (int i = 0; i < N; i++) {
        a[i] = i + 1; // Apenas um exemplo de inicialização
        b[i] = 2 * i; // Apenas um exemplo de inicialização
    }

    // Imprime a configuração visual das barras de progresso
    printf("Progresso:\n");
    for (int t = 0; t < num_threads; t++) {
        printf("Thread %d: [", t);
        for (int i = 0; i < chunk_size; i++) {
            printf("=");
        }
        printf("]\n");
    }
    printf("\n");

    // Processamento paralelo com OpenMP
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < N; i++) {
        // Simula processamento
        for (int j = 0; j < 1000000; j++);

        // Atualiza a barra de progresso da thread
        int thread_id = omp_get_thread_num();
        printf("\033[%dA", num_threads - thread_id); // Move o cursor para cima
        printf("\033[%dC", chunk_size + 2); // Move o cursor para a direita
        fflush(stdout); // Limpa o buffer de saída
        printf("*"); // Marca o progresso
        fflush(stdout); // Limpa o buffer de saída
        printf("\033[%dB", num_threads - thread_id); // Move o cursor para baixo
        fflush(stdout); // Limpa o buffer de saída

        // Processamento real
        sum += a[i] * b[i];
    }

    // Finaliza a barra de progresso
    printf("\n");

    // Imprime o resultado final
    printf("A soma é: %d\n", sum);

    return 0;
}
