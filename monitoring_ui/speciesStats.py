import pygame
import helpers
import scipy.stats as stats
import speciesStat
#Draw the species stats in the color of the species

def detectSpecies(speciesStatList):
	minNumber = 1
	minDist = 10
	speciesList = []
	recordingSpeciesNow = 0
	minHash = -1
	distanceSinceLastSawSpecies = -1
	for i in enumerate(speciesStatList):
		numberOfAgents = int(i[1].getDecisions()/i[1].getSimReportSize())
		if(recordingSpeciesNow == 0):#Not recording a species yet
			if(numberOfAgents >= minNumber):
				minHash = i[0]
				distanceSinceLastSawSpecies = 0
				recordingSpeciesNow = 1
		else:#Recording species
			if(numberOfAgents >= minNumber):
				distanceSinceLastSawSpecies = 0
			else:
				distanceSinceLastSawSpecies += 1
				if(distanceSinceLastSawSpecies >= minDist):
					speciesList.append([minHash,i[0] - minDist])
					recordingSpeciesNow = 0
	return speciesList
			
 
def detectSpecies_KDE(speciesStatList):
	#First we need to change the species stats to be a list of hash values, not sums of the number in a given hash
	speciesHashList = []
	hashMin = -1
	hashMax = -1
	for i in enumerate(speciesStatList):
		numberOfAgents = int(i[1].getDecisions()/i[1].getSimReportSize()) 
		for j in range(0,numberOfAgents): #The first value of the species stat list is how many of them there are
			speciesHashList.append(i[0])
		if(numberOfAgents >= 0.5):#Really, it's greater than 0 but it's a float value
			hashMax = i[0] #Save the highest value hash present
			if(hashMin == -1):
				hashMin = i[0] #Save the lowest value hash present
	print "HashMin:%i, HashMax:%i"%(hashMin,hashMax)
	if( hashMin == -1 or hashMax == -1):
		print "Didn't find any agents"
		return []
	#Then run the kernel to find out the species
	kernel = stats.gaussian_kde(speciesHashList)	
	print "Finished building the kernel"
	#Then walk through the list looking for where the minimas are, those minimas are the limits between species.
	speciesList = []
	speciesFound = 0
	previousMin = hashMin
	searchGrainularity = 1
	for i in range(hashMin,hashMax+1,int(searchGrainularity)): #TODO: Checking the kernel is expensive. We need to be more intelligent about it.
		if(kernel.evaluate(i) < kernel.evaluate(i+1) and kernel.evaluate(i) < kernel.evaluate(i-1)):
			print "found min at %i"%i
			speciesList.append([previousMin,i])
			previousMin = i
	speciesList.append([previousMin,hashMax])
	print "returning species list: ",speciesList
	return speciesList
	#return [[0,10000]]

def collectStatsBySpecies(speciesStatList,speciesHashRangeList):
	statsBySpecies = []
	for speciesLimits in enumerate(speciesHashRangeList): #For each species...
		statsBySpecies.append(speciesStat.speciesStat(''))
		statsBySpecies[speciesLimits[0]].hashMed = 0
		statsBySpecies[speciesLimits[0]].hashMin = speciesLimits[1][0]
		statsBySpecies[speciesLimits[0]].hashMax = speciesLimits[1][1] #First thing, save the species hash limits
		#For each hash, get all the metrics
		for hashNumber in range(speciesLimits[1][0],speciesLimits[1][1]+1): #For each hash
			for statNumber in range(0,len(speciesStatList[0].statList)): #For each stat
				if(statNumber == 0): #Record the number of agents as part of finding the hash med
					statsBySpecies[speciesLimits[0]].hashMed += float(speciesStatList[hashNumber].statList[statNumber])*hashNumber
				if(statNumber == 11): #The sim duration is an exception
					statsBySpecies[speciesLimits[0]].statList[statNumber] = float(speciesStatList[hashNumber].statList[statNumber])
				else:
					statsBySpecies[speciesLimits[0]].statList[statNumber] += float(speciesStatList[hashNumber].statList[statNumber])
		#print "about to compute the hashMed",statsBySpecies[speciesLimits[0]].hashMed,statsBySpecies[speciesLimits[0]].getDecisions() 
		statsBySpecies[speciesLimits[0]].hashMed = statsBySpecies[speciesLimits[0]].hashMed / statsBySpecies[speciesLimits[0]].getDecisions() 
	return statsBySpecies
	
