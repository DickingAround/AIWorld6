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

//-----------------------------------------------
//  The set/get functions for the data structure
//-----------------------------------------------
void simulationMonitor_addDecisionsForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][0] += x;
int simulationMonitor_getDecisionsForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][0];

void simulationMonitor_addMovesForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][1] += x;
int simulationMonitor_getMovesForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][1];

void simulationMonitor_addTurnsForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][2] += x;
int simulationMonitor_getTurnsForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][2];

void simulationMonitor_addAttacksForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][3] += x;
int simulationMonitor_getAttacksForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][3];

void simulationMonitor_addGrowsForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][4] += x;
int simulationMonitor_getGrowsForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][4];

void simulationMonitor_addASexualReplicationsForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][5] += x;
int simulationMonitor_getASexualReplicationsForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][5];

void simulationMonitor_addSexualReplicationsForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][6] += x;
int simulationMonitor_getSexualReplicationsForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][6];

void simulationMonitor_addFailedMovesForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][7] += x;
int simulationMonitor_getFailedMovesForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][7];

void simulationMonitor_addFailedReplicationsForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][8] += x;
int simulationMonitor_getFailedReplicationsForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][8];

void simulationMonitor_addFailedAttacksForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][9] += x;
int simulationMonitor_getFailedAttacksForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][9];

void simulationMonitor_addFailedGrowsForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][10] += x;
int simulationMonitor_getFailedGrowsForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][10];

void simulationMonitor_addKilledBySeedingForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][11] += x;
int simulationMonitor_getKilledBySeedingForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][11];

void simulationMonitor_addKilledByAttacksForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][12] += x;
int simulationMonitor_getKilledByAttacksForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][12];

void simulationMonitor_addKilledByStarvingForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][13] += x;
int simulationMonitor_getKilledByStarvingForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][13];

void simulationMonitor_addAveBrainSizeForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][14] += x;
int simulationMonitor_getAveBrainSizeForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][14];

/*void simulationMonitor_addNewBrainForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][15] += x;
int simulationMonitor_getNewBrainForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][15];
*/ //Legacy
void simulationMonitor_addAveEnergyForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][16] += x;
int simulationMonitor_getAveEnergyForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][16];

void simulationMonitor_addAveAgeForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][17] += x;
int simulationMonitor_getAveAgeForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][17];

void simulationMonitor_addAveGenerationForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][18] += x;
int simulationMonitor_getAveGenerationForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][18];

void simulationMonitor_addNumberOfAgentsForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][19] += x;
int simulationMonitor_getNumberOfAgentsForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][19];

//-----------------------
// The per agent metrics
//-----------------------
void simulationMonitor_findPerAgentMetrics() {
 for(i = 0; i < sm.w.numbAgents; i++) {
  if(sm.w.agents[i].status == AG_STATUS_ALIVE) {
   simulationMonitor_addNumberOfAgentsForHash(sm.w.agents[i].br.hash,1);
   simulationMonitor_addAveGenerationForHash( sm.w.agents[i].br.hash,sm.w.agents[i].generation);
   simulationMonitor_addAveAgeForHash( sm.w.agents[i].br.hash,sm.w.agents[i].age);
   simulationMonitor_addAveEnergyForHash( sm.w.agents[i].br.hash,sm.w.agents[i].energy);
   simulationMonitor_addAveBrainSizeForHash(sm.w.agents[i].br.hash,sm.w.agents[i].);
  }
 }
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
  return 0; 
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
 return speciesNumber;
}

void simulationMonitor_collectSpeciesMetrics(int **speciesList, int numberOfSpecies) {
 int speciesNumber,metricNumber,hashNumber;
 for(speciesNumber=0; speciesNumber < numberOfSpecies; speciesNumber++) {
  //Clear the species metrics from last time
  for(metricNumber =0; metricNumber < SPECIES_NUMBER_OF_METRICS; metricNumber++) {
   sm.smon.perSpeciesMetrics[speciesNumber][metricNumber] = 0; 
  } 
  //Compute the species metrics by adding up the metrics from all the hashes in that range
  for(hashNumber = speciesList[0]; hashNumber < speciesList[1]; hashNumber++) {
   for(metricNumber=0; metricNumber < SPECIES_NUMBER_OF_METRICS; metricNumber++) {
    sm.smon.perSpeciesMetrics[speciesNumber][metricNumber] += sm.smon.perHashMetrics[hashNumber][metricNumber];
   }
  }
 }
}

void simulationMonitor_emitMonitors() {
 int speciesList[SPECIES_MAX_TYPES][3];
 //Gather the per-agent metrics
 simulationMonitor_findPerAgentMetrics();
 //Find the species
 numberOfSpecies = simulationMonitor_findSpecies(speciesList);
 if(numberOfSpecies <= 0)
  return; //If no species found, just abort
 //Collect the metrics based on the species
 simulationMonitor_collectSpeciesMetrics(speciesList, numberOfSpecies);
 //Emit the metrics files
 simulationMonitor_writeMetricsFiles();
}

