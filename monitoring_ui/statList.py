import pygame
import time
import statGraphs
import helpers
def statList_draw(window,xLoc,yLoc):
	font = pygame.font.SysFont(None,18)
	i = 0
	statList = helpers.getStats()
	statGraphs.saveStats(statList)
	if(statList == 0):
		return 0 #In case there's no data to get
	for stat in statList:
		textToDisplay = "%s: %s"%(stat[0].strip(),stat[1].strip()) #You cant use \t here
		sur = font.render(textToDisplay,1,(150,150,150))
		window.blit(sur,(xLoc,yLoc+i*18))
		i = i+1
	
	
