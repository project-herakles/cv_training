#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <algorithm>
#include <curses.h>

using namespace std;
using namespace cv;

vector<vector<Point>> contour_filtering(vector<vector<Point>> contour, int min_conut, double height_to_width_ratio, double percentage_error);
Mat image_binary_processing(Mat image, int thres);
bool sort_by_size(pair<int, int> const& lhs, pair<int, int> const& rhs);

int main(){
    VideoCapture cap("10.1_Assignment_video_clip.mp4");
    Mat imgFrame, image_copied, image_binary;
    vector<vector<Point>> contour;
    vector<Vec4i> hierarchy;
    if(!cap.isOpened()){
        cout << "can't read the video";
        waitKey(0);
        return(-1);
    }

    if(cap.get(CV_CAP_PROP_FRAME_COUNT) < 1){
        cout << "error: video file must have at least one frame";
        waitKey(0);
        return(-1);
    }

    cap.read(imgFrame);
    
    char chCheckForEscKey = 0;
    while (cap.isOpened() && chCheckForEscKey != 27){
        imshow("imgFrame", imgFrame);
        imgFrame.copyTo(image_copied);
        image_binary = image_binary_processing(image_copied, 200);
        findContours(image_binary, contour, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0,0));
        contour_filtering(contour, 10, 2, 0.3);
        if((cap.get(CV_CAP_PROP_POS_FRAMES) + 1) < cap.get(CV_CAP_PROP_FRAME_COUNT)){
            cap.read(imgFrame);
        }
    
        else{
            cout << "the end of video\n";
            break;
    
        }

        //show the processed frame
        imshow("Processed", image_binary);
        waitKey(30);
    }

    if (chCheckForEscKey != 27){
        waitKey(0);
    }
    return(0);
}

Mat image_binary_processing(Mat image, int thres){
    Mat image_gray, image_binary;
    cvtColor(image, image_gray, CV_BGR2GRAY);
    threshold(image_gray, image_binary, thres, 255, THRESH_BINARY);
    return image_binary;
}

vector<vector<Point>> contour_filtering(vector<vector<Point>> contour, int min_count, double height_to_width_ratio, double percentage_error){
   //filter for the useless small porint first
    int index = 0;
    vector<pair<int ,int>> mapping_sizeToIndex;
    for (vector<vector<Point>>::iterator it = contour.begin(); it != contour.end(); index++){
        // cout << "contour " << index << "(" <<  (*it)[0].x << " ," << (*it)[0].y << "), number of points:" << (*it).size() << endl;
        if ((*it).size() < min_count){
            /*cout << "erased " << endl;*/
            it = contour.erase(it);
            
        }
    }
           
    // after erasing the invalid count, try to calculate its dimension, size and ratio
    for (vector<vector<Point>>::iterator it = contour.begin(); it != contour.end(); index++, it++){       
        Rect rect = boundingRect((*it));
            
        Point top_left_corner = rect.tl();
        Point buttom_right_corner = rect.br() - Point(1, 1);
            
        cout << "(" << top_left_corner.x << ',' << top_left_corner.y << ") (" << buttom_right_corner.x << ", " << buttom_right_corner.y << ")" << endl;
        double rect_length = buttom_right_corner.x - top_left_corner.x;
        double rect_width = buttom_right_corner.y - top_left_corner.y;
        int rect_area = rect_length * rect_width;
        mapping_sizeToIndex.push_back(make_pair(rect_area, index));
        
    }
    // sort them by size, largest come first
    sort(mapping_sizeToIndex.begin(), mapping_sizeToIndex.end(), sort_by_size);
    contour.erase(contour.begin()+9, contour.end());
    cout << "finsihed";
    return contour;
}

bool sort_by_size(pair<int, int> const& lhs, pair<int, int> const& rhs){
    return lhs.first > rhs.first;
}