def drawSpeciesStats(window,x,y,thisSpeciesStats,positionNumber):
	#Get the location to draw them in (we only draw up to four)
	xOffset = 0
	yOffset = 0	
	if(positionNumber == 0):
		xOffset = 0
	elif(positionNumber == 1):
		xOffset = 140
	elif(positionNumber == 2):
		xOffset = 280
	elif(positionNumber == 4):
		xOffset = 420
	elif(positionNumber == 5):
		xOffset = 560
	else:
		return 0 #last one goes to 700
	'''if(positionNumber == 1):
		yOffset = 220
	elif(positionNumber == 2):
		xOffset = 140
	elif(positionNumber == 3):
		xOffset = 140
		yOffset = 220
	elif(positionNumber == 4):
		xOffset = 280
	elif(positionNumber == 5):
		xOffset = 280
		yOffset = 220
	elif(positionNumber >= 6):
		return 0  #We only display 6'''
	
	#Get the species color
	color = helpers.getColorOfHash(thisSpeciesStats.hashMed)
	#Draw out the stats we care about
	numberOfAgents = thisSpeciesStats.getDecisions()/thisSpeciesStats.getSimReportSize() #The number of decisions divided by the number of iterations 
	statList = [['avePopulation',numberOfAgents]]
	statList.append(['aveAge',thisSpeciesStats.getAveAge()/thisSpeciesStats.getDecisions()])
	statList.append(['aveGeneration',thisSpeciesStats.getAveGen()/thisSpeciesStats.getDecisions()])
	statList.append(['aveEnergy',thisSpeciesStats.getAveEnergy()/thisSpeciesStats.getDecisions()])
	statList.append(['aveBrainSize',thisSpeciesStats.getAveBrainSize()/thisSpeciesStats.getDecisions()])
	statList.append(['attacks',thisSpeciesStats.getAttacks()/thisSpeciesStats.getDecisions()])
	statList.append(['grows',thisSpeciesStats.getGrows()/thisSpeciesStats.getDecisions()])
	statList.append(['move/turn',(thisSpeciesStats.getMoves()+thisSpeciesStats.getTurns())/thisSpeciesStats.getDecisions()])
	if(thisSpeciesStats.getAttacks() != 0):
		statList.append(['failedAttacks',thisSpeciesStats.getFailedAttacks()/thisSpeciesStats.getAttacks()])
	statList.append(['killedByAttack',thisSpeciesStats.getKilledByAttack()])
	statList.append(['killedByStarving',thisSpeciesStats.getKilledByStarving()])
	statList.append(['asexReplications',thisSpeciesStats.getAsexReps()/thisSpeciesStats.getDecisions()])
	statList.append(['sexReplications',thisSpeciesStats.getSexReps()/thisSpeciesStats.getDecisions()])
	statList.append(['speciesHashMin',thisSpeciesStats.hashMin])
	statList.append(['speciesHashMax',thisSpeciesStats.hashMax])
	fontSize = 14
        font = pygame.font.Font(None,fontSize)
	i = 0
	for stat in statList:
		label = "%s:%f"%(stat[0],stat[1])
		if(stat[0] == 'speciesHashMin' or stat[0] == 'speciesHashMax'):
			sur = font.render(label,1,helpers.getColorOfHash(stat[1]))
		else:
			sur = font.render(label,1,color)
		window.blit(sur,(x+xOffset,y+yOffset+(fontSize)*i))
		i += 1

#0:decisions, 1:Moves, 2:Turns, 3:Attacks, 4:Grows, 5:AsexReps, 6:SexReps, 7:FailedMoves, 8:FailedReps, 9:FailedAttacks, 10:FailedGrows, 11:SimReportSize, 12:KilledByAttack, 13:KilledByStarving, 14:AveBrainSize, 15:AveEnergy, 16:AveAge, 17:AveGen
def drawStats(window,x,y,speciesStatList):
	#Do clustering
	speciesHashRangeList = detectSpecies(speciesStatList)
	statsBySpecies = collectStatsBySpecies(speciesStatList,speciesHashRangeList)
	#print "statsBySpecies is", statsBySpecies
	sortedStats = sorted(statsBySpecies, key=lambda k: k.getDecisions(), reverse=True)
	for statList in enumerate(sortedStats):
		#print "About to draw some stats for", statList
		drawSpeciesStats(window,x,y,statList[1],statList[0])
	
'''	#Do stat printing
	for i in range(0,len(speciesStatList)):
		print " %i of %i ------------------------------------------- "%(i,len(speciesStatList))
		print speciesStatList[i]
	newStatList = sorted(speciesStatList, key=lambda k: k['numberOfAgents'], reverse=True)
	for i in range(0,min(len(newStatList),4)):
		drawSpeciesStats(window,x,y,newStatList[i],i)
'''
def test2():
	displayX = 1280 #300 for text, 450 more for graphs (750), 900-1200 is time graphs
	displayY = 720 #WorldGraph is 400ish
	x = 1280 - 400
	y = 0
	pygame.init()
	window = pygame.display.set_mode((displayX,displayY))
	pygame.display.flip()
	speciesStatList = getTopSpeciesStats("./speciesStats_testData.txt") #This should be an array of ductionaries.
	for i in range(0,len(speciesStatList)):
		drawSpeciesStats(window,x,y,speciesStatList[i],i)
	pygame.display.flip()

def test():
	displayX = 1280 #300 for text, 450 more for graphs (750), 900-1200 is time graphs
	displayY = 720 #WorldGraph is 400ish
	x = 1280 - 400
	y = 0
	pygame.init()
	window = pygame.display.set_mode((displayX,displayY))
	pygame.draw.line(window,(100,100,100),(0,0),(100,100),1)
	pygame.display.flip()
	speciesStatList = [{'speciesHashMin':1000,'speciesHashMax':1010,'numberOfAgents':10,'aveAge':11,'aveGeneration':12,'aveEnergy':13,'attacks':14,'grows':15,'asexReplications':16,'sexReplications':17},{'speciesHashMin':2000,'speciesHashMax':2010,'numberOfAgents':20,'aveAge':21,'aveGeneration':22,'aveEnergy':23,'attacks':24,'grows':25,'asexReplications':26,'sexReplications':27},{'speciesHashMin':3000,'speciesHashMax':3010,'numberOfAgents':30,'aveAge':31,'aveGeneration':32,'aveEnergy':33,'attacks':34,'grows':35,'asexReplications':36,'sexReplications':37}] 
	for i in range(0,len(speciesStatList)):
		drawSpeciesStats(window,x,y,speciesStatList[i],i)
	pygame.display.flip()
