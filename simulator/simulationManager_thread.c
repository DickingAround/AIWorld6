#ifndef simulationManager_thread_c
#define simulationManager_thread_c
#include "simulationManager.h"
extern simulationManager sm;
void simulationManager_thread_runDecisions(simulationManager_thread_control *tc, functiontype decisionFunction);
void simulationManager_thread_gatherInputs(simulationManager_thread_control *tc);
void *simulationManager_thread_run(void *ptr) {
 int i;
 double j; 
 j = 0;
 simulationManager_thread_control* tc = (simulationManager_thread_control*)ptr; 
 while(tc->done == 0)
 {
  if(tc->runAgentDecision == 1) {
   /*for(i = 0; i < 10000000; i++) {
    j = j / 2 + i;
   } */
   simulationManager_thread_runDecisions(tc,agent_gatherInputs);
   simulationManager_thread_runDecisions(tc,agent_makeDecision);
   tc->runAgentDecision = 0;
  }
  else if (tc->runAgentAction == 1) {
  ; //For now the main thread will handle all actions
  }
  notifyParentOfWorkCompleted(tc); //This will block 
 }
}

void simulationManager_thread_runDecisions(simulationManager_thread_control *tc, functiontype decisionFunction) {
 int i,j;
 world *w;
 w = &sm.w;
 for(i = tc->childNumber*w->worldSize/tc->numberOfChildren; i < (tc->childNumber+1)*(w->worldSize)/tc->numberOfChildren; i++) {
  for(j = 0; j < w->worldSize; j++) {
    if( w->locs[i][j].a != NULL) {
      decisionFunction(w->locs[i][j].a); 
    }
  }
 }
}
//-------------------------------
//    TESTS
//-------------------------------
void simulationManager_thread_test_decFunc(agent *ag) {
  static int agentsChecked = 0;
  agentsChecked++;
  ag->energy = (float)agentsChecked; 
}
int simulationManager_thread_test() {
  int i,j;
  agent ags[10][10];
  simulationManager_thread_control tc;
  for(i = 0; i < 10; i++)
    for(j = 0; j < 10; j++)
      sm.w.locs[i][j].a = &(ags[i][j]);
  sm.w.worldSize = 10;
  tc.numberOfChildren = 4;
  tc.childNumber = 0;
  simulationManager_thread_runDecisions(&tc,simulationManager_thread_test_decFunc);
  tc.childNumber = 1;
  simulationManager_thread_runDecisions(&tc,simulationManager_thread_test_decFunc);
  tc.childNumber = 2;
  simulationManager_thread_runDecisions(&tc,simulationManager_thread_test_decFunc);
  tc.childNumber = 3;
  simulationManager_thread_runDecisions(&tc,simulationManager_thread_test_decFunc);
  if(ags[9][9].energy == 100)
    return 1;
  else { 
    printf("Last agent %f\n",ags[9][9].energy); 
    return 0;
  } 
}
#endif
