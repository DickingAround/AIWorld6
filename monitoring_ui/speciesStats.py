import pygame
import helpers
#Draw the species stats in the color of the species

def detectSpecies(speciesStatList):
	#TODO: Run some sort of speciation code.
	return [[0,10000]]

def collectStatsBySpecies(speciesStatList,speciesHashRangeList):
	statsBySpecies = []
	for speciesLimits in enumerate(speciesHashRangeList):
		statsBySpecies.append([speciesLimits[1][0],speciesLimits[1][1]]) #First thing, save the species hash limits
		#Init the metrics for this species to all zeros
		for statNumber in range(0,len(speciesStatList[0])):
			statsBySpecies[speciesLimits[0]].append(0)
		#For each hash, get all the metrics
		for hashNumber in range(speciesLimits[1][0],speciesLimits[1][1]):
			for statNumber in range(0,len(speciesStatList[0])):
				statsBySpecies[speciesLimits[0]][statNumber+2] += float(speciesStatList[hashNumber][statNumber])
	return statsBySpecies
	
#TODO: This is almost certaintly wrong, fix it
def drawSpeciesStats(window,x,y,speciesStats,positionNumber):
	print "drawing species stat",x,y,positionNumber
	#Get the location to draw them in (we only draw up to four)
	xOffset = 0
	yOffset = 0	
	if(positionNumber == 1):
		yOffset = 350
	elif(positionNumber == 2):
		xOffset = 200	
	elif(positionNumber == 3):
		xOffset = 200
		yOffset = 350
	elif(positionNumber >= 4):
		return 0  #We only display 4
	#Get the species color
	color = helpers.getColorOfHash((speciesStats[0]+speciesStats[1])/2)
	#Draw out the stats we care about
	statList = [['numberOfAgents',speciesStats[2]]]
	numberOfAgents = speciesStats[2]/speciesStats[13] #The number of decisions divided by the number of iterations 
	statList.append(['aveAge',speciesStats[18]/numberOfAgents])
	statList.append(['aveGeneration',speciesStats[19]/numberOfAgents])
	statList.append(['aveEnergy',speciesStats[17]/numberOfAgents])
	statList.append(['attacks',speciesStats[5]/speciesStats[2]])
	statList.append(['grows',speciesStats[6]/speciesStats[2]])
	statList.append(['asexReplications',speciesStats[7]/speciesStats[2]])
	statList.append(['sexReplications',speciesStats[8]/speciesStats[2]])
	statList.append(['speciesHashMin',speciesStats[0]])
	statList.append(['speciesHashMax',speciesStats[1]])
	fontSize = 18
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
	print "statsBySpecies is", statsBySpecies
	sortedStats = sorted(statsBySpecies, key=lambda k: k[0], reverse=True)
	for statList in enumerate(sortedStats):
		print "About to draw some stats for", statList
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
