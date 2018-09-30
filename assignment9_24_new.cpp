#include <iostream>
#include <cmath>
using namespace std;
struct Vector {
  int x;
  int y;
  int width;
  int height;
};
void printout(Vector target_1, Vector target_2);
double err(double prop, double a, double b, double c, double d);

int main(){
  int target_number;
  cin >> target_number;

  int real_distance = 15, real_width=2, real_height=10;

  Vector * target = new Vector [target_number] ;
  for (int i=0; i< target_number; i++){
    cin >> target[i].x >> target[i].y
    >> target[i].width >> target[i].height;
  }

  double error;
  double max_prop = 0;
  int final_k = 0 , final_j = 1;

  for (int k=0; k<target_number-1; k++){
    for (int j=k+1; j<target_number; j++){
      if (target[k].y == target[j].y){
      double prop_distance = (double) abs((target[k].x - target[j].x))/real_distance ;
      double prop_width_k = (double) target[k].width / real_width ;
      double prop_width_j = (double) target[j].width / real_width ;
      double prop_height_k = (double) target[k].height / real_height ;
      double prop_height_j = (double) target[j].height / real_height ;
      error = err(prop_distance,prop_width_k, prop_width_j, prop_height_k, prop_height_j);
      // for check
      //cout << error<< " ";
      //printout(target[k], target[j]);
      if (error <= 0.1){      // when it change to error <= 0.01, it doesn't work????
        if (prop_distance > max_prop){
          final_k = k;
          final_j = j;
          max_prop = prop_distance;
        }
      }
     }
    }
  }

 printout(target[final_k], target[final_j]);
 delete [] target;
 return 0;
}

void printout(Vector target_1, Vector target_2){
  cout << "[" << "("
       <<target_1.x << ","<< target_1.y<< ","
       << target_1.width<< "," << target_1.height
       << ")" << ","<<"("
       <<target_2.x << ","<< target_2.y<< ","
       << target_2.width<< "," << target_2.height
       << ")"<<"]"
       << endl;
}

double err(double prop, double a, double b, double c, double d){
  double result;
  result = pow(a-prop,2)+ pow(b-prop,2)+pow(c-prop,2)+pow(d-prop,2);
  return result;
}
