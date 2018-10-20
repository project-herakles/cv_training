#include "include/ArmorFind/armorfind.h"

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
void ArmorFind::process(vector<vector<Point> > contours,const Mat &input,Mat &output,vector<Point> &result,bool ismono){
    Clear();
    // Ivan - copy the input Mat into output Mat
    output = input.clone();
    RotatedRect RRect;
    // first judgement lightbar contours
    for(int i=0;i<contours.size();i++){
        //Ivan - find the minimum area of the rotated rectangle
        //       fabs() -> find the absolute value 
        //       case 1: angle < 45 and height of the Rrect > width of the Rrect
        //       case 2: angle > 45 and width of the Rrect > height of the Rrect
        //       Push back the Rotated rectangle to hte first result
        //       overall speaking, this loop find the qualified Rotated rectangle that is thin
        RRect = minAreaRect(contours[i]);
        if((fabs(RRect.angle) < 45.0 && RRect.size.height > RRect.size.width)
                || (fabs(RRect.angle) > 45.0 && RRect.size.width > RRect.size.height)
                ){
                RectfirstResult.push_back(RRect);
        }
    }
    // Ivan - if the Rotated rectangle found < 2
    //        then clear the armorcenter and return nothing
    if(RectfirstResult.size() < 2){
        ArmorCenters.clear();
        return;
    }
    // Ivan - sort the Rotated retangle according the ascending order of  x coordinate of their center
    sort(RectfirstResult.begin(),RectfirstResult.end(),RotateRectSort);
    // Ivan - update the RectResult using GetArmorlights() that having rectangle of armor lights
    GetArmorLights();
    // Ivan - sort again the Rectangle according to the ascending order of their x-center-coordinate
    sort(RectResults.begin(),RectResults.end(),RotateRectSort);
    for(int i=0;i<RectResults.size();i++){
        //std::cout<<"("<<RectResults[i].center.x<<","<<RectResults[i].center.y<<")"<<std::endl;
        // Ivan - draw blue ellipse on the Rectangle found on the output picture
        ellipse(output,RectResults[i],Scalar(255,0,0),2);
        /*ostringstream ss;
        ss<<i;
        putText(output,ss.str(),Point(RectResults[i].center.x,RectResults[i].center.y-5),FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0,255,255));
        */
    }
    GetArmors(output,ismono);
    // Ivan - this draw the cross on the center of the armor
    for(int i=0;i<ArmorCenters.size();i++){
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

// Ivan - this function draw cross on the center
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
    // Ivan - overall speaking, this function find the largest Rotated rectangle area in each group
    //        /individual and store it into rectResult
    //        
    // Ivan - size store the size of RectfirstResult
    //        Groups store RectfirstResult
    //        cellmaxsize = first Rrect area
    //        if it is > 2500. reset it as 0
    size_t size = RectfirstResult.size();
    vector<RotatedRect> Groups;
    int cellmaxsize;
    Groups.push_back(RectfirstResult[0]);
    cellmaxsize = RectfirstResult[0].size.height * RectfirstResult[0].size.width;
    if(cellmaxsize > 2500) cellmaxsize = 0;
    int maxsize;
    // Ivan - Group the Rrect according x-center-distance < 10(near), find the cellmaxsize among each group
    //        fo those grouping Rrect(x-center-distance < 10), omit the maxSize if the area > 2500(noise, to close)
    //        for those single Rrect, simply put them in a individual slot and record their area
    for(int i=1;i<size;i++){
        // Ivan - why no need fabs? because we sorted them already
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
    // Ivan - these two lines push the last group/individual to armorlist and cellmaxs
    Armorlists.push_back(Groups);
    CellMaxs.push_back(cellmaxsize);
    // Ivan - size is the number of Group/individuals 
    //        Gsize is number of element for each group/individuals
    //        GroupMax is the maximum area among each group/individuals
  
    size = Armorlists.size();
    for(int i=0;i<size;i++){
        int Gsize = Armorlists[i].size();
        int GroupMax = CellMaxs[i];
        // If the area > 5, push_back the largest rectangle into RectResults 
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
    ArmorCenters.clear();
    // Ivan - if the Rectsults < 2, return nothing
    if(size < 2){
        return;
    }
    // IVAN - Point2f is for floating point value point
    Point2f L1,L2;
    float K,angleabs = 0.0,angleL1,angleL2;
    float divscale,areaL1,areaL2;
    float ydis = 0;
    float maxangle,xdis,heightmax,hwdiv;
    Point2f _pt[4],pt[4];
    auto ptangle = [](const Point2f &p1,const Point2f &p2){
        // Ivan - find the absolute value of arc tangent of something and return it in degree
        //        overall speaking, it return the angle between 2 point 
        return fabs(atan2(p2.y-p1.y,p2.x-p1.x)*180.0/CV_PI);
    };
    // Ivan - I guess this is using matrix to calculate traingular area of 3 point 
    //        area = 1/2 * determinant of matrix
    auto GetAreaofp3 = [](const Point2f &p1,const Point2f &p2,const Point2f &p3){
        Mat matrix = (Mat_<double>(3,3)<<p1.x,p2.y,1,p2.x,p2.y,1,p3.x,p3.y,1);
        return 0.5*determinant(matrix);
    };

    for(int i=0;i<size-1;i++){
        // Ivan - angleL1 store the absolute angle of rotation of Rrect
        //        L1 store the center of rectResult
        //        areaL1 store the its area
        //        _pt store its 4 vertice
        angleL1 = fabs(RectResults[i].angle);
        L1 = RectResults[i].center;
        areaL1 = RectResults[i].size.height * RectResults[i].size.width;
        RectResults[i].points(_pt);
         /*pt
          * 0 2
          * 1 3
          * */
        // Ivan - store the rightmost vertice of Rectangle L1 according to the degree making to the ground
        if(angleL1 > 45.0){
         pt[0] = _pt[3];
         pt[1] = _pt[0];
        }else{
         pt[0] = _pt[2];
         pt[1] = _pt[3];
        }
        // Ivan - this is loop structure to compare every pair of rectangles inside the RectResult
        for(int j=i+1;j<size;j++){
            L2 = RectResults[j].center;
            if(L1.x != L2.x){
                // Ivan - K store the slope of L1/L2
                K = GetK(L1,L2);
                // Ivan - Find the absolute y-distance 
                if(L1.y > L2.y){
                    ydis = L1.y - L2.y;
                }else{
                    ydis = L2.y - L1.y;
                }
                // Ivan - find area of next rectangle and calculate divscale as larger rect/smaller rect
                areaL2 = RectResults[j].size.height * RectResults[j].size.width;
                if(areaL1 > areaL2){
                    divscale = areaL1 / areaL2;
                }else{
                    divscale = areaL2 / areaL1;
                }
                angleL2 = fabs(RectResults[j].angle);

                RectResults[j].points(_pt);
                // Ivan - store the leftmost vertice of Rectangle L2 according to the degree making to the ground
                if(angleL2 > 45.0){
                 pt[2] = _pt[2];
                 pt[3] = _pt[1];
                }else{
                 pt[2] = _pt[1];
                 pt[3] = _pt[0];
                }
                // Ivan - store the larger point angle of 2 rectangles into maxangle
                maxangle = MAX(ptangle(pt[0],pt[2]),ptangle(pt[1],pt[3]));
                //std::cout<<"angle:"<<maxangle<<std::endl;
               // maxangle = 0;
                // Ivan - calculate the absolute angle difference, for first two case, its because there are 
                //        extreme cases that program recognize contour as rotated angle of ~-90degree while
                //        its upright
                //        maxangle vs angleabs
                //        - maxangle is checking verticle difference 
                //        - anglebbs only focus on rotation difference 
                //        then calculate the and x-distance of them 
                if(angleL1 > 45.0 && angleL2 < 45.0){
                    // Ivan - satisfying case for angleans < 7
                    //        angleL2          angleL1       angleabs
                    //           0               90              0
                    //           3               87              6
                    angleabs = 90.0 - angleL1 + angleL2;
                }else if(angleL1 <= 45.0 && angleL2 >= 45.0){
                    // Ivan - satisfying case for angleabs < 7
                    //        angleL2           angleL1     angleabs
                    //           90               0             0
                    //           87               3             6 
                    angleabs = 90.0 - angleL2 + angleL1;
                }else{
                    if(angleL1 > angleL2) angleabs = angleL1 - angleL2;
                    else angleabs = angleL2 - angleL1;
                }
                xdis = fabs(L1.x - L2.x);
                // Ivan - find the longest side 
                heightmax =MAX(MAX(RectResults[i].size.width,RectResults[j].size.width),MAX(RectResults[i].size.height,RectResults[j].size.height));
                // Ivan - hwdiv store the ratio of the maximum height and xdis
                hwdiv = xdis/heightmax;
                // Ivan - If the absolute slope between their center < 0.5(avoid high verticle-diff in center) 
                //        the larger rect is not 3 times larger then the smaller rect (light size not differ too much)
                //        the maximum angle between their vertices < 20 (avoid high verticle-diff in upline)
                //        the ratio of maximum height and x-distance < 10 (rect in-between cant be too flat)
                //        the y distance < 0.4*maximum rectangle height (svoif high verticle diff in center)
                //        then it's either small or big armor
                if(fabs(K) < 0.5 && divscale < 3.0 && maxangle < 20.0 && hwdiv < 10.0 && ydis < 0.4*heightmax){//&& ydis < armormin
                    if(angleabs < 7){
                        // Ivan - if they are roughly parallel without(angleabs < 7)     
                        if(ismono){
                            float armor_area = GetAreaofp3(pt[0],pt[1],pt[2]) + GetAreaofp3(pt[1],pt[2],pt[3]);
                            //std::cout<<"area:"<<armor_area<<std::endl;
                            Monodata pushdata;
                            pushdata.area = armor_area;
                            pushdata.center = Point(0.5*(L1.x+L2.x),0.5*(L1.y+L2.y));
                            // Ivan - find the area, center of the armor
                            //        if the ratio of x-distance/heightmax > 5, let say,
                            //        if it is flat, then it is big armor, otherwise its 
                            //        small armor
                            if(hwdiv > 5.0){
                                pushdata.armor = pushdata.big_armor;
                            }else{
                                pushdata.armor = pushdata.small_armor;
                            }
                            monodata.push_back(pushdata);
                            ArmorCenters.push_back(Point(0.5*(L1.x+L2.x),0.5*(L1.y+L2.y)));
                            ArmorLostDelay = 0;
                        }else{
                            // Ivan - if its not monocamera, just pushback the armorcenter
                            ArmorCenters.push_back(Point(0.5*(L1.x+L2.x),0.5*(L1.y+L2.y)));
                            ArmorLostDelay = 0;
                        }
                    }
                }
            }
        }
    }
    if(ArmorCenters.size()==0){
        // Ivan - to see if there is armor lost, if armor lost for just shortly, use old center as reference
        //        otherwise dont use
        ArmorLostDelay++;
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
double ArmorFind::GetK(Point2f L1,Point2f L2){
    return (L1.y - L2.y) / (L1.x - L2.x);
}

// Ivan - ArmorFind finds the rotatedRect upline and downline(shorter side)
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
// Ivan - moment is a certain particular weighted average of the image pixes' intensity
//        x, y is the mass center(centriod) using xavg = m10/m00, yavg = m01/m00
void ArmorFind::ContourCenter(const vector<Point> contour,Point &center){
    Moments mu;
    mu = moments(contour,false);
    center.x = mu.m10 / mu.m00;
    center.y = mu.m01 / mu.m00;
}


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
