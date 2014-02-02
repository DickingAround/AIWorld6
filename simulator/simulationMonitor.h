#ifndef simulationmonitor_h 
#define simulationmonitor_h 

typedef struct simulationMonitor
{
 int moves, turns, attacks, grows, replications;
 double speed, speedDecision, speedAction, speedSeed, speedIntelTests;
 int failedMoves, failedReplications, failedAttacks, failedGrows;
 int killedBySeeding, killedByAttacks, killedByStarving;
 int addedCon, didntAddCon, removedCon, didntRemoveCon;
 long newBrainSize;
 int newBrains;
} simulationMonitor; 

int simulationMontior_test();
void simulationMonitor_emitMonitors();
void simulationMonitor_runIntelligenceTests(); //These are all simulations, the static analysis should be done continously.
#endif
