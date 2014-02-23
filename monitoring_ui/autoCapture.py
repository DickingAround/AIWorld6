import helpers
import os
class autoCapture:
	oldListOfStats = []
def checkForAndSaveSpecialSituations():
	newListOfStats = helpers.getStats()
	if newListOfStats == 0:
		return
	if autoCapture.oldListOfStats == 0:
		return 
	
	#Auto-capture if there's been a die-off
	oldNumberOfAgents = helpers.findStat("agents",autoCapture.oldListOfStats)
	newNumberOfAgents = helpers.findStat("agents",newListOfStats)
	if(oldNumberOfAgents != None and newNumberOfAgents != None):
		if( float(oldNumberOfAgents) > 15000 and float(newNumberOfAgents) < 5000 ):
			os.system('cp -f ./data_from_simulations/* ./data_from_simulations/auto_capture_before_last_dieoff/')

	#Save this stat list in the old stats
	autoCapture.oldListOfStats = newListOfStats
