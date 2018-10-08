#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

Mat crop(Mat name, int n, int x, int y, int width, int height){
    Mat image=name;

    Rect croppedRectangle=Rect(x,y,width,height);

    Mat CroppedImage = image(croppedRectangle);

   return CroppedImage;
}

int main(){

     // Create a VideoCapture object and open the input file
  // If the input is the web camera, pass 0 instead of the video file name
  VideoCapture cap("chaplin.mp4");

  while(1){

    Mat frame;
    // Capture frame-by-frame
    cap >> frame;

    // If the frame is empty, break immediately
    if (frame.empty())
      break;
    else{

    crop(frame,1,618,246,111,65);
    crop(frame,2,766,250,111,65);
    crop(frame,3,916,247,111,65);
    crop(frame,4,627,337,111,65);
    crop(frame,5,772,340,111,65);
    crop(frame,6,919,345,111,65);
    crop(frame,7,639,421,111,65);
    crop(frame,8,778,430,111,65);
    crop(frame,9,922,430,111,65);




    }

    // Display the resulting frame
    imshow( "Frame", frame );

    // Press  ESC on keyboard to exit
    char c=(char)waitKey(25);
    if(c==27)
      break;
  }

  // When everything done, release the video capture object
  cap.release();

  // Closes all the frames
  destroyAllWindows();

  return 0;

}

