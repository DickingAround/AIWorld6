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
void simulationMonitor_addDecisionsForHash(int hash, int x) {
 sm.smon.perHashMetrics[hash][0] += x; }
int simulationMonitor_getDecisionsForHash(int hash){
 return sm.smon.perHashMetrics[hash][0];}
int simulationMonitor_getDecisionsForSpecies(int species){
 return sm.smon.perSpeciesMetrics[species][0];}

void simulationMonitor_addMovesForHash(int hash, int x){
 sm.smon.perHashMetrics[hash][1] += x;}
int simulationMonitor_getMovesForSpecies(int species){
 return sm.smon.perSpeciesMetrics[species][1];}

void simulationMonitor_addTurnsForHash(int hash, int x){
 sm.smon.perHashMetrics[hash][2] += x;}
int simulationMonitor_getTurnsForSpecies(int species){
 return sm.smon.perSpeciesMetrics[species][2];}

void simulationMonitor_addAttacksForHash(int hash, int x){
 sm.smon.perHashMetrics[hash][3] += x;}
int simulationMonitor_getAttacksForSpecies(int species){
 return sm.smon.perSpeciesMetrics[species][3];}

void simulationMonitor_addGrowsForHash(int hash, int x){
 sm.smon.perHashMetrics[hash][4] += x;}
int simulationMonitor_getGrowsForSpecies(int species){
 return sm.smon.perSpeciesMetrics[species][4];}

void simulationMonitor_addASexualReplicationsForHash(int hash, int x){
 sm.smon.perHashMetrics[hash][5] += x;}
int simulationMonitor_getASexualReplicationsForSpecies(int species){
 return sm.smon.perSpeciesMetrics[species][5];}

void simulationMonitor_addSexualReplicationsForHash(int hash, int x){
 sm.smon.perHashMetrics[hash][6] += x;}
int simulationMonitor_getSexualReplicationsForSpecies(int species){
 return sm.smon.perSpeciesMetrics[species][6];}

void simulationMonitor_addFailedMovesForHash(int hash, int x){
 sm.smon.perHashMetrics[hash][7] += x;}
int simulationMonitor_getFailedMovesForSpecies(int species){
 return sm.smon.perSpeciesMetrics[species][7];}

void simulationMonitor_addFailedReplicationsForHash(int hash, int x){
 sm.smon.perHashMetrics[hash][8] += x;}
int simulationMonitor_getFailedReplicationsForSpecies(int species){
 return sm.smon.perSpeciesMetrics[species][8];}

void simulationMonitor_addFailedAttacksForHash(int hash, int x){
 sm.smon.perHashMetrics[hash][9] += x;}
int simulationMonitor_getFailedAttacksForSpecies(int species){
 return sm.smon.perSpeciesMetrics[species][9];}

void simulationMonitor_addFailedGrowsForHash(int hash, int x){
 sm.smon.perHashMetrics[hash][10] += x;}
int simulationMonitor_getFailedGrowsForSpecies(int species){
 return sm.smon.perSpeciesMetrics[species][10];}

void simulationMonitor_addKilledBySeedingForHash(int hash, int x){
 sm.smon.perHashMetrics[hash][11] += x;}
int simulationMonitor_getKilledBySeedingForSpecies(int species){
 return sm.smon.perSpeciesMetrics[species][11];}

void simulationMonitor_addKilledByAttacksForHash(int hash, int x){
 sm.smon.perHashMetrics[hash][12] += x;}
int simulationMonitor_getKilledByAttacksForSpecies(int species){
 return sm.smon.perSpeciesMetrics[species][12];}

void simulationMonitor_addKilledByStarvingForHash(int hash, int x){
 sm.smon.perHashMetrics[hash][13] += x;}
int simulationMonitor_getKilledByStarvingForSpecies(int species){
 return sm.smon.perSpeciesMetrics[species][13];}

void simulationMonitor_addAveBrainSizeForHash(int hash, int x){
 sm.smon.perHashMetrics[hash][14] += x;}
int simulationMonitor_getAveBrainSizeForSpecies(int species){
 return sm.smon.perSpeciesMetrics[species][14];}

