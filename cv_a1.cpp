#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;
struct point{
	double x, y, width, height, widthScale, heightScale; 
}; 
bool comp(point p1, point p2); //compare function to sort the array that holds the points

int main(){
	cout << "Input number of targets and the data for each target:" << endl;
	int n;
	cin >> n;
	point p[n];
	for (int i = 0; i < n; i++){
		cin >> p[i].x >> p[i].y >> p[i].width >> p[i].height;
		p[i].widthScale = round(p[i].width / 2);
    	p[i].heightScale = round(p[i].height / 10); //compute width and height proportion on input of each point
	}

	sort(p, p+n, comp); //Sort the array of points by descending order of width and height
	
	int pair1 = -1;
	int pair2 = -1;
	
    for (int i = 0; i < n; i++){
    	if(p[i].widthScale == p[i].heightScale) {
    		int j = i+1;
    		while(p[j].widthScale == p[j].heightScale && p[i].widthScale == p[j].widthScale && j < n) { //if the two rectangles have same proportion of width and height
    			double d = sqrt((p[i].x-p[j].x)*(p[i].x-p[j].x) + (p[i].y-p[j].y)*(p[i].y-p[j].y));
    			int distanceScale = round(d/15);
    			if (distanceScale == p[i].widthScale) { //check if distance is in proportion too
    				pair1 = i;
    				pair2 = j;
    				break; //exit at first pair found, since it will be the largest therefore nearest pair
				}
				j++;
			}
			if(pair1>0) break;
		}
	}
	cout << "Shoot this pair:" << endl;
	cout << p[pair1].x << " " << p[pair1].y << " " << p[pair1].width << " " << p[pair1].height << endl;
   	cout << p[pair2].x << " " << p[pair2].y << " " << p[pair2].width << " " << p[pair2].height << endl;
	
	
} 

bool comp(point p1, point p2){
	if (p1.width == p2.width) return p1.height > p2.height;
	else return p1.width > p2.width;
}