void simulationMonitor_writeMetricsFiles(int numberOfSpecies) {
 int i, c, totalActions;
 float aveE, aveA, aveG;
 FILE *outFile;
 //Output the file
 outFile = fopen(MONITOR_FILE_LOC,"a");
 fprintf(outFile,"time,");
 simulationMonitor_writeTimeStamp(outFile); //TimeStamp doesn't write spaces so you'll need one of those
 fprintf(outFile," treatment,%s",sm.treatment);
 fprintf(outFile," agents,%i iterations,%lu aveGenerations,%f aveAge,%f aveEnergy,%f speed,%f speedD,%f speedA,%f speedS,%f",c,sm.i,aveG/(float)c,aveA/(float)c,aveE/(float)c,sm.smon.speed,sm.smon.speedDecision,sm.smon.speedAction,sm.smon.speedSeed);
 for(speciesNumber = 0; speciesNumber < numberOfSpecies; speciesNumber++) {
  //Per decision
  fprintf(outFile," moves,%f",float(simulationMonitor_getMovesForSpecies(speciesNumber))/float(simulationMonitor_getDecisionsForSpecies(speciesNumber)));
  fprintf(outFile," turns,%f",float(simulationMonitor_getTurnsForSpecies(speciesNumber))/float(simulationMonitor_getDecisionsForSpecies(speciesNumber)));
  fprintf(outFile," attacks,%f",float(simulationMonitor_getAttacksForSpecies(speciesNumber))/float(simulationMonitor_getDecisionsForSpecies(speciesNumber)));
  fprintf(outFile," grows,%f",float(simulationMonitor_getGrowsForSpecies(speciesNumber))/float(simulationMonitor_getDecisionsForSpecies(speciesNumber)));
  fprintf(outFile," asexReplications,%f",float(simulationMonitor_getASexualReplicationsForSpecies(speciesNumber))/float(simulationMonitor_getDecisionsForSpecies(speciesNumber)));
  fprintf(outFile," sexReplications,%f",float(simulationMonitor_getSexualReplicationsForSpecies(speciesNumber))/float(simulationMonitor_getDecisionsForSpecies(speciesNumber)));
  //Decision specific
  fprintf(outFile," moveFailed,%f",float(simulationMonitor_getFailedMovesForSpecies(speciesNumber))/float(simulationMonitor_getMovesForSpecies(speciesNumber)));
  fprintf(outFile," replicationFailed,%f",float(simulationMonitor_getFailedReplicationForSpecies(speciesNumber))/float(simulationMonitor_getASexualReplicationsForSpecies(speciesNumber) +simulationMonitor_getSexualReplicationsForSpecies(speciesNumber)));
  fprintf(outFile," attackFailed,%f",float(simulationMonitor_getFailedAttacksForSpecies(speciesNumber))/float(simulationMonitor_getAttacksForSpecies(speciesNumber)));
  fprintf(outFile," growFailed,%f",float(simulationMonitor_getFailedGrowForSpecies(speciesNumber))/float(simulationMonitor_getGrowsForSpecies(speciesNumber)));
  fprintf(outFile," killedBySeeding,%i",simulationMonitor_getKilledBySeedingForSpecies(speciesNumber));
  fprintf(outFile," killedByAttacks,%i",simulationMonitor_getKilledByAttacksForSpecies(speciesNumber));
  fprintf(outFile," killedByStarving,%i",simulationMonitor_getKilledByStarvingForSpecies(speciesNumber));
  fprintf(outFile," newBrainSize,%i",simulationMonitor_getKilledBySeedingForSpecies(speciesNumber));
  fprintf(outFile," killedBySeeding,%i",simulationMonitor_getKilledBySeedingForSpecies(speciesNumber));
  fprintf(outFile," killedBySeeding,%i",simulationMonitor_getKilledBySeedingForSpecies(speciesNumber));
  fprintf(outFile," killedBySeeding,%i",simulationMonitor_getKilledBySeedingForSpecies(speciesNumber));
 }

void simulationMonitor_addAveEnergyForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][16] += x;
int simulationMonitor_getAveEnergyForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][16];

void simulationMonitor_addAveAgeForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][17] += x;
int simulationMonitor_getAveAgeForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][17];

void simulationMonitor_addAveGenerationForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][18] += x;
int simulationMonitor_getAveGenerationForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][18];

void simulationMonitor_addNumberOfAgentsForHash(int hash, int x)
 sm.smon.perHashMetrics[hash][19] += x;
int simulationMonitor_getNumberOfAgentsForSpecies(int species, int x)
 return sm.smon.perSpeciesMetrics[hash][19];


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
