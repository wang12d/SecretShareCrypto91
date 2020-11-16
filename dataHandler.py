from functools import reduce

filename = 'exp.txt'
avgname = 'avgexp.txt'
n = 2
avgData = []
allData = []
addList = lambda x, y: [i+j for i, j in zip(x, y)]
with open(filename, 'r') as fp:
    lines = fp.readlines()
    for l in lines:
        allData.append([float(i) for i in l.strip().split()])
    for i in range(0, len(allData), n):
        ls = allData[i:i+n]
        res = list(reduce(addList, ls))
        avgData.append(list(map(lambda x: x / n, res)))
with open(avgname, 'a') as fp:
    for avg in avgData:
        print(','.join(list(map(lambda x: str(x), avg))), file=fp)
    