import cv2
import copy
cap=cv2.VideoCapture('rob.mp4')
while(cap.isOpened()):
    ret, frame=cap.read()
    gray=cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
    ret,thresh=cv2.threshold(gray,127,255,0)
    contours=cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)[1]
    cont_good=list()
    for i in range(len(contours)-1):
        cnt=contours[i]
        epsilon = 0.4*cv2.arcLength(cnt,True)
        approx = cv2.approxPolyDP(cnt,epsilon,True)
        area=cv2.contourArea(approx)
        x,y,w,h=cv2.boundingRect(approx)
        area2=w*h
        if abs(area2-area)<100:
            cont_good.append(contours[i])       
    cv2.drawContours(frame, cont_good, -1, (0,255,0), 3)
    cv2.imshow('image',frame)
    if cv2.waitKey(1)&0xFF ==ord('q'):
        break
    

        
        
        


