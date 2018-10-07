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
		Mat frame;
		cap >> frame;
		if(frame.empty())
			break;
		imshow("Frame", frame);
		char c=(char)waitKey(500);
		if (c==27)
			break;
	}
	cap.release();
	destroyAllWindows();
	return 0;
}
