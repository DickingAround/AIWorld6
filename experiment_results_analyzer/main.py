def test():
	res = """treatment,a1 iterations,2 aveEnergy,30
treatment,a2 iterations,2 aveEnergy,0
treatment,a1 iterations,3 aveEnergy,40
treatment,b1 iterations,2 aveEnergy,10
treatment,b2 iterations,2 aveEnergy,10"""
	finalRes = getFinalStatsOfEachRun(res)
	compareStats(finalRes)	
	exit(1)

def checkDataVersion():
	try:
		f = open("./data_from_simulations/world_whichFileToUse.txt")
		s = f.readlines()
		f.close()
		if(s[0] == checkDataVersion.lastVersion):
			return 'x'
		else:
			checkDataVersion.lastVersion = s[0];
			return s[0]
	except:
		print "Didn't find which file to use"
		return 'x'
checkDataVersion.lastVersion = 'c';

def compareStats(res):
	averages = {}#{stat:(a,b)}
	dataPoints = 0
	for run in res.iteritems():
		dataPoints += 1
		treatment = run[0][0] #Should be either 'a' or 'b'
		if(treatment != 'a' and treatment != 'b'):
			print "Wasn't able to recognize the treatment"
		stats = run[1][1] #Should be a list of stats
		for stat in stats.iteritems():
			if not stat[0] in averages:
				averages[stat[0]] = [0,0,0,0]
			try:	
				if treatment == 'a':
					averages[stat[0]][0] += float(stat[1])
					averages[stat[0]][1] += float(stat[1])*float(stat[1])
				elif treatment == 'b':
					averages[stat[0]][2] += float(stat[1])
					averages[stat[0]][3] += float(stat[1])*float(stat[1])
			except:
				1 ==1	
				#print "didnt understand %s"%stat[1]
	print "Averages"
	print averages
			
#form of output is { treatment:[iter,{dictOfStats}], ...}
def getFinalStatsOfEachRun(res):
	#Now get only the last results of each simulation
	finalRes = {}
	for line in res.split('\n'):
		stats = line.split(' ')
		statDict = {}
		for stat in stats:
			pairs = stat.split(',')
			statDict[pairs[0]] = pairs[1]
		treatment = statDict['treatment']
		iteration = statDict['iterations']
		if not (treatment in finalRes):
			finalRes[treatment] = [0,{}]
		if(finalRes[treatment][0] <= iteration):
			finalRes[treatment][0] = iteration
			finalRes[treatment][1] = statDict	
	return finalRes

if __name__ == '__main__':
	test()
	version = checkDataVersion();
	if(version == 'x'):
		exit(1)
	f = open("./data_from_simulations/monitor_%c.txt"%version) 
	res = f.readlines()
	finalRes = getFinalStatsOfEachRun(res)
	print finalRes
	compareStats(finalRes)	
