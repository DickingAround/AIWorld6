import pygame
import time
import worldMaps
import statList
import statGraphs
import autoCapture
import helpers
import speciesTree
import speciesStats
#import simulationStats
def clearDisplay(window):
	window.fill((0,0,0))

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

def saveScreen(window,name):
	pygame.image.save(window,"./images_and_video/%04d.png"%name)

if __name__ == '__main__':
        import sys
	displayX = 1280 #300 for text, 450 more for graphs (750), 900-1200 is time graphs
	displayY = 720 #WorldGraph is 400ish
        imgNumb = 0
        imgMax = 10000
	pygame.init()
	window = pygame.display.set_mode((displayX,displayY))
	#pygame.draw.line(window,(100,100,100),(0,0),(100,100),1)
	pygame.display.flip()
	keepGoing = 1
	while keepGoing == 1:
		time.sleep(0.3)
		version = checkDataVersion()
		if(version != 'x'):
			clearDisplay(window)
			autoCapture.checkForAndSaveSpecialSituations()
			listOfAgents = helpers.getListOfAgents(version)
			simulationStatsDict, speciesStatsList = helpers.getListsOfStats(helpers.getMonitorFileName())
			worldMaps.drawMain(window,0,0,listOfAgents,3) #600 x 600, spacing = 3
			#worldMaps.drawEnergy(window,600,0) #200 x 200
			#worldMaps.drawSignal(window,600,200) #200 x 200
			speciesTree.saveNewSetOfAgents(listOfAgents)
			speciesTree.drawTree(window,600,400) #200 x 320
			speciesStats.drawStats(window,0,800,speciesStatsList) #480 x 720
			#simulationStats.drawStats(window,0,600,simulationStatsDict) #120 x 600
			pygame.display.flip()	
			if(imgNumb < imgMax):
				saveScreen(window,imgNumb)
				imgNumb += 1
		for event in pygame.event.get():
			if event.type == pygame.QUIT or event.type == pygame.KEYDOWN:
				keepGoing = 0
