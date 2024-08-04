#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> // Adiciona o cabeçalho string.h

#define WIDTH 800
#define HEIGHT 800
#define MASTER 0

typedef struct {
    unsigned char r, g, b;
} Pixel;

void write_bmp(const char *filename, Pixel *img, int width, int height);
void mandelbrot(Pixel *img, int width, int height, double xi, double yi, double xf, double yf, int max_iter);

int main(int argc, char *argv[]) {
    int num_procs, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int max_iter = 1000;
    double xi = -2.5, yi = -1.0, xf = 1.0, yf = 1.0;

    int num_quadrants = 10 * num_procs;
    int quadrant_height = HEIGHT / num_quadrants;

    if (rank == MASTER) {
        Pixel *img = (Pixel *)malloc(WIDTH * HEIGHT * sizeof(Pixel));
        if (img == NULL) {
            fprintf(stderr, "Error allocating memory\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        printf("Master: Sending tasks to workers...\n");
        for (int i = 0; i < num_quadrants; i++) {
            int worker_rank = i % (num_procs - 1) + 1;
            double quadrant_yi = yi + (yf - yi) * (i * quadrant_height) / HEIGHT;
            double quadrant_yf = yi + (yf - yi) * ((i + 1) * quadrant_height) / HEIGHT;
            MPI_Send(&quadrant_yi, 1, MPI_DOUBLE, worker_rank, 0, MPI_COMM_WORLD);
            MPI_Send(&quadrant_yf, 1, MPI_DOUBLE, worker_rank, 0, MPI_COMM_WORLD);
            printf("Master: Sent quadrant %d to worker %d\n", i, worker_rank);
        }

        for (int i = 0; i < num_quadrants; i++) {
            Pixel *quadrant_img = (Pixel *)malloc(WIDTH * quadrant_height * sizeof(Pixel));
            int worker_rank = i % (num_procs - 1) + 1;
            MPI_Recv(quadrant_img, WIDTH * quadrant_height * sizeof(Pixel), MPI_BYTE, worker_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Master: Received quadrant %d from worker %d\n", i, worker_rank);
            for (int j = 0; j < quadrant_height; j++) {
                memcpy(&img[(i * quadrant_height + j) * WIDTH], &quadrant_img[j * WIDTH], WIDTH * sizeof(Pixel));
            }
            free(quadrant_img);
        }

        printf("Master: Writing BMP file...\n");
        write_bmp("mandelbrot.bmp", img, WIDTH, HEIGHT);
        free(img);
        printf("Master: Done.\n");
    } else {
        while (1) {
            MPI_Status status;
            double quadrant_yi, quadrant_yf;
            MPI_Recv(&quadrant_yi, 1, MPI_DOUBLE, MASTER, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&quadrant_yf, 1, MPI_DOUBLE, MASTER, 0, MPI_COMM_WORLD, &status);
            if (quadrant_yi == -1 && quadrant_yf == -1) break;

            printf("Worker %d: Received task for quadrant (%f, %f)\n", rank, quadrant_yi, quadrant_yf);
            Pixel *quadrant_img = (Pixel *)malloc(WIDTH * quadrant_height * sizeof(Pixel));
            mandelbrot(quadrant_img, WIDTH, quadrant_height, xi, quadrant_yi, xf, quadrant_yf, max_iter);
            MPI_Send(quadrant_img, WIDTH * quadrant_height * sizeof(Pixel), MPI_BYTE, MASTER, 0, MPI_COMM_WORLD);
            printf("Worker %d: Sent result for quadrant (%f, %f)\n", rank, quadrant_yi, quadrant_yf);
            free(quadrant_img);
        }
    }

    MPI_Finalize();
    return 0;
}

void mandelbrot(Pixel *img, int width, int height, double xi, double yi, double xf, double yf, int max_iter) {
    double x0, y0, x, y, xtemp;
    int iteration;
    for (int px = 0; px < width; px++) {
        for (int py = 0; py < height; py++) {
            x0 = xi + (xf - xi) * px / width;
            y0 = yi + (yf - yi) * py / height;
            x = 0.0;
            y = 0.0;
            iteration = 0;
            while (x*x + y*y <= 4 && iteration < max_iter) {
                xtemp = x*x - y*y + x0;
                y = 2*x*y + y0;
                x = xtemp;
                iteration++;
            }
            Pixel *p = &img[py * width + px];
            if (iteration == max_iter) {
                p->r = 0;
                p->g = 0;
                p->b = 0;
            } else {
                p->r = (iteration % 256);
                p->g = (iteration % 256);
                p->b = (iteration % 256);
            }
        }
    }
}

void write_bmp(const char *filename, Pixel *img, int width, int height) {
    FILE *f;
    int filesize = 54 + 3 * width * height;
    unsigned char bmpfileheader[14] = {
        'B', 'M',
        0, 0, 0, 0,
        0, 0,
        0, 0,
        54, 0, 0, 0
    };
    unsigned char bmpinfoheader[40] = {
        40, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        1, 0,
        24, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    
    int filesize_offset = filesize;
    bmpfileheader[2] = (unsigned char)(filesize_offset);
    bmpfileheader[3] = (unsigned char)(filesize_offset >> 8);
    bmpfileheader[4] = (unsigned char)(filesize_offset >> 16);
    bmpfileheader[5] = (unsigned char)(filesize_offset >> 24);
    
    int width_offset = width;
    bmpinfoheader[4] = (unsigned char)(width_offset);
    bmpinfoheader[5] = (unsigned char)(width_offset >> 8);
    bmpinfoheader[6] = (unsigned char)(width_offset >> 16);
    bmpinfoheader[7] = (unsigned char)(width_offset >> 24);
    
    int height_offset = height;
    bmpinfoheader[8] = (unsigned char)(height_offset);
    bmpinfoheader[9] = (unsigned char)(height_offset >> 8);
    bmpinfoheader[10] = (unsigned char)(height_offset >> 16);
    bmpinfoheader[11] = (unsigned char)(height_offset >> 24);
    
    f = fopen(filename, "wb");
    fwrite(bmpfileheader, 1, 14, f);
    fwrite(bmpinfoheader, 1, 40, f);
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Pixel p = img[(height - i - 1) * width + j];
            unsigned char color[3] = {p.b, p.g, p.r};
            fwrite(color, 1, 3, f);
        }
    }
    
    fclose(f);
}

