#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <string>
using namespace cv;
using namespace std;

Mat gray_src;
int thresh = 100;
RNG rng(12345);
int max_binary_value=255;

void threshold( );

int main( int argc, char** argv )
{
    //2.a
    String srcName( "Rune2" ); // by default or we can ask the user for input
    int i=0;

    if( argc > 1)
    {
        srcName = argv[1];
    }
    Mat src;

    src = imread( srcName+".jpg", IMREAD_COLOR ); 
    i=1;
    if (src.empty()){
		src = imread( srcName+".png", IMREAD_COLOR );
        i=2;
    }

    if( src.empty() )                      
    {
        cout <<  "Could not open or find the src" << std::endl ;
        return -1;
    }
	namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
    imshow( "Display window", src );  // Show our src inside it.

    //2.b
    cout<<srcName<<" size: "<<src.size()<<endl;
    Mat src_copy=src.clone();
    imwrite(srcName+"_copy.jpg", src_copy);

	//2.c
	String srcFile;
	if (i==1){
		srcFile=srcName+=".jpg";
	}else if(i==2){
		srcFile=srcName+=".png";
	}

	cvtColor(src, gray_src, COLOR_BGR2GRAY);

	Mat binary_src;
	if (i==1){
		threshold( gray_src, binary_src, 196, max_binary_value, 0 );
	}else if (i==2){
		threshold( gray_src, binary_src, 221, max_binary_value, 0 );
	}
	namedWindow( "Binary: ", WINDOW_AUTOSIZE );
	imshow( "Binary: ", binary_src ); 

	threshold( );

    waitKey(0); // Wait for a keystroke in the window
    return 0;
}

void threshold( )
{
    Mat canny_output;
    Canny( gray_src, canny_output, thresh, thresh*2 );
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
		color = Scalar( 0, 0,255);
        drawContours( drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0 );
    }
    imshow( "Contours", drawing );
}
