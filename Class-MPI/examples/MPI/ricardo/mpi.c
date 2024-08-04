#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

#define N 1000 // Tamanho da matriz

// Função para inicializar a matriz com valores aleatórios
void inicializar_matriz(int *matriz, int tamanho)
{
    srand(time(NULL));
    for (int i = 0; i < tamanho * tamanho; i++)
    {
        matriz[i] = (rand() % 20001) - 10000; // Gera valores entre -10000 e 10000
    }
}

// Função para calcular a média e o desvio padrão
void calcular_estatisticas(int *matriz, int tamanho, double *media, double *desvio_padrao)
{
    double soma = 0.0;
    double soma_quadrados = 0.0;

    for (int i = 0; i < tamanho * tamanho; i++)
    {
        soma += matriz[i];
        soma_quadrados += matriz[i] * matriz[i];
    }

    *media = soma / (tamanho * tamanho);
    *desvio_padrao = sqrt((soma_quadrados / (tamanho * tamanho)) - (*media) * (*media));
}

// Função para contar valores e determinar se a matriz é esparsa
void analisar_matriz(int *matriz, int tamanho, int *contagem_negativos, int *menor_valor, int *maior_valor, int *contagem_zeros, int *eh_esparsa)
{
    *contagem_negativos = 0;
    *contagem_zeros = 0;
    *menor_valor = matriz[0];
    *maior_valor = matriz[0];

    for (int i = 0; i < tamanho * tamanho; i++)
    {
        if (matriz[i] < 0)
            (*contagem_negativos)++;
        if (matriz[i] == 0)
            (*contagem_zeros)++;
        if (matriz[i] < *menor_valor)
            *menor_valor = matriz[i];
        if (matriz[i] > *maior_valor)
            *maior_valor = matriz[i];
    }

    // Determina se a matriz é esparsa
    *eh_esparsa = (*contagem_zeros > (tamanho * tamanho / 2)) ? 1 : 0;
}

int main(int argc, char **argv)
{
    int rank, tamanho_comunicador;
    int *matriz = NULL;
    int contagem_negativos_local, contagem_negativos_total;
    int contagem_zeros_local, contagem_zeros_total;
    int menor_valor_local, menor_valor_global;
    int maior_valor_local, maior_valor_global;
    double media, desvio_padrao;
    int eh_esparsa;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &tamanho_comunicador);

    int elementos_por_processo = (N * N) / tamanho_comunicador;
    int *matriz_local = (int *)malloc(elementos_por_processo * sizeof(int));

    if (rank == 0)
    {
        matriz = (int *)malloc(N * N * sizeof(int));
        inicializar_matriz(matriz, N);
    }

    // Distribuição dos dados
    MPI_Scatter(matriz, elementos_por_processo, MPI_INT, matriz_local, elementos_por_processo, MPI_INT, 0, MPI_COMM_WORLD);

    // Cálculo local
    analisar_matriz(matriz_local, N / tamanho_comunicador, &contagem_negativos_local, &menor_valor_local, &maior_valor_local, &contagem_zeros_local, &eh_esparsa);

    // Redução dos valores
    MPI_Reduce(&contagem_negativos_local, &contagem_negativos_total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&contagem_zeros_local, &contagem_zeros_total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&menor_valor_local, &menor_valor_global, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&maior_valor_local, &maior_valor_global, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        calcular_estatisticas(matriz, N, &media, &desvio_padrao);
        printf("Número de elementos negativos: %d\n", contagem_negativos_total);
        printf("Maior valor: %d\n", maior_valor_global);
        printf("Menor valor: %d\n", menor_valor_global);
        printf("Média: %.2f\n", media);
        printf("Desvio padrão: %.2f\n", desvio_padrao);
        printf("Número de zeros: %d\n", contagem_zeros_total);
        printf("A matriz é %sesparsa\n", (eh_esparsa ? "" : "não "));
        free(matriz);
    }

    free(matriz_local);
    MPI_Finalize();
    return 0;
}
