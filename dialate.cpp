void colorReduce(Mat &image){
  int nl = image.rows;
  int nc = image.cols;
  for (int j=2; j<nl-2; j=j+3){//from 3rd rows, until the last 2 rows
    const unchar* r1= image.ptr<const unchar>(j-2);
    const unchar* r2= image.ptr<const unchar>(j-1);
    const unchar* r3= image.ptr<const unchar>(j);
    const unchar* r4= image.ptr<const unchar>(j+1);
    const unchar* r5= image.ptr<const unchar>(j+2);
    for (int i=2; i<nc-2; i=i+3){
      if (r3[i]==255)
        r3[i-2] = 255;
        r3[i-1] = 255;
        r3[i+1] = 255;
        r3[i+2] = 255;
    }
  }
}
