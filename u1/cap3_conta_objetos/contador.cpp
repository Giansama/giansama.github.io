#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    Mat image, mask;
    int width, height;
    int nobjects, noburacos;

    CvPoint p;
    image = imread("../imagens/bolhas.png", CV_LOAD_IMAGE_GRAYSCALE);

    if (!image.data) {
        cout << "imagem nao carregou corretamente\n";
        return (-1);
    }
    width = image.size().width;         //colunas
    height = image.size().height;       //linhas

    p.x = 0;
    p.y = 0;

    // Elimina os objetos que tocam as laterais da imagem, rotulando-os com a cor de fundo;
    for (int k = 0; k < height; k++) {
        floodFill(image, CvPoint(k,0), 0);
        floodFill(image, CvPoint(k,255), 0);
        floodFill(image, CvPoint(0,k), 0);
        floodFill(image, CvPoint(255,k), 0);
    }


    // Contagem de objetos
    nobjects = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (image.at < uchar > (i, j) == 255) {
                // achou um objeto
                nobjects++;
                p.x = j;
                p.y = i;

                //Todos os objetos são rotulados no tom de cinza 200;
                floodFill(image, p, 200);
            }
        }
    }

    // Contagem de objetos com buracos
    //Uma forma de contar o número de buracos é através de rótulos, ou seja, como os buracos
    //possuem o mesmo tom de cinza do fundo, podemos alterar a cor de fundo e buscar os objetos,
    //que terão seu tom de cinza igual ao do fundo. Depois podemos bunscar na imagem o número de
    //regiões de tom de cinza 0, que equivale ao buraco.

    floodFill(image,cvPoint(0,0), 50);                  // utilizando o floodFill(), mudaremos a cor de fundo

    noburacos = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (image.at < uchar > (i, j) == 0) {       //busca objetos da cor de fundo original
                p.x = j;
                p.y = i;
                //Todos os buracos são rotulados no tom de cinza 50;
                floodFill(image, p, 50);

                if(image.at< uchar>(i, j-1) == 200)
                {
                    noburacos++;
                    // preenche objeto com buraco com cor diferente, de forma que se acharmos um novo buraco e
                    //um pixel anterior for de cor diferente de 200, ele irá saber que a bolha possui outros
                    //buracos e não contabilizará novos buracos para aquela bolha.
                    floodFill(image,cvPoint(p.x-1,p.y), 150);
                }
            }
        }
    }

    cout << "Número de objetos: " << nobjects << endl;
    cout << "Número de buracos: " << noburacos << endl;
    cout << "Número de objetos sem buracos: " << nobjects - noburacos << endl;


    imshow("image", image);
    imwrite("labeling.png", image);
    waitKey();
    return 0;
}    
