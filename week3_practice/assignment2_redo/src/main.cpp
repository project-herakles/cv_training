#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <algorithm>
#include <vector>
#include <curses.h>
#include "image_processing_function.h"
#include "contour_filtering_function.h"

using namespace cv;
using namespace std;

int main(){
    VideoCapture cap("../media/10.1_Assignment_video_clip.mp4");
    Mat imageFrame, imageFrameProcessed;
    vector<vector<Point>> contour;
    vector<Vec4i> hierarchy;
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
        
        //Staring image processing by grayscale, thresholding and blurring
        imageFrame.copyTo(imageFrameProcessed);
        imageFrameProcessed = image_processing(imageFrameProcessed, 210, 15);
        
        //Find contour
        findContours(imageFrameProcessed, contour, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0,0));

        //Draw red line for contour that is unfiltered
        for (int i = 0; i < contour.size(); i++){
            drawContours(imageFrame, contour, i, Scalar(0, 0, 255), 3, LINE_8, hierarchy, 0, Point());
        }
        //contour filtering
        contour = contour_filtering_size(contour, 20, 150);
        contour = contour_filtering_area(contour, 12);
        contour = contour_filtering_position(contour,20);
        //Draw green line for contour that is filtered
        for (int i = 0; i < contour.size(); i++){
            drawContours(imageFrame, contour, i, Scalar(0, 255, 0), 3, LINE_8, hierarchy, 0, Point());
        }


        //Display the imageFrame with processing and with contour drawn 
        image_cropping(imageFrame, contour);
        imshow("Processed", imageFrameProcessed);
        imshow("imageFrame", imageFrame);
             
        if((cap.get(CV_CAP_PROP_POS_FRAMES) + 1) < cap.get(CV_CAP_PROP_FRAME_COUNT)){
            cap.read(imageFrame);
        }

        else{
            cout << "the end of the video\n";
            break;
        }
        //cin.get();
        chCheckForEscKey = waitKey(1);
    }

    if (chCheckForEscKey != 27){
        waitKey(0);
    }
    cap.release();
    destroyAllWindows();
    return(0);
    
}

