#ifndef TRANSCODER_H
#define TRANSCODER_H

#include <stdlib.h>  // Inclui a biblioteca padrão para utilitários gerais, como `size_t`.

/**
 * @brief Transcodifica um vídeo para diferentes resoluções.
 *
 * Esta função usa o FFmpeg para processar o vídeo de entrada e gerar
 * arquivos de saída com as resoluções especificadas. O prefixo do arquivo
 * de saída é adicionado a cada resolução para criar nomes distintos para
 * os arquivos resultantes.
 *
 * @param input_file O caminho para o arquivo de vídeo de entrada.
 * @param resolutions Um array de strings contendo as resoluções desejadas
 *                    no formato "LARGURAxALTURA", como "1920x1080".
 * @param num_resolutions O número de resoluções especificadas no array.
 * @param output_file_prefix O prefixo que será adicionado ao nome de cada
 *                           arquivo de saída. A resolução será concatenada
 *                           com esse prefixo para formar o nome completo do
 *                           arquivo de saída.
 */
void transcode_video(const char* input_file, const char* resolutions[], size_t num_resolutions, const char* output_file_prefix, int thread_id);

#endif // TRANSCODER_H

