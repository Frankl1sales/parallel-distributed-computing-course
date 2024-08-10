#include <stdio.h>        // Inclui a biblioteca padrão de entrada/saída para operações de impressão e formatação.
#include <stdlib.h>       // Inclui a biblioteca padrão de utilitários para operações gerais, como manipulação de argumentos e alocação de memória.
#include "transcoder.h"   // Inclui o cabeçalho que define a função `transcode_video`.

void transcode_video(const char* input_file, const char* resolutions[], size_t num_resolutions, const char* output_file_prefix, int thread_id) {
    // Itera sobre cada resolução fornecida.
    for (size_t i = 0; i < num_resolutions; ++i) {
        // Cria o nome do arquivo de saída formatado com o prefixo e a resolução.
        char output_file[256];
        snprintf(output_file, sizeof(output_file), "%s_%s.mp4", output_file_prefix, resolutions[i]);

        // Monta o comando FFmpeg para transcodificar o vídeo.
        char command[1024];
        snprintf(command, sizeof(command), "ffmpeg -i %s -vf scale=%s -c:v libx264 -preset slow -crf 22 -c:a aac -b:a 192k %s", input_file, resolutions[i], output_file);

        // Adiciona uma mensagem de log para indicar o comando executado e a thread que o está executando.
        FILE *logfile = fopen("build_log.txt", "a");
        if (logfile) {
            fprintf(logfile, "Thread %d executando comando: %s\n", thread_id, command);
            fclose(logfile);
        }

        // Executa o comando FFmpeg.
        int ret = system(command);
        if (ret != 0) {
            fprintf(stderr, "Command failed with return code %d: %s\n", ret, command);
        }
    }
}

