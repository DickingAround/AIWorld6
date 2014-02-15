#ifndef simulationmonitor_h 
#define simulationmonitor_h 

typedef struct simulationMonitor
{
 int addedCon, didntAddCon, removedCon, didntRemoveCon;
 double speed, speedDecision, speedAction, speedSeed, speedIntelTests;
 //Decisions 
 unsigned int decisions[AG_MAX_HASH];
 unsigned int moves[AG_MAX_HASH];
 unsigned int turns[AG_MAX_HASH];
 unsigned int attacks[AG_MAX_HASH];
 unsigned int grows[AG_MAX_HAS];
 unsigned int asexReplications[AG_MAX_HASH], sexReplications[AG_MAX_HASH];
 //Decision outcomes
 unsigned int failedMoves[AG_MAX_HAS], failedReplications[AG_MAX_HAS], failedAttacks[AG_MAX_HAS], failedGrows[AG_MAX_HAS];
 unsigned int killedBySeeding[AG_MAX_HAS], killedByAttacks[AG_MAX_HAS], killedByStarving[AG_MAX_HAS];
 //New agents
 unsigned int newBrainSize[AG_MAX_HAS], newBrains[AG_MAX_HAS];
} simulationMonitor; 

int simulationMontior_test();
void simulationMonitor_emitMonitors();
void simulationMonitor_runIntelligenceTests(); //These are all simulations, the static analysis should be done continously.
#endif
