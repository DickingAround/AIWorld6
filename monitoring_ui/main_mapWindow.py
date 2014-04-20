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
import mapProperties
#import simulationStats
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
		version = helpers.checkDataVersion()
		if(version != 'x'):
			helpers.clearDisplay(window)
			#autoCapture.checkForAndSaveSpecialSituations()
			stage_start = time.clock()
			listOfAgents = helpers.getListOfAgents(version)
			stage_loadAgents = time.clock()
			listOfLocations = helpers.getListOfLocations(version)
			stage_loadLocations = time.clock()
			listOfSimulationStats = helpers.getListOfSimulationStats(helpers.getMonitorFileName())
			stage_loadSimulationStats = time.clock()
			#listOfSpeciesStats = helpers.getListOfSpeciesStats()
			#stage_speciesStats = time.clock()
			print "LoadParseTimes: listOfAgents:%f, listOfLocations:%f, simStats:%f"%(stage_loadAgents - stage_start, stage_loadLocations - stage_loadAgents, stage_loadSimulationStats - stage_loadLocations)
			stage_start = time.clock()	
			worldMaps.drawAgents(window,mapProperties.mapProperties(0,0, 200,200, 400,400, 3),listOfAgents,listOfLocations) #600 x 600, spacing = 3	
			stage_zoomView = time.clock()	
			worldMaps.drawAgents(window,mapProperties.mapProperties(650,50, 0,0, 400,400, 0),listOfAgents,listOfLocations) #600 x 600, spacing = 3
			stage_worldView = time.clock()	
			simulationStats.drawStats(window,0,600,listOfSimulationStats,120) #120 x 600 (this function needs to know where the bottom is)
			stage_simulationStats = time.clock()
			print "RenderTimes: zoomView:%f, worldView:%f, simulationStats:%f"%(stage_zoomView - stage_start, stage_worldView - stage_zoomView, stage_simulationStats - stage_worldView)
			pygame.display.flip()	
			if(imgNumb < imgMax):
				#helpers.saveScreen(window,imgNumb)
				imgNumb += 1
		for event in pygame.event.get():
			if event.type == pygame.QUIT or event.type == pygame.KEYDOWN:
				keepGoing = 0




