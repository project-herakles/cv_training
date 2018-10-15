#include <iostream>
#include <vector>
#include "armorfind.h"

using namespace std;
using namespace cv;
//Neo - define the kernel (size of matrix) used for dilation of the image
Mat contourThreadkernel = getStructuringElement(MORPH_ELLIPSE, Size(9,9));
//Neo - find the contour of the input image
void ContourThread(int mode,Mat input,Mat &binary,int thres,vector<vector<Point> > &contours)
{
    Mat thres_whole;
    vector<Mat> splited;
    //Neo - split the input image into 3 color channels namely Blue, Green and Red
    split(input,splited);
    cvtColor(input,thres_whole,CV_BGR2GRAY);
    threshold(thres_whole,thres_whole,100,255,THRESH_BINARY);
    if(mode == 0){
	//for finding red target, substract the blue channel from the red channel (for clearer contours)
        subtract(splited[2],splited[0],binary);
        threshold(binary,binary,thres,255,THRESH_BINARY);
    }else{
	//for finding blue target, substract the red channel from the blue channel (for clearer contours)
        subtract(splited[0],splited[2],binary);
        threshold(binary,binary,thres,255,THRESH_BINARY);
    }
    //Neo - dilate the binary image with the kernel size specified above (dilation increases the size of a white area of the binary image)
    dilate(binary,binary,contourThreadkernel);
    //Neo - combining the dilated binary image with the initial threshold processed image to get the details of the possible light bar parts
    binary = binary & thres_whole; 
    //Neo - find the contour of the final processed binary image
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
