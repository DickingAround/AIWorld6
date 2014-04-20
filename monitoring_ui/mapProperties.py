class mapProperties:
	uiX = 0
	uiY = 0
	worldMinX = 0
	worldMinY = 0
	worldMaxX = 0
	worldMaxY = 0
	compression = 0 #0 means 1:1, -1 means smaller (generally 2x), +1 means bigger (generally 3x)	
	
	def __init__(self,ux,uy,wminx,wminy,wmaxx,wmaxy,comp):
		self.uiX = ux
		self.uiY = uy
		self.worldMinX = wminx
		self.worldMinY = wminy
		self.worldMaxX = wmaxx
		self.worldMaxY = wmaxy
		self.compression = comp

	def convertToLocationInUI(self,thisPoint,offsetPoint):
		#print thisPoint, self.worldMinX, self.worldMaxX, self.worldMinY, self.worldMaxY
		if(self.compression == 0):
			comp = 1
		else:
			comp = self.compression
		if(thisPoint[0] >= self.worldMinX and thisPoint[1] >= self.worldMinY and thisPoint[0] < self.worldMaxX and thisPoint[1] < self.worldMaxY):		
			return [self.uiX+(thisPoint[0]-self.worldMinX)*comp+offsetPoint[0],self.uiY+(thisPoint[1]-self.worldMinY)*comp+offsetPoint[1]]
		else:
			return None 
