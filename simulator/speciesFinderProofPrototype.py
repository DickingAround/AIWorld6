import random
p = []
pop = 800
hist = {}
minShow = 0
maxShow = 30
for i in range(0,pop):
	p.append(int(random.random()*3)+10) #number from 1 to 100
for i in range(1,1000):	
	#Do the histogram and viewing
	for j in range(minShow,maxShow):
		hist[j] = 0
	for j in range(0,pop):
		if(p[j] >= minShow and p[j] < maxShow):
			hist[p[j]] += 1
	s = ''
	for j in range(minShow,maxShow):
		s += "%3i "%hist[j]
	print s	

	#Do the population simulations
	if(i % 5 == 0): #Cull the herd
		start = int(random.random()*pop)
		agentA = p[start]
		agentB = p[(start + pop/2)%pop]
		agentC = p[(start + pop/4)%pop]
		for agentNumb in range(0,pop):
			if(agentNumb < pop/4): #We'll keep two of the lucky winners
				p[agentNumb] = agentC
			elif(agentNumb < pop/2):
				p[agentNumb] = agentB
			else:
				p[agentNumb] = agentA
	else: #Replicate
		for agentNumb in range(0,pop):	
			if(random.random() > 0.5):
				p[agentNumb] += 1
			else:
				p[agentNumb] -= 1
		
