TODO:
* Make the agent detection number paginating so that we can see it different even when they wrap around. 
* Make the entire UI based on the species (do we need to have species within the C code??!)
* Add lineage to the agents
* Show communication patterns

* Add age to the agents
* Add mutation rate measure to the agents - DONE
* Create the first test, running many iterations and then an *automatic* analysis of it. What metics were statistically significantly different? 
** The first test is: Are they using their signaling or not? We turn off signaling. What then?? Or perhaps even, howe long does it take for them to react without it?? No, the real first test is an A/A test!... but then signaling.
* Show on the output their signals
* Load needs to also load the terrain

-----------------------------------
GETTING STARTED
-----------------------------------
* You will need to install the python library 'pygame' to launch the UI (search for 'apt-get pygame' for how to do this)
* You will need to install 'sudo apt-get libav-tools' which is used when turning all the images into a movie
* You will need to install imageMagic which is used for turning all the images into a gif
* Run the command 'bash make' to build and run the program.

-----------------------------------
RUNNING EXPERIMENTS
-----------------------------------
Experiments should have code in three places:
* In the config.h as a #define statement telling the compiler to include/exclude code
* In the main.c file printing out the name of the experiment so it's always clear what code is running
* In the simulation itself wherever needed. You can always locate all impacts of the experiment easily by searching for the name in the initial define statement.

Experiments that have been coded:
* A vs. A test
* No communication
* Aging - NOT IMPLEMENTED
* More communication - NOT IMPLEMENTED
* Only sexual or only asexual reproduction - NOT IMPLEMENTED (should be just another call to asex) 

-----------------------------------
IMPLEMENTATION
-----------------------------------

  PROGRAM STRUCTURE

There is a multi-threaded c program that runs the main simulation. It outputs files to ./outputs/ which are then consumed by a python UI program. The make scrip noted above compiles the c program, launches the python UI, runs the unit-tests on the c program, and then launches the c program to actually run the simulation.

  USE OF MEMORY

There are no 'malloc' memory allocations in this program. All memory used by the simulation is allocated on startup and then maintained within the program. This was done for the sake of speed given that many agents are created and destroyed on a very rapid basis. It also provides a degree of memory stability; if we allocated more memory as life grew you may only find out you allowed the life to grow too much when it crashes the program many hours or days into a run.

  USE OF CLASSES

The program has a rough class structure where structs are used as object classes. They're always saved in a .h file. All functions that operate on a class will start with the name of the class. For example there's 'world.h'/'world.c' the files and then world's functions will start with 'world_' and take a pointer to a world struct as their first argument.

  FILE DESCRIPTIONS

main_ui.py - The python UI

main.c - The main program, just input parsing and calling simulationManager

config.h - This defines many constants used within the program. Some can be changed without having to change anything else such as 'NUMBER_OF_THREADS' or 'AG_MUTATION_RATE'. Others are more tightly coupled such as the mapping of outputs from the brain of an agent.

simulationManager.h/.c - There is a single global instance of this class so that everything can access it without having to be given a pointer to it. The simulation manager is in charge of running the time iterations of the simulation and knowing when to run statistics. Each time-step of the world is broken into a decision phase and an action phase. The simulation manager launches and calls several threads that do decision making and then it's own thread handles the actions. The actions cant be easily multi-threaded because there is so much cross-involvement it wasn't worth doing in V1.

simulationManager_thread.c - This is not a class, but just a file with some complex mutex locks that are used by the simulationManager to wake up and communicate with the threads. It's implemented using several locks, which is complex, but it allows for the simulationManager and the threads to communicate without either of them ever having to use a sleep command and thus losing any time in the back-and-forth.

simulationManager_thread_control.h - This is the data the threads need to be passed initially.

world.h/.c - The world maintains a 2D array of all 'locations'. It also maintains the array of agents, which is  block of memory that it also uses when other parts of the program call it to 'allocate' a new agent in memory. The simulation manager and simulation monitor use that array when they want to iterate through all the agents that exist.

location.h/.c - A location knows what the cost to pass over this location is, how much food is there, and what agent is at this location.

agent.h/.c - An agent has an energy level, is facing a direction, and has a brain. It's functions include the implementation of actions it can take.

brain.h/.c - A brain is a 2-level sparse neural network. That means that instead of every possible connection being defined it's a list with only some connections being defined. This is done because the agent's brains are more likely to act like decision trees or fussy deciders than they are to act like complex pattern matchers. As such, it's more efficient computationally to model them this way. The brain also contains the complex replication logic.

