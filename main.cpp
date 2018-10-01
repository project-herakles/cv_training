#include <iostream>
#include "rec.h"

using namespace std;



int main()
{
    int n;
    cin >> n;

    rec a[n];
    rec b1,b2;

    int d=1000; //distance between two rectangles of the same size;

    for (int i=0; i<n; i++){
         a[i].setNumber();
    };

    for (int i=0; i<n; i++){
        for (int j=i+1; j<n; j++){

            if (judgeSize(a[i],a[j]) && d>=getDistance(a[i],a[j])){

                d=getDistance(a[i],a[j])
                b1=a[i];
                b2=a[j];

            }
            else continue;
        }

    }

    b1.getNumber();
    b2.getNumber();

    return 0;



}

