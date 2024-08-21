#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>
#include <time.h>
#include <string.h>

#define MAX_LOG_SIZE 1024
#define MAX_COMMAND_SIZE 1024

// Função para obter o tempo atual em formato de string
void get_current_time_str(char* buffer, int buffer_size) {
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", timeinfo);
}

// Função para escrever no arquivo de log com o timestamp e identificador de rank/thread
void log_message(const char* log_filename, int rank, int thread_id, const char* message) {
    char time_str[100];
    get_current_time_str(time_str, sizeof(time_str));

    FILE* log_file = fopen(log_filename, "a");
    if (log_file != NULL) {
        if (thread_id >= 0) {
            // Identificar logs OpenMP
            fprintf(log_file, "[%s] [Rank %d - Thread %d] %s\n", time_str, rank, thread_id, message);
        } else {
            // Identificar logs MPI
            fprintf(log_file, "[%s] [Rank %d] %s\n", time_str, rank, message);
        }
        fclose(log_file);
    } else {
        printf("Erro ao abrir o arquivo de log.\n");
    }
}

// Função para comprimir o vídeo usando FFmpeg
void compress_video_segment(const char* input_filename, const char* output_filename, int quality, int start_time, int duration, const char* log_filename, int rank) {
    char command[MAX_COMMAND_SIZE];
    char log_msg[MAX_LOG_SIZE];
    
    // Monta o comando FFmpeg para compressão com diferentes qualidades e segmentos
    snprintf(command, sizeof(command),
             "ffmpeg -i %s -ss %d -t %d -vcodec libx264 -crf %d %s",
             input_filename, start_time, duration, quality, output_filename);
    
    // Verificar truncamento
    if (snprintf(log_msg, sizeof(log_msg), "Executando comando: %s", command) >= sizeof(log_msg)) {
        snprintf(log_msg, sizeof(log_msg), "Executando comando: (truncado)");
    }

    log_message(log_filename, rank, -1, log_msg);
    
    // Executa o comando
    time_t start_exec = time(NULL); // Tempo de início da execução
    system(command);
    time_t end_exec = time(NULL);   // Tempo de término da execução
    
    // Log após compressão com tempo de execução
    double elapsed_time = difftime(end_exec, start_exec);
    snprintf(log_msg, sizeof(log_msg), "Compressão concluída para %s em %.2f segundos", output_filename, elapsed_time);
    log_message(log_filename, rank, -1, log_msg);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    int num_threads = 4;  // Número de threads OpenMP
    omp_set_num_threads(num_threads);

    // Definindo diferentes níveis de compressão para cada processo
    int compress_qualities[] = {23, 28, 35, 40};  // Exemplo de diferentes qualidades de compressão
    int segment_duration = 67;  // Duração de cada segmento em segundos (ajuste conforme necessário)

    // Definindo o nome do arquivo de log único
    const char* log_filename = "compression_log.txt";

    // Tempo de início geral
    if (world_rank == 0) {
        char start_time_str[100];
        get_current_time_str(start_time_str, sizeof(start_time_str));
        log_message(log_filename, world_rank, -1, "Início do processamento geral");
        log_message(log_filename, world_rank, -1, start_time_str);
    }
    
    MPI_Barrier(MPI_COMM_WORLD); // Sincronizar todos os processos antes de começar

    // Início do log para o processamento específico
    log_message(log_filename, world_rank, -1, "Iniciando processamento");

    if (world_rank == 0) {
        // Distribuir o trabalho para os outros processos
        log_message(log_filename, world_rank, -1, "Distribuindo trabalho para outros processos MPI");
        for (int i = 1; i < world_size; i++) {
            MPI_Send(&compress_qualities[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            int start_time = i * segment_duration;  // Incrementar tempo de início com base no rank
            MPI_Send(&start_time, 1, MPI_INT, i, 1, MPI_COMM_WORLD);  // Enviar o tempo de início para o segmento
            char log_msg[256];
            snprintf(log_msg, sizeof(log_msg), "Enviado tempo de início %d para o rank %d", start_time, i);
            log_message(log_filename, world_rank, -1, log_msg);
        }
        
        // Processar o primeiro segmento no nó mestre
        log_message(log_filename, world_rank, -1, "Processando segmento no nó mestre");
        compress_video_segment("input.mp4", "output_segment_0.mp4", compress_qualities[0], 0, segment_duration, log_filename, world_rank);
        
        // Receber os resultados dos outros nós
        for (int i = 1; i < world_size; i++) {
            int segment;
            MPI_Recv(&segment, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            char log_msg[256];
            snprintf(log_msg, sizeof(log_msg), "Segmento %d processado com CRF %d pelo rank %d", segment, compress_qualities[i], i);
            log_message(log_filename, world_rank, -1, log_msg);
        }
        
        // Tempo de término geral
        char end_time_str[100];
        get_current_time_str(end_time_str, sizeof(end_time_str));
        log_message(log_filename, world_rank, -1, "Fim do processamento geral");
        log_message(log_filename, world_rank, -1, end_time_str);
    } else {
        int quality;
        MPI_Recv(&quality, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int start_time;
        MPI_Recv(&start_time, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        // Log do recebimento de trabalho
        char log_msg[256];
        snprintf(log_msg, sizeof(log_msg), "Recebido CRF %d para compressão, tempo de início %d", quality, start_time);
        log_message(log_filename, world_rank, -1, log_msg);
        
        // Paralelizar a compressão para o segmento recebido com a qualidade específica
        #pragma omp parallel
        {
            int thread_id = omp_get_thread_num();  // Obtém o ID da thread OpenMP
            #pragma omp single
            {
                log_message(log_filename, world_rank, thread_id, "Iniciando compressão com OpenMP");
                char output_filename[256];
                snprintf(output_filename, sizeof(output_filename), "output_segment_%d.mp4", world_rank);
                compress_video_segment("input.mp4", output_filename, quality, start_time, segment_duration, log_filename, world_rank);
                
                // Notificar o nó mestre que o segmento foi processado
                log_message(log_filename, world_rank, thread_id, "Notificando nó mestre sobre a conclusão da compressão");
                MPI_Send(&world_rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }
    }
    
    log_message(log_filename, world_rank, -1, "Processamento finalizado");
    MPI_Finalize();
    return 0;
}

