#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

int main(int agrc, char** argv){

Mat src;
src = imread(argv[1], 1);

Mat src_gray;
cvtColor(src, src_gray, COLOR_BGR2GRAY);

Mat src_binary;
double thresholdValue = 190;
double max = 255;//rgb value the maxium is 255
threshold(src_gray, src_binary, thresholdValue, max, THRESH_BINARY);


vector< vector<Point> > contours;
//contours[i][j].x      means i-th contour's j-th point's x-value
vector<Vec4i> hierarchy;
findContours(src_binary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

for (int i=0; i<contours.size(); i++){

}

drawContours(


}
