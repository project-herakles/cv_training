#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <string>
#include <algorithm>
using namespace cv;
using namespace std;

Mat gray_src;
int thresh = 100;
RNG rng(12345);
int max_binary_value=255;

void threshold0( Mat m);
void threshold1( Mat m);
bool comp(const vector<int>& , const vector<int>& ) ;
//why it does not work with name compare??????

int main( int argc, char** argv )
{
    //2.a
    String srcName( "Rune2" ); // by default or we can ask the user for input
    int i=0;

    if( argc > 1)
    {
        srcName = argv[4];
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
	//threshold0( binary_src);
	threshold1( binary_src);
	
    waitKey(0); // Wait for a keystroke in the window
    return 0;
}

void threshold0(Mat binary_src )//this draws contour
{
    Mat canny_output;
    Canny( binary_src, canny_output, thresh, thresh*2 );
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
	
	//print out contours
	/*
	for (int i=0; i<contours.size(); i++){
		for (int j=0; j<contours[i].size(); j++){
			cout<< "( " << contours[i][j].x<<" , "<<contours[i][j].y<<" )  ";
		}
		cout<<endl;
	}
	*/	
	
}

void threshold1(Mat binary_src )//this draws bounding Rect
{
    Mat canny_output;
    Canny( binary_src, canny_output, thresh, thresh*2 );

    vector<vector<Point> > contours;
    //findContours( canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );
	findContours( canny_output, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE );

    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );

    for( size_t i = 0; i < contours.size(); i++ )
    {
        approxPolyDP( contours[i], contours_poly[i], 3, true );
        boundRect[i] = boundingRect( contours_poly[i] );
    }

    Mat drawing1 = Mat::zeros( canny_output.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
		color = Scalar( 0, 0,255);
        drawContours( drawing1, contours_poly, (int)i, color );
		color = Scalar( 0, 255,0);
        rectangle( drawing1, boundRect[i].tl(), boundRect[i].br(), color, 2 );
    }
	//print out pointes of bounding rectangles.
	cout<<"boudRect:"<<endl;
	for (int i=0; i<boundRect.size(); i++){
		
		cout<< "( " << boundRect[i].x<<" , "<<boundRect[i].y<< " , " <<boundRect[i].width<<" , "<<boundRect[i].height<<" ) ";
		cout<<endl;
	}

	imshow( "Contours1", drawing1 );

	
	//We can take only those 9 largest rectangles in the list and draw them on the secreen
	
	Mat drawing2 = Mat::zeros( canny_output.size(), CV_8UC3 );
    
	//we sort boundRect according to size
	cout<<"rectWithArea(sorted with area): "<<endl;
	vector< vector<int> > rectWithArea(contours.size());

	for (int i=0; i<boundRect.size(); i++){
		rectWithArea[i][0]=boundRect[i].x;
		rectWithArea[i][1]=boundRect[i].y;
		rectWithArea[i][2]=boundRect[i].width;
		rectWithArea[i][3]=boundRect[i].height;
		rectWithArea[i][4]=(boundRect[i].width)*(boundRect[i].height);
	}
	sort(rectWithArea.begin(), rectWithArea.end(), comp);
	
	for (int i=0; i<rectWithArea.size(); i++){
		cout<< "( " ;
		cout<<rectWithArea[i][0]<<" , ";
		cout<<rectWithArea[i][1]<< " , ";
		cout<<rectWithArea[i][2]<<" , ";
		cout<<rectWithArea[i][3]<<" , ";
		cout<<rectWithArea[i][4]<<" ) ";
		cout<<endl;
	}

	vector<Rect> nineRects(9);
	for (int i=0; i<9; i++){
		nineRects[i].x=rectWithArea[i][0];
		nineRects[i].y=rectWithArea[i][1];
		nineRects[i].width=rectWithArea[i][2];
		nineRects[i].height=rectWithArea[i][3];
	}

	for (int i=0; i<nineRects.size();i++){
		Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
		rectangle( drawing2, nineRects[i].tl(), nineRects[i].br(), color, 2 );
	}
	
}

bool comp(const vector<int>& a, const vector<int>& b)  {
	return (a[1]>b[1]);//sort in decending order
}
