import sys
import math

class UPGMA:
    def __init__(self, matrix):
        self.__matDims = len(matrix)
        self.matrix = matrix
        self.__clusterCount = [[1 for x in range(self.__matDims)] for y in range(self.__matDims)]

    
    def cluster(self):
        clusterCount = self.__clusterCount
        matDims = self.__matDims
        minimum = sys.maxsize
        minRow = 0
        minCol = 0
        row = 0
        col = 0

        for matRow in self.matrix:
            for value in matRow:
                if (value < minimum) & (value > 0):
                    minimum = value
                    minRow = row
                    minCol = col
                col += 1
            row += 1
            col = 0

        tmpMatrix = [[0 for x in range(matDims - 1)] for y in range(matDims - 1)]
        tmpClusterCount = [[1 for x in range(matDims - 1)] for y in range(matDims - 1)]

        tmpPos = 1

        score = 0

        """ for num in range(matDims):
            if (num == 0) | (num == minCol):
                continue
            else:
                cluster1 = clusterCount[minRow][num]
                cluster2 = clusterCount[minCol][num]
                score = ((self.matrix[minRow][num] * cluster1) + (self.matrix[minCol][num] * cluster2))/(cluster1 + cluster2)
                tmpMatrix[minRow][tmpPos] = score
                tmpMatrix[tmpPos][minRow] = score
                if minRow != tmpPos:
                    tmpClusterCount[minRow][tmpPos] += 1
                    tmpClusterCount[tmpPos][minRow] += 1
                else:
                    tmpClusterCount[minRow][tmpPos] += 1
                tmpPos += 1 """

        tmpRow = 0
        tmpCol = 1

        missing = []

        for r in range(matDims):
            for c in range(matDims):
                if ((r == c) | (r == minCol) | (c == minRow) | ((r == minRow) & (c == minRow)) | ((r == minRow) & (c == minCol)) | ((r == minCol) & (c == minRow)) | ((r == minCol) & (c == minCol))):
                    continue
                elif ((r == minRow)):
                    cluster1 = clusterCount[minRow][c]
                    cluster2 = clusterCount[minCol][c]
                    tmpMatrix[minRow][tmpCol] = ((self.matrix[minRow][c] * cluster1) + self.matrix[minCol][c])/(cluster1 + cluster2)
                    tmpClusterCount[minRow][tmpCol] += 1
                    tmpCol += 1
                    if (tmpCol == self.__matDims - 1):
                        tmpRow += 1
                        tmpCol = 0
                elif((c == minCol)):
                    cluster1 = clusterCount[r][minRow]
                    cluster2 = clusterCount[r][minCol]
                    tmpMatrix[tmpRow][minRow] = ((self.matrix[r][minCol] * cluster1) + (self.matrix[r][minRow] * cluster1))/(cluster1 + cluster2)
                    tmpClusterCount[tmpRow][minRow] += 1
                    tmpRow += 1

        for r in range(matDims - 1):
            for c in range(matDims - 1):
                if ((r != c) & (tmpMatrix[r][c] == 0.0)):
                    missing.append((r, c))

        
        for r in range(matDims):
            for c in range(matDims):
                if ((r == c) | (r == minRow) | (c == minRow) | (r == minCol) | (c == minCol)):
                    continue
                else:
                    
                    tmpRow, tmpCol = missing.pop(0)
                    tmpMatrix[tmpRow][tmpCol] = self.matrix[r][c]

        self.matrix = tmpMatrix
        self.__matDims = len(tmpMatrix)
        self.__clusterCount = tmpClusterCount

            

        
        

matrix = [[0, 17, 21, 31, 23], [17, 0, 30, 34, 21], [21, 30, 0, 28, 39], [31, 34, 28, 0, 43], [23, 21, 39, 43, 0]]

c = UPGMA(matrix)

c.cluster()
for r in c.matrix:
    print(r)
print("\n")
c.cluster()
for r in c.matrix:
    print(r)
print("\n")
c.cluster()
for r in c.matrix:
    print(r)





