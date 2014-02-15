#ifndef simulationmonitor_c
#define simulationmonitor_c
#include <stdio.h>
#include <time.h>
#include "simulationMonitor.h"
extern simulationManager sm;
void simulationMonitor_writeTimeStamp(FILE *f) {
 time_t rawtime;
 struct tm * timeinfo;
 char buffer [80];
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,80,"%F_%T",timeinfo);
  fprintf(f,"%s",buffer); 
}
 //-----------------------------------------------------------
 //  Detect the species so we can collect the data into them. 
 //-----------------------------------------------------------
 /*        speciesNumber = -1
        gapNeeded = 5 #Min number of speaces between colors to count as different species
        minNumb = 10 #Min number for this to be counted as occupied
        waitingToSeeNewSpecies = False
        speciesList = []
        distanceFromSpecies = 0
        startingSpot = 0
        #print speciesTree.agentCount[0]
        #Go till you find a detectable species, starting spot will be the exact number where a new one begins
        for i in range(0,speciesTree.width):
                count = speciesTree.agentCount[(speciesTree.iterationsSeen-roundsAgo)%speciesTree.duration][i]
                if(count <= minNumb):
                        distanceFromSpecies += 1
                else:
                        distanceFromSpecies = 0
                if(distanceFromSpecies >= gapNeeded):
                        startingSpot = i - 1
                        waitingToSeeNewSpecies = True
                        break
        #print "starting at %i, going to %i"%(startingSpot,speciesTree.width+startingSpot+1)
        if(not waitingToSeeNewSpecies): #Didn't find a gap
                return []
        #Then start parsing to find the species
        for i in range(startingSpot,speciesTree.width+startingSpot+gapNeeded):
                count = speciesTree.agentCount[(speciesTree.iterationsSeen-roundsAgo)%speciesTree.duration][i%speciesTree.width]
                if(count <= minNumb): #No agents here
                        if(distanceFromSpecies >= gapNeeded and (not waitingToSeeNewSpecies)): #end of a species
                                waitingToSeeNewSpecies = True #Start looking for a new speices
                                speciesList[speciesNumber][1] = (i - gapNeeded)%speciesTree.width #The max is one past the real number
                        distanceFromSpecies += 1
                else: #Agents here
                        distanceFromSpecies = 0
                        if(waitingToSeeNewSpecies): #New species found! (unless it wraps around)
                                waitingToSeeNewSpecies = False
                                speciesList.append([i%speciesTree.width,-1,0]) #minColor, maxColor, counter(for sorting)
                                speciesNumber += 1
                        speciesList[speciesNumber][2] += count
        #print speciesList
        return speciesList */
 //Find the detectable species based on the decisions
bool simulationMonitor_findSpecies(int **speciesList) { //SpeciesList[SPECIES_MAX_TYPES][3]
 int i;
 int startingSpot;
 bool waitingToSeeNewSpecies = false;
 int speciesNumber = 0;
 //Find the start of the first species 
 for(i=0; i < AG_MAX_HASH; i++) {
  c = sm.smon.decisions[i];
  if(c <= SPECIES_DETECTION_POPULATION_MIN) {
   distanceFromSpecies++;
  }
  else {
   distanceFromSpecies = 0; 
  }
  if(distanceFromSpecies >= SPECIES_DETECTION_POPULATION_DIF_MIN) {
   startingSpot = i - 1;
   waitingToSeeNewSpecies = true;
   break;
  }
 }
 if(!waitingToSeeNewSpecies) { //Didn't find a species??
  printf("Simulation Monitor: Didn't ever find the beginning of a new species...\n"); 
  return false; 
 }
 //Detect species as we go
 for(i=startingSpot; i < AG_MAX_HASH + SPECIES_DETECTION_POPULATION_DEF_MIN) {
  c = sm.smon.decision[i%AG_MAX_HASH];
  if(c <= SPECIES_DETECTION_POPULATION_MIN) {
   if(distanceFromSpecies >= SPECIES_DETECTION_POPULATION_DIF_MIN && (!waitingToSeeNewSpecies)) {
    waitingToSeeNewSpecies = true; //Start looking for a new species
    speciesList[speciesNumber][1] = (i - SPECIES_DETECTION_POPULATION_DIF_MIN)%AG_MAX_HASH;
   }
   distanceFromSpecies++; 
  }
  else {
   distanceFromSpecies = 0;
   if(waitingToSeeNewSpecies) { //New species found (unless it wraps around)
    waitingToSeeNewSpecies = false;
    speciesList[speciesNumber][0] = i%AG_MAX_HASH;
    speciesNumber++;
   }
   speciesList[speciesNumber][2] += c;
  }
 }
 return true;
}

