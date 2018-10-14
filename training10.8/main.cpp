#include <iostream>
#include <vector>
#include "armorfind.h"

using namespace std;
using namespace cv;

Mat contourThreadkernel = getStructuringElement(MORPH_ELLIPSE, Size(9,9));

void ContourThread(int mode,Mat input,Mat &binary,int thres,vector<vector<Point> > &contours)
{
    Mat thres_whole;
    vector<Mat> splited;
    split(input,splited);
    cvtColor(input,thres_whole,CV_BGR2GRAY);
    threshold(thres_whole,thres_whole,100,255,THRESH_BINARY);
    if(mode == 0){
        subtract(splited[2],splited[0],binary);
        threshold(binary,binary,thres,255,THRESH_BINARY);
    }else{
        subtract(splited[0],splited[2],binary);
        threshold(binary,binary,thres,255,THRESH_BINARY);
    }
    dilate(binary,binary,contourThreadkernel);
    binary = binary & thres_whole; 
    findContours(binary,contours,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE);
}

vector<vector<Point>> contours;
vector<Point> armor_result; 
Mat pic_input, pic_output, binary;

int main()
{
	VideoCapture cap ("video.mp4");
	if (!cap.isOpened())
	{
		cout << "Error in opening video" << endl;
		return -1;
	}
	while(1)
	{
		cap >> pic_input;
		if(pic_input.empty())
		{
			break;
		}
		ArmorFind finder;
		ContourThread(1, pic_input, binary,60,contours); 
		finder.process(contours, pic_input, pic_output, armor_result, false);
		imshow ("output", pic_output);
		char c = (char)waitKey(25);
		if (c==27)
			break;
	}
	cap.release();
	destroyAllWindows();
	return 0;
}
