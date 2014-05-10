#ifndef world_c
#define world_c
#include <stdio.h>
#include <stdlib.h>
#include "world.h"

/*void world_checkForConsistentAgents(world *w) {
 int i,j,k;
 for(i = 0; i < w->numbAgents && w->agents[i].status != AG_STATUS_END_OF_LIST; i++) {
  if(w->agents[i].status == AG_STATUS_ALIVE) {
   if(sm.w.locs[sm.w.agents[i].xLoc][sm.w.agents[i].yLoc].a != &(sm.w.agents[i])) {
    printf("WARNING: Consistency checker has found agent thinks he's at %i %i but is at..\n");
    for(j = 0; j < 100; j++) {
     for(k = 0; k < 100; k++) {
      if(sm.w.locs[j][k].a == &(sm.w.agents[i]))
       printf("Found at %i %i\n",j,k);
     }
    }
    printf("Done searching\n");
    return;
   }
  }
 }
 printf("All consistent\n");
}*/
void world_makeRandomTerrain(world *w);
void world_clear(world *w);
void world_createFromScratch(world *w)
{
 world_clear(w);
 world_makeRandomTerrain(w);
}

void world_clear(world *w) {
 int x,y;
 w->lastEmptyAgent = 0;
 w->worldSize = WORLD_SIZE;
 w->worldBorder = WORLD_BORDER;
 w->numbAgents = AG_TOTAL_ALLOWED;
 for(x = 0; x < w->worldSize; x++) { 
   for(y = 0; y < w->worldSize; y++) {
     w->locs[x][y].a = NULL;
   } 
 }
 world_setupAgentList(w);
}
void world_makeRandomTerrain(world *w)
{
 int x,y;
 for(x = 0; x < w->worldSize; x++) {
  for(y = 0; y < w->worldSize; y++) {
   #ifndef SIM_COMPLEX_WORLD_TERRAIN
   w->locs[x][y].f = rand() / (float)RAND_MAX * WORLD_FOOD_MULT;
   w->locs[x][y].p = rand() / (float)RAND_MAX * WORLD_PASS_COST_MULT;
   #endif
   #ifdef SIM_COMPLEX_WORLD_TERRAIN 
   //Making the different parts of the world different styles. Breif ascii map below to describe them (F is low food, P is low passability)
   // __ P  P  __
   // F  FP FP F
   // F  FP FP F   <-- Lower half also has random obstacles
   // __ P  P  __
   if(x >= w->worldSize*0.25 && x < w->worldSize*0.75) { 
    w->locs[x][y].f = rand() / (float)RAND_MAX * WORLD_FOOD_MULT * 0.01;
   }
   else {
    w->locs[x][y].f = rand() / (float)RAND_MAX * WORLD_FOOD_MULT;
   }
   if(y >= w->worldSize*0.25 && y < w->worldSize*0.75) {
    w->locs[x][y].p = rand() / (float)RAND_MAX * WORLD_PASS_COST_MULT * 100;
   }
   else {
    w->locs[x][y].p = rand() / (float)RAND_MAX * WORLD_PASS_COST_MULT;
   }
   if(y >= w->worldSize*0.5 && rand()/(float)RAND_MAX < WORLD_BARRIER_DENSITY) {
    w->locs[x][y].p = PASS_IMPASSIBLE;
   }
   //Making barriers the split the world
   /*if(x > (w->worldSize/2 - w->worldBorder) && x < (w->worldSize/2 + w->worldBorder) && y > 5*w->worldBorder && y < (w->worldSize-5*w->worldBorder) ) {
    w->locs[x][y].p = PASS_IMPASSIBLE;
   } 
   if(y > (w->worldSize/2 - w->worldBorder) && y < (w->worldSize/2 + w->worldBorder) && x > 5*w->worldBorder && x < (w->worldSize-5*w->worldBorder) ) {
    w->locs[x][y].p = PASS_IMPASSIBLE;
   }*/ 
   #endif
   //Make sure no one cane walk off the end of the world 
   if(x < w->worldBorder || y < w->worldBorder || x >= (w->worldSize - w->worldBorder) || y >= (w->worldSize - w->worldBorder)) {
    w->locs[x][y].p = PASS_IMPASSIBLE;
   }  
  }
 }
}
void world_save_toAOrB(world *w, char aOrB) {
 int i,j; 
 FILE *outFile;
 //Save the agents
 if(aOrB == 'a') //We have two files and switch between them so the reader can read a compete file in real time.
  outFile = fopen(WORLD_AGENTS_FILE_LOC_A,"w");
 else
  outFile = fopen(WORLD_AGENTS_FILE_LOC_B,"w");
 for(i = 0; i < w->numbAgents && w->agents[i].status != AG_STATUS_END_OF_LIST; i++) {
  if(w->agents[i].status == AG_STATUS_ALIVE) {
   if(sm.w.locs[sm.w.agents[i].xLoc][sm.w.agents[i].yLoc].a != &(sm.w.agents[i]))
     printf("WARNING: World's save function has found this agent's location is incorrect\n");
   agent_save(&(w->agents[i]), outFile);
  }
 }
 fclose(outFile);
 //Save the terrain 
 if(aOrB == 'a') //We have two files and switch between them so the reader can read a compete file in real time.
  outFile = fopen(WORLD_LOCS_FILE_LOC_A,"w");
 else
  outFile = fopen(WORLD_LOCS_FILE_LOC_B,"w");
 fprintf(outFile,"%i,%i\n",w->worldSize,AG_SIGNAL_NUMB);
 for(i = 0; i < w->worldSize; i++) {
  for(j = 0; j < w->worldSize; j++) {
   location_save(&(w->locs[i][j]),i,j,outFile);
  }
 }
 fclose(outFile);
}
void world_save(world *w) {
 int i; 
 FILE *outFile;
 if(w->whichFileToUse == 'a') //We have two files and switch between them so the reader can read a compete file in real time.
  world_save_toAOrB(w,'b');
 else
  world_save_toAOrB(w,'a');
 //Now set the next state
 outFile = fopen(WORLD_WHICH_FILE_TO_USE_FILE_LOC, "w");
 if(w->whichFileToUse == 'a'){ 
  fprintf(outFile,"b");
  w->whichFileToUse = 'b';
 }
 else {
  fprintf(outFile,"a");
  w->whichFileToUse = 'a';
 }
 fclose(outFile);
}
void world_load_detailed(world *w, char aOrB, int clear, int xMin, int xMax, int yMin, int yMax)
{
 FILE *inFile;
 char str[AG_MAX_BUFFER_NEEDED];
 if(clear == 1)
  world_clear(w);
 if(aOrB == 'a')
  inFile = fopen(WORLD_AGENTS_FILE_LOC_A,"r");
 else
  inFile = fopen(WORLD_AGENTS_FILE_LOC_B,"r");
 while(fgets(str,AG_MAX_BUFFER_NEEDED,inFile) != NULL) {
  agent_load(str,AG_MAX_BUFFER_NEEDED,xMin,xMax,yMin,yMax);  
 } 
 fclose(inFile);
}
void world_load(world *w) {
 FILE *inFile;
 char str[3];
 inFile = fopen(WORLD_WHICH_FILE_TO_USE_FILE_LOC,"r");
 fgets(str,3,inFile);
 if(str[0] == 'a') {
  printf("Loading from a\n");
  world_load_detailed(w,'a',1,WORLD_BORDER,w->worldSize-WORLD_BORDER,WORLD_BORDER,w->worldSize-WORLD_BORDER); 
  printf("Loaded from a\n");
 }
 else if(str[0] == 'b') {
  printf("Loading from b\n");
  world_load_detailed(w,'b',1,WORLD_BORDER,w->worldSize-WORLD_BORDER,WORLD_BORDER,w->worldSize-WORLD_BORDER);
  printf("Loaded from a\n");
 }
 else
  printf("World didn't understand what file to load from %c\n",str[0]);
}
void world_loadTwoWorlds(world *w) { //The two world load assumes the worlds live in each of a and b
 world_load_detailed(w,'a',1,WORLD_BORDER,WORLD_SIZE/2           ,WORLD_BORDER,WORLD_SIZE-WORLD_BORDER);
 world_load_detailed(w,'b',0,WORLD_SIZE/2,WORLD_SIZE-WORLD_BORDER,WORLD_BORDER,WORLD_SIZE-WORLD_BORDER);
}

