#include <iostream>
#include <cmath>

using namespace std;

int main()
{
    int n;
    cin >> n;

   //step 1 :input data
    int input [n][4];
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<4; j++)
            cin >> input[i][j];
    }

    //step 2:identify possible targets(in such a dumb way!!)
    // 1.find m
    int m=0;
    for (int i=0; i<n; i++)
    {
        float shape=input[i][3] / input[i][2];
        if (abs(shape-5) <= 1)
        {
            m += 1;
        }
    }

    //2.find target
    int target [m][4];
    m=0;
    for (int i=0; i<n; i++)
    {
        float shape=input[i][3] / input[i][2];
        if (abs(shape-5) <= 1)
        {
            for (int j=0; j<4; j++)
                target[m][j]= input[i][j];
            m += 1;
        }
    }

    // step 3 : pair targets
    int targetpair[m][m] = {0};
    for (int i=0; i<m; i++)
        targetpair[0][i] = 0;

    for (int i=0; i<m; i++)
    {
        for (int i_=i+1; i_<m; i_++)
        {
            if (abs(target[i][3] - target[i_][3]) < 2 &&
                abs(target[i][1] - target[i_][1]) < 2 &&
                abs((abs(target[i][0]-target[i_][0])/target[i_][3])- 1.5) < 0.2)
                targetpair[i][i_] = 1;
        }
    }

   //step 4: find nearest pair
   int temp = 0;
   int result[2] = {0};
   for (int i=0; i<m; i++)
   {
       for (int j=0; j<m; j++)
       {
           if (targetpair[i][j] == 1 &&
               target[i][3] > temp)
            {
                temp = target[i][3];
                result[0]=i;
                result[1]=j;
            }
       }
   }

    cout << "\nresult:\n{(";
    for (int j=0; j<4; j++)
        cout << target[result[0]][j] << ",";
    cout << "),(";
    for (int j=0; j<4; j++)
        cout << target[result[1]][j] << ",";
    cout << ")}\n";

    cin >> n;

    return 0;
}
