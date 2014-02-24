from numpy import array
from scipy.cluster.vq import vq, kmeans, whiten
#features = array([[ 1.9,2.3],[ 1.5,2.5],[ 0.8,0.6],[ 0.4,1.8],[ 0.1,0.1],[ 0.2,1.8],[ 2.0,0.5],[ 0.3,1.5],[ 1.0,1.0]])
#features = [[1],[1],[1],[2],[80],[70],[70],[100],[80],[80],[80]]
features = []
f = open('./data_from_simulations/speciesHist.txt','r')
for line in f.readlines():
	parts = line.split('\t')
	for i in range(0,int(parts[1])):
		features.append([float(parts[0])])	
featArray = array(features)
whitened = whiten(features)
print features
print whitened
book = array(([14900],[15050],[15300],[15500],[15650],[15900],[16100]))
#book = array((whitened[0],whitened[2]))
print book
#book = 6
#print kmeans(whitened,book)
print kmeans(featArray,book)
