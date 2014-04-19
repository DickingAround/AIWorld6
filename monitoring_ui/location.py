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

	def drawSignal(self,window,mapProps):
		#print r,g,b
		try:
			r = int(self.signals[0]+0.5)*255
			if(r > 255):
				r = 255
			if(r < 0):
				r = 0
			g = int(self.signals[1]+0.5)*255
			if(g > 255):
				g = 255
			if(g < 0):
				g = 0
			b = int(self.signals[2]+0.5)*255
			if(b > 255):
				b = 255
			if(b < 0):
				b = 0
			window.set_at((self.xLoc+mapProps.uiX,self.yLoc+mapProps.uiY),(r,g,b))
			#window.set_at((self.xLoc+xOffset,self.yLoc+yOffset),(int(abs(self.signals[0])*255),int(abs(self.signals[1])*255),int(abs(self.signals[2])*255)))
		except:
			print "Land maps crashed trying to parse this one"
			print self.xLoc,self.yLoc,self.signals
	
		
