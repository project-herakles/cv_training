void colorReduce(Mat &image){
  int nl = image.rows;
  int nc = image.cols;
  for (int j=2; j<nl-2; j=j+3){//from 3rd rows, until the last 2 rows
    const unchar* r1= image.ptr<const unchar>(j-2);
    const unchar* r2= image.ptr<const unchar>(j-1);
    const unchar* r3= image.ptr<const unchar>(j);
    const unchar* r4= image.ptr<const unchar>(j+1);
    const unchar* r5= image.ptr<const unchar>(j+2);
    for (int i=2; i<nc-2; i=i+3){
      if (r3[i]==255)
        r3[i-2] = 255;
        r3[i-1] = 255;
        r3[i+1] = 255;
        r3[i+2] = 255;
    }
  }
}


#include <opencv2/highgui/highgui.hpp>    
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
 
using namespace cv;
using namespace std;

void mydilate(Mat &image,int rangeN);

int main(int argc, char *argv[])
{

	Mat image = imread(argv[1],IMREAD_COLOR);
    
    cvtColor(image, image, COLOR_RGB2GRAY); 
        threshold(image, image, 130, 255, THRESH_BINARY);
        imshow("d",image);
	mydilate(image,-11);
	imshow("w", image);
	waitKey();
	return 0;
}


void mydilate(Mat &image, int rangeN){
  int range = rangeN*(-1);
  int nl = image.rows;
  int nc = image.cols;
  for (int j=range; j<nl-range; j=j+range+1){
    uchar* centerRow = image.ptr<uchar>(j);
      for (int i=range; i<nc-range; i=i+range+1){
        if (centerRow[i]== 255){
         vector<uchar*> r;
         for (int m=rangeN; m<=range; m++)
            r.push_back(image.ptr<uchar>(j+m));
         for (int b=0; b<range*2; b++){
           for (int a=rangeN; a<=range;a++)
             r[b][i+a]=255;  
         }
      }
    }
  }
}



void mydilate(Mat &image, int rangeN){
  Mat copyImage;
  image.copyTo(copyImage);
  int range = rangeN*(-1);
  int nl = image.rows;
  int nc = image.cols;
  for (int j=range; j<nl-range; j=j+1){
    uchar* centerRow = copyImage.ptr<uchar>(j);
      for (int i=range; i<nc-range; i=i+1){
        if (centerRow[i]== 255){
         vector<uchar*> r;
         for (int m=rangeN; m<=range; m++)
            r.push_back(image.ptr<uchar>(j+m));
         for (int b=0; b<range*2; b++){
           for (int a=rangeN; a<=range;a++)
             r[b][i+a]=255;  
         }
      }
    }
  }
}
