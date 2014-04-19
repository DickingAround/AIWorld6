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
