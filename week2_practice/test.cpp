#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include<iostream>
#include<curses.h>           // may have to modify this line if not using Windows
#define CV_BGR2GRAY cv::COLOR_BGRA2GRAY
///////////////////////////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace cv;

int main() {
	string original_window_title, processed_window_title;   
    Mat image[2], image_copied[2],  image_gray[2];
	Mat image_processed[2];
    vector<vector<Point>> contour1, contour2;
	vector<Vec4i> hierarchy1, hierarchy2;
	image[0] = imread("Rune1.jpg");
    image[1] = imread("Rune2.png");
	// Validation of the image
	if (!image[0].data && !image[1].data) {
		cout << "I couldnt read the image";
		return -1;
	}
	else {
		for(int i = 0; i < 2; i++){
            image[i].copyTo(image_copied[i]);
        }
	}

	// First convert it into grayscale picture
    for(int i = 0; i < 2; i++){ 
        cvtColor(image_copied[i], image_gray[i], CV_BGR2GRAY);
	    // then do thresholding to convert in into a binary picture
	    threshold(image_gray[i], image_processed[i], 200, 200, THRESH_BINARY);
    }
    
    // Finding contour, contours mean a closed curve
    findContours(image_processed[0], contour1, hierarchy1, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
    cout << "number of parent contours1 detected:" << contour1.size() << endl;
/*	for (int i = 0; i < contour.size(); i++) {
		drawContours(image_processed, contour, i, Scalar(0, 0, 255), 3, 8, hierarchy, 0, Point());
	}*/
	for(int i = 0; i < 2; i++){
        original_window_title = "Original image" + to_string(i);
	    processed_window_title = "Processed image" + to_string(i);
        namedWindow(original_window_title, WINDOW_AUTOSIZE);
	    namedWindow(processed_window_title, WINDOW_AUTOSIZE);
	    imshow(original_window_title, image[i]);
	    imshow(processed_window_title, image_processed[i]);
    }

	//imwrite("Rune1_processed_binary.jpg", image_processed);
	waitKey(0);
	return 0;
}
