#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <mpi.h>
#include <omp.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    IplImage* img = NULL;
    if (world_rank == 0) {
        img = cvLoadImage("sua_imagem.jpg", CV_LOAD_IMAGE_COLOR);
        if (!img) {
            printf("Erro ao carregar a imagem!\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    int img_rows = img ? img->height : 0;
    int img_cols = img ? img->width : 0;
    MPI_Bcast(&img_rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&img_cols, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int block_size = img_rows / world_size;
    IplImage* sub_img = cvCreateImage(cvSize(img_cols, block_size), IPL_DEPTH_8U, 3);

    MPI_Scatter(img->imageData, block_size * img_cols * 3, MPI_BYTE,
                sub_img->imageData, block_size * img_cols * 3, MPI_BYTE,
                0, MPI_COMM_WORLD);

    #pragma omp parallel for
    for (int i = 0; i < sub_img->height; ++i) {
        for (int j = 0; j < sub_img->width; ++j) {
            CvScalar pixel = cvGet2D(sub_img, i, j);
            pixel.val[0] = pixel.val[0] * 0.9;
            pixel.val[1] = pixel.val[1] * 0.9;
            pixel.val[2] = pixel.val[2] * 0.9;
            cvSet2D(sub_img, i, j, pixel);
        }
    }

    MPI_Gather(sub_img->imageData, block_size * img_cols * 3, MPI_BYTE,
               img->imageData, block_size * img_cols * 3, MPI_BYTE,
               0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        cvSaveImage("imagem_processada.jpg", img, 0);
        printf("Imagem processada e salva como 'imagem_processada.jpg'!\n");
        cvReleaseImage(&img);
    }

    cvReleaseImage(&sub_img);
    MPI_Finalize();
    return 0;
}
