#ifndef simulationmonitor_h 
#define simulationmonitor_h 

//Three kinds of metrics
// per decision
// - per birth
// - per attack
// - per etc.
// per existing agent (these don't need to be gathered here, they get gathered in a snapshot)
typedef struct simulationMonitor
{
 int addedCon, didntAddCon, removedCon, didntRemoveCon;
 double speed, speedDecision, speedAction, speedSeed, speedIntelTests;

 int perHashMetrics[AG_MAX_HASH][SPECIES_NUMBER_OF_METRICS];
 int perSpeciesMetrics[SPECIES_TYPES_MAX][SPECIES_NUMBER_OF_METRICS];

 // -- ALL HASHES METRICS -- 
 //Decisions 
// unsigned int decisions[AG_MAX_HASH], moves[AG_MAX_HASH], turns[AG_MAX_HASH], attacks[AG_MAX_HASH], grows[AG_MAX_HASH], asexReplications[AG_MAX_HASH], sexReplications[AG_MAX_HASH];
 //Decision specifics
 //unsigned int failedMoves[AG_MAX_HASH], failedReplications[AG_MAX_HASH], failedAttacks[AG_MAX_HASH], failedGrows[AG_MAX_HASH], killedBySeeding[AG_MAX_HASH], killedByAttacks[AG_MAX_HASH], killedByStarving[AG_MAX_HASH], newBrainSize[AG_MAX_HASH], newBrains[AG_MAX_HASH];

 // -- METRICS BY SPECIES --
 //Decisions 
 //unsigned int species_decisions[SPECIES_TYPES_MAX], species_moves[SPECIES_TYPES_MAX], species_turns[SPECIES_TYPES_MAX], species_attacks[SPECIES_TYPES_MAX], species_grows[SPECIES_TYPES_MAX], species_asexReplications[SPECIES_TYPES_MAX], species_sexReplications[SPECIES_TYPES_MAX];
 //Decision specifics
 //unsigned int species_failedMoves[SPECIES_TYPES_MAX], species_failedReplications[SPECIES_TYPES_MAX], species_failedAttacks[SPECIES_TYPES_MAX], species_failedGrows[SPECIES_TYPES_MAX], species_killedBySeeding[SPECIES_TYPES_MAX], species_killedByAttacks[SPECIES_TYPES_MAX], species_killedByStarving[SPECIES_TYPES_MAX], species_newBrainSize[SPECIES_TYPES_MAX], species_newBrains[SPECIES_TYPES_MAX];

 // -- PER EXISTING AGENT METRICS BY SPECIES --
 unsigned int species_aveEnergy[SPECIES_TYPES_MAX], species_aveAge[SPECIES_TYPES_MAX], species_aveGeneration[SPECIES_TYPES_MAX];
} simulationMonitor; 

void simulationMonitor_addDecisionsForHash(int hash, int x);
int simulationMonitor_getDecisionsForHash(int hash);
int simulationMonitor_getDecisionsForSpecies(int species);

void simulationMonitor_addMovesForHash(int hash, int x);
int simulationMonitor_getMovesForSpecies(int species);

void simulationMonitor_addTurnsForHash(int hash, int x);
int simulationMonitor_getTurnsForSpecies(int species);

void simulationMonitor_addAttacksForHash(int hash, int x);
int simulationMonitor_getAttacksForSpecies(int species);

void simulationMonitor_addGrowsForHash(int hash, int x);
int simulationMonitor_getGrowsForSpecies(int species);

void simulationMonitor_addASexualReplicationsForHash(int hash, int x);
int simulationMonitor_getASexualReplicationsForSpecies(int species);

void simulationMonitor_addSexualReplicationsForHash(int hash, int x);
int simulationMonitor_getSexualReplicationsForSpecies(int species);

void simulationMonitor_addFailedMovesForHash(int hash, int x);
int simulationMonitor_getFailedMovesForSpecies(int species);

void simulationMonitor_addFailedReplicationsForHash(int hash, int x);
int simulationMonitor_getFailedReplicationsForSpecies(int species);

void simulationMonitor_addFailedAttacksForHash(int hash, int x);
int simulationMonitor_getFailedAttacksForSpecies(int species);

void simulationMonitor_addFailedGrowsForHash(int hash, int x);
int simulationMonitor_getFailedGrowsForSpecies(int species);

void simulationMonitor_addKilledBySeedingForHash(int hash, int x);
int simulationMonitor_getKilledBySeedingForSpecies(int species);

void simulationMonitor_addKilledByAttacksForHash(int hash, int x);
int simulationMonitor_getKilledByAttacksForSpecies(int species);

void simulationMonitor_addKilledByStarvingForHash(int hash, int x);
int simulationMonitor_getKilledByStarvingForSpecies(int species);

void simulationMonitor_addAveBrainSizeForHash(int hash, int x);
int simulationMonitor_getAveBrainSizeForSpecies(int species);

void simulationMonitor_addAveEnergyForHash(int hash, int x);
int simulationMonitor_getAveEnergyForSpecies(int species);

void simulationMonitor_addAveAgeForHash(int hash, int x);
int simulationMonitor_getAveAgeForSpecies(int species);

void simulationMonitor_addAveGenerationForHash(int hash, int x);
int simulationMonitor_getAveGenerationForSpecies(int species);

void simulationMonitor_addNumberOfAgentsForHash(int hash, int x);
int simulationMonitor_getNumberOfAgentsForSpecies(int species);

void simulationMonitor_clear();

int simulationMontior_test();
void simulationMonitor_writeMetricsFiles(int numberOfSpecies);
void simulationMonitor_emitMonitors();
void simulationMonitor_runIntelligenceTests(); //These are all simulations, the static analysis should be done continously.
#endif
