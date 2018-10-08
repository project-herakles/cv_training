#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using namespace cv;

bool cmp(const vector<Point> &lhs, const vector<Point> &rhs)
{
    return contourArea(lhs) > contourArea(rhs);
}

string window_name(int num)
{
    string str = "Number ";
    return str + (char)(num + '0');
}

const vector<pair<int, int>> coord = { {0, 0}, {0, 100}, {0, 200}, {100, 0}, {100, 100}, {100, 200}, {200, 0}, {200, 100}, {200, 200} };

int main()
{
    VideoCapture video("10.1_Assignment_video_clip.mp4");
    namedWindow("Video feed", WINDOW_AUTOSIZE);
    for(int i = 1; i <= 9; i++)
        namedWindow(window_name(i), WINDOW_AUTOSIZE);
    Mat frame, bw, cropped[10];
    while(video.isOpened())
    {
        video.read(frame);
        imshow("Video feed", frame);
        moveWindow("Video feed", 0, 0);
        cvtColor(frame, bw, COLOR_RGB2GRAY);
        threshold(bw, bw, 200, 255, THRESH_BINARY);
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(bw, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
        vector<vector<Point>> vec;
        for( int i = 0; i< contours.size(); i++ )
        {
            Rect rect = boundingRect(contours[i]);
            if(rect.width * rect.height < 10)
                continue;
            if(fabs(contourArea(contours[i]) / (rect.width * rect.height) - 1.0) > 0.5)
                continue;
            if(fabs(rect.width * 1.0 / rect.height - 49.0/27.0) > 0.2)
                continue;
            vec.push_back(contours[i]);
        }
        sort(vec.begin(), vec.end(), cmp);
        for( int i = 0; i < (vec.size() > 9 ? 9 : vec.size()); ++i)
        {
            cropped[i] = frame(boundingRect(vec[i]));
            imshow(window_name(i+1), cropped[i]);
            moveWindow(window_name(i+1), coord[i].first, coord[i].second);
        }
        waitKey(100);
    }
    waitKey(0);
    return 0;
}
