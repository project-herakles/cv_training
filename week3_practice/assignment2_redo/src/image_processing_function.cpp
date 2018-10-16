#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <algorithm>
#include "image_processing_function.h"
#include <string>
#include <vector>

using namespace std;
using namespace cv;

Mat image_processing(Mat image, int thres, int blurSize){
    Mat image_gray, image_binary, image_processed;
    cvtColor(image, image_gray, CV_BGR2GRAY);
    threshold(image_gray, image_binary, thres, 255, THRESH_BINARY);
    GaussianBlur(image_binary, image_processed, Size(blurSize, blurSize), 0, 0);
    return image_processed;
}

void image_cropping(Mat image, vector<vector<Point>> contour){
    for (int i = 0; i < contour.size(); i++){
        Rect rect = boundingRect(contour[i]);
        Rect myROI (rect.tl().x, rect.tl().y, rect.width, rect.height);
        Mat imageCropped = image(myROI);
        
        string message = "frame" + to_string(i + 1);
        imshow(message, imageCropped);
    }
}
