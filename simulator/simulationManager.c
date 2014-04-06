#ifndef SIMULATIONMANAGER_C
#define SIMULATIONMANAGER_C 
#include <time.h>
#include <unistd.h>
#include "simulationManager.h"
#include "simulationManager_thread.c"
#include "threadManager.c"
#include "simulationMonitor.c"
#include <pthread.h>
extern simulationManager sm;
void simulationManager_run(int newWorld)
{
 //Load world, run all agent decisions, then act on all agent decisions, then do statistics gathering on this turn, then when it's all done do assessments
 world_createFromScratch(&(sm.w));
 sm.seedWorld = 0;
 if(newWorld == 1)
  sm.seedWorld = 1;
 else if(newWorld == 0)
  simulationManager_load();
 else if(newWorld == 2)
  simulationManager_loadTwoWorlds();
 simulationManager_setupThreads();
 simulationManager_runIterations_advanced(SIM_ITERATIONS,SIM_SEED_INTERVAL,SIM_SEED_DURATION,SIM_INTEL_TEST_INTERVAL);
 simulationManager_cleanupThreads();
}

void simulationManager_setupThreads() {
 int i;
 for(i = 0; i < NUMBER_OF_THREADS; i++) {
   initThread(&(sm.threadControls[i]), &(sm.threads[i]), simulationManager_thread_run,i);
 }
}
void simulationManager_cleanupThreads() {
 int i;
 printf("cleaning threads");
 for(i = 0; i < NUMBER_OF_THREADS; i++) {
  printf("didn't do anything to clean up threads");
  //pthread_join( sm.threads[i], NULL);
 }
}

void simulationManager_runIterations_basic(int iterations) {
 for(sm.i = 0; sm.i < iterations; sm.i++) {
  simulationManager_runAgentDecisions(); //Multi-threaded
  simulationManager_runAgentActions(); //Single-threaded
 }
}
void simulationManager_runIterations_advanced(int iterations, int seedInterval, int seedDuration, int intelTestInterval) {
 clock_t timerA, timerB, timerDecision, timerAction, timerSeed, timerIntel;
 sm.i = 0;
 timerA = clock();
 for(; sm.i < iterations; sm.i++) {
  if(sm.i % SIM_REPORT_INTERVAL == 0) {
   printf("Sim has performed %lu intervals\n",sm.i);
   timerB = clock(); 
   sm.smon.speed = timerB - timerA;
   timerA = clock();
   sm.smon.speed = SIM_REPORT_INTERVAL/(sm.smon.speed/(float)CLOCKS_PER_SEC);
   sm.smon.speedDecision = (sm.smon.speedDecision/(float)CLOCKS_PER_SEC);
   sm.smon.speedAction = (sm.smon.speedAction/(float)CLOCKS_PER_SEC);
   sm.smon.speedSeed = (sm.smon.speedSeed/(float)CLOCKS_PER_SEC);
   sm.smon.speedIntelTests = (sm.smon.speedIntelTests/(float)CLOCKS_PER_SEC);
   simulationMonitor_emitMonitors();
   simulationMonitor_clear(); //Also computes the old median for use in replicating
   world_save(&(sm.w));
  } 
  if(sm.i % seedInterval == 0 && sm.i < seedDuration && sm.seedWorld == 1) {
   timerSeed = clock(); 
   simulationManager_seedAgents();
   sm.smon.speedSeed += clock() - timerSeed;
  }
  if(sm.i % intelTestInterval == 0) {
   timerIntel = clock();
   simulationManager_runIntelligenceTests();
   sm.smon.speedIntelTests += clock() - timerIntel;
  }
  timerDecision = clock();
  simulationManager_runAgentDecisions(); //Multi-threaded
  sm.smon.speedDecision += clock() - timerDecision;
  timerAction = clock();
  simulationManager_runAgentActions(); //Single-threaded
  sm.smon.speedAction += clock() - timerAction;
  #ifdef GO_SLOW
  sleep(GO_SLOW);
  #endif
 } 
}
void simulationManager_seedAgents() { 
 int i,j;
 for(i = WORLD_BORDER; i < SIM_SEED_SIZE; i += 2) {
  for(j = WORLD_BORDER; j < SIM_SEED_SIZE; j += 2) {
   agent_mallocAgent_fromScratch((int)(rand()/(float)RAND_MAX*SIM_SEED_SIZE)+WORLD_BORDER,(int)(rand()/(float)RAND_MAX*SIM_SEED_SIZE)+WORLD_BORDER,SIM_SEED_ENERGY);
  }
 } 
}
void simulationManager_signalThreadsToGo() {
 int i;
 for(i = 0; i < NUMBER_OF_THREADS; i++)  {
   notifyChildOfWorkToDo(&(sm.threadControls[i])); //This will *not* block
 }
 for(i = 0; i < NUMBER_OF_THREADS; i++) {
   blockOnChildFinishingWork(&(sm.threadControls[i]));  //This will block
 }
}
void simulationManager_runAgentDecisions() { //Multi-threaded
 int i;
 for(i = 0; i < NUMBER_OF_THREADS; i++) {
  sm.threadControls[i].runAgentDecision = 1; //The thread will turn it back to zero when done 
 }
 simulationManager_signalThreadsToGo(); //Thread is also expected to gather inputs
}
void simulationManager_runAgentActions() { //Single threaded
 int i;
 int j,k;
 for(i = 0; i < sm.w.numbAgents && sm.w.agents[i].status != AG_STATUS_END_OF_LIST; i++) {
  if(sm.w.agents[i].status == AG_STATUS_ALIVE) {
   if(sm.w.agents[i].energy >= AG_MAX_ENERGY) {
    sm.w.agents[i].energy = AG_MAX_ENERGY;
   } 
   if(sm.w.agents[i].energy <= 0) {
    agent_kill(&(sm.w.agents[i]));
    #ifndef LESS_METRICS 
    simulationMonitor_addKilledByStarvingForHash(sm.w.agents[i].br.speciesHash,1);
    #endif
   }
   else {
    #ifndef LESS_METRICS
    simulationMonitor_addDecisionsForHash(sm.w.agents[i].br.speciesHash,1);
    simulationMonitor_addAveGenerationForHash(sm.w.agents[i].br.speciesHash,sm.w.agents[i].generation);
    simulationMonitor_addAveAgeForHash(sm.w.agents[i].br.speciesHash,sm.w.agents[i].age);
    simulationMonitor_addAveEnergyForHash(sm.w.agents[i].br.speciesHash,sm.w.agents[i].energy);
    simulationMonitor_addAveBrainSizeForHash(sm.w.agents[i].br.speciesHash,sm.w.agents[i].br.brainSize);
    //TODO: Remove this if it never fires 
    if(sm.w.agents[i].generation < 0 || sm.w.agents[i].age < 0 || sm.w.agents[i].energy < 0 || sm.w.agents[i].br.brainSize < 0)
     printf("Problem with an agent giving metrics.. gen:%lu, age:%lu, ene:%f, brain:%i\n",sm.w.agents[i].generation, sm.w.agents[i].age, sm.w.agents[i].energy, sm.w.agents[i].br.brainSize );
    #endif
    agent_performDecidedAction(&(sm.w.agents[i]));
   }
  }
 }
}

void simulationManager_runIntelligenceTests() {
 simulationMonitor_runIntelligenceTests();
}

void simulationManager_load() {
 world_load(&sm.w); 
}

void simulationManager_loadTwoWorlds() {
 world_loadTwoWorlds(&sm.w);
}

#endif
