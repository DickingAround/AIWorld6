import pygame
import time
from helpers import *
import speciesTree
import landMaps

# ------------------
# -- Map of energy--
# ------------------
def drawEnergy(window,renderProps,listOfAgents):
	font = pygame.font.SysFont(None,20)
	sur = font.render("Energy map (low:red,high:green)",1,(150,150,150))
	window.blit(sur,(renderProps.uiX,renderProps.uiY+5))	
	renderProps.uiY = renderProps.uiY+20 #Add space for the title	
	for a in listOfAgents:
		a.drawEnergy(window,renderProps)

# -------------------
# -- Map of signal --
# -------------------
def drawSignal(window,renderProps,listOfLocations):
	font = pygame.font.SysFont(None,20)
	sur = font.render("Signal map",1,(150,150,150))
	window.blit(sur,(renderProps.uiX,renderProps.uiY+5))
	renderProps.uiY = renderProps.uiY+20 #Add space for the title	
	for l in listOfLocations:
		l.drawSignal(window,renderProps)

# ---------------------
# -- Map of barriers --
# ---------------------
def drawBarriers(window,renderProps,listOfLocations):
	for l in listOfLocations:
		l.drawBarrier(window,renderProps)

# ---------------------------
# -- Map of action/species --
# ---------------------------
def drawAgents(window,renderProps,listOfAgents,listOfLocations):
	#If it's a small-size view, then show a label
	if(renderProps.compression == 0):
		font = pygame.font.SysFont(None,20)
		sur = font.render("Agent map (with barriers in grey)",1,(150,150,150))
		window.blit(sur,(renderProps.uiX,renderProps.uiY+5))
		renderProps.uiY = renderProps.uiY+20 #Add space for the title 	
	drawBarriers(window,renderProps,listOfLocations) #Only draw the locations if we're in litte-map mode for now
	for a in listOfAgents:
		a.drawAgent(window,renderProps)
