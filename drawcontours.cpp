
#include <string>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
       cout << "incorrect command"<< endl;
    }

    Mat binaryImage = imread(argv[1],0);	
	
	Mat colorImage = imread("Rune1.jpg", 1);
    vector<vector<Point> > contours;
    findContours( binaryImage, contours, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE );

cout << contours.size();

for(int i= 0; i < contours.size(); i++)
{
    for(int j= 0; j < contours[i].size();j++) // run until j < contours[i].size();
    {
        cout << contours[i][j] << endl; //do whatever
    }
}

vector<vector<Point> > newcontours;
newcontours = boundingRect(contours);

 //Mat image = Mat::zeros(binaryImage.rows, binaryImage.cols, CV_8UC3);

    //int idx = 0;
    //for( ; idx >= 0; idx = hierarchy[idx][0] )
    //{
        drawContours(colorImage , newcontours, -1, Scalar(0,0,255), 1);
    //}
    imshow("window", colorImage);
waitKey(0);
    return 0;

}
