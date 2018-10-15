#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <algorithm>
#include <curses.h>

using namespace std;
using namespace cv;

vector<vector<Point>> contour_filtering(vector<vector<Point>> contour, pair<int, int> conut, double height_to_width_ratio, double percentage_error);

Mat image_binary_processing(Mat image, int thres);

bool sort_by_size(pair<int, int> const& lhs, pair<int, int> const& rhs);

vector<Mat> image_cropping(vector<vector<Point>> contour, int num); 

int main(){
    VideoCapture cap("10.1_Assignment_video_clip.mp4");
    Mat imgFrame, image_copied, image_binary, image_processed;
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
        cout << "showed";
        imgFrame.copyTo(image_copied);
        image_binary = image_binary_processing(image_copied, 210);
        findContours(image_binary, contour, hierarchy,  CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE, Point(0,0));
       
        //draw contour without processing
        for (int i = 0; i < contour.size(); i++){
            drawContours(image_copied, contour, i, Scalar(0, 0,255), 3, LINE_8, hierarchy, 0, Point());
        }
        contour = contour_filtering(contour, make_pair(10, 220), 1.5, 0.3);
        image_cropping(contour, 9);
        //draw contour after filtering
        for (int i = 0; i < contour.size(); i++){
            drawContours(image_copied, contour, i, Scalar(0, 255, 0), 3, LINE_8, hierarchy, 0, Point());
        }
        
        image_processed = image_copied;
        if((cap.get(CV_CAP_PROP_POS_FRAMES) + 1) < cap.get(CV_CAP_PROP_FRAME_COUNT)){
            cap.read(imgFrame);
        }
    
        else{
            cout << "the end of video\n";
            break;
    
        }

        //show the processed frame
        imshow("Processed", image_processed);
        chCheckForEscKey = waitKey(100);
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

vector<vector<Point>> contour_filtering(vector<vector<Point>> contour, pair<int, int> count, double height_to_width_ratio, double percentage_error){
    // 1. filter for the useless small porint first
    int index = 0;
    vector<pair<int ,int>> mapping_sizeToIndex;
    vector<vector<Point>> filtered_contour;
    for (vector<vector<Point>>::iterator it = contour.begin(); it != contour.end(); index++){
        //cout << "contour " << index << "(" <<  (*it)[0].x << " ," << (*it)[0].y << "), number of points:" << (*it).size() << endl;
        Rect rect = boundingRect((*it)); 
        Point top_left_corner = rect.tl();
        Point buttom_right_corner = rect.br() - Point(1, 1);
        double rect_length = buttom_right_corner.x - top_left_corner.x;
        double rect_width = buttom_right_corner.y - top_left_corner.y;
        double rect_scale = rect_length/rect_width;
        double error = abs(rect_scale - height_to_width_ratio)/ height_to_width_ratio;
        if (!((*it).size() > count.first && (*it).size() < count.second) || error > percentage_error){
            //cout << "erased " << endl;
            it = contour.erase(it);
            
        }
        else{
           //cout << "passed" << endl;
           ++it;
        }
       //cin.get(); i
    }
        
    
    // cout << "passed first filtering";  
    // 2. after erasing the invalid count, try to calculate its dimension, size and ratio
    index = 0;
    for (vector<vector<Point>>::iterator it = contour.begin(); it != contour.end(); index++, ++it){       
        Rect rect = boundingRect((*it));
        Point top_left_corner = rect.tl();
        Point buttom_right_corner = rect.br() - Point(1, 1);
            
        double rect_length = buttom_right_corner.x - top_left_corner.x;
        double rect_width = buttom_right_corner.y - top_left_corner.y;
        //cout << "(" << rect_length << ',' << rect_width << ")";
        int rect_area = rect_length * rect_width;
        //cout << "area of rect " << index << ": " <<  rect_area << endl;
        // cin.get();
        mapping_sizeToIndex.push_back(make_pair(rect_area, index));
        
    }

   
    // sort them by size, largest come first
    sort(mapping_sizeToIndex.begin(), mapping_sizeToIndex.end(), sort_by_size);
    
    for(int i = 0; i < 11;i++){
        // assign the largest 9 contour to the filtered_contour
       /* cout << "assigned rect" << mapping_sizeToIndex[i].second << "with area" << mapping_sizeToIndex[i].first << endl;*/
        filtered_contour.push_back(contour[mapping_sizeToIndex[i].second]);
        cout << mapping_sizeToIndex[i].first << " " << mapping_sizeToIndex[i].second << endl;
    }

    cout << "finsihed";
    return filtered_contour;
}

vector<Mat> image_cropping(vector<vector<Point>> contour, int num){
    vector<Mat> img;
    for (int i = 0; i < num; i++){
        Rect rect = boundingRect(contour[i]);
        //Rect myROI = (rect.tl().x , rect.br().y, rect.width, rect.height);
        //img.push_back(rect(myROI));

    }
    return img;
}

bool sort_by_size(pair<int, int> const& lhs, pair<int, int> const& rhs){
    return lhs.first > rhs.first;
}
