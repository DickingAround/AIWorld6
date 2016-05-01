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
 clock_t timerA, timerB, timerDecision, timerAction, timerSeed, timerIntel, timerSave;
 sm.i = 0;
 timerA = clock();
 timerSave = clock();
 for(; sm.i < iterations; sm.i++) {
  if(sm.i % SIM_REPORT_INTERVAL == 0) {
   printf("Sim has performed %lu intervals\n",sm.i);
   timerB = clock(); 
   sm.smon.speed = timerB - timerA;
   timerA = clock();
   simulationMonitor_emitMonitors();
   simulationMonitor_clear(); //Also computes the old median for use in replicating
  }
  if((clock() - timerSave)/CLOCKS_PER_SEC > SIM_SAVE_TIME_INTERVAL) {
   world_save(&(sm.w));
   timerSave = clock();
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
  sm.smon.speedActionTurn += clock() - timerAction;
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
 agent ag;
 int i;
 int j,k;
 clock_t timerA;
 world *w = &(sm.w);
 for(i = 0; i < sm.w.numbAgents && sm.w.agents[i].status != AG_STATUS_END_OF_LIST; i++) {
  #ifndef LESS_TIMERS 
  timerA = clock();
  #endif
  ag = sm.w.agents[i]; 
  if(ag.status == AG_STATUS_ALIVE) {
   if(ag.energy >= AG_MAX_ENERGY) {
    ag.energy = AG_MAX_ENERGY;
   }
   #ifdef AG_ENABLE_AGE_LIMIT
   if(ag.age > AG_MAX_AGE) {
    agent_kill(&(sm.w.agents[i]));
    #ifndef LESS_METRICS 
    simulationMonitor_addKilledByAgeForHash(ag.br.speciesHash,1);
    #endif
    #ifndef LESS_TIMERS
    sm.smon.speedActionKillMax += clock() - timerA;
    #endif 
   }
   #endif 
   if(ag.energy <= 0) {
    agent_kill(&(sm.w.agents[i]));
    #ifndef LESS_METRICS 
    simulationMonitor_addKilledByStarvingForHash(ag.br.speciesHash,1);
    #endif
    #ifndef LESS_TIMERS
    sm.smon.speedActionKillMax += clock() - timerA;
    #endif
   }
   else {
    simulationMonitor_addDecisionsForHash(ag.br.speciesHash,1);
    #ifndef LESS_METRICS
    simulationMonitor_addAveGenerationForHash(ag.br.speciesHash,ag.generation);
    simulationMonitor_addAveAgeForHash(ag.br.speciesHash,ag.age);
    simulationMonitor_addAveEnergyForHash(ag.br.speciesHash,ag.energy);
    simulationMonitor_addAveBrainSizeForHash(ag.br.speciesHash,ag.br.brainSize);
    #endif
    #ifndef LESS_TIMERS 
    sm.smon.speedActionKillMax += clock() - timerA;
    //TODO: Remove this if it never fires 
    //if(sm.w.agents[i].generation < 0 || sm.w.agents[i].age < 0 || sm.w.agents[i].energy < 0 || sm.w.agents[i].br.brainSize < 0)
    // printf("Problem with an agent giving metrics.. gen:%lu, age:%lu, ene:%f, brain:%i\n",sm.w.agents[i].generation, sm.w.agents[i].age, sm.w.agents[i].energy, sm.w.agents[i].br.brainSize );
    timerA = clock();
    #endif
    agent_performDecidedAction(&(sm.w.agents[i]));
    #ifndef LESS_TIMERS
    sm.smon.speedAction += clock() - timerA;
    #endif
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
