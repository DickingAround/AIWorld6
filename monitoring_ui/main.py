import pygame
import time
import worldMaps
import statList
import statGraphs
import autoCapture
import helpers
import speciesTree
import speciesStats
import simulationStats
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
			#autoCapture.checkForAndSaveSpecialSituations()
			stage_start = time.clock()
			listOfAgents = helpers.getListOfAgents(version)
			stage_loadAgents = time.clock()
			listOfLocations = helpers.getListOfLocations(version)
			stage_loadLocations = time.clock()
			listOfSimulationStats = helpers.getListOfSimulationStats(helpers.getMonitorFileName())
			stage_loadSimulationStats = time.clock()
			listOfSpeciesStats = helpers.getListOfSpeciesStats()
			stage_speciesStats = time.clock()
			print "LoadParseTimes: listOfAgents:%f, listOfLocations:%f, simStats:%f, speciesStats:%f"%(stage_loadAgents - stage_start, stage_loadLocations - stage_loadAgents, stage_loadSimulationStats - stage_loadLocations, stage_speciesStats - stage_loadSimulationStats)
			stage_start = time.clock()	
			worldMaps.drawMain(window,0,0,listOfAgents,3) #600 x 600, spacing = 3
			stage_main = time.clock()	
			worldMaps.drawEnergy(window,600,0,listOfAgents) #200 x 230
			stage_energy = time.clock()	
			worldMaps.drawSignal(window,600,230,listOfLocations) #200 x 230, we leave 30 room for the labels
			stage_signal = time.clock()	
			speciesTree.drawTree(window,400,600,listOfSpeciesStats) #400 x 120, room out to 800 wide, remember to change the graph's functions
			stage_tree = time.clock()	
			speciesStats.drawStats(window,810,0,listOfSpeciesStats) #480 x 720
			stage_speciesStats = time.clock()	
			simulationStats.drawStats(window,0,600,listOfSimulationStats,120) #120 x 600 (this function needs to know where the bottom is)
			stage_simulationStats = time.clock()
			print "RenderTimes: map:%f, energy:%f, signal:%f, speciesTree:%f, speciesStats:%f, simulationStats:%f"%(stage_main - stage_start, stage_energy - stage_main, stage_signal - stage_energy, stage_tree - stage_signal, stage_speciesStats - stage_tree, stage_simulationStats - stage_speciesStats)
			pygame.display.flip()	
			if(imgNumb < imgMax):
				saveScreen(window,imgNumb)
				imgNumb += 1
		for event in pygame.event.get():
			if event.type == pygame.QUIT or event.type == pygame.KEYDOWN:
				keepGoing = 0
