#include "armorfind.h"
#include <vector>
using namespace std;
ArmorFind::ArmorFind()
{
    ArmorLostDelay = 0;
    armormin = 10;
    kernel = getStructuringElement(MORPH_ELLIPSE,Size(9,9));
}

/**
  * @brief find the armor center base on the position of the lightbar
  * @param contours: the contours of the lightbar
  * @param input   : the initial image by the camera
  * @param output  : the final output image includes the armor center
  * @param result  : position of armor centers in the image
  * @param ismono  : input 0 for binocular vision process, other for monocular process
  * @return none
  */
//Neo - process contours and return output image with circled light bars and cross on the target
void ArmorFind::process(vector<vector<Point> > contours,const Mat &input,Mat &output,vector<Point> &result,bool ismono){
    Clear();
    output = input.clone();
    RotatedRect RRect;
    // first judgement lightbar contours
    //Neo - filter out the contours which does not resemble a thin upright standing rectangle
    for(int i=0;i<contours.size();i++){
        RRect = minAreaRect(contours[i]);
        if((fabs(RRect.angle) < 45.0 && RRect.size.height > RRect.size.width)
                || (fabs(RRect.angle) > 45.0 && RRect.size.width > RRect.size.height)
                ){
                RectfirstResult.push_back(RRect);
        }
    }
    if(RectfirstResult.size() < 2){
        ArmorCenters.clear();
        return;
    }
    //Neo - sort the filtered rectangle in ascending order according to the x coordinates of the center of the rectangle
    sort(RectfirstResult.begin(),RectfirstResult.end(),RotateRectSort);
    //Neo - this function will get the armor lights from the filtered rect object
    GetArmorLights();
    sort(RectResults.begin(),RectResults.end(),RotateRectSort);
    for(int i=0;i<RectResults.size();i++){
        //std::cout<<"("<<RectResults[i].center.x<<","<<RectResults[i].center.y<<")"<<std::endl;
        ellipse(output,RectResults[i],Scalar(255,0,0),2);
        /*ostringstream ss;
        ss<<i;
        putText(output,ss.str(),Point(RectResults[i].center.x,RectResults[i].center.y-5),FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0,255,255));
        */
    }
    //Neo - This function will search for possible pairs of bar lights and calculate the midpoint between the pairs and output the points for cross drawing purposes later
    GetArmors(output,ismono);
    //Neo - draw crosses on the found target
    for(int i=0;i<ArmorCenters.size();i++){
	//cout << ArmorCenters[i] <<endl;
        DrawCross(output,ArmorCenters[i],20,Scalar(255,0,255),2);
    }
    result = ArmorCenters;
}

/**
  * @brief draw the center of the armor center by a cross in the image
  * @param img: image for drawing
  * @param size: the size of the cross
  * @param color: the color of the cross
  * @param thickness: the thickness of the cross
  * @return none
  */
//Neo - function to draw crosses
void ArmorFind::DrawCross(Mat &img,Point center,int size,Scalar color,int thickness){
    Point L1,L2,B1,B2;
    int xL = center.x - size > 0 ? center.x - size:0;
    int xR = center.x + size;
    int yL = center.y - size > 0 ? center.y - size:0;
    int yR = center.y + size;
    L1 = Point(xL,center.y);
    L2 = Point(xR,center.y);
    B1 = Point(center.x,yL);
    B2 = Point(center.x,yR);
    line(img,L1,L2,color,thickness);
    line(img,B1,B2,color,thickness);

}

/**
  * @brief get the final result of the armor lights
  * @param none
  * @return none
  */
