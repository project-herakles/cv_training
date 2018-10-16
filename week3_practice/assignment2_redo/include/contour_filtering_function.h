#ifndef CONTOUR_FILTERING_FUNCTION_H
#define CONTOUR_FILTERING_FUNCITON_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <algorithm>
#include <curses.h>
#include <vector>

using namespace std;
using namespace cv;

vector<vector<Point>> contour_filtering_size(vector<vector<Point>> inputContour, int minSize, int maxSize);
vector<vector<Point>> contour_filtering_area(vector<vector<Point>> inputContour, int numOfContour);
vector<vector<Point>> contour_filtering_position(vector<vector<Point>> inputContour, int maxError);
vector<vector<Point>> contour_filtering_ratio(vector<vector<Point>> inputContour, double heightTowidthRatio, double percentageError);
bool sort_by_area(pair<int, int> const& lhs, pair<int, int> const& rhs);
bool sort_by_coordinate(pair<Point, int> const& lhs, pair<Point, int> const& rhs);
#endif
