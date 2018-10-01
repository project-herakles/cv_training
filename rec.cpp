
#include <iostream>
#include <cmath>
#include "rec.h"
using namespace std;

void rec::setNumber(){

     cin >> x >> y>> width >> height;


}

void rec::getNumber(){

     cout << "("<<x <<","<< y <<","<< width<<"," << height<<")";

}

bool judgeSize(rec a, rec b){

     if (a.y==b.y && a.width==b.width && (a.height-b.height)<5 && (a.height-b.height)>-5)

        return true;
     else return false;

}

int getDistance(rec a, rec b){

     int x;

     if (a.x>=b.x) x=a.x-b.x;
     else x=b.x-a.x;

     return x;
}

