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
void simulationMonitor_emitMonitors() {
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
