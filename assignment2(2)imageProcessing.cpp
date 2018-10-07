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



/*

int threshold_value = 0;
int threshold_type = 3;
int const max_value = 255;
int const max_type = 4;
int const max_binary_value = 255;
Mat src, src_gray, dst;
const char* window_name = "Threshold Demo";
const char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
const char* trackbar_value = "Value";

static void Threshold_Demo( int, void* )
{
    /* 0: Binary
     1: Binary Inverted
     2: Threshold Truncated
     3: Threshold to Zero
     4: Threshold to Zero Inverted
    */
    
    threshold( src_gray, dst, threshold_value, max_binary_value, threshold_type );
    if (dst.size().width>0 && dst.size().height>0 ){
	imshow( window_name, dst );
    }
	     
}

*/

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

	Mat gray_src;
	cvtColor(src, gray_src, COLOR_BGR2GRAY);

	/*
	
	namedWindow( window_name, WINDOW_AUTOSIZE ); // Create a window to display results
    createTrackbar( trackbar_type,
                    window_name, &threshold_type,
                    max_type, Threshold_Demo ); // Create Trackbar to choose type of Threshold
    createTrackbar( trackbar_value,
                    window_name, &threshold_value,
                    max_value, Threshold_Demo ); // Create Trackbar to choose Threshold value
    Threshold_Demo( 0, 0 ); // Call the function to initialize

	*/
	Mat binary_src;
	if (i==1){
		threshold( gray_src, binary_src, 196, max_binary_value, 0 );
	}else if (i==2){
		threshold( gray_src, binary_src, 221, max_binary_value, 0 );
	}
	namedWindow( "Binary: ", WINDOW_AUTOSIZE );
	imshow( "Binary: ", binary_src ); 

    waitKey(0); // Wait for a keystroke in the window
    return 0;
}
