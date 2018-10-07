#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
    cout << "OpenCV version:" << CV_VERSION << endl;
    cout << "Major version:" << CV_MAJOR_VERSION << endl;
    cout << "Minor version:" << CV_MINOR_VERSION << endl;
    cout << "Subminor version:" << CV_SUBMINOR_VERSION << endl;

    char x = cin.get();
    return 0;
}
