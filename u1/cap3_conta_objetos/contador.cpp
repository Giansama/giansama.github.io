#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
//
int main(int argc, char **argv)
{
    Mat image, mask;
    int width, height;
    int n_objetos, n_buraco;
//
    CvPoint p;
    image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
//
    if (!image.data) {
        cout << "Não foi possível abrir a imagem";
        return (-1);
    }
    height = image.size().height;       //linhas
    width = image.size().width;         //colunas
//
    p.x = 0;
    p.y = 0;
    for (int k = 0; k < height; k++) {
        floodFill(image, CvPoint(k,0), 0);
        floodFill(image, CvPoint(k,255), 0);
        floodFill(image, CvPoint(0,k), 0);
        floodFill(image, CvPoint(255,k), 0);
    }
//
    n_objetos = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (image.at < uchar > (i, j) == 255) {
                n_objetos++;
                p.x = j;
                p.y = i;
//
                floodFill(image, p, 200);
            }
        }
    }
    floodFill(image,cvPoint(0,0), 50);
//
    n_buraco = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (image.at < uchar > (i, j) == 0) {
                p.x = j;
                p.y = i;
                floodFill(image, p, 50);
//
                if(image.at< uchar>(i, j-1) == 200)
                {
                    n_buraco++;
                    floodFill(image,cvPoint(p.x-1,p.y), 150);
                }
            }
        }
    }
//
    cout << "Número de objetos: " << n_objetos << endl;
    cout << "Número de buracos: " << n_buraco << endl;
    cout << "Número de objetos sem buracos: " << n_objetos - n_buraco << endl;
//
    imshow("image", image);
    waitKey();
    return 0;
}