void ArmorFind::GetArmorLights(){
    size_t size = RectfirstResult.size();
    vector<RotatedRect> Groups;
    int cellmaxsize;
    Groups.push_back(RectfirstResult[0]);
    cellmaxsize = RectfirstResult[0].size.height * RectfirstResult[0].size.width;
    if(cellmaxsize > 2500) cellmaxsize = 0;
    int maxsize;
    /*
     * Neo - this loop through the rect object of possible light bars
     * Neo - it will group all the rect objects that are near to each other with respect of x coordinates together (difference < 10)
     * Neo - rect object with area > 2500 will be filtered out (the object is too near for the turret to shoot)
     * Neo - it will also stores the maximum size of the grouped rect object in another vector
     */
    for(int i=1;i<size;i++){
        if(RectfirstResult[i].center.x - RectfirstResult[i-1].center.x <10){
            maxsize = RectfirstResult[i].size.height * RectfirstResult[i].size.width;
            if(maxsize > 2500) continue;
            if(maxsize > cellmaxsize) cellmaxsize = maxsize;
            Groups.push_back(RectfirstResult[i]);
        }else{
            Armorlists.push_back(Groups);
            CellMaxs.push_back(cellmaxsize);
            cellmaxsize = 0;
            maxsize = 0;
            Groups.clear();
            //if(RectfirstResult[i].size.height * RectfirstResult[i].size.width > 2500) continue;
            Groups.push_back(RectfirstResult[i]);
            cellmaxsize = RectfirstResult[i].size.height * RectfirstResult[i].size.width;
        }
        //std::cout<<"max:"<<cellmaxsize<<std::endl;
        //sizescale = (float)RectfirstResult[i].size.height/(float)RectfirstResult[i].size.width;
        //std::cout<<"scale:"<<sizescale<<" width:"<<RectfirstResult[i].size.width<<std::endl;
    }
    Armorlists.push_back(Groups);
    CellMaxs.push_back(cellmaxsize);
    size = Armorlists.size();
    /*
     * Neo - this loop filters out grouped rect objects with area < 5
     * Neo - for each grouped rect objects, this loop will filter out all the rect objects with area != maximum area of the rect object in that particular group
     */
    for(int i=0;i<size;i++){
        int Gsize = Armorlists[i].size();
        int GroupMax = CellMaxs[i];
        if(GroupMax > 5){
            for(int j=0;j<Gsize;j++){
                maxsize = Armorlists[i][j].size.height * Armorlists[i][j].size.width;
                if(maxsize == GroupMax){
                    RectResults.push_back(Armorlists[i][j]);
                }
            }
        }
    }
}

/**
  * @brief get armor centers from lightbar contours
  * @param image: the result will print on this image
  * @param ismono: input 0 for binocular vision process, other for monocular process
  * @return none
  */
void ArmorFind::GetArmors(Mat &image,bool ismono){
    size_t size = RectResults.size();
    ArmorOldCenters = ArmorCenters;
    //Neo - clear centers from previous frame
    ArmorCenters.clear();
    if(size < 2){
        return;
    }
    Point2f L1,L2;
    float K,angleabs = 0.0,angleL1,angleL2;
    float divscale,areaL1,areaL2;
    float ydis = 0;
    float maxangle,xdis,heightmax,hwdiv;
    Point2f _pt[4],pt[4];
    //Neo - lambda function to caltulate the angle between two points
    auto ptangle = [](const Point2f &p1,const Point2f &p2){
        return fabs(atan2(p2.y-p1.y,p2.x-p1.x)*180.0/CV_PI);
    };
    //Neo - lambda function to calculate the area of 3 points by using shoe lace formula
    auto GetAreaofp3 = [](const Point2f &p1,const Point2f &p2,const Point2f &p3){
        Mat matrix = (Mat_<double>(3,3)<<p1.x,p2.y,1,p2.x,p2.y,1,p3.x,p3.y,1);
        return 0.5*determinant(matrix);
    };
    
    for(int i=0;i<size-1;i++){
        angleL1 = fabs(RectResults[i].angle);
        L1 = RectResults[i].center;
        areaL1 = RectResults[i].size.height * RectResults[i].size.width;
        RectResults[i].points(_pt);
         /*pt
          * 0 2
          * 1 3
          * */
	//Neo - store the two right vertices of the object (since the list is being sorted in ascending with respect to x coordinates)
        if(angleL1 > 45.0){
         pt[0] = _pt[3];
         pt[1] = _pt[0];
        }else{
         pt[0] = _pt[2];
         pt[1] = _pt[3];
        }
        for(int j=i+1;j<size;j++){
            L2 = RectResults[j].center;
	    //Neo - find vertical distance difference between two rect
            if(L1.x != L2.x){
                K = GetK(L1,L2);
                if(L1.y > L2.y){
                    ydis = L1.y - L2.y;
                }else{
                    ydis = L2.y - L1.y;
                }
                areaL2 = RectResults[j].size.height * RectResults[j].size.width;
                //Neo - calculate the ratio scale between area of two rect
		if(areaL1 > areaL2){
                    divscale = areaL1 / areaL2;
                }else{
                    divscale = areaL2 / areaL1;
                }
                angleL2 = fabs(RectResults[j].angle);
		
                RectResults[j].points(_pt);
                //Neo - store the two left vertices of the object (for computation with the two right vertices of the reference rect)
		if(angleL2 > 45.0){
                 pt[2] = _pt[2];
                 pt[3] = _pt[1];
                }else{
                 pt[2] = _pt[1];
                 pt[3] = _pt[0];
                }
		//Neo - assign the higher of angle between pairs of opposite vertices to maxangle
                maxangle = MAX(ptangle(pt[0],pt[2]),ptangle(pt[1],pt[3]));
                //std::cout<<"angle:"<<maxangle<<std::endl;
                // maxangle = 0;
		//Neo - another components of consideration for possible pairs of the rect object? (not really sure what is happening here)
                if(angleL1 > 45.0 && angleL2 < 45.0){
                    angleabs = 90.0 - angleL1 + angleL2;
                }else if(angleL1 <= 45.0 && angleL2 >= 45.0){
                    angleabs = 90.0 - angleL2 + angleL1;
                }else{
                    if(angleL1 > angleL2) angleabs = angleL1 - angleL2;
                    else angleabs = angleL2 - angleL1;
                }
		//Neo - horizontal distance difference between 2 rect
                xdis = fabs(L1.x - L2.x);
                heightmax =MAX(MAX(RectResults[i].size.width,RectResults[j].size.width),MAX(RectResults[i].size.height,RectResults[j].size.height));
                //Neo - ratio of the rect (width : height)
		hwdiv = xdis/heightmax;
		//Neo - evaluation from all the data computed above to find possible pairs of rect 
		//Neo - rects which are too vertically will be filtered out
		//Neo - the rectangle between the two rects will also be filtered off if it is too thin
                if(fabs(K) < 0.5 && divscale < 3.0 && maxangle < 20.0 && hwdiv < 10.0 && ydis < 0.4*heightmax){//&& ydis < armormin
                    if(angleabs < 7){
                        if(ismono){
	                    //Neo - get the area of the rectangle enclosed by the two rects (using shoelace formula on three points each to find the area of triangle and sum them up to get the area of the rectangle
                            float armor_area = GetAreaofp3(pt[0],pt[1],pt[2]) + GetAreaofp3(pt[1],pt[2],pt[3]);
                            //std::cout<<"area:"<<armor_area<<std::endl;
                            Monodata pushdata;
                            pushdata.area = armor_area;
			    //Neo - set the center points to be the point right in the middle between two center points of the two rects
                            pushdata.center = Point(0.5*(L1.x+L2.x),0.5*(L1.y+L2.y));
                            if(hwdiv > 5.0){
                                pushdata.armor = pushdata.big_armor;
                            }else{
                                pushdata.armor = pushdata.small_armor;
                            }
                            monodata.push_back(pushdata);
                            ArmorCenters.push_back(Point(0.5*(L1.x+L2.x),0.5*(L1.y+L2.y)));
			    //Neo - reset ArmorLostDelay to 0 (is used for later computation)
                            ArmorLostDelay = 0;
                        }else{
                            ArmorCenters.push_back(Point(0.5*(L1.x+L2.x),0.5*(L1.y+L2.y)));
                            ArmorLostDelay = 0;
                        }
                    }
                }
            }
        }
    }
    if(ArmorCenters.size()==0){
	//Neo - if there are no target found in this particular frame, ArmorLostDelay will increment by 1 
        ArmorLostDelay++;
	//Neo - if the target is lost for less than 10 frames, the program will still take the targets as the latest targets found
        if(ArmorLostDelay < 10){
            ArmorCenters = ArmorOldCenters;
        }
    }
}

