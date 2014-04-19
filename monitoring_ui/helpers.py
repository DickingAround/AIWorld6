import pygame
import time
import sys
import agent
import location
import speciesStat
# -----------------
# -- BRAIN COLOR --
# -----------------
maxRGBNumber = 1530
rgbToNumberDict = {}
numberToRGBDict = {}
def initRGBToNumberDicts():
	#have to cover every iteration or both single and doulbe 0s but with one always saturated
	#b -> 255 (0  ,255,1  )(0  ,255,255) 0-254  -- this one is actually 265 wide, others are not becuase of overlap
	#g -> 0   (0  ,254,255)(0  ,0  ,255) 255-509
	#r -> 255 (1  ,0  ,255)(255,0  ,255) 510-764
	#b -> 0	  (255,0  ,254)(255,0  ,0  ) 765-1019
	#g -> 255 (255,1  ,0  )(255,255,0  ) 1020-1274
	#r -> 0   (254,255,0  )(0  ,255,0  ) 1275-1529
	for i in range(0,maxRGBNumber):
		if(i < 255):
			rgbToNumberDict[     (0    ,255  ,i+1)] = i
			numberToRGBDict[i] = (0    ,255  ,i+1)
		elif(i < 510):
			rgbToNumberDict[     (0    ,255-(i-254),255)] = i
			numberToRGBDict[i] = (0    ,255-(i-254),255)
		elif(i < 765):
			rgbToNumberDict[     (i-509    ,0    ,255)] = i
			numberToRGBDict[i] = (i-509    ,0    ,255)
		elif(i < 1020):
			rgbToNumberDict[     (255  ,0    ,255-(i-764))] = i
			numberToRGBDict[i] = (255  ,0    ,255-(i-764))
		elif(i < 1275):
			rgbToNumberDict[     (255  ,i-1019    ,0)] = i
			numberToRGBDict[i] = (255  ,i-1019    ,0)
		else: #1530
			rgbToNumberDict[     (255-(i-1274),255  ,0)] = i
			numberToRGBDict[i] = (255-(i-1274),255  ,0)
initRGBToNumberDicts()
def rgbToNumber(rgb):
	return rgbToNumberDict[rgb]
def numberToRGB(number):
	return numberToRGBDict[int(number)%maxRGBNumber]	

def getColorOfHash(number):
	return numberToRGB(number) #This is where we'd modify the equation. 

# -------------------------
# -- Simple Data Getters --
# -------------------------
def getListOfAgents(version):
	worldFile = getWorldFile(version)
	if(worldFile == 0):
		return []
	listOfAgents = []
	for line in worldFile.readlines():
		a = agent.agent(line)
		listOfAgents.append(a)
	return listOfAgents

def getListOfLocations(version):
	locationFile = getLocationFile(version)
	if(locationFile == 0):
		return [];
	listOfLocations = []
	locationFile.readline() #Read the first line and throw it away, it's just world size and whatnot
	for line in locationFile.readlines():
		l = location.location(line)
		listOfLocations.append(l)
	return listOfLocations

def getListOfSpeciesStats():
	f = getSpeciesStatFile()
	if(f == 0):
		return []
	speciesStats = [] #List of lists
	i = 0
	for line in f.readlines():
		s = speciesStat.speciesStat(line,i)
		speciesStats.append(s)
		i += 1
	return speciesStats	

import os
def tail(f):
	stdin,stdout = os.popen2("tail -n 1 "+f)
	stdin.close()
	lines = stdout.readlines(); stdout.close()
	return lines[0]

def getListOfSimulationStats(f):
	lastLine = tail(f)
	speciesNumber = -1
	simulationStatList = []
	for stat in lastLine.split(' '):
		keyValuePair = stat.split(',')
		simulationStatList.append(keyValuePair)
	return simulationStatList


# ---------------------
# -- SCREEN HANDLING --
# ---------------------
def clearDisplay(window):
	window.fill((0,0,0))

def saveScreen(window,name):
	pygame.image.save(window,"./images_and_video/%04d.png"%name)

# -------------------
# -- FILE HANDLING --
# -------------------
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

def getLocationFile(version):
	try:
		if(version == 'a'):
			f = open("./data_from_simulations/world_locs_a.txt")
		elif(version == 'b'):
			f = open("./data_from_simulations/world_locs_b.txt")
		else:
			print "Failed to get a valid loc version\n"
			return 0
	except:
		print "Failed to find and or open the loc file\n"
		return 0
	return f

def getSpeciesStatFile():
	try:
		f = open("./data_from_simulations/speciesStats.txt")
	except:
		print "Failed to find and or open the speciesStat file\n"
		return 0
	return f



def getWorldFile(version):
	try:
		if(version == 'a'):
			f = open("./data_from_simulations/world_agents_a.txt")
		elif(version == 'b'):
			f = open("./data_from_simulations/world_agents_b.txt")
		else:
			print "Failed to get a valid version\n"
			return 0
	except:
		print "Failed to find and or open the world file\n"
		return 0
	return f

def getMonitorFileName():
	return "./data_from_simulations/monitor.txt"
def getMonitorFile():
	try:
		f = open("./data_from_simulations/monitor.txt")
	except:
		print "Failed to open the monitor file\n"
		return 0
	return f	
