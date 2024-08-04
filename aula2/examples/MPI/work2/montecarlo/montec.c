// Comando para executar no terminal
// mpicc montec.c -o montec
// mpirun -np 4 --host localhost --oversubscribe ./montec
// Saída esperada: Aproximação de Pi: 3.144204

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_POINTS 1000000

int main(int argc, char** argv) {
    
    int rank, num_tasks;
    int points_in_circle = 0;
    int total_points_in_circle;
    double x, y;
    // o seed foi definido como unsigned, pois a função rand_r
    //  aceita um ponteiro para uma semente do tipo "unsigned int"
    unsigned int seed;

    // Inicialize o ambiente MPI.
    // Cada processo deve obter seu rank e o número total de processos (tasks).
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);


    // Geração de Pontos Aleatórios
    seed = time(NULL) + rank; // Semente única para cada processo
    	// time(null) fornece a hora atual 
    	// para solucionar o caso de processos iniciarem no mesmo segundo,
    	// optou-se em colocar o rank para diferencia-los.
    	// dessa forma, cada processo terá uma seed diferente

    // Cada processo gera pontos e conta quantos estão dentro do círculo
    for (int i = 0; i < NUM_POINTS / num_tasks; i++) {
    	// uso do rand_r é porque ele é thread-safe
    	// sem causar condição de corrida
    	// link da explicação: https://www.ibm.com/docs/pt-br/i/7.5?topic=functions-rand-rand-r-generate-random-number
    	
        x = (double)rand_r(&seed) / RAND_MAX;
        y = (double)rand_r(&seed) / RAND_MAX;
        
        // Contagem dos Pontos Dentro do Círculo
        if (x * x + y * y <= 1.0) {
            points_in_circle++;
        }
    }

    // Redução para somar todos os pontos dentro do círculo de todos os processos
    // exemplo: int MPI_Reduce(
    
    //const void* sbuf, 
    //void* rbuf, 
    //int count,
    //MPI_Datatype stype, 
    //MPI_Op op, 
    //int root,
    //MPI_Comm comm)
    
    // operação
    MPI_Reduce(
    &points_in_circle, 
    &total_points_in_circle, 
    1, 
    MPI_INT, 
    MPI_SUM, 
    0, 
    MPI_COMM_WORLD);
    // sbuf =  &points_in_circle --ponteiros para dados a serem enviado
    // rbuf = total_points_in_circle -- ponteiro p/ onde o resultado será armazenado
    // count = 1 -- número de elementos no buffer de envio
    // Mpi_datatype = Mpi_init
    // op = mpi_sum -- tipo de operação de redução
    // root = 0 -- rank do processo que será o recebidor
    // comm = MPI_COMM_WORLD comunicador que define o grupo de processos

    // execução do cálculo	
    if (rank == 0) {
        double pi = 4.0 * total_points_in_circle / NUM_POINTS;
        // Exibição do Resultado
        printf("Aproximação de Pi: %f\n", pi);
    }

    MPI_Finalize();
    return 0;
}
