//#define EXP_NO_COMMUNICATION

//GO_SLOW makes the system wait x seconds between each action
/*#define GO_SLOW 5
#define NUMBER_OF_THREADS 1
#define SIM_ITERATIONS 10000
#define SIM_REPORT_INTERVAL 10
#define SIM_INTEL_TEST_INTERVAL 100000
#define SIM_SEED_INTERVAL 100000
#define SIM_SEED_DURATION 0
#define SIM_SEED_ENERGY 20
#define SIM_SEED_SIZE 30
*/
//Simulation settings
#define NUMBER_OF_THREADS 3
#define SIM_ITERATIONS 10000000
#define SIM_REPORT_INTERVAL 10000
#define SIM_SAVE_TIME_INTERVAL 1800 //30mins
#define SIM_INTEL_TEST_INTERVAL 100000
#define SIM_SEED_INTERVAL 100
#define SIM_SEED_DURATION 200000
#define SIM_SEED_ENERGY 20
#define SIM_SEED_SIZE 30
//#define LESS_METRICS //Define this to remove most per-agent metrics and accelerate the simulation
//#define LESS_TIMERS //Define this to remove most timers 
//Special simulation functions

//Otherwise the attack is a function of how much evergy an agent already has
#define SIM_CONSTANT_ATTACK_COST 20

//Otherwise the world is consistent in both passability and in terrain
#define SIM_COMPLEX_WORLD_TERRAIN 

//World settings
#define AG_TOTAL_ALLOWED 1000
#define AG_MAX_BUFFER_NEEDED 50000
#define AG_STATUS_ALIVE 1
#define AG_STATUS_DEAD 0
#define AG_STATUS_END_OF_LIST -1
#define AG_NO_LOCATION -99
#define PASS_IMPASSIBLE_INIT -1
#define PASS_IMPASSIBLE -1
#define WORLD_BORDER 3
#define WORLD_SIZE 100

//Agent settings
#define AG_ENABLE_AGE_LIMIT
#define AG_MAX_AGE 100000
#define AG_MUTATION_RATE 0.10
#define AG_MUTATION_POSITIVE_PRESSURE_CONN 0.00 //How much do we want more connections? Allow them to decline (which happens at zero)
#define AG_MUTATION_POSITIVE_PRESSURE_WEIGHT 0.01 //How much do we want higher weights?
#define WORLD_PASS_COST_MULT 0.1
#define WORLD_FOOD_MULT 1
#define WORLD_BARRIER_DENSITY 0.2
#define AG_MAX_ENERGY 10000
#define AG_ATTACK_RATE 0.1
#define AG_ATTACK_EFF 0.5
#define AG_ATTACK_COST 1
#define AG_TURN_COST 0.1
#define AG_MOVE_COST 0.1
#define AG_GROW_RATE 1
#define AG_GROW_COST 0.1
#define AG_REPLICATION_COST 0.5
#define AG_REPLICATION_GIVE 0.3
//Outputs from the brain
#define AG_OUTPUTS 32
#define AG_OUTPUTS_DECISIONS 9
#define AG_M_F 0 
#define AG_M_L 1
#define AG_M_R 2
#define AG_T_L 3
#define AG_T_R 4
#define AG_A_F 5
#define AG_R   6
#define AG_R_F 7
#define AG_GROW 8
#define AG_SIGNAL 9
#define AG_SIGNAL_NUMB 3
#define AG_MEM_OUT 12
#define AG_MEM_NUMB 20
//Remmber, total options is 0-based

//Inputs to the brain - facing down
//  -----> Y axis
//|  0  1  2  3  4
//|  5  6  7  8  9  --Agent is at location 7
//v 10 11 12 13 14
//  15 16 17 18 19
//x 20 21 22 23 24
#define AG_INPUTS 177
#define AG_INPUT_TYPE_SIZE 25
#define AG_IN_RAND 0
#define AG_IN_CONST 1
#define AG_IN_FOOD 2
#define AG_IN_PASS 27
#define AG_IN_AGENE 52
#define AG_IN_SIGNAL 77
//The signal is actually 25*3 =75 deep, mem is 20 deep as noted in AG_MEM_NUMB
#define AG_IN_MEM 152

