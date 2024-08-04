// Comando para gerar o executável:
// gcc -fopenmp trabalho_openMP_Franklin.c -lm -o trabalho

// explicação sobre a flag -lm:
// math.h is not a part of the standard C library, so you have to link to it! 
// link da explicação: https://stackoverflow.com/questions/44175151/what-is-the-meaning-of-lm-in-gcc

// Comando para executar
// ./trabalho

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N 100

int main() {
    int sum = 0;
    int zero_count = 0;
    int a[N][N];
    float media = 0;
    float variance_sum = 0;
    float desvio_p = 0;

    omp_set_num_threads(4);

    // Inicializa a matriz
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            a[i][j] = rand() % 100; // Gera números aleatórios
        }
    }

    // Calcula a soma e conta zeros
    // A cláusula reduction foi escolhida, pois cria uma instância
    // local da variável para cada thread, e cada thread realiza
    // a operação de soma sobre sua própria instância.
    // Ao final, os resultados de todas as threads são combinados.
    // Isso evita condições de corrida.
    // explicação página 53 do PDF: IPPD-Aula8-OpenMP - IPPD-Aula8-OpenMP

    #pragma omp parallel for reduction(+:sum, zero_count)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("Thread %d está processando a iteração %d-%d - valor: %d\n", omp_get_thread_num(), i, j, a[i][j]);
            sum += a[i][j];
            if (a[i][j] == 0) {
                zero_count++;
            }
        }
    }

    // Calcula a média
    media = (float)sum / (N * N);
    printf("A Media:: %f\n", media);

    // Calcula a soma das variâncias
    // variância é o (valor menos a média)tudo elevado a 2
    // a variância não foi aplicada antes, pois havia
    // a necessidade de ter o valor da media

    #pragma omp parallel for reduction(+:variance_sum)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            variance_sum += (a[i][j] - media)*(a[i][j] - media);
        }
    }

    // Calcula o desvio padrão
    desvio_p = sqrt(variance_sum / (N * N));
    printf("O desvio padrao: %f\n", desvio_p);

    // Calcula a esparsidade
    float esparsividade = (float)zero_count / (N * N);
    printf("A matriz tem %d zeros de %d elementos.\n", zero_count, N * N);
    printf("esparsividade: %f\n", esparsividade);

    // se houver mais do que 50% 0's ela esparsa
    if (esparsividade > 0.5) {
        printf("A matriz é esparsa.\n");
    } else {
        printf("A matriz não é esparsa.\n");
    }

    return 0;
}
