#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	VideoCapture cap("10.1_Assignment_video_clip.mp4");
	if (!cap.isOpened())
	{
		cout << "Error in opening video" << endl;
		return -1;
	}

	while(1)
	{
		//cap.set(CAP_PROP_FPS, 0.1);
		Mat frame, processedFrame;
		cap >> frame;
		if(frame.empty())
			break;
		cvtColor( frame, processedFrame, CV_BGR2GRAY );
		threshold( processedFrame, processedFrame, 170, 255, 0 );
		//fastNlMeansDenoising( processedFrame, processedFrame, 20 );
		GaussianBlur( processedFrame, processedFrame, Size (11, 11), 0, 0 );
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours ( processedFrame, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_L1 );
		drawContours( frame, contours, -1, Scalar(0, 255, 0), 1 );
		imshow("Frame", frame);
		char c=(char)waitKey(25);

		if (c==27)
			break;
	}
	cap.release();
	destroyAllWindows();
	return 0;
}
