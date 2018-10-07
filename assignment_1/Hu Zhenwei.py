import itertools
n=int(input())
listofl=[]
l=[]
requiredl=[]
for i in range(0,n):
    numbers=input()
    l=numbers.split(' ')
    listofl.append(l)
for i in range(0,n):
    if (int(listofl[i][3])//int(listofl[i][2])==4 or int(listofl[i][3])//int(listofl[i][2])==5):
        requiredl.append(listofl[i])
for a, b in itertools.combinations(requiredl, 2):
    if (abs(int(a[0])-int(b[0])%15)==0) or (abs(int(a[1])-int(b[1]))%15==0):
        print (a,b)
        
