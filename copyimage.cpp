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

    string imageName = argv[1];
    Mat image;
    image = imread(imageName,IMREAD_COLOR);

    Mat copy;
    image.copyTo(copy);

    int length= imageName.length();
    string copyName = imageName.substr(0,length-4) + "_copy.jpg";
    
    imwrite(copyName,copy);


    return 0;

}
