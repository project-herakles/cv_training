#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <algorithm>
using namespace cv;
using namespace std;

Mat gray_src;
int thresh = 100;
RNG rng(12345);
int max_binary_value=255;

void threshold( Mat m);

bool comp(const vector<int>& a, const vector<int>& b)  {
	return (a[4]>b[4]);//sort in decending order
	 //why after adding this line, it is ok?
}

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
	//namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
    //imshow( "Display window", src );  // Show our src inside it.

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
	
	//3
	threshold( binary_src);
    waitKey(0); // Wait for a keystroke in the window
    return 0;
}


void threshold(Mat binary_src )//this draws bounding Rect
{
//use canny operation
    Mat canny_output;
    Canny( binary_src, canny_output, thresh, thresh*2 );

//findContours
    vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
    findContours( canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );
	//findContours( canny_output, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE );

//find bounding polygons and then bounding rects
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
		Scalar color = Scalar( 0, 0,255);
        drawContours( drawing1, contours, (int)i, color, 1, LINE_8, hierarchy, 0  );
		color = Scalar( 0, 255,0);
        rectangle( drawing1, boundRect[i].tl(), boundRect[i].br(), color, 1 );
    }
//draw contour and bounding Rect
	namedWindow("contours and bounding Rect", WINDOW_AUTOSIZE);
	imshow( "contours and bounding Rect", drawing1 );
	

	
//calculate area of rects and store them in rectWithArea
	
	Mat drawing2 = Mat::zeros( drawing1.size(), CV_8UC3 );
    
	//we sort boundRect according to size
	cout<<"rectWithArea(sorted with area: "<<endl;
	vector< vector<int> > rectWithArea;
	
/*	
	for (int i=0; i<rectWithArea.size(); i++){
				
		rectWithArea[i][0]=boundRect[i].x;
		rectWithArea[i][1]=boundRect[i].y;
		rectWithArea[i][2]=boundRect[i].width;
		rectWithArea[i][3]=boundRect[i].height;
		rectWithArea[i][4]=(boundRect[i].width)*(boundRect[i].height);
		cout<< "( " ;
		cout<<rectWithArea[i][0]<<" , ";
		cout<<rectWithArea[i][1]<< " , ";
		cout<<rectWithArea[i][2]<<" , ";
		cout<<rectWithArea[i][3]<<" , ";
		cout<<rectWithArea[i][4]<<" ) ";
		cout<<endl;
	}
*/

/*
if you use the code above, you will have a segmentation fault, basically you are accessing memeory wrongly.
THere are two main mistakes in the code above:
(1):
The reason is that when you declare a vector of anything, it does not contain anything yet. 
Those rectWIthArea[i][0] or rectWIthArea[i][4] are not yet created. YOu need to use push_back to create them before accessing them with index!!
(2): 
Before accessing rectWithArea[i], remember that it is also not created yet! "vector< vector<int> > rectWithArea(boundRect.size());" will not allocate memory to the 2D vector!

The correct form is as below:

*/
	for (int i=0; i<boundRect.size(); i++){//note that here you cannot use rectWithArea.size(), it is not yet determined
		vector<int> subVector;
		rectWithArea.push_back(subVector);//you create rectWithArea[i]

		rectWithArea[i].push_back(boundRect[i].x);
		//you create rectWithArea[i][0], later you can access it with rectWithArea[i][0]
		rectWithArea[i].push_back(boundRect[i].y);
		rectWithArea[i].push_back(boundRect[i].width);
		rectWithArea[i].push_back(boundRect[i].height);
		rectWithArea[i].push_back((boundRect[i].width)*(boundRect[i].height));
		cout<< "( " ;
		cout<<rectWithArea[i][0]<<" , ";
		cout<<rectWithArea[i][1]<<" , ";
		cout<<rectWithArea[i][2]<<" , ";
		cout<<rectWithArea[i][3]<<" , ";
		cout<<rectWithArea[i][4]<<" ) ";
		cout<<endl;
	}
	
//sort rectWithArae
	sort(rectWithArea.begin(), rectWithArea.end(), comp);

	
//elimitenate rects that are almost the same, make the redundant one all 0
	for (int i=0; i<boundRect.size(); i++){//note that here you cannot use rectWithArea.size(), it is not yet determined
		for (int j=i+1; j<boundRect.size() ; j++){
		
		if ( abs(rectWithArea[i][0]-rectWithArea[j][0]) +  abs(rectWithArea[i][1]-rectWithArea[j][1]) + abs(rectWithArea[i][2]-rectWithArea[j][2]) +abs(rectWithArea[i][3]-rectWithArea[j][3])  <10 ){
			for (int k=0; k< 5; k++){
				rectWithArea[j][k]=0;
			}
		}
		
		}
	}

//sort again
	sort(rectWithArea.begin(), rectWithArea.end(), comp);
	
//draw the largest rectangels
	for (int i=0; i<9; i++){
/*
		if (rectWithArea[i].size()==5){//why it works after this line???
			cout<<i;
			cout<< "( " ;
			cout<<rectWithArea[i][0]<<" , ";
			cout<<rectWithArea[i][1]<<" , ";
			cout<<rectWithArea[i][2]<<" , ";
			cout<<rectWithArea[i][3]<<" , ";
			cout<<rectWithArea[i][4]<<" ) ";
			cout<<endl;
		}
*/
			cout<<i;
			cout<< "( " ;
			cout<<rectWithArea[i][0]<<" , ";
			cout<<rectWithArea[i][1]<<" , ";
			cout<<rectWithArea[i][2]<<" , ";
			cout<<rectWithArea[i][3]<<" , ";
			cout<<rectWithArea[i][4]<<" ) ";
			cout<<endl;
			
			Rect temp;
			temp.x=rectWithArea[i][0];
			temp.y=rectWithArea[i][1];
			temp.width=rectWithArea[i][2];
			temp.height=rectWithArea[i][3];
			Scalar color=Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255) );
			rectangle( drawing2, temp.tl(), temp.br(), color, 2 );
			
	}
	
	namedWindow("9 squares", WINDOW_AUTOSIZE);
	imshow( "9 squares", drawing2 );
	cout<<"end"<<endl;
}


