#ifndef IMAGE_PROCESSING_FUNCTION_H
#define IMAGE_PROCESSING_FUNCTION_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <algorithm>

using namespace cv;

Mat image_processing(Mat image, int thres, int blurSize);

#endif
