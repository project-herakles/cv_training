#include <iostream>
using namespace std;

const int RATIO = 5;

/**
    question: if I create an array in a function call and return the pointer to main. Will the array still exist? You can test yourself.
*/

/*
    9.27 night
    Here to clarify the problem setting: we are given the parameters of rectangles
    in a picture which contains robots. One robot have two armors, which are the
    rectangles captured by the camera. However, sometimes the camera cannot capture both
    two of the armors. Thus the rectangles either appear in pairs or alone. And it is
    possible for a rectangle to appear only once since its counterpart is not inside the
    picture.
    Our goal is to find the closet paired rectangle. By closest we mean the distance between
    the camera and the armor.

    We will store the input data in the fist array. Then we need to find out those in pair
    and store them in another list. Then we use compute the distance.

    The rectangle in pair will have the same y value. Later when refining the program
    we may need to consider the case where different pairs having the same y.
*/

    /** 9.28 Reading reply on Slack:
    The reason to consider proportion of the width and height is that we might mistakenly recognize something which is not the light target. Thus we need to check
    the proportion to select the real targets.
    */

/**
    Possible refinement:

    Should be noted that so far, our criteria for judging whether two rectangles are the same is based
    on checking the y coordinate. However, strictly speaking, the width and height should also be checked to see if they are nearly the same.
    More revision might be necessary.
*/



/** 9.30
    The sample output seems wrong. For [(651,383,4,19),(676,383,4,20)], the [interval, width, height]
    can either be [25, 4, 20] or [21, 4, 20], which does not match the setting [15, 2, 10]=[30, 4, 20].
*/

int printrList5(int l[][5], int s){

    for (int r=0; r<s; r++){
        cout<<(r+1)<<": ";
        for (int c=0; c<5; c++){
            cout<<l[r][c];

            if (l[r][c]<10){
                cout<<"  ";
            }else if(l[r][c]<100){
                cout<<" ";
            }

            cout<<" | ";
        }
        cout<<endl;
    }
    return 0;
}

int printrList4(int l[][4], int s){

    for (int r=0; r<s; r++){
        cout<<(r+1)<<": ";
        for (int c=0; c<4; c++){
            cout<<l[r][c];

            if (l[r][c]<10){
                cout<<"  ";
            }else if(l[r][c]<100){
                cout<<" ";
            }

            cout<<" | ";
        }
        cout<<endl;
    }
    return 0;
}

int readr1(){
    cout<<"Enter number of data you want to enter!"<<endl;
    int n=0;
    while(true){
        cin>>n;
        if (n<2){
            cout<<"Please enter at least two rectangles! Re-enter: ";
        }else{
            return n;
        }
    }

}


int readRects(int l[][5], int n){
    /*
    n is the number of columns. We will have an extra column to store whether
    this rectangle has been checked or not. Its initial value will be 0 and after
    being checked it will be set to 1.
    */
    cout<<"Enter rect data!"<<endl;
    for (int r=0; r<n; r++){
        for (int c=0; c<4; c++){
            cin>>l[r][c];
        }
        l[r][4]=0;
    }
    return 0;
}

/*
    This function takes the starting index and return the index of the counterpart, if
    there is any. If there is  no pair, -1 will be returned.
    */
int searchPair(int i, int rects1[][5], int nRow){

    int y=rects1[i][1];
    int width=rects1[i][2];

    for (int j=i+1;j<nRow; j++ ){
        if (rects1[j][4]==0 && rects1[j][1]==y && rects1[j][2]==width){

            rects1[j][4]=1;//easy to forget
            return j;
        }
    }
    return (-1);
}