void world_setupAgentList(world *w) {
 int i;
 for(i = 0; i < w->numbAgents; i++) {
  w->agents[i].status = AG_STATUS_END_OF_LIST; 
  //w->agents[i].id = i;
  //w->agents[i].energy = -1;
 }
}
agent* world_mallocAgent(world *w,int x,int y) {
 int i; //always start at zero, anything after that one is a null.
 agent *ag;
 if(w->locs[x][y].a != NULL) {
  printf("You tried to allocate an agent on an occupied space?! %i %i\n",x,y);
  //error_handler(); //This will output the error but not stop the program 
  return NULL; 
 }
 for(i = 0; i < w->numbAgents; i++) {
  ag = w->agents + (i+w->lastEmptyAgent)%(w->numbAgents);
  if(ag->status == AG_STATUS_DEAD || ag->status == AG_STATUS_END_OF_LIST) {
   ag->status = AG_STATUS_ALIVE;
   ag->xLoc = x;
   ag->yLoc = y;
   w->locs[x][y].a = ag;
   w->lastEmptyAgent = (i+w->lastEmptyAgent)%(w->numbAgents);
   return ag;
  }
 }
 return NULL;
}
void world_deleteAgent(world *w, agent* a) {
 w->locs[a->xLoc][a->yLoc].a = NULL;
 a->xLoc = AG_NO_LOCATION;
 a->yLoc = AG_NO_LOCATION;
 a->status = AG_STATUS_DEAD;
}

