import pygame
def drawStats(window,x,y,statList,maxDepth):
	fontSize = 16
	font = pygame.font.Font(None,fontSize)
	xOffset = 5 #Give it a little space from the left wall. Y doesn't need it becasue the world has a border and the world map is right above
	yOffset = 0
	lineWidth = 200
	for stat in statList:
		#Write the stat
		sur = font.render("%s:%s"%(stat[0],stat[1]),1,(150,150,150))
		window.blit(sur,(x+xOffset,y+yOffset))
		#Compute the location, which will go down and then to the left as it fills up	
		yOffset += fontSize
		if(yOffset + fontSize > maxDepth):
			xOffset += lineWidth
			yOffset = 0
