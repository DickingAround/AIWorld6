import pygame
from helpers import *

def getLocData(loc):
	try:
		parts = loc.split(',')
		if(len(parts) == 7):
			return parts
		else:
			print "Didn't parse this location"
			print loc
			return 0,0,0,0,0,0,0		
	except:
		print "Got error on this location"
		print loc
		return 0,0,0,0,0,0,0

def drawLocSignal(window,x,y,r,g,b):
	#print r,g,b
	try:
		window.set_at((x,y),(int((r+1)*127),int((g+1)*127),int((b+1)*127)))
	except:
		print "Land maps crashed"
		print x,y,r,g,b

def drawLandMaps(window,xOffset,yOffset,version):
	f = getLocationFile(version)
	if(f == 0):
		return 0
	for loc in f.readlines():
		x,y,p,f,s1,s2,s3 = getLocData(loc.strip())
		if(x != 0 and y != 0 and p != 0 and f != 0):
			drawLocSignal(window,int(x)+xOffset,int(y)+yOffset,float(s1),float(s2),float(s3))
