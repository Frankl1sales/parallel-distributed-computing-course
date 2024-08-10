#include <stdio.h>        // Inclui a biblioteca padrão de entrada/saída para operações de impressão e formatação.
#include <stdlib.h>       // Inclui a biblioteca padrão de utilitários para operações gerais, como manipulação de argumentos e alocação de memória.
#include <omp.h>          // Inclui a biblioteca OpenMP para programação paralela.
#include "transcoder.h"   // Inclui o cabeçalho que pode definir funções e tipos utilizados no código, como a função `transcode_video`.

#define MAX_RESOLUTIONS 10  // Define o número máximo de resoluções que podem ser processadas.
#define MAX_THREADS 4      // Define o número máximo de threads que serão usadas para a execução paralela.

int main(int argc, char* argv[]) {
    // Verifica se o número mínimo de argumentos foi fornecido (pelo menos 2 resoluções e 1 arquivo de entrada).
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <resolution1> <resolution2> ... <resolutionN> <input_file>\n", argv[0]);
        return 1;  // Retorna 1 para indicar que houve um erro.
    }

    // O último argumento é o arquivo de entrada.
    const char* input_file = argv[argc - 1];
    // O número de resoluções é o total de argumentos menos o arquivo de entrada.
    size_t num_resolutions = argc - 2;

    // Verifica se o número de resoluções não excede o máximo permitido.
    if (num_resolutions > MAX_RESOLUTIONS) {
        fprintf(stderr, "Too many resolutions. Maximum allowed is %d.\n", MAX_RESOLUTIONS);
        return 1;  // Retorna 1 para indicar que houve um erro.
    }

    // Array para armazenar os ponteiros para as resoluções fornecidas.
    const char* resolutions[MAX_RESOLUTIONS];
    // Preenche o array de resoluções com os argumentos fornecidos.
    for (size_t i = 0; i < num_resolutions; ++i) {
        resolutions[i] = argv[i + 1];
    }

    // Define o número máximo de threads para a execução paralela usando OpenMP.
    omp_set_num_threads(MAX_THREADS);

    // Inicia uma região paralela, onde múltiplas threads podem executar o código simultaneamente.
    #pragma omp parallel
    {
        // Obtém o identificador da thread atual.
        int thread_id = omp_get_thread_num();
        // Cada thread executa um loop sobre as resoluções atribuídas a ela.
        #pragma omp for
        for (size_t i = 0; i < num_resolutions; ++i) {
            // Imprime uma mensagem indicando qual thread está processando qual resolução.
            printf("Thread %d processando resolução %zu\n", thread_id, i);

            // Cria o prefixo do nome do arquivo de saída baseado no índice da resolução.
            char output_file_prefix[64];
            snprintf(output_file_prefix, sizeof(output_file_prefix), "output_%zu", i + 1);

            // Adiciona uma mensagem de log para indicar o início do processamento da resolução por uma thread específica.
            #pragma omp critical
            {
                FILE *logfile = fopen("build_log.txt", "a");
                if (logfile) {
                    fprintf(logfile, "Thread %d iniciando processamento da resolução %zu\n", thread_id, i);
                    fclose(logfile);
                }
            }

            // Chama a função `transcode_video` para transcodificar o vídeo para a resolução atual.
            transcode_video(input_file, &resolutions[i], 1, output_file_prefix, thread_id);
        }
    }

    return 0;  // Retorna 0 para indicar que o programa foi executado com sucesso.
}
