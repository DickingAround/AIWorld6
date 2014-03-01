import pygame

class location:
	xLoc = 0
	yLoc = 0
	signals = []
	p = 0.0 #passability
	ge = 0.0 #grow energy
	def __init__(self,stringOfData):
		parts = stringOfData.split(',')
		if(len(parts) == 7):
			self.xLoc = int(parts[0])
			self.yLoc = int(parts[1])
			self.p = float(parts[2])
			self.ge = float(parts[3])
			self.signals = [float(parts[4]),float(parts[5]),float(parts[6])]
		else:
			print "Location data wasn't the right size:%i, %s"%(len(parts),stringOfData)

	def drawSignal(self,window,xOffset,yOffset):
		#print r,g,b
		try:
			if(self.p < -0.5): #Impassable terrain
				window.set_at((self.xLoc+xOffset,self.yLoc+yOffset),(255,255,255))
			else:
				window.set_at((self.xLoc+xOffset,self.yLoc+yOffset),(int(abs(self.signals[0])*255),int(abs(self.signals[1])*255),int(abs(self.signals[2])*255)))
		except:
			print "Land maps crashed trying to parse this one"
			print self.xLoc,self.yLoc,self.signals
	
		
