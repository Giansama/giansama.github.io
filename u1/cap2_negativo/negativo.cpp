#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
//
int main(int argc, char** argv){
    int p1[2], p2[2];
// Abrindo a imagem
    Mat image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    if(!image.data){
        cout << "Não foi possível abrir a imagem biel.png" << endl;
    }
// Capturando Coordenadas
    cout << "Entre com as coordenadas de P1 (X, Y): ";
    cin >> p1[0] >> p1[1];
    cout << "Entre com as coordenadas de P2 (X, Y): ";
    cin >> p2[0] >> p2[1];
// Transformando a regição no negativo
    for (int i = p1[0]; i < p2[0]; i++) {
        for (int j = p1[1]; j < p2[1]; j++) {
            image.at<uchar>(i,j) = 255 - image.at<uchar>(i,j);
        }
    }
// Exibindo a janela
    imshow("Negativo", image);
    waitKey();
    return 0;
}
