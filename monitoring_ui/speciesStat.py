class speciesStat:
	hashNumb = 0
	statList = 0
	hashMin = 0 #Not all species stats use these
	hashMax = 0
	#0:decisions, 1:Moves, 2:Turns, 3:Attacks, 4:Grows, 5:AsexReps, 6:SexReps, 7:FailedMoves, 8:FailedReps, 9:FailedAttacks, 10:FailedGrows, 11:SimReportSize, 12:KilledByAttack, 13:KilledByStarving, 14:AveBrainSize, 15:AveEnergy, 16:AveAge, 17:AveGen
	def __init__(self,stringOfData='',hashNumb=0):
		self.statList = []
		if stringOfData == '': #It's an empty species stat	
			for i in range(0,18):
				self.statList.append(0.0)
		else: #It's a lsit of data from a file
			self.hashNumb = hashNumb
			parts = stringOfData.split(',')
			for part in parts:
				self.statList.append(float(part))
	def getDecisions(self):
		return self.statList[0]
	def getMoves(self):
		return self.statList[1]	
	def getTurns(self):
		return self.statList[2]
	def getAttacks(self):
		return self.statList[3]	
	def getGrows(self):
		return self.statList[4]	
	def getAsexReps(self):
		return self.statList[5]	
	def getSexReps(self):
		return self.statList[6]	
	def getFailedMoves(self):
		return self.statList[7]	
	def getFailedReps(self):
		return self.statList[8]	
	def getFailedAttacks(self):
		return self.statList[9]	
	def getFailedGrows(self):
		return self.statList[10]	
	def getSimReportSize(self):
		return self.statList[11]	
	def getKilledByAttack(self):
		return self.statList[12]	
	def getKilledByStarving(self):
		return self.statList[13]	
	def getAveBrainSize(self):
		return self.statList[14]	
	def getAveEnergy(self):
		return self.statList[15]	
	def getAveAge(self):
		return self.statList[16]	
	def getAveGen(self):
		return self.statList[17]	
