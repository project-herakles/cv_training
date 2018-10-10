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
double thresholdValue = 190;
double max = 255;//rgb value the maxium is 255
threshold(src_gray, src_binary, thresholdvalue, max,THRESH_BINARY);


vector< vector<Point> > contours;
//contours[i][j].x      means i-th contour's j-th point's x-value
vector<Vec4i> hierarchy;
findContours(src_binary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

  vector<Rect> boundRect( contours.size() );
 // Rect (a,b,c,d) a:x b:y c:width d:height
for (size_t i=0; i<contours.size(); i++)
    boundRect[i] = boundingRect( Mat(contours[i]) );

//find the 9 rectangles
  int rectangleNumber = 9;
  int count = 0;
  vector<Rect> correctRect(rectangleNumber);
for (size_t i=0; i<boundRect.size();i++){ //search all rectangles
  if (boundRect[i].area() > 100){ //omit some small dots we assume that larger than 10*10 can be regard as a target
    Rect rect = boundRect[i];
    int count2 = 0;
    for (size_t j=0;j<boundRect.size();j++){
      Rect rect2 = boundRect[j];
      if (rect2.x == rect.x && rect2.area() == rect.area() )
        count2 ++;
    } 
    if (count2 >= 3){ //means rect is a potential target
      int count3 =0;
      for (size_t k=0;k<boundRect.size();k++){
          Rect rect3= boundRect[k];
          if (rect3.y == rect.y && rect3.area() == rect.area() )
            count3 ++;
          if (count3 >=3){ //means rect is the target
             count++;
            correctRect[count] = rect;
          }
      }
  }
}
  
for (size_t i=0; i<contours.size();i++)
    rectangle(src, correctRect[i].tl(), correctRect[i].br(), Scalar(255,0,0),2);

imshow("window", src);
waitKey(0);

return 0;
}
