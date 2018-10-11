
#include <iostream> // for standard I/O
#include <string>   // for strings

#include <cmath>
#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>  // OpenCV window I/O

using namespace cv;
using namespace std;

bool compare (Rect rect1, Rect rect2);

int main(int argc, char** argv){

VideoCapture video(argv[1]);

int frameNumber = video.get(CAP_PROP_FRAME_COUNT);

for (int pos = 0; pos < frameNumber; pos++){
video.set(CAP_PROP_POS_FRAMES, pos);
Mat src;
video >> src;

//Mat src;
//src = imread(argv[1], IMREAD_COLOR);

Mat src_gray;
cvtColor(src, src_gray, COLOR_RGB2GRAY);

Mat src_binary;
double thresholdValue = 200;
double max = 255;//rgb value the maxium is 255
threshold(src_gray, src_binary, thresholdValue, max,THRESH_BINARY);
blur( src_binary, src_binary, Size(3,3) );

vector< vector<Point> > contours;
//contours[i][j].x      means i-th contour's j-th point's x-value
vector<Vec4i> hierarchy;
findContours(src_binary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

//for test_______________________________________________________________________
   // drawContours(src, contours, -1,Scalar(255,255,0),10);


vector<Rect> boundRect( contours.size() );
 // Rect (a,b,c,d) a:x b:y c:width d:height
for (int i=0; i<contours.size(); i++)
    boundRect[i] = boundingRect( Mat(contours[i]) );

//for test_______________________________________________________________________
//for (size_t i=0; i<boundRect.size();i++)
    //rectangle(src, boundRect[i].tl(), boundRect[i].br(), Scalar(255,0,0),5);
//cout << boundRect.size() << endl;


//erase some small dots we assume that larger than 100*100 can be regard as a target
int size = contours.size();
int k = 0;
while (k<size){
  if (boundRect[k].width <= 50 || boundRect[k].height <= 50){
    boundRect.erase (boundRect.begin()+k);
    size --;
  }
  else
    k++;
}

//for test_______________________________________________________________________
//cout << boundRect.size() << endl;

//for test_______________________________________________________________________
//for (int i=0; i<boundRect.size();i++)
   //rectangle(src, boundRect[i].tl(), boundRect[i].br(), Scalar(0,255,0),3);


//if the rectangle has more than 9 similar size rectangles, 
//assume it's the  POTENCIAL target
vector<Rect> rawRect;
int count;
for (int i=0; i<boundRect.size(); i++){
  count = 0;
  for (int j=0; j<boundRect.size(); j++){
    if ( compare(boundRect[i], boundRect[j]) )
      count ++;
  }
  if (count >= 9) //not the number 9!!!!!
     rawRect.push_back (boundRect[i]);
}

//for test______________________________________________________________________
//cout << rawRect.size() << endl;

//check x, y has 3 same size rectangles
vector<Rect> nineRect;
for (int i =0; i<rawRect.size(); i++){
  count =0;
  for (int j =0; j<rawRect.size(); j++){
    Rect rect1 = rawRect[i];
    Rect rect2 = rawRect[j];
    if (abs(rect1.x - rect2.x) <= 40)
       count ++;
  }
  if (count >= 3){
    count =0;
    for (int j =0; j<rawRect.size(); j++){
    Rect rect1 = rawRect[i];
    Rect rect2 = rawRect[j];
    if (abs(rect1.y - rect2.y) <= 40)
       count ++;
    }
    if (count >= 3)
        nineRect.push_back (rawRect[i]);
  }
    
}

//for test______________________________________________________________________
//cout << nineRect.size() << endl;


for (int i=0; i<nineRect.size();i++){
    rectangle(src, nineRect[i].tl(), nineRect[i].br(), Scalar(0,0,255),2);
   // Mat target (src, nineRect[i]);
    //imshow("target", target);
    //waitKey(100);   
    }
imshow("window", src);//_binary);
waitKey(1);

}
   return 0;
}


bool compare (Rect rect1, Rect rect2){
  if (abs(rect1.width-rect2.width)<=20 && 
      abs(rect1.height-rect2.height)<=20)    {
   return true;
  }
    return false;
}
