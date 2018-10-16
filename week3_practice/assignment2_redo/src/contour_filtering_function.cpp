#include "contour_filtering_function.h"
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

vector<vector<Point>> contour_filtering_size(vector<vector<Point>> inputContour, int minSize, int maxSize){
    for (vector<vector<Point>>::iterator it = inputContour.begin(); it != inputContour.end();){
        if (it->size() >= minSize && it->size() <= maxSize){
            ++it;
        }
        else{
            it = inputContour.erase(it);            
        }
    }
    return inputContour;
}



vector<vector<Point>> contour_filtering_area(vector<vector<Point>> inputContour, int numOfContour){
    int index = 0;
    vector<vector<Point>> filteredContour;
    vector<pair<int, int>> sortedMapping_areaToIndex;
    for (vector<vector<Point>>::iterator it = inputContour.begin(); it != inputContour.end(); index++, ++it){
        Rect rect = boundingRect((*it));
        sortedMapping_areaToIndex.push_back(make_pair(rect.area(),index));
    }
    sort(sortedMapping_areaToIndex.begin(), sortedMapping_areaToIndex.end(), sort_by_area);

    for(int i = 0; i < numOfContour; i++){
        filteredContour.push_back(inputContour[sortedMapping_areaToIndex[i].second]);
    }

    return filteredContour;

}

vector<vector<Point>> contour_filtering_position(vector<vector<Point>> inputContour, int maxError){
    int index = 0;
    int numOfCommonVertRect = 0;
    int numOfCommonHoriRect = 0;
    vector<pair<Point, int>> mapping_midPtToIndex;
    vector<pair<bool, int>> mapping_filterResultToIndex;
    vector<vector<Point>> filteredContour;
    for (vector<vector<Point>>::iterator it = inputContour.begin(); it != inputContour.end(); index++, ++it){
        Rect rect = boundingRect((*it));
        Point tlCorner = rect.tl();
        Point brCorner = rect.br();
        Point midPt;
        midPt.x = (tlCorner.x + brCorner.x) / 2;
        midPt.y = (tlCorner.y + brCorner.y) / 2;
        mapping_midPtToIndex.push_back(make_pair(midPt, index));
    }
    sort(mapping_midPtToIndex.begin(), mapping_midPtToIndex.end(), sort_by_coordinate);
    for (int i = 0; i < mapping_midPtToIndex.size(); i++){
        //cout << mapping_midPtToIndex[i].first.x << "," << mapping_midPtToIndex[i].first.y;
        numOfCommonHoriRect = 0;
        numOfCommonVertRect = 0;
        for (int indexToCal = 0; indexToCal < mapping_midPtToIndex.size(); indexToCal++){
            //Calcualte number of common verticle and horizontal partner
            if(i != indexToCal){ //not counting on itself
                if(abs(mapping_midPtToIndex[indexToCal].first.x - mapping_midPtToIndex[i].first.x) < maxError){
                    numOfCommonVertRect++;
                }
                if(abs(mapping_midPtToIndex[indexToCal].first.y - mapping_midPtToIndex[i].first.y) < maxError){
                    numOfCommonHoriRect++;
                }
            }
        }
        if (numOfCommonHoriRect > 1 && numOfCommonVertRect > 1){
            mapping_filterResultToIndex.push_back(make_pair(true, mapping_midPtToIndex[i].second));
            filteredContour.push_back(inputContour[mapping_midPtToIndex[i].second]);
        }
        else{
            mapping_filterResultToIndex.push_back(make_pair(false, mapping_midPtToIndex[i].second));
        }
        //cout << " " << numOfCommonVertRect << "," << numOfCommonHoriRect << " " << mapping_filterResultToIndex[i].first << endl;
    }
     
    
    return filteredContour;
}


vector<vector<Point>> contour_filtering_ratio(vector<vector<Point>> inputContour, double heightTowidthRatio, double percentageError);

bool sort_by_area(pair<int, int> const& lhs, pair<int, int> const& rhs){
    return lhs.first > rhs.first;
}

bool sort_by_coordinate(pair<Point, int> const& lhs, pair<Point, int> const& rhs){
    if (lhs.first.y == rhs.first.y) return lhs.first.x < rhs.first.x;
    return lhs.first.y > rhs.first.y;
}
