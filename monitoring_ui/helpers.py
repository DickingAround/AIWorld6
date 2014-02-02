import pygame
import time
import sys
'''def getBrainColor(brainString):
	#L1;4:5.4:3;L2;3:4:2,2	
	try:
		parts = brainString.split(';')
		level = 0
		r = 0
		g = 0
		b = 0
		for part in parts:
			if(part == "L1"):
				level = 1
			elif(part == "L2"):
				level = 2
			else:
				conn = part.split(':')
				if(level == 1):
					r += int(conn[0])
					b += int(conn[2])
				elif(level == 2):
					g += int(conn[2])
		r = (r/2)%255
		g = (g/2)%255	
		b = (b/2)%255
		#Saturate the colors, where at least 1 is 255 so that we can render the species tree using the saturation of the colors to show population %s
		m = max(r,g,b)
		r = (int)(r * 255.0/m)
		g = (int)(g * 255.0/m)
		g = (int)(b * 255.0/m)
		return (r,g,b)
	except:
		return (0,0,0)
'''
maxRGBNumber = 1535
rgbToNumberDict = {}
numberToRGBDict = {}
#TODO: I know this function is slightly broken, try 0,255,254->254 and then 0,255,255->256; it's off by 1 on each round. The rounds are actually 255 wide, not 256 because they overlap with the next. But it's not a big functional difference.
def initRGBToNumberDicts():
	#have to cover every iteration or both single and doulbe 0s but with one always saturated
	#b -> 255 (0  ,255,255) 0-255
	#g -> 0   (0  ,0  ,255) 256-511
	#r -> 255 (255,0  ,255) 512-767
	#b -> 0	  (255,0  ,0  ) 768-1023
	#g -> 255 (255,255,0  ) 1024-1279
	#r -> 0   (0  ,255,0  ) 1280-1535
	for i in range(0,maxRGBNumber):
		if(i < 256):
			rgbToNumberDict[     (0    ,255  ,i)] = i
			numberToRGBDict[i] = (0    ,255  ,i)
		elif(i < 512):
			rgbToNumberDict[     (0    ,255-(i-256),255)] = i
			numberToRGBDict[i] = (0    ,255-(i-256),255)
		elif(i < 768):
			rgbToNumberDict[     (i-512    ,0    ,255)] = i
			numberToRGBDict[i] = (i-512    ,0    ,255)
		elif(i < 1024):
			rgbToNumberDict[     (255  ,0    ,255-(i-768))] = i
			numberToRGBDict[i] = (255  ,0    ,255-(i-768))
		elif(i < 1280):
			rgbToNumberDict[     (255  ,i-1024    ,0)] = i
			numberToRGBDict[i] = (255  ,i-1024    ,0)
		else: #1536
			rgbToNumberDict[     (255-(i-1280),255  ,0)] = i
			numberToRGBDict[i] = (255-(i-1280),255  ,0)
initRGBToNumberDicts()
def rgbToNumber(rgb):
	return rgbToNumberDict[rgb]
def numberToRGB(number):
	return numberToRGBDict[number]	
	
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
					tot += ((int(conn[2]))*i)%50 #There are few outputs
		return numberToRGB(int(tot/2.0)%maxRGBNumber)
	except:
		print "GetBrainColor: Unexpected error: %i,%i,%i"%(tot,maxRGBNumber,int(tot/4.0)%maxRGBNumber), sys.exc_info()[0]
		return (0,0,0)
	
def getAgentData(line):
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
		return [x,y,e,a,ld,bs]
	except:
		print "Failed to understand agent"
		return [-1,-1,-1,-1,-1,-1]

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

