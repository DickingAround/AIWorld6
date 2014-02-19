import pygame
import helpers
#Draw the species stats in the color of the species

def drawSpeciesStats(window,x,y,statDict,positionNumber):
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
	#Get the species color
	color = helpers.numberToRGB((statDict['speciesHashMin']+statDict['speciesHashMax'])/2)
	#Draw out the stats we care about
	statList = ['numberOfAgents','aveAge','aveGeneration','aveEnergy','attacks','grows','asexReplications','sexReplications']
	fontSize = 18
        font = pygame.font.Font(None,fontSize)
	i = 0
	for stat in statList:
		label = "%s:%f"%(stat,statDict[stat])
		sur = font.render(label,1,color)
		window.blit(sur,(x+xOffset,y+yOffset+(fontSize)*i))
		i += 1

def drawStats(window,x,y,speciesStatList):
	for i in range(0,len(speciesStatList)):
		drawSpeciesStats(window,x,y,speciesStatList[i],i)

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