/*void simulationMonitor_addNewBrainForHash(int hash, int x){
 sm.smon.perHashMetrics[hash][15] += x;}
int simulationMonitor_getNewBrainForSpecies(int species){
 return sm.smon.perSpeciesMetrics[species][15];}
*/ //Legacy
void simulationMonitor_addAveEnergyForHash(int hash, int x){
 sm.smon.perHashMetrics[hash][16] += x;}
int simulationMonitor_getAveEnergyForSpecies(int species){
 return sm.smon.perSpeciesMetrics[species][16];}

void simulationMonitor_addAveAgeForHash(int hash, int x){
 sm.smon.perHashMetrics[hash][17] += x;}
int simulationMonitor_getAveAgeForSpecies(int species){
 return sm.smon.perSpeciesMetrics[species][17];}

void simulationMonitor_addAveGenerationForHash(int hash, int x){
 sm.smon.perHashMetrics[hash][18] += x;}
int simulationMonitor_getAveGenerationForSpecies(int species){
 return sm.smon.perSpeciesMetrics[species][18];}

void simulationMonitor_addNumberOfAgentsForHash(int hash, int x){
 sm.smon.perHashMetrics[hash][19] += x;}
int simulationMonitor_getNumberOfAgentsForSpecies(int species){
 return sm.smon.perSpeciesMetrics[species][19];}

