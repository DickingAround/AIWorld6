import pygame
import helpers
class agent:
	xLoc = 0
	yLoc = 0
	energy = 0
	species = 0
	speciesHash = 0
	latestDecision = 0
	facingDirection = 0
	
	def __init__(self,stringOfData):
		#Agent save...	
		#fprintf(file,"AG xLoc,%i yLoc,%i energy,%f facingDirection,%i latestDecision,%i age,%lu generation,%lu br,",a->xLoc,a->yLoc,a->energy,a->facingDirection,a->br.latestDecision,a->age,a->generation);
		#Brain save...	
		# fprintf(file,"M%f;H%i;",b->mutationRate,b->speciesHash);
		# fprintf(file,"L1");
		# while(b->inL1[i] != AG_CONN_END) {
		#  fprintf(file,";%i:%f:%i",b->inL1[i],b->multL1[i],b->outL1[i]);
		#  i++;
		# }
		# fprintf(file,";L2");
		# i = 0;
		# while(b->inL2[i] != AG_CONN_END) {
		#  fprintf(file,";%i:%f:%i",b->inL2[i],b->multL2[i],b->outL2[i]);
		#  i++;
		# }

		for agentAttribute in stringOfData.split(' '):
			if agentAttribute[0:4] == 'xLoc':
				pair = agentAttribute.split(',')
				self.xLoc = int(pair[1])
			elif agentAttribute[0:4] == 'yLoc':
				pair = agentAttribute.split(',')
				self.yLoc = int(pair[1])
			elif agentAttribute[0:6] == 'energy':
				pair = agentAttribute.split(',')
				self.energy = int(float(pair[1]))
			elif agentAttribute[0:15] == 'facingDirection':
				pair = agentAttribute.split(',')
				self.facingDirection = int(pair[1])
			elif agentAttribute[0:14] == 'latestDecision':
				pair = agentAttribute.split(',')
				self.latestDecision = int(pair[1])
			elif agentAttribute[0:2] == 'br':
				pair = agentAttribute.split(',')
				for brainPart in agentAttribute[2:].split(';'):
					if(brainPart[0] == 'H'):
						self.speciesHash = int(brainPart[1:])
#define AG_M_F 0 
#define AG_M_L 1
#define AG_M_R 2
#define AG_T_L 3
#define AG_T_R 4
#define AG_A_F 5
#define AG_R   6
#define AG_R_F 7
#define AG_GROW 8
# Attack - Orange
# Grow - Dark green
# Replicate - purple
# Move/turn - light blue
	def getDecisionColor(self):
		d = self.latestDecision	
		color = (0,0,0)
		if(d >= 0 and d <= 2):
			color = (0,125,255)
		if(d >= 3 and d <= 4):
			color = (0,125,255)
		if(d == 5):
			color = (255,125,0)
		if(d >= 6 and d <= 7):
			color = (255,0,255)
		if(d == 8):
			color = (0,180,0)
		return color;
	def getSpeciesColor(self):
		return helpers.numberToRGB(self.speciesHash)	
	def getPointsToColor(self): #First list is decision, second is species
		if(self.facingDirection == 0): #UP
			return([[[-2,1],[-2,0],[-1,0],[-1,-1],[0,-1],[0,-2],[1,0],[1,-1],[2,1],[2,0]],[[-2,2],[-1,2],[-1,1],[0,2],[0,1],[0,0],[1,2],[1,1],[2,2]]])
		elif(self.facingDirection == 1): #RIGHT
			return([[[-1,-2],[0,-2],[0,-1],[1,-1],[1,0],[2,0],[0,1],[1,1],[-1,2],[0,2]],[[-2,-2],[-2,-1],[-1,-1],[-2,0],[-1,0],[0,0],[-2,1],[-1,1],[-2,2]]])
		elif(self.facingDirection == 2): #DOWN
			return([[[-2,-1],[-2,0],[-1,0],[-1,1],[0,1],[0,2],[1,0],[1,1],[2,-1],[2,0]],[[-2,-2],[-1,-2],[-1,-1],[0,-2],[0,-1],[0,0],[1,-2],[1,-1],[2,-2]]])
		elif(self.facingDirection == 3): #LEFT
			return([[[0,-2],[1,-2],[-1,-1],[0,-1],[-2,0],[-1,0],[-1,1],[0,1],[0,2],[1,2]],[[2,-2],[1,-1],[2,-1],[0,0],[1,0],[2,0],[1,1],[2,1],[2,2]]])

	def draw(self,window,x,y):
		decisionColor = self.getDecisionColor()
		speciesColor = self.getSpeciesColor()
		points = self.getPointsToColor() #Gives both species set and the decision set
	       	for point in points[0]: 
			print "Drawing decision at %i,%i"%(x+self.xLoc+point[0],y+self.yLoc+point[1])
			window.set_at((x+self.xLoc+point[0],y+self.yLoc+point[1]),decisionColor)
	       	for point in points[1]: 
			print "Drawing species at %i,%i"%(x+self.xLoc+point[0],y+self.yLoc+point[1])
			window.set_at((x+self.xLoc+point[0],y+self.yLoc+point[1]),speciesColor)

def test():
	agentCode = "AG xLoc,15 yLoc,262 energy,8536.459961 facingDirection,2 latestDecision,5 age,609221 generation,14861 br,M0.1;H234;L1;0:0.000000:4;0:0.000000:63;0:0.000000:5;0:-0.000000:73;112:0.000000:23;63:0.000000:80;48:-0.000000:36;82:0.000000:75;8:-0.000000:48;63:-0.000000:84;92:0.000000:3;15:0.000000:32;40:-0.000000:35;32:-0.000001:35;21:0.000000:75;29:-0.000000:6;67:-0.000000:13;66:-0.000000:17;64:-0.000000:39;58:-0.000000:33;63:0.000000:42;31:0.000000:74;30:0.000000:57;13:-0.000000:24;80:-0.000000:24;71:-0.000000:83;80:0.000000:28;63:0.000000:43;8:0.000091:44;22:-0.000000:2;70:0.000000:97;14:-0.000000:48;27:-0.000000:77;118:-0.000000:29;58:0.000000:5;27:0.004583:43;94:0.000000:17;37:0.000000:29;76:-0.000000:40;68:-0.000000:17;101:-0.000000:71;53:-0.000000:96;124:-0.000000:95;123:0.000000:88;71:0.000000:9;12:-0.000000:89;87:-0.000000:18;\n"  
      	a = agent(agentCode) 
	displayX = 1280 #300 for text, 450 more for graphs (750), 900-1200 is time graphs
	displayY = 720 #WorldGraph is 400ish
	pygame.init()
	window = pygame.display.set_mode((displayX,displayY))
	pygame.draw.line(window,(100,100,100),(0,0),(100,100),1)
	a.draw(window,0,0)
	pygame.display.flip()
	for event in pygame.event.get():
		if event.type == pygame.QUIT or event.type == pygame.KEYDOWN:
			keepGoing = 0