/**
  * @brief return the slope of the line connected by 2 points
  * @param L1 L2: points input
  * @return slope of the line by these 2 points
  */
//Neo - find the slope between two points
double ArmorFind::GetK(Point2f L1,Point2f L2){
    return (L1.y - L2.y) / (L1.x - L2.x);
}

void ArmorFind::RotateRectLine(RotatedRect rect,Point &lineup,Point &linedown){
    Point2f pt[4];
    rect.points(pt);
    Point rep[2];
    if(Pointdis(pt[0],pt[1]) < Pointdis(pt[1],pt[2])){
        rep[0] = PointBetween(pt[0],pt[1]);
        rep[1] = PointBetween(pt[2],pt[3]);
    }else{
        rep[0] = PointBetween(pt[1],pt[2]);
        rep[1] = PointBetween(pt[0],pt[3]);
    }
    if(rep[0].y > rep[1].y){
        lineup = rep[1];
        linedown = rep[0];
    }else{
        lineup = rep[0];
        linedown = rep[1];
    }
}
/**
  * @brief calculate the distance of 2 points
  * @param p1 p2: points for calculation
  * @return the distance of 2 points
  */
double ArmorFind::Pointdis(const Point &p1,const Point &p2){
    return sqrt( (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

Point ArmorFind::PointBetween(const Point &p1,const Point &p2){
    return Point((p1.x + p2.x) / 2,(p1.y + p2.y) / 2);
}

/**
  * @brief find the center of a contour
  * @param contour: the point vector of the contour
  * @param center : output center
  * @return none
  */
void ArmorFind::ContourCenter(const vector<Point> contour,Point &center){
    Moments mu;
    mu = moments(contour,false);
    center.x = mu.m10 / mu.m00;
    center.y = mu.m01 / mu.m00;
}

//Neo - clear all the previous stored data
void ArmorFind::Clear(void){
    monodata.clear();
    CellMaxs.clear();
    Armorlists.clear();
    RectfirstResult.clear();
    RectResults.clear();
    Target.clear();
    final.clear();
    PairContour.clear();
    centers.clear();
}
