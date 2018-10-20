#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <algorithm>
#include <vector>
#include <curses.h>
#include "imageProcess.h"
#include "armorfind.h"
using namespace cv;
using namespace std;


int main(){
    VideoCapture cap("../media/10.8_Assignment_video_clip.mp4.mp4");
    vector<vector<Point>> contour;
    vector<Point> armor;
    ArmorFind armorFinder;
    Mat imageVideo, imageInput, imageOutput, imageProcessed;
    if(!cap.isOpened()){
        cout << "can't read the video\n";
        waitKey(0);
        return -1;
    }

    if((cap.get(CV_CAP_PROP_FRAME_COUNT) < 1)){
            cout << "error: video file must have at least one frame\n";
            waitKey(0);
            return -2;
    }

    cap.read(imageVideo);

    char chCheckForEscKey = 0;

    while (cap.isOpened() && chCheckForEscKey != 27){
        imageVideo.copyTo(imageInput);
        imageVideo.copyTo(imageProcessed);
        imageToContour(imageInput, imageOutput, contour, 70);
        armorFinder.process(contour, imageVideo, imageProcessed, armor, false);
        imshow("original with armor process", imageProcessed);
        imshow("original with contour", imageInput);
        imshow("output after image process", imageOutput);
        if((cap.get(CV_CAP_PROP_POS_FRAMES) + 1) < cap.get(CV_CAP_PROP_FRAME_COUNT)){
            cap.read(imageVideo);
         }

         else{
            cout << "the end of the video\n";
         }

         chCheckForEscKey = waitKey(30);
    }

    if (chCheckForEscKey != 27){
        waitKey(0);
    }
    cap.release();
    destroyAllWindows();
    return 0;
}

