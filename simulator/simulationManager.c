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
 if(newWorld == 0)
  simulationManager_load();
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
 double ms, decisionMS, actionMS, seedMS, intelMS;
 sm.i = 0;
 timerA = clock();
 for(; sm.i < iterations; sm.i++) {
  if(sm.i % SIM_REPORT_INTERVAL == 0) {
   printf("Sim has performed %lu intervals\n",sm.i);
   timerB = clock(); 
   ms = timerB - timerA;
   timerA = clock();
   sm.smon.speed = SIM_REPORT_INTERVAL/(ms/(float)CLOCKS_PER_SEC);
   sm.smon.speedDecision = (decisionMS/(float)CLOCKS_PER_SEC);
   sm.smon.speedAction = (actionMS/(float)CLOCKS_PER_SEC);
   sm.smon.speedSeed = (seedMS/(float)CLOCKS_PER_SEC);
   sm.smon.speedIntelTests = (intelMS/(float)CLOCKS_PER_SEC);
   simulationMonitor_emitMonitors();
   simulationMonitor_clear();
   world_save(&(sm.w));
   seedMS = 0;
   actionMS = 0;
   decisionMS = 0;
   intelMS = 0; 
  } 
  if(sm.i % seedInterval == 0 && sm.i < seedDuration) {
   timerSeed = clock(); 
   simulationManager_seedAgents();
   seedMS += clock() - timerSeed;
  }
  if(sm.i % intelTestInterval == 0) {
   timerIntel = clock();
   simulationManager_runIntelligenceTests();
   intelMS += clock() - timerIntel;
  }
  timerDecision = clock();
  simulationManager_runAgentDecisions(); //Multi-threaded
  decisionMS += clock() - timerDecision;
  timerAction = clock();
  simulationManager_runAgentActions(); //Single-threaded
  actionMS += clock() - timerAction;
  #ifdef GO_SLOW
  sleep(20);
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

#endif
