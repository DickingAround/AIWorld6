import pygame
import time
import worldMaps
import statList
import statGraphs
def clearDisplay(window):
	window.fill((0,0,0))

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

def saveScreen(window,name):
	pygame.image.save(window,"./images_and_video/%04d.png"%name)

if __name__ == '__main__':
        import sys
	displayX = 1280 #300 for text, 450 more for graphs (750), 900-1200 is time graphs
	displayY = 720 #WorldGraph is 400ish
        imgNumb = 0
        imgMax = 10000
	pygame.init()
	window = pygame.display.set_mode((displayX,displayY))
	#pygame.draw.line(window,(100,100,100),(0,0),(100,100),1)
	pygame.display.flip()
	keepGoing = 1
	while keepGoing == 1:
		time.sleep(0.3)
		version = checkDataVersion()
		if(version != 'x'):
			clearDisplay(window)
			autoCapture.checkForAndSaveSpecialSituations()
			statList.statList_draw(window,10,10)
			worldMaps.worldMaps_draw(window,375,0,version) #NOLONGERTRUE? Draw world will clear the display if needed
			statGraphs.statGraphs_draw(window,175,10,200,650)	
			pygame.display.flip()	
			if(imgNumb < imgMax):
				saveScreen(window,imgNumb)
				imgNumb += 1
		for event in pygame.event.get():
			if event.type == pygame.QUIT or event.type == pygame.KEYDOWN:
				keepGoing = 0
