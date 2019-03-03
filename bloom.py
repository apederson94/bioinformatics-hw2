class BloomFilter:
    def __init__(self, size):
        self.size = size
        self.filter = [0] * size

    def add(self, element):
        self.filter[self.__hash1(element, self.size)] = 1
        self.filter[self.__hash2(element, self.size)] = 1

    def __hash1(self, toHash, size):
        num = 0
        for sym in toHash:
            num += 300 * ord(sym) - 99
        num = num % size
        return num

    def __hash2(self, toHash, size):
        num = 0
        for sym in toHash:
            num += + 200 * ord(sym) + 900
        num = num % size
        return num

    def print(self):
        for num in self.filter:
            print(num)

    def find(self, element):
        if self.filter[self.__hash1(element, self.size)] == 1:
            print("match")
        elif self.filter[self.__hash2(element, self.size)] == 1:
            print("match")
        else:
            print("not in filter")

if __name__ == "__main__":
    bloom = BloomFilter(1000)

    bloom.add("YELLOW")

    print("YELLOW: ")

    bloom.find("YELLOW")

    print("PINK: ")

    bloom.find("PINK")