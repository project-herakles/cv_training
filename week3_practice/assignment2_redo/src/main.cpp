#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <algorithm>
#include <curses.h>

int main(){
    VideoCapture cap("../media/10.1_Assignment_video_clip.mp4");
    Mat imageFrame;
    if(!cap.isOpened()){
        cout << "can't read the video\n";
        waitKey(0);
        return(-1);
    }
    
    if(cap.get(CV_CAP_PROP_FRAME_COUNT) < 1){
        cout << "error: video file must have at least one frame\n";
        waitKey(0);
        return(-1);
    }

    cap.read(imageFrame);

    char chCheckForEscKey = 0;
    while (cap.isOpened() && chCheckForEscKey != 27){
        imshow("imageFrame", imageFrame);

        if((cap.get(CV_CAP_PROP_POS_FRAMGES) + 1) < cap.get(CV_CAP_PROP_FRAME_COUNT)){
            cap.read(imageFrame);
        }

        else{
            cout << "the end of the video\n";
            break;
        }

        chCheckForEscKey = waitKey(100);
    }

    if (chCheckForEscKey != 27){
        waitkey(0)
    }

    return(0);
    
}

