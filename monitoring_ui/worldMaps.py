import pygame
import time
from helpers import *
import speciesTree
import landMaps



# ------------------
# -- Map of energy--
# ------------------
def drawEnergy(window,x,y,listOfAgents):
	for a in listOfAgents:
		a.drawEnergy(window,x,y)

# -------------------
# -- Map of signal --
# -------------------
def drawSignal(window,x,y,listOfLocations):
	for l in listOfLocations:
		l.drawSignal(window,x,y)

# ---------------------------
# -- Map of action/species --
# ---------------------------
def drawMain(window,x,y,listOfAgents,spacing):
	#TODO: Also we want to draw the terrain
	for a in listOfAgents:
		a.drawAgent(window,x,y,spacing)

def drawAgentDecision(window,x,y,size,d):
	color = (0,0,0)
	if(d >= 0 and d <= 2):
		color = (0,0,255)
	if(d >= 3 and d <= 4):
		color = (0,125,255)
	if(d == 5):
		color = (255,0,0)
	if(d >= 6 and d <= 7):
		color = (255,0,255)
	if(d == 8):
		color = (0,255,0)
	#pygame.draw.rect(window,color,(x*size+xOffset,y*size+yOffset,size,size))		
	window.set_at((x,y),color)
#define AG_M_F 0 
#define AG_M_L 1
#define AG_M_R 2
#define AG_T_L 3
#define AG_T_R 4
#define AG_A_F 5
#define AG_R   6
#define AG_R_F 7
#define AG_GROW 8

def drawAgentEnergy(window,x,y,size,e):
	#Want to increase in color from green to red as they get stronger (red:255,0,0 green:0,255,0)
	if(e < 1000): #All Red-to-yellow ratio
		r = 255
		g = e/1000*255
		b = 0
	elif(e < 10000):
		r = e/10000*255
		g = 255
		b = 0
	elif(e < 100000):
		r = 255
		g = 255
		b = e/100000*255
	else:
		r = 255
		g = 255
		b = 255
	try:
		#pygame.draw.rect(window,(r,g,b),(x*size+xOffset,y*size+yOffset,size,size))		
		window.set_at((x,y),(r,g,b));
	except:
		print "Problem with the agent's energy color:"
		print e
		print (r,g,b)

def drawAgentBrain(window,x,y,size,brainString):
	color = getBrainColor(brainString)
#	pygame.draw.rect(window,color,(xLoc*size+xOffset,yLoc*size+yOffset,size,size))
	window.set_at((x,y),color)

def worldMaps_draw(window,xOffset,yOffset,version):
	f = getWorldFile(version)
	if( f == 0):
		return 0
	speciesTree.prepNewSpeciesLevel()	
	for line in f.readlines():
		x,y,e,a,gen,ld,bs = getAgentData(line)
		drawAgentBrain(window,x+xOffset,y+yOffset+30,1,bs)
		speciesTree.saveSpeciesData(e,a,gen,ld,bs)	
		drawAgentEnergy(window,x+xOffset,y+yOffset+350,1,e)
		drawAgentDecision(window,x+xOffset+300,y+yOffset+350,1,ld)
	speciesTree.drawSpeciesTree(window,xOffset+300,yOffset+30)
	speciesTree.drawDetectedSpecies(window,xOffset+690,yOffset+30)
	landMaps.drawLandMaps(window,xOffset+600,yOffset+350,version)
	font = pygame.font.Font(None,25)
	label = "Hash of agent's neural network"
	sur = font.render(label,1,(150,150,150));
	window.blit(sur,(xOffset,yOffset+10))
	label = "Species tree"
	sur = font.render(label,1,(150,150,150));
	window.blit(sur,(xOffset+300,yOffset+10))
	label = "Agent's energy"
	sur = font.render(label,1,(150,150,150));
	window.blit(sur,(xOffset,yOffset+330))
	label = "Agent's last decision"
	sur = font.render(label,1,(150,150,150));
	window.blit(sur,(xOffset+300,yOffset+330))
	label = "Signals sent from Agents"
	sur = font.render(label,1,(150,150,150));
	window.blit(sur,(xOffset+600,yOffset+330))
	label = "Decision color key:"
	sur = font.render(label,1,(150,150,150));
	window.blit(sur,(xOffset+80,yOffset+650))
	label = "Moving/Turning"
	sur = font.render(label,1,(0,0,255));
	window.blit(sur,(xOffset+240,yOffset+650))
	label = "Growing"
	sur = font.render(label,1,(0,255,0));
	window.blit(sur,(xOffset+380,yOffset+650))
	label = "Attacking"
	sur = font.render(label,1,(255,0,0));
	window.blit(sur,(xOffset+455,yOffset+650))
	label = "Replicating"
	sur = font.render(label,1,(255,0,255));
	window.blit(sur,(xOffset+545,yOffset+650))
	f.close()