#define AG_INPUT_MAX 0xFF
//#define AG_INPUT_MIN -- No inputs are possible to be lower numbers
#define AG_MULT_MAX 0x7F
#define AG_MULT_MIN -0x7F
#define AG_MULT_INIT_RANGE 0x2F
//We have a range of initalization that's different from the max in an effort to collect the values
#define AG_INPUTS_INIT 77 //Don't need to do signals or memory
#define AG_MIDS_INIT 50 //Don't  need to use most middle connections
#define AG_OUTPUTS_INIT 9 //Don't need to use memory or signal
#define AG_MID_NODES 100
#define AG_CONNS_INIT 50
#define AG_CONNS_L1 2000
#define AG_CONNS_L2 2000
#define AG_CONN_END 255 //AG_CONN_END must be larger than the number of inputs or outputs, maxes at 255 for u-char
//Species detection systems
//The color spectrum is 0 to 1530 (not including it)
//#define SPECIES_HASH_MIN 0 - assumed to be zero
#define SPECIES_HASH_MAX 15300
#define SPECIES_HASH_INIT_MIN 8000
#define SPECIES_HASH_INIT_SPREAD 20
#define SPECIES_HASH_BIAS_STRENGTH 0.1
#define SPECIES_HASH_BIAS_DIST 600

//#define AG_HASH_MOD 50
//#define AG_MAX_HASH 100000
//#define SPECIES_DETECTION_POPULATION_MIN 1
//#define SPECIES_DETECTION_POPULATION_DIF_MIN 6
//#define SPECIES_TYPES_MAX 1000
#define SPECIES_NUMBER_OF_METRICS 19
//Values for making decisions
#define AG_INT_CONVERSION 0xFFFF
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

//Fast rand settings
// Note that fast rand array length is 100 longer because we're allowing space for multi-threading issues we've intentionally allowed for the sake of speed.
#define FAST_RAND_LEN 10000
#define FAST_RAND_ARRAY_LEN 10100

#define INTELTESTS_FILE_LOC "/media/user/RaidDrive/CharliesStuff/production/AIWorld6/data_from_simulations/intelTests.txt" 
#define MONITOR_FILE_LOC "/media/user/RaidDrive/CharliesStuff/production/AIWorld6/data_from_simulations/monitor.txt"
#define SPECIES_STATS_FILE_LOC "/media/user/RaidDrive/CharliesStuff/production/AIWorld6/data_from_simulations/speciesStats.txt"
#define SPECIES_HIST_FILE_LOC "/media/user/RaidDrive/CharliesStuff/production/AIWorld6/data_from_simulations/speciesHist.txt"
//The world files switch off between A and B in order to maintain consistency in case we kill the process in the middle of saving. We only have to do this for things we might load from.
#define WORLD_AGENTS_FILE_LOC_A "/media/user/RaidDrive/CharliesStuff/production/AIWorld6/data_from_simulations/world_agents_a.txt"
#define WORLD_AGENTS_FILE_LOC_B "/media/user/RaidDrive/CharliesStuff/production/AIWorld6/data_from_simulations/world_agents_b.txt"
#define WORLD_LOCS_FILE_LOC_A "/media/user/RaidDrive/CharliesStuff/production/AIWorld6/data_from_simulations/world_locs_a.txt"
#define WORLD_LOCS_FILE_LOC_B "/media/user/RaidDrive/CharliesStuff/production/AIWorld6/data_from_simulations/world_locs_b.txt"
#define WORLD_WHICH_FILE_TO_USE_FILE_LOC "/media/user/RaidDrive/CharliesStuff/production/AIWorld6/data_from_simulations/world_whichFileToUse.txt"
