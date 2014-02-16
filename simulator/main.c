
// includes, system
#include "config.c"
#include <stdlib.h>
#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "simulationManager.h" //Includes Pthread.h
#include "simulationMonitor.h"
#include "agent.h"
#include "location.h"
#include "world.h"//includes location.h
simulationManager sm;
void error_handler(int sig); 
#include "quickSigmoid.c"
#include "location.c"
#include "world.c"
#include "threadManager.c"
#include "brain.c"
#include "agent.c"
#include "simulationManager.c"//includes simulationManager.h
void runTests();
void runSimulation(int newWorld);
void runIntelTests();
void runExperiment();
int main(int argc, char** argv)
{
 //srand(138159158); //9);
 srand(time(NULL));
 quickSigmoid_init();
 signal(SIGSEGV,error_handler);
 if(argc != 2) //Assume we're running tests now
  runTests();
 else
  switch (argv[1][1])
  {
   case 't': //Run tests
    runTests();
    break;
   case 'c': //Continue simulation
    runSimulation(0); 
    break;
   case 'r': //Run simulation
    runSimulation(1);
    break;
   case 'i': //Run the intel tests on whatever agents were last
    runIntelTests();
    break;
   case 'e': //Run an experiment
    runExperiment();
    break;
  }
}
//Having this top level class as external not only reduces how much we need to pass it around, it also prevents us from having to cross link it at the thread control class, which greatly reduces the #include fancy footwork
void runSimulation(int newWorld)
{
 sm.treatment = "a0"; 
 simulationManager_run(newWorld); 
}
void runIntelTests() {
 simulationManager_load(); 
 simulationManager_runIntelligenceTests();
}
void runExperiment() {
 int i;
 #ifdef EXP_NO_COMMUNICATION
 printf("Running experiment: No communication\n");
 #endif
 #ifdef EXP_AA_TEST
 printf("Running experiment: A vs. A test\n");
 #endif
 for(i = 0; i < 10; i++) {
  sm.treatment = "a%i",i;
  simulationManager_run(1);
  sm.treatment = "b%i",i;
  simulationManager_run(1);
 }
}
void runTests()
{
 world *w;
 w = &(sm.w);
 
 /*
 if(threadManager_test() == 1)
   printf("Passed: Thread manager tests\n");
 else
   printf("Failed: Thread manager tests\n");
 if(simulationManager_thread_test() == 1)
   printf("Passed: Simulation Manager Thread tests\n");
 else
   printf("Failed: Simulation Manager Thread tests\n");
 if(brain_test() == 1)
   printf("Passed: Brain tests\n");
 else
   printf("Failed: Brain tests\n");
 if(agent_test() == 1)
   printf("Passed: Agent tests\n");
 else
   printf("Failed: Agent tests\n");
 if(world_test() == 1)
   printf("Passed: World tests\n");
 else
   printf("Failed: World tests\n");*/
 if(simulationMonitor_test() == 1)
   printf("Passed: IntelTest tests\n");
 else
   printf("Failed: IntelTest tests\n");
}
void error_handler(int sig) {
  void *array[40];
  size_t size;
  // get void*'s for all entries on the stack
  size = backtrace(array, 40);
  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, 1);
  exit(1); //-- Keep going if possible
}