//-----------------------
// The per agent metrics
//-----------------------
void simulationMonitor_findPerAgentMetrics() {
 int i;
 for(i = 0; i < sm.w.numbAgents; i++) {
  if(sm.w.agents[i].status == AG_STATUS_ALIVE) {
   simulationMonitor_addNumberOfAgentsForHash(sm.w.agents[i].br.speciesHash,1);
   simulationMonitor_addAveGenerationForHash( sm.w.agents[i].br.speciesHash,sm.w.agents[i].generation);
   simulationMonitor_addAveAgeForHash( sm.w.agents[i].br.speciesHash,sm.w.agents[i].age);
   simulationMonitor_addAveEnergyForHash( sm.w.agents[i].br.speciesHash,sm.w.agents[i].energy);
   simulationMonitor_addAveBrainSizeForHash(sm.w.agents[i].br.speciesHash,sm.w.agents[i].br.brainSize);
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
int simulationMonitor_findSpecies(int speciesList[][3], int numberOfPossibleHashes) { //SpeciesList[SPECIES_TYPES_MAX][3]
 int i,c;
 int startingSpot;
 int distanceFromSpecies = 0;
 int waitingToSeeNewSpecies = 0;
 int speciesNumber = 0;
 //Find the start of the first species 
 for(i=0; i < numberOfPossibleHashes; i++) {
  c = simulationMonitor_getDecisionsForHash(i); 
  if(c <= SPECIES_DETECTION_POPULATION_MIN) {
   distanceFromSpecies++;
  }
  else {
   distanceFromSpecies = 0; 
  }
  if(distanceFromSpecies >= SPECIES_DETECTION_POPULATION_DIF_MIN) {
   startingSpot = i - 1;
   waitingToSeeNewSpecies = 1;
   break;
  }
 }
 if(waitingToSeeNewSpecies == 0) { //Didn't find a species??
  printf("Simulation Monitor: Didn't ever find the beginning of a new species...\n"); 
  return 0; 
 }
 //Detect species as we go
 for(i=startingSpot; i < numberOfPossibleHashes + SPECIES_DETECTION_POPULATION_DIF_MIN; i++) {
  c = simulationMonitor_getDecisionsForHash(i%numberOfPossibleHashes);
  if(c <= SPECIES_DETECTION_POPULATION_MIN) {
   if(distanceFromSpecies >= SPECIES_DETECTION_POPULATION_DIF_MIN && (!waitingToSeeNewSpecies)) {
    waitingToSeeNewSpecies = 1; //Start looking for a new species
    speciesList[speciesNumber][1] = (i - SPECIES_DETECTION_POPULATION_DIF_MIN)%numberOfPossibleHashes;
   }
   distanceFromSpecies++; 
  }
  else {
   distanceFromSpecies = 0;
   if(waitingToSeeNewSpecies == 1) { //New species found (unless it wraps around)
    waitingToSeeNewSpecies = 0;
    speciesList[speciesNumber][0] = i%numberOfPossibleHashes;
    speciesNumber++;
   }
   speciesList[speciesNumber][2] += c;
  }
 }
 return speciesNumber;
}

void simulationMonitor_clear() {
 int i,j;
 sm.smon.addedCon = 0;
 sm.smon.didntAddCon = 0;
 sm.smon.removedCon = 0;
 sm.smon.didntRemoveCon = 0;
 for(i=0;i<AG_MAX_HASH;i++) {
  for(j=0;j<SPECIES_NUMBER_OF_METRICS;j++) {
   sm.smon.perHashMetrics[i][j] = 0;
  }
 }
 for(i=0;i<SPECIES_TYPES_MAX;i++) {
  for(j=0;j<SPECIES_NUMBER_OF_METRICS;j++) {
   sm.smon.perSpeciesMetrics[i][j] = 0; 
  }
 }
}
void simulationMonitor_collectSpeciesMetrics(int speciesList[][3], int numberOfSpecies) {
 int speciesNumber,metricNumber,hashNumber;
 for(speciesNumber=0; speciesNumber < numberOfSpecies; speciesNumber++) {
  //Clear the species metrics from last time - Don't need to anymore since the clear function works again
  /*for(metricNumber =0; metricNumber < SPECIES_NUMBER_OF_METRICS; metricNumber++) {
   sm.smon.perSpeciesMetrics[speciesNumber][metricNumber] = 0; 
  } */
  //Compute the species metrics by adding up the metrics from all the hashes in that range
  for(hashNumber = speciesList[speciesNumber][0]; hashNumber < speciesList[speciesNumber][1]; hashNumber++) {
   for(metricNumber=0; metricNumber < SPECIES_NUMBER_OF_METRICS; metricNumber++) {
    sm.smon.perSpeciesMetrics[speciesNumber][metricNumber] += sm.smon.perHashMetrics[hashNumber][metricNumber];
   }
  }
 }
}

void simulationMonitor_emitMonitors() {
 int speciesList[SPECIES_TYPES_MAX][3];
 int numberOfSpecies;
 //Gather the per-agent metrics
 simulationMonitor_findPerAgentMetrics();
 //Find the species
 numberOfSpecies = simulationMonitor_findSpecies(speciesList,AG_MAX_HASH);
 if(numberOfSpecies <= 0)
  return; //If no species found, just abort
 //Collect the metrics based on the species
 simulationMonitor_collectSpeciesMetrics(speciesList, numberOfSpecies);
 //Emit the metrics files
 simulationMonitor_writeMetricsFiles(numberOfSpecies);
}

void simulationMonitor_writeMetricsFiles(int numberOfSpecies) {
 int speciesNumber;
 FILE *outFile;
 //Output the file
 outFile = fopen(MONITOR_FILE_LOC,"a");
 fprintf(outFile,"time,");
 simulationMonitor_writeTimeStamp(outFile); //TimeStamp doesn't write spaces so you'll need one of those
 fprintf(outFile," treatment,%s",sm.treatment);
 fprintf(outFile," iterations,%lu speed,%f speedD,%f speedA,%f speedS,%f",sm.i,sm.smon.speed,sm.smon.speedDecision,sm.smon.speedAction,sm.smon.speedSeed);
 if((sm.smon.addedCon + sm.smon.didntAddCon) != 0)
  fprintf(outFile," addConRate,%f",(float)sm.smon.addedCon/(float)(sm.smon.addedCon + sm.smon.didntAddCon));
 if((sm.smon.removedCon + sm.smon.didntRemoveCon) != 0)
  fprintf(outFile," removedConRate,%f",(float)sm.smon.removedCon/(float)(sm.smon.removedCon + sm.smon.didntRemoveCon)); 
 for(speciesNumber = 0; speciesNumber < numberOfSpecies; speciesNumber++) {
  //Per agent
  fprintf(outFile," speciesNumber,%i",speciesNumber);
  fprintf(outFile," numberOfAgents,%i",simulationMonitor_getNumberOfAgentsForSpecies(speciesNumber));
  if(simulationMonitor_getNumberOfAgentsForSpecies(speciesNumber) > 0) { 
   fprintf(outFile," aveAge,%f",((float)simulationMonitor_getAveAgeForSpecies(speciesNumber))/((float)simulationMonitor_getNumberOfAgentsForSpecies(speciesNumber)));
   fprintf(outFile," aveGeneration,%f",((float)simulationMonitor_getAveGenerationForSpecies(speciesNumber))/((float)simulationMonitor_getNumberOfAgentsForSpecies(speciesNumber)));
   fprintf(outFile," aveEnergy,%f",((float)simulationMonitor_getAveEnergyForSpecies(speciesNumber))/((float)simulationMonitor_getNumberOfAgentsForSpecies(speciesNumber)));
  }
  //Per decision
  if(simulationMonitor_getDecisionsForSpecies(speciesNumber) > 0) {
   fprintf(outFile," moves,%f",((float)simulationMonitor_getMovesForSpecies(speciesNumber))/((float)simulationMonitor_getDecisionsForSpecies(speciesNumber)));
   fprintf(outFile," turns,%f",((float)simulationMonitor_getTurnsForSpecies(speciesNumber))/((float)simulationMonitor_getDecisionsForSpecies(speciesNumber)));
   fprintf(outFile," attacks,%f",((float)simulationMonitor_getAttacksForSpecies(speciesNumber))/((float)simulationMonitor_getDecisionsForSpecies(speciesNumber)));
   fprintf(outFile," grows,%f",((float)simulationMonitor_getGrowsForSpecies(speciesNumber))/((float)simulationMonitor_getDecisionsForSpecies(speciesNumber)));
   fprintf(outFile," asexReplications,%f",((float)simulationMonitor_getASexualReplicationsForSpecies(speciesNumber))/((float)simulationMonitor_getDecisionsForSpecies(speciesNumber)));
   fprintf(outFile," sexReplications,%f",((float)simulationMonitor_getSexualReplicationsForSpecies(speciesNumber))/((float)simulationMonitor_getDecisionsForSpecies(speciesNumber)));
  }
  //Decision specific
  if(simulationMonitor_getMovesForSpecies(speciesNumber) > 0)
   fprintf(outFile," moveFailed,%f",((float)simulationMonitor_getFailedMovesForSpecies(speciesNumber))/((float)simulationMonitor_getMovesForSpecies(speciesNumber)));
  if((simulationMonitor_getASexualReplicationsForSpecies(speciesNumber) > 0) && (simulationMonitor_getASexualReplicationsForSpecies(speciesNumber) > 0 ))
   fprintf(outFile," replicationFailed,%f",((float)simulationMonitor_getFailedReplicationsForSpecies(speciesNumber))/((float)simulationMonitor_getASexualReplicationsForSpecies(speciesNumber) +simulationMonitor_getSexualReplicationsForSpecies(speciesNumber)));
  if(simulationMonitor_getAttacksForSpecies(speciesNumber) > 0)
   fprintf(outFile," attackFailed,%f",((float)simulationMonitor_getFailedAttacksForSpecies(speciesNumber))/((float)simulationMonitor_getAttacksForSpecies(speciesNumber)));
  if(simulationMonitor_getGrowsForSpecies(speciesNumber) > 0)
   fprintf(outFile," growFailed,%f",((float)simulationMonitor_getFailedGrowsForSpecies(speciesNumber))/((float)simulationMonitor_getGrowsForSpecies(speciesNumber)));
  fprintf(outFile," killedBySeeding,%i",simulationMonitor_getKilledBySeedingForSpecies(speciesNumber));
  fprintf(outFile," killedByAttacks,%i",simulationMonitor_getKilledByAttacksForSpecies(speciesNumber));
  fprintf(outFile," killedByStarving,%i",simulationMonitor_getKilledByStarvingForSpecies(speciesNumber));
 }
 fprintf(outFile,"\n"); 
 fclose(outFile);
}

void simulationMonitor_runIntelligenceTests() { //These are all simulations, the static analysis should be run continuously
 printf("Did nothing to run the intelligence tests\n");
}

//-----------
// TESTS
//-----------
int simulationMonitor_test() {
 int i; 
 int speciesList[2][3];
 for(i = 0;i<20;i++)
  sm.smon.perHashMetrics[i][0] = 0;
 sm.smon.perHashMetrics[6][0] = 100;
 sm.smon.perHashMetrics[7][0] = 100;
 sm.smon.perHashMetrics[8][0] = 100;
 sm.smon.perHashMetrics[19][0] = 100;
 simulationMonitor_findSpecies(speciesList,20); //SpeciesList[SPECIES_TYPES_MAX][3]
 printf("%i,%i,%i",speciesList[0][0],speciesList[0][1],speciesList[0][2]); 
 return 0;
}
#endif
