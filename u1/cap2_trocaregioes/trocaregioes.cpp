#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
//
int main(int argc, char** argv)
{
    Mat image, aux, aux2;
    image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    if (!image.data)
	{
        cout << "Não foi possível abrir ou encontrar a imagem." << endl;
        return 0;
    }
//
    aux = image(Rect(0, 0, 128, 128)).clone();
    aux2 = image(Rect(128, 128, 128, 128)).clone();
    aux.copyTo(image(Rect(128, 128, 128, 128)));
    aux2.copyTo(image(Rect(0, 0, 128, 128)));
//
    aux = image(Rect(0, 128, 128, 128)).clone();
    aux2 = image(Rect(128, 0, 128, 128)).clone();
    aux.copyTo(image(Rect(128, 0, 128, 128)));
    aux2.copyTo(image(Rect(0, 128, 128, 128)));
//
    namedWindow("Imagem", WINDOW_AUTOSIZE);
    imshow("Imagem", image);
    waitKey();
    return 0;
}
