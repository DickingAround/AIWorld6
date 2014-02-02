import pygame
import math
class statGraphs:
	iterationsSeen = 0
	statList = [0] * 100
def statGraphs_draw(window,x,y,xSize,ySize):
	stats = ['agents','attacks','attackFailRate','killedByAttacks','killedByStarving','aveEnergy']
	for i in range(0,len(stats)):
		statGraphs_drawAGraph(window,stats[i],x,y+i*ySize/len(stats),xSize,ySize/len(stats))
	return 0
def statGraphs_drawAGraph(window,stat,x,y,xSize,ySize):
	#Draw the label
	font = pygame.font.SysFont(None,20)
	sur = font.render(stat,1,(200,200,200))
	window.blit(sur,(x,y))
	#Get the data	
	ma,mi,listOfData = gatherData(stat)
	if(listOfData == []):
		return 0	
	#Draw the data labels
	surMax = font.render(str(ma),1,(150,150,150))	
	surMin = font.render(str(mi),1,(150,150,150))	
	window.blit(surMax,(x,y+15))
	window.blit(surMin,(x,y+ySize-15))
	#Draw the box
	pygame.draw.rect(window,(100,100,100),(x-1,y,xSize,ySize),1)	
	#Plot the data	
	y += 30 #Move down the graph to make room for the text we added
	ySize -= 45
	for i in range(0,len(listOfData)-1): #Only looking at pairs since we draw two lines
		if((listOfData[i] != None) and (listOfData[i+1] != None)):
			pygame.draw.line(window,(200,200,200),
				(int(x+(i)*(xSize/float(len(statGraphs.statList)))),int(y+ySize-(listOfData[i]-mi)*ySize/(ma-mi))),
				(int(x+(i+1)*(xSize/float(len(statGraphs.statList)))),int(y+ySize-(listOfData[i+1]-mi)*ySize/(ma-mi))),
				1)
			#print "data y is d:%f, y:%f, ySize:%f"%(listOfData[i],y,ySize)
			#print "ma:%f, mi:%f"%(ma,mi)
			#print "Pos is %f %f"%(x+(i)*(xSize/len(statGraphs.statList)),y+(listOfData[i]-mi)*ySize/(ma-mi))
			#(x+(i)*(xSize/len(statGraphs.statList)),y+(listOfData[i]-mi)*ySize/(ma-mi)),
			#(x+(i+1)*(xSize/len(statGraphs.statList)),y+(listOfData[i+1]-mi)*ySize/(ma-mi)))
def gatherData(stat):
	listOfData = []
	for i in range(0,len(statGraphs.statList)):
		try:	
			#print "iters seen %i, i %i, statList %i"%(statGraphs.iterationsSeen,i,len(statGraphs.statList))
			tmp = statGraphs.statList[(statGraphs.iterationsSeen+i)%len(statGraphs.statList)][stat]
			#print "Tmp is %s"%str(tmp)
			listOfData.append(tmp)
		except:
			1 == 1;
	if(len(listOfData) < 2):
		return 0,0,[] #Nothing to show
	ma = max(listOfData)
	mi = min(listOfData)
	if ma == mi:
		ma += 0.5
		mi -= 0.5
	return ma,mi,listOfData	
	

def saveStats(statList):
	statGraphs.statList[statGraphs.iterationsSeen] = {}
	for stat in statList:	
		try:	
			if(stat[0] != "time" and not math.isnan(float(stat[1])) and not math.isinf(float(stat[1])) ):
				statGraphs.statList[statGraphs.iterationsSeen][stat[0]] = float(stat[1])
				#print "I just saved the stat %s from %s"%(str(float(stat[1])),stat[1])
		except:
			print "Wasn't able to save the stat '%s' '%s'"%(stat[0],stat[1])
			#statGraphs.statList[statGraphs.iterationsSeen][stat[0]] = None
	statGraphs.iterationsSeen += 1
	if(statGraphs.iterationsSeen >= len(statGraphs.statList)):
		statGraphs.iterationsSeen = 0
	#Input is a list of pairs
	return 0
