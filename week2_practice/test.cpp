#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include<iostream>
#include<curses.h>           // may have to modify this line if not using Windows
#include<algorithm>
#define CV_BGR2GRAY cv::COLOR_BGRA2GRAY
///////////////////////////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace cv;
const int THRESHOLD_1 = 197;
const int THRESHOLD_2 = 212;

void window_printing(int index, Mat image, Mat image_processed){
	string original_window_title, processed_window_title;   
    original_window_title = "Original image " + to_string(index);
    processed_window_title = "Processed image " + to_string(index);
    namedWindow(original_window_title, WINDOW_AUTOSIZE);
    namedWindow(processed_window_title, WINDOW_AUTOSIZE);
    imshow(original_window_title, image);
    imshow(processed_window_title, image_processed);
}


void print_all_contour_location(vector<vector<Point>> contour){
    int index = 0;
    for(vector<vector<Point>>::iterator it = contour.begin(); it != contour.end(); it++, index++){
        cout << "contour " << index << "(" <<  (*it)[0].x << " ," << (*it)[0].y << "), number of points:" << (*it).size() << endl;
    }
}

void draw_all_contours(Mat image, vector<vector<Point>> contour, vector<Vec4i> hierarchy, Scalar color, double thickness){
    for (int i = 0; i < contour.size(); i++) {
		drawContours(image, contour, i, color, thickness, 8, hierarchy, 0, Point());
	}
}

vector<vector<Point>> contour_filtering_for_rectangle(vector<vector<Point>> contour, int minimum_number_of_point, double height_to_width_scale_threshold, double percentage_error){
    //filter for the useless small porint first
    int index = 0;
    for (vector<vector<Point>>::iterator it = contour.begin(); it != contour.end(); index++){
        // cout << "contour " << index << "(" <<  (*it)[0].x << " ," << (*it)[0].y << "), number of points:" << (*it).size() << endl;
        if ((*it).size() < minimum_number_of_point){
            /*cout << "erased " << endl;*/
            it = contour.erase(it);
            
        }
        else{
           
            //try to calculate its dimension and ratio
            Rect rect = boundingRect((*it));
            
            Point top_left_corner = rect.tl();
            Point buttom_right_corner = rect.br() - Point(1, 1);
            
            cout << "(" << top_left_corner.x << ',' << top_left_corner.y << ") (" << buttom_right_corner.x << ", " << buttom_right_corner.y << ")" << endl;
            double rect_length = buttom_right_corner.x - top_left_corner.x;
            double rect_width = buttom_right_corner.y - top_left_corner.y;
            double actual_height_to_width_scale = rect_length/rect_width;
            double error = abs(actual_height_to_width_scale - height_to_width_scale_threshold)/ height_to_width_scale_threshold;
                
            cout << "length:" << rect_length << " width:" << rect_width << endl;
            if(error > percentage_error){
                cout << "exceed the ratio, rejected with ratio"<< round(rect_length/rect_width) <<  endl;
                it = contour.erase(it);
            }
            else{
                ++it;
                cout << "qualifed" << endl;
            } 
        }
        
    }
    cout << "finsihed";
    return contour;
} 
int main() {
    Mat image[2], image_copied[2],  image_gray[2];
	Mat image_processed[2];
    vector<vector<Point>> contour1, contour2;
	vector<Vec4i> hierarchy1, hierarchy2;
	image[0] = imread("Rune1.jpg");
    image[1] = imread("Rune2.png");
	// Validation of the image
	if (!image[0].data && !image[1].data) {
		cout << "I couldnt read the image";
		return -1;
	}
	else {
		for(int i = 0; i < 2; i++){
            image[i].copyTo(image_copied[i]);
        }
	}

	// First convert it into grayscale picture
    for(int i = 0; i < 2; i++){ 
        cvtColor(image_copied[i], image_gray[i], CV_BGR2GRAY);
    }
    
// then do thresholding to convert in into a binary picture separately
    threshold(image_gray[0], image_processed[0], THRESHOLD_1, 255, THRESH_BINARY);
    threshold(image_gray[1], image_processed[1], THRESHOLD_2, 255, THRESH_BINARY);
    // Finding contour, contours mean a closed curve
    findContours(image_processed[0], contour1, hierarchy1, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
    findContours(image_processed[1], contour2, hierarchy2, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
    cout << "number of parent contour1 detected" << "using threshold " << THRESHOLD_1 << ": " <<  contour1.size() << endl;
    cout << "number of parent contour2 detected" << "using threshold " << THRESHOLD_2 << ": " <<  contour2.size() << endl;
    contour1 = contour_filtering_for_rectangle(contour1, 10, 2, 0.5);
    contour2 = contour_filtering_for_rectangle(contour2, 10, 2, 0.5);
    draw_all_contours(image_copied[0], contour1, hierarchy1, (0, 0, 255), 2);
    draw_all_contours(image_copied[1], contour2, hierarchy2, (0, 0, 255), 2);
    for (int i = 0; i < 2; i++){
        image_processed[i] = image_copied[i];
        window_printing(i, image[i], image_processed[i]);
    }

	//imwrite("Rune1_processed_binary.jpg", image_processed);
	waitKey(0);
	return 0;
}