//----------
// TESTS
//----------
int world_test_generalLoadSave();
int world_test_stressLoadSave();
int world_test() {
 if(world_test_generalLoadSave() == 0)
  return 0;
 if(world_test_stressLoadSave() == 0)
  return 0;
 return 1;
}
int world_test_stressLoadSave() {
 int i,j;
 world_createFromScratch(&sm.w);
 for(i = 0; i < WORLD_SIZE; i++) {
  for(j = 0; j < WORLD_SIZE; j++) {
   agent_mallocAgent_fromScratch(i,j,100);
  }
 }
 world_save_toAOrB(&sm.w,'a');
 world_createFromScratch(&sm.w);
 world_load_detailed(&sm.w,'a',1,0,1000,0,1000);
 for(i = 0; i < WORLD_SIZE; i++) {
  for(j = 0; j < WORLD_SIZE;j++) {
   if(sm.w.locs[i][j].a == NULL) {
    printf("Missing an agent at %i, %i\n",i,j);
    return 0;
   }
   if(sm.w.locs[i][j].a->xLoc != i || sm.w.locs[i][j].a->yLoc != j) {
    printf("Agent doesn't know their own location %i = %i , %i = %i\n",sm.w.locs[i][j].a->xLoc,i,sm.w.locs[i][j].a->yLoc,j);
    return 0;
   }
  }
 }
 return 1;
}
int world_test_generalLoadSave() {
 agent *ag; 
 int br_inL1_3, br_outL1_2;
 float br_multL2_0;
 world_createFromScratch(&(sm.w)); 
 agent_mallocAgent_fromScratch(5,7,10); 
 ag = sm.w.locs[5][7].a;
 br_inL1_3 = ag->br.inL1[3];
 br_outL1_2 = ag->br.outL1[2];
 br_multL2_0 = ag->br.multL2[0];
 agent_mallocAgent_fromScratch(10,11,100); 
 world_save_toAOrB(&sm.w,'a');
 world_createFromScratch(&(sm.w)); //I now expect it to be empty again
 if(sm.w.locs[5][7].a != NULL || sm.w.locs[10][11].a != NULL) {
  printf("World test: Agents were still there after a create-from-scratch.\n"); 
  return 0;
 }
 agent_mallocAgent_fromScratch(2,3,10); 
 agent_mallocAgent_fromScratch(4,5,100); 
 world_save_toAOrB(&sm.w,'b');
 world_load_detailed(&sm.w,'a',1,0,1000,0,1000);
 if(sm.w.locs[2][3].a != NULL || sm.w.locs[4][5].a != NULL) {
  printf("World test: Agents were still there after a load.\n"); 
  return 0;
 }
 ag = sm.w.locs[5][7].a;
 if(ag == NULL) {
  printf("World test: I failed to find the agent I wanted \n");
  return 0;
 }
 if(ag->br.inL1[3] != br_inL1_3 || ag->br.outL1[2] != br_outL1_2 || ag->br.multL2[0] < br_multL2_0 - 0.001 ||ag->br.multL2[0] > br_multL2_0 + 0.001) { 
  printf("World test: Agents didn't have the values expected. Wanted %i %i %f, got %i %i %f\n",br_inL1_3,br_outL1_2,br_multL2_0,ag->br.inL1[3],ag->br.outL1[2],ag->br.multL2[0]); 
  return 0;
 }
 //Also, what about clearing the old agents in memory in the world??
 return 1;
}
#endif
