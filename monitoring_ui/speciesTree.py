import pygame
import agent
from helpers import *
import speciesStats

#Just a color chart, forever scrolling, with how many things are in each bucket.
class speciesTree():
	iterationsSeen = -1 #Get incremented immediately on prep
	duration = 300
	width = 383 #If the colors are at 1530 grainularity, this will be about 4 different collors into one bin
	agentCount  = [[0 for x in xrange(width)] for x in xrange(duration)] 	

#0:decisions, 1:Moves, 2:Turns, 3:Attacks, 4:Grows, 5:AsexReps, 6:SexReps, 7:FailedMoves, 8:FailedReps, 9:FailedAttacks, 10:FailedGrows, 11:SimReportSize, 12:KilledByAttack, 13:KilledByStarving, 14:AveBrainSize, 15:AveEnergy, 16:AveAge, 17:AveGen
def drawTree(window,x,y,speciesStatList):
	#Save the latest round of stats
	prepNewSpeciesLevel()
	for thisSpeciesStats in speciesStatList:
		saveSpeciesData(thisSpeciesStats.getDecisions(),thisSpeciesStats.hashNumb)
	#Draw the tree
	drawSpeciesTree(window,x,y)	

def resetSpeciesStatCounters():
	for i in range(0,speciesTree.width):
		speciesTree.agentCount[speciesTree.iterationsSeen][i] = 0

def prepNewSpeciesLevel():
	#Clear this line
	speciesTree.iterationsSeen += 1 
	if speciesTree.iterationsSeen >= speciesTree.duration:
		speciesTree.iterationsSeen = 0
	for i in range(0,speciesTree.width):
		speciesTree.agentCount[speciesTree.iterationsSeen][i] = 0
	resetSpeciesStatCounters()

def saveSpeciesData(c,hashNumb):
	color = getColorOfHash(hashNumb)
	numb = convertRGBToCountIndex(color)
	speciesTree.agentCount[speciesTree.iterationsSeen][numb] += c

#define AG_M_F 0 
#define AG_M_L 1
#define AG_M_R 2
#define AG_T_L 3
#define AG_T_R 4
#define AG_A_F 5
#define AG_R   6
#define AG_R_F 7
#define AG_GROW 8

'''def detectSpeciesInGivenRound(roundsAgo):
	speciesNumber = -1
	gapNeeded = 5 #Min number of speaces between colors to count as different species
	minNumb = 10 #Min number for this to be counted as occupied
	waitingToSeeNewSpecies = False
	speciesList = []
	distanceFromSpecies = 0
	startingSpot = 0
	#print speciesTree.agentCount[0]
	#Go till you find a detectable species, starting spot will be the exact number where a new one begins
	for i in range(0,speciesTree.width):
		count = speciesTree.agentCount[(speciesTree.iterationsSeen-roundsAgo)%speciesTree.duration][i]
		if(count <= minNumb):
			distanceFromSpecies += 1
		else:
			distanceFromSpecies = 0
		if(distanceFromSpecies >= gapNeeded):
			startingSpot = i - 1
			waitingToSeeNewSpecies = True
			break	
	#print "starting at %i, going to %i"%(startingSpot,speciesTree.width+startingSpot+1)
	if(not waitingToSeeNewSpecies): #Didn't find a gap
		return []
	#Then start parsing to find the species
	for i in range(startingSpot,speciesTree.width+startingSpot+gapNeeded):
		count = speciesTree.agentCount[(speciesTree.iterationsSeen-roundsAgo)%speciesTree.duration][i%speciesTree.width]
		if(count <= minNumb): #No agents here
			if(distanceFromSpecies >= gapNeeded and (not waitingToSeeNewSpecies)): #end of a species
				waitingToSeeNewSpecies = True #Start looking for a new speices
				speciesList[speciesNumber][1] = (i - gapNeeded)%speciesTree.width #The max is one past the real number
			distanceFromSpecies += 1
		else: #Agents here
			distanceFromSpecies = 0
			if(waitingToSeeNewSpecies): #New species found! (unless it wraps around)
				waitingToSeeNewSpecies = False
				speciesList.append([i%speciesTree.width,-1,0]) #minColor, maxColor, counter(for sorting)
				speciesNumber += 1	
			speciesList[speciesNumber][2] += count
	#print speciesList
	return speciesList
'''
def convertRGBToCountIndex(color):
	#Take the color, convert it to a number, and then scale it relative to the conversion
	return int(rgbToNumber(color)*speciesTree.width/maxRGBNumber)
def convertCountIndexToRGB(n):
	#Take the number, scale it relative to the conversion, then convert to a color
	return numberToRGB(int((n)*maxRGBNumber/speciesTree.width))

