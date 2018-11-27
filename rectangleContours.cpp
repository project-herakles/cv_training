#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

bool compare (Rect rect1, Rect rect2);
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
 //erase some small dots we assume that larger than 10*10 can be regard as a target
for (size_t i=0; i<boundRect.size();i++){
  if (boundRect[i].area() <= 100)
    boundRect.erase(boundRect.begin()+i);
} 


//if the rectangle has 9 similar size rectangles, assume it's the target
vector<Rect> nineRect;
for (size_t i=0; i<boundRect.size(); i++){
  int count = 0;
  for (size_t j=0; j<boundRect.size(); j++){
    if ( compare(boundRect[i], boundRect[j]) )
      count ++;
  }
  if (count == 9)
     nineRect.push_back (boundRect[i]);
}



for (size_t i=0; i<nineRect.size();i++)
    rectangle(src, nineRect[i].tl(), nineRect[i].br(), Scalar(255,0,0),2);

    

imshow("window", src);
waitKey(0);

return 0;
}
bool compare (Rect rect1, Rect rect2){
  int error;
  error = pow(rect1.width-rect2.width,2)+ pow(rect1.height-rect2.height,2);
  return (error <= 100? true:false) ;
}

1.rm myfile (touch mydir will not return wrong message but doesn't work)
2.cp -t mydir myfile: cp -t copy all source arguments into directory
cat<myfile>mydir/myfile is also ok
3.ls|grep ab
find . -name 'ab' is find the name exactly 'ab'
4. cd~
cd home/c1123
cd ../../
cd
5. 1>output.txt 2>output.txt
>output.txt 2>&1
f.ls -1 |wc -w or wc -l
-l will count more than one and -a is what?
(a hard link of a file is 2 or more?)
g. -lS
h. only (ab)*(bc)*(cd) don't match
i. c and d
review diff part
j. sort c|uniq -c
