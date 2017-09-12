#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
//
int height;
int MAX = 100;
double d = 6;
int d_slider = 0;
int t_slider = 0;
int t = 0;
int h = 20;
int h_slider = 0;
Mat image1, image2, alpha, beta;
char TrackbarName[50];
//
void tiltShift(){
	Mat mWeighted1, mWeighted2, tiltshift;
	int l1 = -h/2;
    int l2 = -l1;
    alpha = Mat::zeros(image1.rows, image1.cols, CV_32F);
    beta = Mat::zeros(image1.rows, image1.cols, CV_32F);
    int i, j;
//
    for (i = 0; i < alpha.rows; i++) {
        int x = i - (t + h/2);
        float alfa = 0.5f * (tanh((x - l1)/d) - tanh((x - l2)/d));
        for (j = 0; j < alpha.cols; j++) {
            alpha.at<float>(i, j) = alfa;
            beta.at<float>(i, j) = 1 - alfa;
        }
    }
	//
    Mat auxA[] = {alpha, alpha, alpha};
    Mat auxB[] = {beta, beta, beta};
    merge(auxA, 3, alpha);
    merge(auxB, 3, beta);
//
    image1.convertTo(mWeighted1, CV_32FC3);
    image2.convertTo(mWeighted2, CV_32FC3);
    multiply(mWeighted1, alpha, mWeighted1);
    multiply(mWeighted2, beta, mWeighted2);
//
    add(mWeighted1, mWeighted2, tiltshift);
    tiltshift.convertTo(tiltshift, CV_8UC3);
    imshow("tiltshift", tiltshift);
//
    imshow("imagem", alpha);
	imshow("borramento", beta);
}
//
void on_trackbar_d(int, void*){
    d = (double) d_slider;
    if (d < 1) {
        d = 1;
    }
    tiltShift();
}
//
void on_trackbar_t(int, void *){
    t = t_slider*height/MAX;
    tiltShift();
}
//
void on_trackbar_h(int, void*) {
    h = h_slider*height/MAX;
    if (h == 0) {
        h = 1;
    }
    if (h > height) {
        h = height;
    }
    tiltShift();
}
//
int main(int argvc, char** argv){
	image1 = imread("../imagens/trem.jpg");
	height = image1.size().height;
	image2 = image1.clone();
//
	Mat aux, mask, mask1;
    float media[] = {1,1,1,
                     1,1,1,
                     1,1,1};
    mask = Mat(3, 3, CV_32F, media);
    scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask1);
    mask = mask1;
    image2.convertTo(aux, CV_32F);
//
    for (int i = 0; i < 10; i++) {
        filter2D(aux, aux, aux.depth(), mask, Point(1, 1), 0);
    }
//
    aux = abs(aux);
    aux.convertTo(image2, CV_8UC3);
    namedWindow("tiltshift", 1);
//
    sprintf( TrackbarName, "Altura");
    createTrackbar( TrackbarName, "tiltshift",
                    &h_slider,
                    MAX,
                    on_trackbar_h);
//
    sprintf( TrackbarName, "Decaimento");
    createTrackbar( TrackbarName, "tiltshift",
                    &d_slider,
                    MAX,
                    on_trackbar_d);
//
    sprintf( TrackbarName, "Vertical");
    createTrackbar( TrackbarName, "tiltshift",
                    &t_slider,
                    MAX,
                    on_trackbar_t );
    tiltShift();
//
  waitKey(0);
  return 0;
}
