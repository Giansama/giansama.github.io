#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>
//
using namespace std;
using namespace cv;
//
#define STEP 5
#define JITTER 3
#define RAIO 3
//
int top_slider = 1;
int top_slider_max = 50;
char TrackbarName[50];
vector<int> yrange;
vector<int> xrange;
Mat image, border, points;
//
void desenha(int val){
      int gray, x, y;
      int width, height;
//
      width=image.size().width;
      height=image.size().height;
      xrange.resize(height/STEP);
      yrange.resize(width/STEP);
//
      iota(xrange.begin(), xrange.end(), 0);
      iota(yrange.begin(), yrange.end(), 0);
//
      for(uint i=0; i<xrange.size(); i++){
              xrange[i]= xrange[i]*STEP+STEP/2;
      }
      for(uint i=0; i<yrange.size(); i++){
              yrange[i]= yrange[i]*STEP+STEP/2;
      }
//
      points = Mat(height, width, CV_8U, Scalar(0));
      random_shuffle(xrange.begin(), xrange.end());
      //points = Scalar::all(0);
      image.copyTo(points, border);
//
      int aux;
      for(auto i : xrange){
          random_shuffle(yrange.begin(), yrange.end());
          for(auto j : yrange){
//
          x = i+rand()%(2*JITTER)-JITTER+1;
          y = j+rand()%(2*JITTER)-JITTER+1;
          gray = image.at<uchar>(x,y);
//
                    if(val >= 0 && val < 5){
                            aux = 0.8*val;
                    }else if(val >= 5){
                            aux = val*0.2;
                    }else{
                            aux = 1;
                    }
//
          circle(points,
                 cv::Point(y,x),
                 aux,
                 CV_RGB(gray,gray,gray),
                 -1,
                 CV_AA);
          }
      }
      imshow("pontos",points);
}
//
void on_trackbar_canny(int, void*){
  Canny(image, border, top_slider, 3*top_slider);
  imshow("canny", border);
  desenha(top_slider);
}
//
int main(int argc, char** argv){
//
  image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  srand(time(0));
//
  if(!image.data){
        cout << "nao abriu" << argv[1] << endl;
    cout << argv[0] << " imagem.jpg";
    exit(0);
  }
  sprintf( TrackbarName, "Threshold inferior", top_slider_max );
//
  namedWindow("canny",1);
  createTrackbar( TrackbarName, "canny",
                &top_slider,
                top_slider_max,
                on_trackbar_canny );
//
        on_trackbar_canny(top_slider, 0 );
        waitKey();
          imwrite("cannyborders.png", points);
  return 0;
}