'''
def drawSpeciesStats(window,x,y,species,i):
	fontSize = 18
	font = pygame.font.Font(None,fontSize)
	if(species[0] <= species[1]): 
		color = convertCountIndexToRGB(int((species[0]+species[1])/2.0))
		agentsLabel = "Agents: %i"%species[2]
		energyLabel = "AveEnergy: %f"%(float(sum(speciesTree.agentEnergy[species[0]:species[1]]))/float(species[2]))
		attackLabel = "AttackRate: %f"%(float(sum(speciesTree.agentAttacks[species[0]:species[1]]))/float(species[2]))
		ageLabel = "Age: %f"%(float(sum(speciesTree.agentAge[species[0]:species[1]]))/float(species[2]))
		generationLabel = "Generation: %f"%(float(sum(speciesTree.agentGeneration[species[0]:species[1]]))/float(species[2]))
	else:	#The color from the max can wrap around past the min
		color = convertCountIndexToRGB(int(((species[0]+species[1]+speciesTree.width)/2.0)%speciesTree.width))
		agentsLabel = "Agents: %i"%species[2]
		energyLabel = "AveEnergy: %f"%(float(sum(speciesTree.agentEnergy[:species[1]])+sum(speciesTree.agentEnergy[species[0]:]))/float(species[2]))
		attackLabel = "AttackRate: %f"%(float(sum(speciesTree.agentAttacks[:species[1]])+sum(speciesTree.agentAttacks[species[0]:]))/float(species[2]))
		ageLabel = "Age: %f"%(float(sum(speciesTree.agentAge[:species[1]])+sum(speciesTree.agentAge[species[0]:]))/float(species[2]))
		generationLabel = "Generation: %f"%(float(sum(speciesTree.agentGeneration[:species[1]])+sum(speciesTree.agentGeneration[species[0]:]))/float(species[2]))
	numbStats = 5
	sur = font.render(agentsLabel,1,color)
	window.blit(sur,(x,y+(numbStats*fontSize)*i))
	sur = font.render(energyLabel,1,color)
	window.blit(sur,(x,y+fontSize+(numbStats*fontSize)*i))
	sur = font.render(attackLabel,1,color)
	window.blit(sur,(x,y+fontSize*2+(numbStats*fontSize)*i))
	sur = font.render(ageLabel,1,color)
	window.blit(sur,(x,y+fontSize*3+(numbStats*fontSize)*i))
	sur = font.render(generationLabel,1,color)
	window.blit(sur,(x,y+fontSize*4+(numbStats*fontSize)*i))

#If a species has been around for 5 data-points and is at least 5 colors distant from any other, it's a species.
#Every agent must be part of a species. The most-agents color is the color.	
def drawDetectedSpecies(window,x,y):
	speciesList = detectSpeciesInGivenRound(0) #(((minColor1),(maxColor1)),((minColor2),(maxColor2)))
	speciesList.sort(key = lambda row: row[2], reverse=True)
	for i in range(0,len(speciesList)):
		if(i < 3 and speciesList[i][2] > 10): #There must be more than 10 of them for them to be a species...
			drawSpeciesStats(window,x,y,speciesList[i],i)	
		else:
			break
'''
def drawSpeciesTree(window,x,y):
	for j in range(0,speciesTree.duration):
		m = float(max(speciesTree.agentCount[(speciesTree.iterationsSeen-j)%speciesTree.duration]))
		if not m == 0:
			for i in range(0,speciesTree.width):
				color = convertCountIndexToRGB(i) #numberToRGB(int((i+0.5)*maxRGBNumber/speciesTree.width)) 
				tmp = [int(t * (speciesTree.agentCount[(speciesTree.iterationsSeen-j)%speciesTree.duration][i] / m)) for t in color]
				r = tmp[0]
				g = tmp[1]
				b = tmp[2]
				colorWithIntensity = (r,g,b)
				try:
					window.set_at((x+i,y+j),colorWithIntensity);
					#window.set_at((x+i,y+100),color);
				except:
					print "DrawSpeciesTree: Bad color?"
					print colorWithIntensity
					raise

def speciesTree_test():
	speciesTree.width = 20
	speciesTree.agentCount[0] = [1,1,0,0,0, 0,0,0,1,1, 0,1,1,1,0, 0,0,0,0,0]
	speciesTree.iterationsSeen = 0
	print detectSpeciesInGivenRound(0)

'''
[0, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 17, 10, 32, 231, 481, 649, 405, 1012, 1785, 773, 1198, 1170, 2794, 2256, 2635, 2091, 1978, 824, 528, 438, 442, 285, 242, 151, 503, 660, 1075, 982, 1788, 1058, 1973, 1301, 537, 756, 1084, 1242, 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 8, 4, 9, 1, 6, 12, 10, 19, 46, 54, 67, 36, 36, 64, 174, 180, 188, 66, 33, 24, 11, 6, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[[162, 198, 35417], [234, 258, 1064], [281, 281, 1], [1, -1, 6]]
'''
