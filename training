import cv2
import copy
cap=cv2.VideoCapture('rob.mp4')
while(cap.isOpened()):
    ret, frame=cap.read()
    gray=cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
    cv2.imshow('frame',gray)
    if cv2.waitKey(1)&0xFF ==ord('q'):
        break
cap.releas()
ret,thresh=cv2.threshold(p_gray,127,255,cv2.THRESH_BINARY)
cv2.imwrite('p_binary.jpg',thresh)
image=cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
img = cv2.drawContours(image, -1, 0, (0,255,0), 3)
cv2.imshow('image',img)
cv2.waitKey(0)
s=Scalar(img)
number=s(0)
dict_a=dict()
x=dict()
y=dict()
w=dict()
h=dict()
lit=list()
for i in range(number):
    a=s(i+1)
    dict_a[i]=a
    t=a.split()
    x[i]=int(ti[0])
    y[i]=int(ti[1])
    w[i]=int(ti[2])
    h[i]=int(ti[3])
t=list()
def screen1(a):
    ratio=w[a]/h[a]
    if abs(ratio-5)<1:
        pass
    else:
        break    
def screen2(a):
    p_w=(w[a]/2)
    p_h=(h[a]/10)
    if abs(p_w-p_h)<0.1:
        pass
    else:
        break
def screen1(a,b):
    p=dict()
    p[a]=h[a]/10
    p[b]=h[b]/10
    if abs(p[a]-p[b])< 0.5:
        if abs(y[a]*p[a]-y[b]*p[b])<1:
            pass
        else:
            break
    else:
        break
def screen2(a,b):
    if abs(15-distance*p[a])<2:
        pass
    else:
        break
for a in range(1,i+1):

    for b in range(a):
        dx=x[b]-x[a]
        dy=y[b]-y[a]
        p_w=(w[a]/2)
        p_h=(h[a]/10)
        
        if abs(p_w-p_h)>0.1:
            continue
        distance=(dx^2+dy^2)^(1/2)
        screen1(a)
        screen2(a)
        screen1(b)
        screen2(b)
        screen1(a,b)
        screen2(a,b)
        def proposion(a,b):
            proposion(a,b)=p[a]
            return proposion(a,b)
        lit.append(proportion(a,b))
        pair(a,b)=dict_a[a]+dict_a[b]
        d=dict()
        dict[proportion(a,b)]=pair(a,b)
short=min(lit)
print(d[short])

        
        
        