void zeroRow5(int r, int list[][5]){
    for (int i=0; i<4;i++){
        list[r][i]=0;
    }
}
void zeroRow4(int r, int list[][4]){
    for (int i=0; i<4;i++){
        list[r][i]=0;
    }
}
int zerorList1(int rList1[][5], int nRow){
    /*
    This function will zero the single rectangles in rList1 and return the
    actual size of rList1 after cleaning.

    We will copy those remaining rectangles to another rList2 in another function. The
    reason behind is that we want to get the actual size of rList1 first so that
    when can create a rList2 with all entries occupied, instead of a rList2 with empty
    entries since it will be difficult to manage.
    */
    for (int i=0; i<nRow; i++){

        if (rList1[i][4]==0){//if this rectangle has not been checked before
            rList1[i][4]=1;//easy to forget

            int index=searchPair(i, rList1, nRow);

            if (index==-1){
                //if the rectangle is single, we make all its entires to be 0, except for the last cell.
                zeroRow5(i, rList1);
            }
        }
    }
    return 0;
}
int nonzeroRowNum(int rList1[][5], int nRow){
    int counter=nRow;
    for (int i=0; i<nRow; i++){

        if (rList1[i][0]==0 && rList1[i][1]==0 &&
            rList1[i][2]==0 && rList1[i][3]==0 && rList1[i][4]==1){
                //if this is a zeroed row
                counter--;
            }
    }
    return counter;
}

void updaterList2(int r1, int r2, int rList2[][4], int &m, int rList1[][5]){

    for (int j=0; j<4; j++){
        rList2[m][j]=rList1[r1][j];
    }
    m++;
    for (int j=0; j<4; j++){
        rList2[m][j]=rList1[r2][j];
    }
    m++;

}
int getrList2(int rList1[][5], int rList2[][4], int r1){
    /*
    This function pair non-zero rows in rListTemp into rList2
    placed together. r1 is the # of rows in rList1.
    */

    int m=0; //this m is used for update rList2

    for (int i=0; i<r1; i++){
        rList1[i][4]=0;
    }
    //previously you have set all rows'[4] to be 1 and you need to reset them back to
    //0 so that you can do the marking again in this function

    for (int i=0; i<r1; i++){

        if (rList1[i][4]==0){
            int index=searchPair(i, rList1, r1);

            if(index!=-1){
                rList1[i][4]=1;
                rList1[index][4]=1;
                updaterList2(i, index, rList2, m, rList1);
            }

        }
    }

    return 0;
}

int divideRound(int a, int b){
    double c= (double)a/(double)b;
    if (c>= a/b+0.5){
        return a/b+1;
    }else{
        return a/b;
    }
}

int checkProportion1(int rList1[][5], int n){
    /*
    This function will take rList2 and eliminate the rows that does not satisfy the proportion test.
    It will return the number or rows that pass the test.
    */
    int counter=n;
    for (int i=0; i<n; i++){
        int p=divideRound(rList1[i][3], rList1[i][2]);
        if (rList1[i][2]>0 && p==5 ){
            //pass proportion test
        }else{
            //fail proportion test; Be careful that we should delete BOTH rows.


            zeroRow5(i, rList1);
            counter-=2;
        }
    }
    return counter;
}

int checkProportion2(int rList1[][5], int n){
    /*
    This function will take rList2 and eliminate the rows that does not satisfy the proportion test.
    It will return the number or rows that pass the test.
    */
    int counter=n;
    for (int i=0; i<n; i++){

        if (rList1[i][2]>0 && rList1[i][3]<=rList1[i][2]*5+1 &&
            rList1[i][3]>=rList1[i][2]*5-1){
            //pass proportion test
        }else{
            //fail proportion test; Be careful that we should delete BOTH rows.


            zeroRow5(i, rList1);
            counter-=2;
        }
    }
    return counter;
}

void getrList3(int rList2[][4], int size, int rList3[][4]){
    //size is the size of rList2,
    //now the rList contains only zero rows and paired rectangles that have already passed the proportion test.

    int n=0;
    for (int i=0; i<size; i++){
        if (rList2[i][0]!=0 || rList2[i][1]!=0 || rList2[i][2]!=0 || rList2[i][3]!=0){
            for (int j=0; j<4; j++){
                rList3[n][j]=rList2[i][j];
            }
            n++;
        }
    }
}

void getDistance(double distance[], int nPairs, int rList3[][4]){
    for (int i=0; i<nPairs; i++){

        double x=(rList3[2*i][2]+rList3[2*i+1][2])/(2.0)/(2.0);
        distance[i]=x;
    }
}

void print1DArray(double list[], int n){
    for(int i=0; i<n; i++){
        cout<< list[i]<<" | ";
    }
    cout<<endl;
}

int findMaxIndex(double list[], int size){
    double max=list[0];
    int index=0;
    for (int i=1; i<size; i++){
        if (list[i]>max){
            max=list[i];
            index=i;
        }
    }
    return index;
}

