#include <stdio.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
       cout << "incorrect command"<< endl;
    }

    string copyName = argv[1];
    Mat copyImage;
    copyImage = imread(copyName,IMREAD_COLOR);

    Mat grayImage,binaryImage;
    cvtColor(copyImage, grayImage, COLOR_RGB2GRAY); //convert to a grayscale image (only by light)
    // what is threshold value and max?????????
    threshold(grayImage, binaryImage,100,255,THRESH_BINARY); //works better
    
    int length= copyName.length();
    string binaryName = copyName.substr(0,length-9) + "binary.jpg";
    imwrite(binaryName,binaryImage);


    return 0;

}
