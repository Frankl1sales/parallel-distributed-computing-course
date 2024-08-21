#include <opencv2/opencv.hpp>
#include <mpi.h>
#include <omp.h>
#include <iostream>

using namespace cv;

void applyBlur(Mat& img) {
    #pragma omp parallel for
    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < img.cols; ++j) {
            // Aplicar uma operação de suavização (blur) em cada pixel
            img.at<Vec3b>(i, j) = img.at<Vec3b>(i, j) * 0.9;
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    Mat img;
    if (world_rank == 0) {
        img = imread("sua_imagem.jpg"); // Substitua pelo caminho de sua imagem
        if (img.empty()) {
            std::cerr << "Erro ao carregar a imagem!" << std::endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    int img_rows = img.rows;
    int img_cols = img.cols;
    MPI_Bcast(&img_rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&img_cols, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int block_size = img_rows / world_size;
    Mat sub_img(block_size, img_cols, CV_8UC3);

    MPI_Scatter(img.data, block_size * img_cols * 3, MPI_BYTE,
                sub_img.data, block_size * img_cols * 3, MPI_BYTE,
                0, MPI_COMM_WORLD);

    applyBlur(sub_img);

    MPI_Gather(sub_img.data, block_size * img_cols * 3, MPI_BYTE,
               img.data, block_size * img_cols * 3, MPI_BYTE,
               0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        imwrite("imagem_processada.jpg", img);
        std::cout << "Imagem processada e salva como 'imagem_processada.jpg'!" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
