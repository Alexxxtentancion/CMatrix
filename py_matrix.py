import myMatrix


def matrixmult(m1,m2):
    s=0     #сумма
    t=[]    #временная матрица
    m3=[] # конечная матрица
    if len(m2)!=len(m1[0]):
        print ("Матрицы не могут быть перемножены")
    else:
        r1=len(m1)
        c1=len(m1[0])
        r2=c1
        c2=len(m2[0])
        for z in range(0,r1):
            for j in range(0,c2):
                for i in range(0,c1):
                   s=s+m1[z][i]*m2[i][j]
                t.append(s)
                s=0
            m3.append(t)
            t=[]
    return m3


x = [[1,2,5],[3,4,5]]
y = [[1,2],[3,4],[3,3]]

if __name__ == '__main__':
    m1 = myMatrix.Matrix([[1,2,3],[12,3,4]])
    m2 = myMatrix.Matrix([[1, 2, 3], [12, 3, 4]])
    m3 = myMatrix.add(m1,m2)
    print(str(m3))