void simulationMonitor_emitMonitors() {
 //Find the species
 int speciesList[SPECIES_MAX_TYPES][3];
 simulationMonitor_findSpecies(speciesList);
 //Collect the metrics based on the species
 int i, c, totalActions;
 float aveE, aveA, aveG;
 FILE *outFile;
 //Gather the stats to output 
 c = 0;
 aveE = 0;
 aveA = 0;
 for(i = 0; i < sm.w.numbAgents; i++) {
  if(sm.w.agents[i].status == AG_STATUS_ALIVE) {
   c++; 
   aveE += sm.w.agents[i].energy;
   aveA += sm.w.agents[i].age;
   aveG += sm.w.agents[i].generation;
  }
 }
 //Output the file
 outFile = fopen(MONITOR_FILE_LOC,"a");
 fprintf(outFile,"time,");
 simulationMonitor_writeTimeStamp(outFile); //TimeStamp doesn't write spaces so you'll need one of those
 fprintf(outFile," treatment,%s",sm.treatment);
 fprintf(outFile," agents,%i iterations,%lu aveGenerations,%f aveAge,%f aveEnergy,%f speed,%f speedD,%f speedA,%f speedS,%f",c,sm.i,aveG/(float)c,aveA/(float)c,aveE/(float)c,sm.smon.speed,sm.smon.speedDecision,sm.smon.speedAction,sm.smon.speedSeed);
 totalActions = (sm.smon.moves+sm.smon.turns+sm.smon.attacks+sm.smon.grows+sm.smon.replications);
 fprintf(outFile," moves,%f turns,%f attacks,%f grows,%f replications,%f",(float)sm.smon.moves/(float)totalActions,(float)sm.smon.turns/(float)totalActions,(float)sm.smon.attacks/(float)totalActions,(float)sm.smon.grows/(float)totalActions,(float)sm.smon.replications/(float)totalActions);
 fprintf(outFile," killedBySeed,%i killedByAttacks,%i killedByStarving,%i",sm.smon.killedBySeeding,sm.smon.killedByAttacks,sm.smon.killedByStarving);
 if(sm.smon.moves != 0)
  fprintf(outFile," moveFailureRate,%f",(float)sm.smon.failedMoves/(float)sm.smon.moves);
 if(sm.smon.replications != 0)
  fprintf(outFile," replicateFailRate,%f",(float)sm.smon.failedReplications/(float)sm.smon.replications);
 if(sm.smon.attacks != 0)
  fprintf(outFile," attackFailRate,%f",(float)sm.smon.failedAttacks/(float)sm.smon.attacks);
 if(sm.smon.grows != 0)
  fprintf(outFile," growFailRate,%f",(float)sm.smon.failedGrows/(float)sm.smon.grows);
 if(sm.smon.didntAddCon != 0)
  fprintf(outFile," addConRate,%f",(float)sm.smon.addedCon/(float)(sm.smon.addedCon + sm.smon.didntAddCon));
 if(sm.smon.didntAddCon != 0)
  fprintf(outFile," removedConRate,%f",(float)sm.smon.removedCon/(float)(sm.smon.removedCon + sm.smon.didntRemoveCon));
 if(c > 0)
  fprintf(outFile," newBrainSize,%f",((float)sm.smon.newBrainSize)/(float)(sm.smon.newBrains));
 fprintf(outFile,"\n"); 
 fclose(outFile);
}
void simulationMonitor_clear() {
 sm.smon.moves = 0;
 sm.smon.turns = 0;
 sm.smon.attacks = 0;
 sm.smon.grows = 0;
 sm.smon.replications = 0;
 sm.smon.killedBySeeding = 0; 
 sm.smon.killedByAttacks = 0;
 sm.smon.killedByStarving = 0;
 sm.smon.failedMoves = 0;
 sm.smon.failedReplications = 0;
 sm.smon.failedAttacks = 0;
 sm.smon.failedGrows = 0;
 sm.smon.addedCon = 0;
 sm.smon.didntAddCon = 0;
 sm.smon.removedCon = 0;
 sm.smon.didntRemoveCon = 0;
 sm.smon.newBrainSize = 0;
 sm.smon.newBrains = 0;
}

void simulationMonitor_runIntelligenceTests() { //These are all simulations, the static analysis should be run continuously
 printf("Did nothing to run the intelligence tests\n");
}

//-----------
// TESTS
//-----------
int simulationMonitor_test() {
 return 0;
}
#endif
