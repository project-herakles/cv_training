#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<curses.h>           // may have to modify this line if not using Windows
#define CV_BGR2GRAY cv::COLOR_BGRA2GRAY
///////////////////////////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace cv;

int main() {
	Mat image, image_gray;
	Mat image_processed;
	vector<vector<Point>> contour;
	vector<Vec4i> hierarchy;
	image = imread("Rune1.jpg");

	// Validation of the image
	if (!image.data) {
		cout << "I couldnt read the image";
		return -1;
	}
	else {
		image.copyTo(image_processed);
	}

	// First convert it into grayscale picture 
	cvtColor(image_processed, image_gray, CV_BGR2GRAY);
	// then do thresholding to convert in into a binary picture
	threshold(image_gray, image_processed, 100, 255, 1);
	// Finding contour
	findContours(image_processed, contour, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	for (int i = 0; i < contour.size(); i++) {
		drawContours(image_processed, contour, i, Scalar(0, 0, 255), 3, 8, hierarchy, 0, Point());
	}
	
	namedWindow("Original image", WINDOW_AUTOSIZE);
	namedWindow("Processed image", WINDOW_AUTOSIZE);
	imshow("Original image", image);
	imshow("Processed image", image_processed);


	imwrite("Rune1_processed_binary.jpg", image_processed);
	waitKey(0);
	return 0;
}
