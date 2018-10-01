#include <iostream>
#include <cmath>

using namespace std;

int main()
{
    int n;
    cin>>n;
    int a[4][n];
    int b[n]={0};
    int p=0,q=0;
    for(int i=0;i<n;i++){
        for(int j=0;j<4;j++){
            cin>>a[j][i];
        }
    }
    int min_d = 1000;
    for(int i=0;i<n;i++){
        if(b[i]==0){
            int y=a[1][i];
            b[i]=1;
            for(int j=i+1;j<n;j++){
                if(a[1][j]==y){
                    b[j]=1;
                    int judge=a[3][i]-a[3][j];
                    judge=abs(judge);
                    if(judge<5){
                        int distance=a[0][i]-a[0][j];
                        distance=abs(distance);
                        if(distance<min_d){
                            min_d=distance;
                            p=i;
                            q=j;
                        }
                    }
                }
            }
        }
    }
    cout<<"(";
    for(int i=0;i<4;i++){
        cout<<a[i][p]<<",";
    }
    cout<<")";
    cout<<"(";
    for(int i=0;i<4;i++){
        cout<<a[i][q]<<",";
    }
    cout<<")";

    return 0;
}
