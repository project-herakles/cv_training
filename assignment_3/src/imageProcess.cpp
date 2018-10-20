#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <algorithm>
#include "imageProcess.h"
#include <string>
#include <vector>


using namespace std;
using namespace cv;

void imageToContour(Mat &imageInput, Mat &imageOutput, vector<vector<Point>> &contour, const int thres){
    Mat dilationKernel = getStructuringElement(MORPH_ELLIPSE, Size(9, 9));
    Mat closingKernel = getStructuringElement(MORPH_ELLIPSE, Size(9,9));
    Mat imageCopied, imageFilteredBlue, imageBinary;
    vector<Vec4i> hierarchy;
    Scalar hsv_l(0, 210, 210);
    Scalar hsv_h(255,255,255);
    imageInput.copyTo(imageCopied);
    cvtColor(imageCopied, imageFilteredBlue, CV_BGR2HSV);
    inRange(imageFilteredBlue, hsv_l, hsv_h, imageFilteredBlue);
    imshow("Blue", imageFilteredBlue);
    threshold(imageFilteredBlue, imageBinary, thres, 255, THRESH_BINARY);
    //adaptiveThreshold(imageFilteredBlue, imageBinary, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 9, 0);
    //imageBinary.copyTo(imageOutput);
    //GaussianBlur(imageBinary, imageBinary, Size(3,3), 0, 0);
    dilate(imageBinary, imageOutput, dilationKernel);
    morphologyEx(imageOutput, imageOutput, MORPH_CLOSE, closingKernel);
    findContours(imageOutput, contour, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    for (int i = 0; i < contour.size(); i++){
            drawContours(imageInput, contour, i, Scalar(0, 255, 0), 1, LINE_8, hierarchy, 0, Point());
    }    
}


