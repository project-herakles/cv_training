#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

void findRect(vector<vector<Point>> contours);
vector<Rect> boundRect(9);

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
		Mat frame, processedFrame;
		cap >> frame;
		if(frame.empty())
			break;
		//convert to grayscale
		cvtColor( frame, processedFrame, CV_BGR2GRAY );
		//convert to binary
		threshold( processedFrame, processedFrame, 210, 255, 0 );
		//blur the image to eliminate edges details
		GaussianBlur( processedFrame, processedFrame, Size (13, 13), 0, 0 );
		//finding contours
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours ( processedFrame, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_L1 );
		//function call to find Rect object of the number on the screen
		findRect (contours);
		//display the cropped part of frame
		for (int i = 0; i < 9; i++)
		{
			rectangle (frame, boundRect[i], Scalar(0, 255, 0));
	  		Rect myROI (boundRect[i].tl().x, boundRect[i].tl().y, boundRect[i].br().x-boundRect[i].tl().x, boundRect[i].br().y-boundRect[i].tl().y);
	  		Mat croppedImage = frame(myROI);
	  		string message = "image"+to_string(i+1);
	  		imshow( message, croppedImage );
		}
		//display original frame
		imshow("Frame", frame);
		char c=(char)waitKey(25);
		if (c==27)
			break;
	}
	cap.release();
	destroyAllWindows();
	return 0;
}

bool sortMethod(pair<int, int> const& lhs, pair<int, int> const&rhs) {return (lhs.first > rhs.first);}

void findRect(vector<vector<Point>> contours)
{
	//loop through all the contours, find the contours with 50 or less coordinates(more likely to be the area enclosed) , calculate its area and store it
	int size = contours.size();
	vector<pair<int, int> > pair_index;
	for (int i = 0; i < size; i ++ )
	{
		if(contours[i].size() > 50)
		{
			pair_index.push_back(make_pair(0, i));
		}
		else
		{
			pair_index.push_back(make_pair( contourArea(contours[i]),i));
		}
	}
	//take the biggest 9 area and store its Rect area
	sort(pair_index.begin(), pair_index.end(), sortMethod);
	for (int i = 0; i < 9; i ++)
	{
		boundRect[i]=boundingRect(contours[pair_index[i].second]);
	}
}
