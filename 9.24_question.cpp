#include <iostream>
#include <cmath>
using namespace std;
//define struct Bar with properties x, y, height, width
struct Bar
{
  int x;
  int y;
  double height;
  double width;
};

Bar createBar(int x, int y, double width, double height);
void merge(Bar arr[], int l, int m, int r, char indicator);
void mergeSort(Bar* arr, int l, int r, char indicator);
Bar* findPair (Bar* segmented_bar, int n);
bool analysePair(Bar bar1, Bar bar2);

int main() 
{
  int x, y;
  double height, width;
  int size;
  //prompt user input size of input
  cin >> size;
  //create a dynamic array of Bar with size input by user
  Bar* input_bar = new Bar[size];
  //create a Bar address to store found target
  Bar* pair = new Bar;
  //loop through user's input
  for (int i = 0; i < size; i ++)
  {
    cin >> x >> y >> width >> height;
    input_bar[i] = createBar(x, y, width, height);
  }
  //sort the array of Bar in descending order with respect of their width
  mergeSort(input_bar, 0 , size-1, 'w');

  int i = 0;

  while (i < size)
  {
    int n = 1;
    int start = i;
    //split the sorted array of Bar in descending with respect of their width
    while(input_bar[i].width == input_bar[i+1].width)
    {
      i ++;
      n ++;
    }

    if (i == start)
    {
      i ++;
    }

    if (n != 1)
    {
      Bar* segmented_bar = new Bar[n];
      for (int j = 0; j < n; j ++)
      {
        segmented_bar[j] = input_bar[start];
        start ++;
      }
      //sort the grouped Bar with respect of their y coordinates
      mergeSort(segmented_bar, 0 , n-1, 'y');
      //find the target
      pair = findPair(segmented_bar, n);
      delete []segmented_bar;
      //if a pair is found, break the loop
      if (pair)
      {
        break;
      }
    }
  }
cout << pair[0].x << " " <<pair[0]. y << " " << pair[0].width << " " << pair[0].height << endl;

cout << pair[1].x << " " <<pair[1]. y << " " << pair[1].width << " " << pair[1].height << endl;

delete []input_bar;
delete pair;
input_bar = nullptr;
pair = nullptr;
return 0;

}
//function to assign values to Bar
Bar createBar(int x, int y, double width, double height)
{
  Bar temp;
  temp.x = x;
  temp.y = y;
  temp.height = height;
  temp.width = width;
  return temp;
}
//merge sort algorithm
void merge(Bar arr[], int l, int m, int r, char indicator) 
{ 
  int i, j, k; 
  int n1 = m - l + 1; 
  int n2 =  r - m; 

  Bar L[n1], R[n2]; 

  for (i = 0; i < n1; i++) 
    L[i] = arr[l + i]; 
  for (j = 0; j < n2; j++) 
    R[j] = arr[m + 1+ j]; 

  i = 0; 
  j = 0; 
  k = l; 
  while (i < n1 && j < n2) 
  { 
    if (indicator == 'w')
    {
      if (L[i].width >= R[j].width) 
      { 
        arr[k] = L[i]; 
        i++; 
      } 
      else
      { 
        arr[k] = R[j]; 
        j++; 
      } 
      k++; 
    }
    else if (indicator == 'y')
    {
      if (L[i].y >= R[j].y) 
      { 
        arr[k] = L[i]; 
        i++; 
      } 
      else
      { 
        arr[k] = R[j]; 
        j++; 
      } 
      k++; 
    }
  } 

  while (i < n1) 
  { 
    arr[k] = L[i]; 
    i++; 
    k++; 
  }

  while (j < n2) 
  { 
    arr[k] = R[j]; 
    j++; 
    k++; 
  }
}
  
void mergeSort(Bar* arr, int l, int r, char indicator) 
{ 
  if (l < r) 
  { 
    int m = l+(r-l)/2; 

    mergeSort(arr, l, m, indicator); 
    mergeSort(arr, m+1, r, indicator); 

    merge(arr, l, m, r, indicator); 
  }
}

Bar* findPair (Bar* segmented_bar, int n)
{
  //loop through the grouped Bar
  for (int i = 0; i < n; i ++)
  {
    for (int j = i+1; j < n; j ++)
    {  
      //exit the loop if y of subsequent Bar is not equal
      if(segmented_bar[i].y!=segmented_bar[j].y)
        break;
      else
      {
        //analyse the potential pair of Bar and return the target if found
        if(analysePair(segmented_bar[i], segmented_bar[j]))
        {
          Bar* pair = new Bar[2];
          pair[0] = segmented_bar[i];
          pair[1] = segmented_bar[j];
          return pair;
        }
      }
    }
  }
  //return null pointer if no target is found
  return nullptr;
}

bool analysePair(Bar bar1, Bar bar2)
{
  //set allowed discrepancy to 17%
  double discrepancy = 0.17;
  double actual_ratio = 10/2;
  int screen_distance = abs(bar1.x - bar2.x);
  double scale;
  //set the scale value if the screen ratio of the Bar is equal to the actual Bar
  if (round(bar1.height/bar1.width)==round(bar2.height/bar2.width) && (round(bar1.height/bar1.width)==actual_ratio))
  {
    scale = bar1.height/10;
  }
  //calculate the expected screen distance using the scale above
  int expected_screen_distance = round(15*scale);
  //if the difference between the expected screen distance and screen distance is within acceptable range, return true
  if ((abs(round(expected_screen_distance-screen_distance)))<=(abs(round(expected_screen_distance*discrepancy))))
  {
    return true;
  }
  
  return false;
  
} 