#ifndef IMAGE_PROCESS_H
#define IMAEG_PROCESS_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace cv;
using namespace std;


void imageToContour(Mat &imageInput, Mat &imageOutput, vector<vector<Point>> &contour, const int thres);

#endif