void printPair(int index, int rList3[][4]){
    //index is the index in the distance list;
    cout<<"{ ";
    for (int i=0; i<2; i++){
        cout<<" [ ";

        for (int j=0; j<4; j++){
            cout<<rList3[2*index+i][j];
            if (j<3) cout<<" | ";
        }

        if (i<2-1) cout<<" ] , ";

    }
    cout<<" }";
}


void intorListTemp(int rList1[][5], int rListTemp[][5], int r1, int rTemp){
    int k=0;

    for (int i=0; i<r1; i++){
        if (rList1[i][0]!=0 || rList1[i][1]!=0 || rList1[i][2]!=0 || rList1[i][3]!=0){
            for (int j=0; j<4; j++){
                rListTemp[k][j]=rList1[i][j];
            }
            rListTemp[k][4]=0;
            k++;
        }
    }

}

bool IntervalTest(int rList2[][4], int r){
    int x1=rList2[r][0];
    int h=rList2[r][3];
    int x2=rList2[r+1][0];
    int w=rList2[r][2];

    int interval=x1-x2;
    if (interval<0) interval=-interval;

    //now we get the distance between x1 and x2;
    //possibilities: 1. need to deduct the width? 2. need to check proportional interval?

    return (divideRound(2*(interval-w), 3*h)==1);
}

int checkInterval(int rList2[][4], int size){
    int m=size;
    for (int i=0; i<size; i+=2){
        if(!IntervalTest(rList2, i)){
            zeroRow4(i, rList2);
            zeroRow4(i+1, rList2);
            m-=2;
        }
    }

    return m;
}

int main()
{
    int r1=readr1();
    int rList1[r1][5]={0};//rList1 will store input.

    readRects(rList1, r1);
    cout<<"Initial rList1:"<<endl;
    printrList5(rList1, r1);
    cout<<"---------------------------------------------------------------------"<<endl;

    int nProportionalRects=checkProportion1(rList1, r1);
    //nProportionalRects is the number of proportioned rows/ number of rects detected in rList1 after checking for proportion.
    cout<<"rList1 after checking proportion!"<<endl;
    printrList5(rList1, r1);
    cout<<"---------------------------------------------------------------------"<<endl;


    zerorList1(rList1, r1);
    cout<<"rList1's single rows zeroed!"<<endl;
    printrList5(rList1, r1);
    cout<<"---------------------------------------------------------------------"<<endl;



    int nProportionalNonsingleRects=nonzeroRowNum(rList1, r1);
    //nProportionalNonsingleRects1 is the number of nonzero rows in rList2 after eliminating single rectangles.
    cout<<"nProportionalNonsingleRects: "<<nProportionalNonsingleRects<<endl;


    if (nProportionalNonsingleRects>=2){
        int rListTemp[nProportionalNonsingleRects][5]={0};
        intorListTemp(rList1, rListTemp, r1, nProportionalNonsingleRects);
        cout<<"rListTemp: "<<endl;
        printrList5(rListTemp, nProportionalNonsingleRects);

        int rList2[nProportionalNonsingleRects][4]={0};


        getrList2(rListTemp, rList2, nProportionalNonsingleRects);
        cout<<"updated rList2!"<<endl;
        printrList4(rList2,nProportionalNonsingleRects);
        cout<<"---------------------------------------------------------------------"<<endl;

        int nProportionalNonsingleIntervalRects=checkInterval(rList2, nProportionalNonsingleRects);
        int rList3[nProportionalNonsingleIntervalRects][4]={0};
        getrList3(rList2, nProportionalNonsingleRects, rList3);

        cout<<"Interval checked rlist3: "<<endl;
        printrList4(rList3, nProportionalNonsingleIntervalRects);

        int nPairs=nProportionalNonsingleIntervalRects/2;
        //here we make distance a list of doubles, instead of integers
        double distance[nPairs]={0};
        getDistance(distance, nPairs, rList3);
        cout<<"Distance list!"<<endl;
        print1DArray(distance, nPairs);
        cout<<"---------------------------------------------------------------------"<<endl;

        int maxIndex=findMaxIndex(distance, nPairs);
        printPair(maxIndex, rList3);
    }else{
        cout<<"Less than two valid rectangles, no pair."<<endl;
    }

    return 0;
}

