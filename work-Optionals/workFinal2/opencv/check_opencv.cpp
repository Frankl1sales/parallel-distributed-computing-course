#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    std::cout << "OpenCV Version: " << CV_VERSION << std::endl;

    cv::Mat image = cv::imread("sua_imagem.jpg"); // Substitua pelo caminho de uma imagem

    if(image.empty()) {
        std::cerr << "Erro ao carregar a imagem!" << std::endl;
        return -1;
    }

    cv::imshow("Imagem carregada", image);
    cv::waitKey(0);

    return 0;
}

