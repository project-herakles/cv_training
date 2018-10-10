#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char** argv){

Mat src;
src = imread(argv[1], IMREAD_COLOR);

Mat src_gray;
cvtColor(src, src_gray, COLOR_RGB2GRAY);

Mat src_binary;
//double thresholdValue = 190;
//double max = 255;//rgb value the maxium is 255
threshold(src_gray, src_binary, 190,255,THRESH_BINARY);


vector< vector<Point> > contours;
//contours[i][j].x      means i-th contour's j-th point's x-value
vector<Vec4i> hierarchy;
findContours(src_binary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

//vector<Rect> boundRect( contours.size() );
  //vector<vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect( contours.size() );
 // Rect (a,b,c,d) a:x b:y c:width d:height
for (size_t i=0; i<contours.size(); i++)
    //approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
    //boundRect[i] = boundingRect( Mat(contours_poly[i]) );
    boundRect[i] = boundingRect( Mat(contours[i]) );


for (size_t i=0; i<contours.size();i++){
    //drawContours(src, contours_poly, int(i), Scalar(0,0,255),3);
    rectangle(src, boundRect[i].tl(), boundRect[i].br(), Scalar(255,0,0),2);
}

imshow("window", src);
waitKey(0);

return 0;
}
