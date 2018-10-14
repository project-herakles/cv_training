#include <vector>
#ifndef ARMORFIND_H
#define ARMORFIND_H
#include "Header.h"
using namespace cv;
using namespace std;

//Neo - sorting rules
static inline bool PairSort(std::pair<Point,Rect> a1,std::pair<Point,Rect> a2){
    return a1.first.x < a2.first.x;
}

//Neo - sorting rules (sort the possible light bar RotatedRect object ascendingly according to the x coordinates of the center)
static inline bool RotateRectSort(RotatedRect a1,RotatedRect a2){
    return a1.center.x < a2.center.x;
}

//declare a class ArmorFind to contain all the finding armor related values and functions
class ArmorFind
{
public:
    ArmorFind();
    //function to process the contours acquired from the input image and return output image with circled light bars and crossed target
    void process(vector<vector<Point>> contours,const Mat &input,Mat &output,vector<Point> &result,bool ismono);
private:
    void ContourCenter(const vector<Point> contour,Point &center);
    void RotateRectLine(RotatedRect rect,Point &lineup,Point &linedown);
    double Pointdis(const Point &p1,const Point &p2);
    Point PointBetween(const Point &p1,const Point &p2);
    void Clear(void);
    //get the first filter of possible armor light bars
    void GetArmorLights(void);
    //get the possible armor light bar pairs and return the points of the possbile pairs
    void GetArmors(Mat &image,bool ismono);
    //get the gradient of the 2 points
    double GetK(Point2f L1,Point2f L2);
    //draw crosses for the found target
    void DrawCross(Mat &img,Point center,int size,Scalar color,int thickness = 2);

public:
    Mat binary;
    vector<Point> ArmorCenters,ArmorOldCenters;
    vector<Monodata> monodata;
private:
    int ArmorLostDelay;
    int armormin;
    Mat kernel;
    vector<vector<RotatedRect>> Armorlists;
    vector<int> CellMaxs;
    vector<Mat> splited;
    vector<vector<Point>> final;
    vector<Rect> Target;
    vector<Point> centers;
    vector<std::pair<Point,Rect>> PairContour;
    vector<RotatedRect> RectfirstResult,RectResults;
    Mat armor_roi;

};


#endif // ARMORFIND_H
