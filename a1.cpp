#include <iostream> 
#include <cmath>
#include <cstdlib>
#include <algorithm>

using namespace std;

struct square {
	double x, y, width, height, widthscale, heightscale, scale, area;
};

bool descend (int i,int j) { return (i>j); }

int main(){
	
	int n = 0;	
	cout << "Input the number of target points:" << endl;
	cin >> n;
	square a[n];
	square b[];
	
	cout << "Input " << n << "suqares' x, y, width, height:"<< endl;
	
	for (int i = 0; i < n; i = i + 1){
		cin >> a[i].x >> a[i].y >> a[i].width >> a[i].height;
		a[i].widthscale = round (a[i].width/2);
		a[i].heightscale = round (a[j].height/2);
		a[i].scale = round (a[i].widthscale / a[i].heightscale);
		a[i].area = a[i].width * a[i].height;
	}

	// gather the points which are closest to each other in x-axis
	sort (a[0].x, a[n].x); 

	// find the possible pairs
	for (int j = 0, int m = 0; j < n; j = j + 1){
		for (int k = j + 1; k < n; k = k + 1){
			if (a[j].y == a[k].y && a[j].scale == a[k].scale)
			{	
				b[m] = a[j];
				b[m + 1] = a[k];
				m = m + 2;
			}
			
		}
	}
	
	// count the number of possible pairs
	int pair = 0;
	pair = (sizeof(b) / sizeof(b[0])) / 2;

	if (pair < 1)
		cout << "No target found." << endl;
	
	if (pair = 1)
		{
			cout << "Target found: " << endl;
			cout << b[0].x << b[0].y << b[0].width << b[0].height << endl;
			cout << b[1].x << b[1].y << b[1].width << b[1].height << endl;
	if (pair > 1)
	{ 
		int num = 0;
		double max = b[num].area;
		for (int i = 2; i < pair * 2; i = i + 2){
			if (b[i].area > max){
				max = b[i].area;
				num = i;
			}
			
		}
		cout << "Target found:" << endl;
		cout << b[num].x << b[num].y << b[num].width << b[num].height << endl;
		cout << b[num + 1].x << b[num + 1].y << b[num + 1].width << b[num + 1].height << endl;
	}
	
	return 0;
}
