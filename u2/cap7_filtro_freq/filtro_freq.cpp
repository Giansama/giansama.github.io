#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//
#define RADIUS 20
#define MAX_GAMA_H  100.0
#define MAX_GAMA_L  100.0
#define MAX_D0      100.0
#define MAX_C       100.0
//
using namespace cv;
using namespace std;
//
int gama_H;
int gama_L;
int d0;
int c;
int dft_M, dft_N;
Mat filter, tmp;
//
void on_filter(int, void*)
{
  float _d0 =    ((float) 2*d0/ (float) MAX_D0) * sqrt( dft_M*dft_M + dft_N*dft_N )/2;
  float _gamaL = ((float) 10*gama_L)/ (float) MAX_GAMA_L;
  float _gamaH = ((float) 10*gama_H)/ (float) MAX_GAMA_H;
  float _c =     ((float) 10*c)/ (float) MAX_C;
//
  cout << "d0: " << _d0 << "  L: " << _gamaL << "  H: "  << _gamaH << "  c: " << _c << endl;
//
  double n1, n2;
  double n = 0.0;
  double dGamas = _gamaH - _gamaL;
  double d02 = _d0*_d0;
//
  for(int i=0; i<dft_M; i++)
  {
    for(int j=0; j<dft_N; j++)
    {
        n1 = (i - dft_M/2)*(i - dft_M/2);
        n2 = (j - dft_N/2)*(j - dft_N/2);
        n = _c * (n1 + n2);
//
        tmp.at<float>(i,j) = dGamas * (1 - exp(-n/d02)) + _gamaL;
    }
  }
//
  Mat comps[]= {tmp, tmp};
  merge(comps, 2, filter);
}
//
void deslocaDFT(Mat& image )
{
  Mat aux, A, B, C, D;
//
  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols/2;
  int cy = image.rows/2;
//
  // reorganiza os quadrantes da transformada
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));
//
  A.copyTo(aux);  D.copyTo(A);  aux.copyTo(D); // A <-> D
  C.copyTo(aux);  B.copyTo(C);  aux.copyTo(B); // C <-> B
}
//
int main(int , char**)
{
  VideoCapture cap;
  Mat imaginaryInput, complexImage, multsp;
  Mat padded, mag;
  Mat image, imagegray;
  Mat_<float> realInput, zeros;
  vector<Mat> planos;
//
  char key;
  cap.open(0);
  if(!cap.isOpened()) return -1;
//
  cap >> image;
  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);
//
  // realiza o padding da imagem
  copyMakeBorder(image, padded, 0,
                 dft_M - image.rows, 0,
                 dft_N - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));
//
  // preenche a parte imaginaria da matriz com zeros
  zeros = Mat_<float>::zeros(padded.size());
//
  // prepara a matriz complexa para ser preenchida
  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));
//
  filter = complexImage.clone();
  tmp = Mat(dft_M, dft_N, CV_32F);
//
  char title[25];
  namedWindow("filtrada", 1);
//
  sprintf       ( title, "Gama H x %.1lf", MAX_GAMA_H );
  createTrackbar( title, "filtrada",   &gama_H, MAX_GAMA_H, on_filter );
//
  sprintf       ( title, "Gama L x %.1lf", MAX_GAMA_L );
  createTrackbar( title, "filtrada",   &gama_L, MAX_GAMA_L, on_filter );
//
  sprintf       ( title, "D0 x %.1lf",  MAX_D0 );
  createTrackbar( title, "filtrada", &d0, MAX_D0, on_filter );
//
  sprintf       ( title, "c x %.1lf",   MAX_C );
  createTrackbar( title, "filtrada", &c, MAX_C, on_filter );
//
  on_filter(0, 0);
//
  for(;;){
    cap >> image;
    cvtColor(image, imagegray, CV_BGR2GRAY);
    imshow("original", imagegray);
//
    // realiza o padding da imagem
    copyMakeBorder(imagegray, padded, 0,
                   dft_M - image.rows, 0,
                   dft_N - image.cols,
                   BORDER_CONSTANT, Scalar::all(0));
//
    planos.clear();
    realInput = Mat_<float>(padded);
    planos.push_back(realInput);
    planos.push_back(zeros);
//
    realInput = realInput + Scalar::all(1);
    log(realInput,realInput);
//
    merge(planos, complexImage);
    // calcula o dft
    dft(complexImage, complexImage);
    // realiza a troca de quadrantes
    deslocaDFT(complexImage);
    // aplica o filtro frequencial
    mulSpectrums(complexImage,filter,complexImage,0);
    // limpa o array de planos
    planos.clear();
    // separa as partes real e imaginaria para modifica-las
    split(complexImage, planos);
    // usa o valor medio do espectro para dosar o ruido
    // recompoe os planos em uma unica matriz complexa
    merge(planos, complexImage);
    // troca novamente os quadrantes
    deslocaDFT(complexImage);
    // calcula a DFT inversa
    idft(complexImage, complexImage);
    // limpa o array de planos
    planos.clear();
    // imagem filtrada
    split(complexImage, planos);
//
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    exp(planos[0],planos[0]);
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    imshow("filtrada", planos[0]);
//
    key = (char) waitKey(10);
    if(key == 27) break;
  }
  return 0;
}
