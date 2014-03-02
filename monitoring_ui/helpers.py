import pygame
import time
import sys
import agent
import location
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
"""	
def getBrainColor(brainString):
	#L1;4:5.4:3;L2;3:4:2,2	
	try:
		parts = brainString.split(';')
		level = 0
		tot = 0	
		i = 0	
		for part in parts:
			if(part == "L1"):
				level = 1
				i = 0
			elif(part == "L2"):
				level = 2
				i = 0
			else:
				conn = part.split(':')
				if(level == 1):
					i += 1
					tot += int(conn[0])%50 #make it all inthe same range so it doesn't get too far off...
					tot += int(conn[2])%50
				elif(level == 2):
					i += 1
					tot += int(conn[0])%50
					tot += ((int(conn[2]))*i)%50 #There are few outputs, but why the fuck use 'i'?
		return numberToRGB(int(tot/2.0)%maxRGBNumber)
	except:
		print "GetBrainColor: Unexpected error: %i,%i,%i"%(tot,maxRGBNumber,int(tot/4.0)%maxRGBNumber), sys.exc_info()[0]
		return (0,0,0)
"""

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
	f = getSpeciesStatFile():
	if(f == 0):
		return []
	speciesStats = [] #List of lists
	for line in f.readlines():
		sl = line.split(',')
		speciesStats.append(sl)
	return speciesStats	

import os
def tail(f):
	stdin,stdout = os.popen2("tail -n 1 "+f)
	stdin.close()
	lines = stdout.readlines(); stdout.close()
	return lines[0]

def getListOfSimulationStats(f):
	#TODO: The species section here is totally unused at this point
	lastLine = tail(f)
	speciesNumber = -1
	speciesStatList = []
	simulationStatList = []
	for stat in lastLine.split(' '):
		keyValuePair = stat.split(',')
		key = keyValuePair[0]
		value = keyValuePair[1]
		if(key == 'speciesNumber'):
			speciesNumber += 1
			speciesStatList.append({}) #Add the dictionary
			speciesStatList[speciesNumber]['speciesNumber'] = float(value) #Save the real species number just in case
		elif(speciesNumber >= 0):
			speciesStatList[speciesNumber][key] = float(value)
		elif(speciesNumber == -1):
			simulationStatList.append([key,value])
	return simulationStatList, speciesStatList


"""def getAgentData(line):
 	try:
		pairs = line.split(' ')
		for pair in pairs:
			stat = pair.split(',')
			if(stat[0] == 'xLoc'):
				x = int(stat[1])
			if(stat[0] == 'yLoc'):
				y = int(stat[1])
			if(stat[0] == 'energy'): 
				e = float(stat[1])
			if(stat[0] == 'latestDecision'):
				ld = int(stat[1])
			if(stat[0] == 'br'):
				bs = stat[1]
			if(stat[0] == 'age'):
				a = int(stat[1])
			if(stat[0] == 'generation'):
				gen = int(stat[1])
		return [x,y,e,a,gen,ld,bs]
	except:
		print "Failed to understand agent"
		return [-1,-1,-1,-1,-1,-1]
# -----------
# -- STATS --
# -----------
def getStats():
	try:
		listOfStats = []
		f = open("./data_from_simulations/monitor.txt")
		line = list(f)[-1] #We only want the last line
		pairs = line.split(' ');
		for pair in pairs:
			stat = pair.split(',')
			if(len(stat) >= 2):
				listOfStats.append([stat[0],stat[1]])
		return listOfStats
	except:
		print "UI Error: I'm not finding the monitor file"
		return 0 #The file might be corrupt because we're in the middle of reading it. That's ok, we'll just wait.

def findStat(name,statList):
	try:
		for stat in statList:
			if(stat[0] == name):
				return stat[1]
		print "UI Error: Didn't find the stat you wanted"
		return None
	except:
		print "IU Error: Took an error when looking up a stat"
		return None	
"""
# -------------------
# -- FILE HANDLING --
# -------------------
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
		else:
			print "Failed to get a valid speciesStat version\n"
			return 0
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
