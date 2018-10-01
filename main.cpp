#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <limits>

using namespace std;

const double actualRatio = 10 / 2;
const double actualHeight = 10;
const double actualWidth = 2;
const double actualDistance = 15;

struct rectangle {
	double x, y, width, height, heightScale, widthScale, heightToWidthScale;
};

bool wayToSort(rectangle r1, rectangle r2) {
	if (r1.y == r2.y) return r1.x > r2.x;
	return r1.y > r2.y;

}

int main() {
	int numOfRect;
	int x;
	double distance, smallestDistance, distanceScale;
	int pair1Index, pair2Index;
	rectangle* rectArray;
	cout << "Plesae input the number of rectangles:";
	cin >> numOfRect;
	rectArray = new rectangle[numOfRect];
	for (int i = 0; i < numOfRect; i++) {
		cin >> rectArray[i].x >> rectArray[i].y >> rectArray[i].width >> rectArray[i].height;
		rectArray[i].heightToWidthScale = round(rectArray[i].height / rectArray[i].width);
		rectArray[i].heightScale = round(rectArray[i].height / actualHeight);
		rectArray[i].widthScale = round(rectArray[i].width / actualWidth);
	};
	// sort the rectangle array in descending of y and descending order of x
	sort(rectArray, rectArray + numOfRect, wayToSort);

	// output the rectangles information for debugging
	cout << "Rectangles information(Total " << numOfRect << ")" << endl;
	for (int i = 0; i < numOfRect; i++) {
		cout << rectArray[i].x << " " << rectArray[i].y << " " << rectArray[i].width << " " << rectArray[i].height << " " << rectArray[i].widthScale << endl;
	}

	// Calculate the smallest distance paired rectangle with same size
	smallestDistance = DBL_MAX;
	for (int i = 0; i < numOfRect; i++) {
		for (int j = i + 1; j < numOfRect - (i + 1); j++) {
			if ((rectArray[i].y == rectArray[j].y) && (rectArray[i].heightScale == rectArray[j].heightScale) && (rectArray[i].widthScale == rectArray[j].widthScale)) {
				distance = sqrt((rectArray[i].x - rectArray[j].x)*(rectArray[i].x - rectArray[j].x) + (rectArray[i].y - rectArray[j].y) * (rectArray[i].y - rectArray[j].y));
				distanceScale = distance / 15;
				if (distance < smallestDistance && distanceScale == rectArray[i].widthScale){
					smallestDistance = distance;
					pair1Index = i;
					pair2Index = j;

				}
			}
		}
	}

	cout << "closet pair:" << endl;
	cout << rectArray[pair1Index].x << " " << rectArray[pair1Index].y << " " << rectArray[pair1Index].width << " " << rectArray[pair1Index].height << endl;
	cout << rectArray[pair2Index].x << " " << rectArray[pair2Index].y << " " << rectArray[pair2Index].width << " " << rectArray[pair2Index].height << endl;
	delete[] rectArray;
	rectArray = NULL;
	cin >> x;
	return 0;
}