intelligenceTests.c - This is a series of tests used to rate how intelligent the agents are. It's not yet complete r tested.

TESTING
Each class has it's own unit tests. In each there is a roll-up function named '<className>_test' that captures all of them and returns either 1 or 0. For example 'world_test()'. All the tests are called by main.c when you pass -t as the command line input. They output their results to the console but won't block the continued execution of the program.

-----------------------------------
DESIGN GOALS / NOTES
-----------------------------------
 * use audacity for audio recording
 * use tunes to tube for audio upload to youtube
 * use pinta to edit the still images
 * use openshot to edit the videos
 
SIMULATION GOALS:
* Evolve sexual reproduction
* Evolve cooperative behavior / multi-cellular behavior

IMPEMENTATION GOALS:
* Avoid the world stagnation problem
* Allocate all memory at the beginning
* Multi-threading in the C itself

IMPLEMENTATION: Classes
* Main - Runs the simulation, runs the tests, runs the display system, allows options for debug, timing, etc
* World - 2D array of locations, can save and load itself
* Location - Attributes of that locations, including the creature pointer * SimulationManager - Lists of threads, syncs threads as needed, runs iterations on the world, calls to get metrics. Threads stripe the world and thus must be synced.  * AgentManager - List of all agents, get and release the memory needed for them * Agent

IMPLEMENTAION: Agent function
* The world has to chang just enough to prevent long-term stagnation. Changing climate? Are we going to stick with the 'you must be alone to grow' concept? it does force long-term thought and movement past eachother... what are the alternatives? 
* Allow agents to stack up - They can't all get energy from the same spot, but they can at least slide past eachother. 

IMPLEMENTATION: Tests for intelligence
How can you tell if something is intelligent?
 * [ANALYSIS] They do exist and live
 * [ANALYSIS] They live in all the places that living is *possible* (not just easy)
 * [SIMULATION] They don't do obviously stupid things - waste energy on moves with no-outcome (we need to have a no-op move)
 * [ANALYSIS] They have the same approach to a similar situation, they will have similar brain structure (the same way living things on earth mostly look the same). 
 * [SIMULATION] Given a new environment, can they survive
 * [SIMULATION] Given a varacious predator, can they survive
 * [SIMULATION] Do they survive better spread out or lumped together
 ??* They save up and then expend in a big way, and can do this in patterns

This should be more like 'things I'd like to prove' - that these complexities benefit them
* Move and attack vs. move-attack
* Move-only vs. move-attack
* Changing climate
* Take a set of agents from an evolved world, place them randomly on a new world vs. place them togethe on a new world
* Give them language...Given 1 hormone vs.0 vs. 5, how do agents perform? (do they benefit from having more, or even no conversation?)
* Give them memory
* Give them the ability to modify the environment

IMPLEMENTATION: Agent senses
* Can see food values (0 to 1 - Value you get from eating here)
* Can see passibility values (0 to 1 - Cost of moving here (other creatures are a 2, and actually impassible))
* Can see log of other agents' energy (0 to inifinity)
* Can see 'hormone' values[triple pair] (-1 to 1)
 ---  3
----- 5
----- 5
--a-- 5
----- 5
23 inputs per type, 6 types = h

IMPLEMENTATION: Agent actions
* hormone set
* decision
** grow (no other agents nearby)
** move forward
** move left
** move right
** move back
** turn left
** turn right
** attack forward
** attack left
** attack right 

How do we want to use the tests? How do we want to get results??
* Run them ocassionally along with the other stats to show how well a long-running world is doing.
* Output them at a specific time with a designed test. Probably into a file, with a timestamp so it's hard to loose and easy to analyze. Also, the numbers will need to be numerous, there's no one number that can sum this so therefore there won't be any small set of numbers either. there will be a big set.
** Will the output file be human readable or not? Not. It will be displayed in the same UI as the rest of the simulation.  

we should do the plumbing first: One simple static analysis, and one simple simulation

What is the metric we get from each?
TEST staticAnalysis
* Of the locations with no-one nearby, how well populated are they? (can they actually fill the world?)
TEST doObviouslyStupidThings
* What % of the time are they moving to a square they can't, what % of the time are they replicating with a square that's not there
TEST replicateWithAnyone
* What % of the time will they replicate with a presented random agent (at random levels of energy)
TEST surviveNewEnvironment
* What 
TEST survivePredator
TEST surviveBetterTogether
