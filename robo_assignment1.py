def prepareData(path):
    InputFile = open(path,'r')
    num = InputFile.readline()
    List = []
    for line in InputFile:
        List.append(line.replace('\n','').split())
    return num,List


if __name__ == '__main__':
    path = './input_sample1'
    num,L = prepareData(path)
    count = 0
    pair = []

    # Assume y axis is accurate, not doing round here
    for i in range(16):
        for j in range(i+1,int(num)):
            if L[i][1] == L[j][1]:
                count += 1
                pair.append( (L[i],L[j]) )
                break

    # Build a bias list to rule out those incorrect pairs 
    bias = []
    propotion = []
    for p in pair:
        data1 = p[0]
        data2 = p[1]
        propotion_WH = (float(data1[2]) / 2 + float(data1[3]) / 10 + float(data2[2]) / 2 + float(data2[3]) / 10) / 4   
        propotion_D = abs(float(data1[0]) - float(data2[0])) / 15
        propotion.append(propotion_D)
        bias.append( abs(propotion_D-propotion_WH) / max(propotion_D,propotion_WH) )
        
    #Pick out the closest pair which is also bias-accepted
    closest = 0
    for index in range(len(pair)):
        if bias[index] < 0.1:
            if propotion[index] > propotion[closest] :
                closest = index
    
    print(pair[closest])
