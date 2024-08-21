#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

void compress_video(const char* input_filename, const char* output_filename, int quality) {
    char command[1024];
    
    // Monta o comando FFmpeg para compressão
    snprintf(command, sizeof(command),
             "ffmpeg -i %s -vcodec libx264 -crf %d %s",
             input_filename, quality, output_filename);
    
    // Executa o comando
    system(command);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    int num_threads = 4;  // Número de threads OpenMP
    omp_set_num_threads(num_threads);

    if (world_rank == 0) {
        // Distribuir o trabalho para os outros processos
        for (int i = 1; i < world_size; i++) {
            MPI_Send(&i, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        
        // Processar o primeiro segmento no nó mestre
        compress_video("input.mp4", "output_segment_0.mp4", 23);
        
        // Receber os resultados dos outros nós
        for (int i = 1; i < world_size; i++) {
            int segment;
            MPI_Recv(&segment, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Segment %d processed by rank %d\n", segment, i);
        }
    } else {
        int segment;
        MPI_Recv(&segment, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        // Paralelizar a compressão para o segmento recebido
        #pragma omp parallel
        {
            #pragma omp single
            {
                char output_filename[256];
                snprintf(output_filename, sizeof(output_filename), "output_segment_%d.mp4", segment);
                compress_video("input.mp4", output_filename, 23);
                
                // Notificar o nó mestre que o segmento foi processado
                MPI_Send(&segment, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }
    }
    
    MPI_Finalize();
    return 0;
}
