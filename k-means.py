import sys
import random
import math
import time

class Kmeans:
    def __init__(self, k, set, tolerance):
        self.__k = k
        self.__set = set
        self.__tolerance = tolerance
        self.__means = []
        self.sectors = {}

    def __createSectors(self, sectors, means, set):
        for num in set:
            dictMean = 0
            lowest = sys.maxsize
            for mean in means:
                dist = math.sqrt((num - mean) ** 2)
                if (dist < lowest):
                    lowest = dist
                    dictMean = mean
            sectors[dictMean].append(num)
        
        return sectors

    def __newMeans(self, sectors):
        means = []
        for key in sectors.keys():
            sector = sectors[key]
            value = 0
            for num in sector:
                value += num
            if (len(sector) != 0):
                mean = value / len(sector)
            else:
                mean = key
            means.append(mean)
        return means


    def run(self):
        k = self.__k
        set = self.__set
        means = []
        sectors = {}
        minimum = min(set)
        maximum = max(set)
        rng = maximum - minimum

        for i in range(1, k+1):
            mean = (i/(k+1) * rng) + minimum
            means.append(mean)

        notEqual = True

        while notEqual:
            sectors = {}
            for mean in means:
                sectors[mean] = []
            sectors = self.__createSectors(sectors, means, set)
            newMeans = self.__newMeans(sectors)
            notEqual = False
            pos = 0
            for newMean in newMeans:
                mean = means[pos]
                pos += 1
                if math.sqrt((newMean - mean) ** 2) > self.__tolerance:
                        notEqual = True
                    
            means = newMeans
        
        self.sectors = sectors

    



if __name__ == "__main__":
    lst = []
    for x in range(200):
        lst.append(random.randint(-500, 500))
    km = Kmeans(10, lst, 0)
    km.run()

    for entry in km.sectors.keys():
        print("centroid:", entry, ": ", km.sectors[entry])