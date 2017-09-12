#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
//
int main(int argc, char** argv){
  Mat image;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes;
  Mat hg_old, hg_new;
//
  Mat histR, histG, histB;
  int nbins = 64;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;
//
  cap.open(0);
  if(!cap.isOpened()){
    cout << "camera indisponivel";
    return -1;
  }
//
  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;
//
  int histw = nbins, histh = nbins/2;
  Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgG(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgB(histh, histw, CV_8UC3, Scalar(0,0,0));
//As análises serão feitas utilizando o primeiro histograma, ou seja, da primeira imagem tirada do ambiente.
//Dessa forma, qualquer alteração dentro do limite x informará movimentação.
  cap >> image;
  split (image, planes);
  calcHist(&planes[1], 1, 0, Mat(), hg_old, 1, &nbins, &histrange, uniform, acummulate);
  double x = 0;
//
  while(1){
    cap >> image;
    resize(image, image, Size(640,360));
// DETECCAO DE MOVIMENTO
    split (image, planes);
    calcHist(&planes[1], 1, 0, Mat(), hg_new, 1, &nbins, &histrange, uniform, acummulate);
    x = compareHist(hg_new, hg_old, CV_COMP_CORREL);
    cout << "x = "<< x<<endl;
//
    if(x<-0.25){
    	putText(image, "!!!", cvPoint(250, 100), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(255, 0, 0), 2);
    }
//
    calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[1], 1, 0, Mat(), histG, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[2], 1, 0, Mat(), histB, 1,
             &nbins, &histrange,
             uniform, acummulate);
//
    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histG, histG, 0, histImgG.rows, NORM_MINMAX, -1, Mat());
    normalize(histB, histB, 0, histImgB.rows, NORM_MINMAX, -1, Mat());
//
    histImgR.setTo(Scalar(0));
    histImgG.setTo(Scalar(0));
    histImgB.setTo(Scalar(0));
//
    for(int i=0; i<nbins; i++){
      line(histImgR,
           Point(i, histh),
           Point(i, histh-cvRound(histR.at<float>(i))),
           Scalar(0, 0, 255), 1, 8, 0);
      line(histImgG,
           Point(i, histh),
           Point(i, histh-cvRound(histG.at<float>(i))),
           Scalar(0, 255, 0), 1, 8, 0);
      line(histImgB,
           Point(i, histh),
           Point(i, histh-cvRound(histB.at<float>(i))),
           Scalar(255, 0, 0), 1, 8, 0);
    }
    histImgR.copyTo(image(Rect(0, 0       ,nbins, histh)));
    histImgG.copyTo(image(Rect(0, histh   ,nbins, histh)));
    histImgB.copyTo(image(Rect(0, 2*histh ,nbins, histh)));
    imshow("image", image);
    if(waitKey(30) >= 0) break;
  }
  return 0;
}
