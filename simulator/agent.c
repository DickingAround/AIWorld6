#ifndef agent_c
#define agent_c
#include <float.h>
#include <stdio.h>
#include "agent.h"
extern simulationManager sm;
extern fastRand fr;

void agent_kill(agent *ag) {
 if(ag == NULL)
  printf("killing null agent??\n");
 world_deleteAgent(&sm.w, ag); 
 ag->energy = -1;
}

//----------------------------
// Inputs and decision making
//----------------------------
void agent_gatherInputs(agent *ag) {
 int i,j,k;
 location *tmpLoc;
 // ---------- Static inputs -----------------------
 //ag->br.inputs[AG_IN_RAND] = rand() / (float)RAND_MAX;
 ag->br.inputs[AG_IN_RAND] = fastRand_getRand(&fr); //This isn't thread safe, but it doesn't matter as long as it doesn't break and we get a value
 ag->br.inputs[AG_IN_CONST] = AG_INT_CONVERSION; //1 * the conversion
 // ---------- Location based inputs ---------------
 for(i = 0; i < 5; i++) {
  for(j = 0; j < 5; j++) {
   if(ag->facingDirection == DOWN) //Set the location we're talking about
    tmpLoc = &(sm.w.locs[ag->xLoc-1+i][ag->yLoc-2+j]);
   if(ag->facingDirection == UP)
    tmpLoc = &(sm.w.locs[ag->xLoc+1-i][ag->yLoc+2-j]);
   if(ag->facingDirection == LEFT)
    tmpLoc = &(sm.w.locs[ag->xLoc-2+j][ag->yLoc+1-i]);
   if(ag->facingDirection == RIGHT)
    tmpLoc = &(sm.w.locs[ag->xLoc+2-j][ag->yLoc-1+i]);
   ag->br.inputs[AG_IN_FOOD+i*5+j]  = tmpLoc->f*AG_INT_CONVERSION;  
   ag->br.inputs[AG_IN_PASS+i*5+j]  = tmpLoc->p*AG_INT_CONVERSION;  
   if(tmpLoc->a == NULL) {
    ag->br.inputs[AG_IN_AGENE+i*5+j] = 0;
   }
   else {
    if(tmpLoc->a->energy/20 > AG_INPUT_MAX)//Special case check to make sure the energy number isn't over the limits of the math
     ag->br.inputs[AG_IN_AGENE+i*5+j] = AG_INPUT_MAX*AG_INT_CONVERSION;
    else
     ag->br.inputs[AG_IN_AGENE+i*5+j] = tmpLoc->a->energy/20*AG_INT_CONVERSION;  //Energy is reduced cuz its a big number ususally
   }
   for(k = 0; k < AG_SIGNAL_NUMB; k++) { //Signals
    #ifndef EXP_NO_COMMUNICATION
    ag->br.inputs[AG_IN_SIGNAL+AG_INPUT_TYPE_SIZE*k+i*5+j] = tmpLoc->s[k]*AG_INT_CONVERSION;  
    #endif
    #ifdef EXP_NO_COMMUNICATION
    ag->br.inputs[AG_IN_SIGNAL+AG_INPUT_TYPE_SIZE*k+i*5+j] = 0;  
    #endif
   }
  }
 }
 // ------------ Memory -------------
 for( i = 0; i < AG_MEM_NUMB; i++) {
  ag->br.inputs[AG_IN_MEM+i] = ag->br.mem[i]; //Memory is always in it's raw, post conversion form
 }
}

void agent_makeDecision(agent *ag) {
 #ifndef LESS_METRICS
 ag->age++;
 #endif
 brain_makeDecision(&(ag->br));
}
 
void agent_performDecidedAction(agent *ag) {
 int i;
 //------ Save signals -------
 for(i = 0; i < AG_SIGNAL_NUMB; i++) { //Perform the signals
  sm.w.locs[ag->xLoc][ag->yLoc].s[i] = (float)(ag->br.outputs[AG_SIGNAL+i])/(float)AG_INT_CONVERSION;
  //if(ag->br.outputs[AG_SIGNAL+i] < -0.001 || ag->br.outputs[AG_SIGNAL+i] > 0.001)
   //printf("Found outputs being printed %i, %i %i, %i, %f\n",AG_SIGNAL+i,ag->xLoc,ag->yLoc,ag->br.outputs[AG_SIGNAL+i],(float)(ag->br.outputs[AG_SIGNAL+i])/(float)AG_INT_CONVERSION);
 }
 //------ Save memory --------
 // This can and is done inside the brain because it doesn't affect the others, thus there are no race conditions when doing it in parallel 
 //------ Save decision ------
 switch(ag->br.latestDecision) {//Each of these is it's own function because they're in the tight loop and every comparison statement counts
  case(AG_M_F):  agent_M_F(ag); break;
  case(AG_M_L):  agent_M_L(ag); break;
  case(AG_M_R):  agent_M_R(ag); break;
  case(AG_T_L):  agent_T_L(ag); break;
  case(AG_T_R):  agent_T_R(ag); break;
  case(AG_A_F):  agent_A_F(ag); break;
  case(AG_R):    agent_R(ag);   break;
  case(AG_R_F):  agent_R_F(ag); break;
  case(AG_GROW): agent_GROW(ag); break;
 }
}
void agent_A_F(agent *ag) { //ATTACK
 agent *otherAgent;
 otherAgent = NULL;
 ag->energy -= AG_ATTACK_COST;
 #ifndef LESS_METRICS
  simulationMonitor_addAttacksForHash(ag->br.speciesHash,1); 
 #endif
 switch(ag->facingDirection) {
  case(UP):    otherAgent = sm.w.locs[ag->xLoc-1][ag->yLoc  ].a; break;
  case(DOWN):  otherAgent = sm.w.locs[ag->xLoc+1][ag->yLoc  ].a; break; 
  case(LEFT):  otherAgent = sm.w.locs[ag->xLoc  ][ag->yLoc-1].a; break;
  case(RIGHT): otherAgent = sm.w.locs[ag->xLoc  ][ag->yLoc+1].a; break;
 }
 if(otherAgent != NULL) {
  if(otherAgent->energy <= 0)
   agent_kill(otherAgent); //Other agent was already empty
  if(ag->energy <= 0)
   agent_kill(ag);
  if(ag->energy * AG_ATTACK_RATE >= otherAgent->energy) {
   ag->energy += otherAgent->energy * AG_ATTACK_EFF - 0.0001;
   agent_kill(otherAgent);
   #ifndef LESS_METRICS
   simulationMonitor_addKilledByAttacksForHash(ag->br.speciesHash,1);
   #endif
  }
  else {
   ag->energy += ag->energy * AG_ATTACK_RATE * AG_ATTACK_EFF -0.0001;
   otherAgent->energy -= ag->energy * AG_ATTACK_RATE + 0.0001; 
  }
 }
 #ifndef LESS_METRICS
 else { //The other agent was empty
  simulationMonitor_addFailedAttacksForHash(ag->br.speciesHash,1);
 } 
 #endif
}
void agent_M(agent *ag, int x, int y) {
 location *newLoc;
 newLoc = &(sm.w.locs[x][y]);
 #ifndef LESS_METRICS
  simulationMonitor_addMovesForHash(ag->br.speciesHash,1);
 #endif
 ag->energy -= AG_MOVE_COST;
 if(newLoc->a == NULL) {
  if(newLoc->p > 0) {
   sm.w.locs[ag->xLoc][ag->yLoc].a = NULL;
   newLoc->a = ag;
   ag->energy -= newLoc->p;
   if(newLoc->p < 0)
    printf("Agent got energy from moving! %f\n",newLoc->p);
   ag->xLoc = x;
   ag->yLoc = y;
  }
 }
 #ifndef LESS_METRICS
 else { //The othe square is already occupied
  simulationMonitor_addFailedMovesForHash(ag->br.speciesHash,1); 
 }
 #endif
}
void agent_M_F(agent *ag) { //MOVE
 int x,y;
 switch(ag->facingDirection) {
  case(UP):    x = ag->xLoc-1; y = ag->yLoc  ; break;
  case(DOWN):  x = ag->xLoc+1; y = ag->yLoc  ; break; 
  case(LEFT):  x = ag->xLoc  ; y = ag->yLoc-1; break;
  case(RIGHT): x = ag->xLoc  ; y = ag->yLoc+1; break;
 }
 agent_M(ag,x,y);
}
void agent_M_L(agent *ag) {
 int x,y;
 switch(ag->facingDirection) {
  case(UP):    x = ag->xLoc  ; y = ag->yLoc-1; break;
  case(DOWN):  x = ag->xLoc  ; y = ag->yLoc+1; break; 
  case(LEFT):  x = ag->xLoc+1; y = ag->yLoc  ; break;
  case(RIGHT): x = ag->xLoc-1; y = ag->yLoc  ; break;
 }
 agent_M(ag,x,y);
}
void agent_M_R(agent *ag) {
 int x,y;
 switch(ag->facingDirection) {
  case(UP):    x = ag->xLoc  ; y = ag->yLoc+1; break;
  case(DOWN):  x = ag->xLoc  ; y = ag->yLoc-1; break; 
  case(LEFT):  x = ag->xLoc-1; y = ag->yLoc  ; break;
  case(RIGHT): x = ag->xLoc+1; y = ag->yLoc  ; break;
 }
 agent_M(ag,x,y);
}
void agent_T_R(agent *ag) { //TURN
 #ifndef LESS_METRICS
  simulationMonitor_addTurnsForHash(ag->br.speciesHash,1); 
 #endif
 ag->energy -= AG_TURN_COST;
 switch(ag->facingDirection) {
  case(UP):    ag->facingDirection = RIGHT; break;
  case(DOWN):  ag->facingDirection = LEFT;  break;
  case(LEFT):  ag->facingDirection = UP;    break;
  case(RIGHT): ag->facingDirection = DOWN;  break;
 }
}
void agent_T_L(agent *ag) {
 #ifndef LESS_METRICS
  simulationMonitor_addTurnsForHash(ag->br.speciesHash,1);
 #endif
 ag->energy -= AG_TURN_COST;
 switch(ag->facingDirection) {
  case(UP):    ag->facingDirection = LEFT;  break;
  case(DOWN):  ag->facingDirection = RIGHT; break;
  case(LEFT):  ag->facingDirection = DOWN;  break;
  case(RIGHT): ag->facingDirection = UP;    break;
 }
}
void agent_R(agent* ag) {
 #ifndef LESS_METRICS
  simulationMonitor_addASexualReplicationsForHash(ag->br.speciesHash,1); 
 #endif
 ag->energy -= AG_REPLICATION_COST; 
 agent_mallocAgent_fromAsex(ag);
}
void agent_R_F(agent* ag) {
 #ifndef LESS_METRICS
  simulationMonitor_addSexualReplicationsForHash(ag->br.speciesHash,1);
 #endif
 agent* otherAg;
 ag->energy -= AG_REPLICATION_COST;
 switch(ag->facingDirection) {
  case(UP):    otherAg = sm.w.locs[ag->xLoc-1][ag->yLoc  ].a; break;
  case(DOWN):  otherAg = sm.w.locs[ag->xLoc+1][ag->yLoc  ].a; break; 
  case(LEFT):  otherAg = sm.w.locs[ag->xLoc  ][ag->yLoc-1].a; break;
  case(RIGHT): otherAg = sm.w.locs[ag->xLoc  ][ag->yLoc+1].a; break;
 }
 if(otherAg != NULL) {
  agent_mallocAgent_fromSex(ag,otherAg); //Costing is done in that function
 }
 #ifndef LESS_METRICS
 else { 
  simulationMonitor_addFailedReplicationsForHash(ag->br.speciesHash,1);
 }
 #endif
}
void agent_GROW(agent *ag) {
 int i,j;
 #ifndef LESS_METRICS
  simulationMonitor_addGrowsForHash(ag->br.speciesHash,1);
 #endif
 for(i = -1; i <= 1; i++) {
  for(j = -1; j <= 1; j++) {
   if(sm.w.locs[ag->xLoc+i][ag->yLoc+j].a != NULL && (i != 0 && j != 0)) { //If found agent, and not at the origin
    ag->energy -= AG_GROW_COST;
    #ifndef LESS_METRICS
     simulationMonitor_addFailedGrowsForHash(ag->br.speciesHash,1);
    #endif
    return; 
   }
  }
 }
 ag->energy += AG_GROW_RATE * sm.w.locs[ag->xLoc][ag->yLoc].f;
}
//---------------------------------------
// Creation and reproduction
//---------------------------------------
agent* agent_mallocAgent(int x, int y, float e, unsigned long gen) {
 agent *a; 
 a = world_mallocAgent(&(sm.w),x,y); 
 if(a == NULL) {
  printf("Didn't get an agent, returning NULL\n");
  return NULL;
 }
 a->energy = e;
 a->age = 0;
 a->generation = gen;
 a->facingDirection = UP; 
 return a;
}
void agent_mallocAgent_fromScratch(int x, int y, float e) {
 agent *a; 
 if(sm.w.locs[x][y].a != NULL) {//Kill the agent if this one lands on them
  if(x != sm.w.locs[x][y].a->xLoc || y != sm.w.locs[x][y].a->yLoc) {
   printf("Agent doesn't know where he is, world says %i %i, agent says %i %i\n",x,y,sm.w.locs[x][y].a->xLoc,sm.w.locs[x][y].a->yLoc);
  }
  agent_kill(sm.w.locs[x][y].a);
  #ifndef LESS_METRICS
  //TODO: This doesn't work since there's not even a brain yet.
  //simulationMonitor_addKilledBySeedingForHash(sm.w.locs[x][y].a->br.speciesHash,1); 
  #endif
 }
 a = agent_mallocAgent(x,y,e,0);
 if(a == NULL)
  return;
 brain_makeFromScratch(&(a->br));
}
agent* agent_mallocAgent_checkAndMake(agent *a) {
 agent *newA; 
 int i,j,x,y;
 float e;
 newA = NULL;
 x = -1; //Look for a location nearby to put this agent in
 for(i = -1; i < 2; i++) {
  for(j = -1; j < 2 && x == -1; j++) {
   if(sm.w.locs[a->xLoc+i][a->yLoc+j].a == NULL && sm.w.locs[a->xLoc+i][a->yLoc+j].p > PASS_IMPASSIBLE) {
    x = a->xLoc+i;
    y = a->yLoc+j;
    break; 
   } 
  }
 }  
 if( x > 0 ) { //We did find a location to put it in 
  e = a->energy*AG_REPLICATION_GIVE;
  newA = agent_mallocAgent(x,y,e,(a->generation)+1);
  if(newA != NULL)
   a->energy -= e;
 }
 return newA;
}
void agent_mallocAgent_fromAsex(agent *a) {
 agent *newA = agent_mallocAgent_checkAndMake(a);
 if(newA != NULL)
  brain_makeFromAsex(&(newA->br),&(a->br));
}
void agent_mallocAgent_fromSex(agent *a, agent *b) {
 agent *newA = agent_mallocAgent_checkAndMake(a);
 if(newA != NULL)
  brain_makeFromSex(&(newA->br),&(a->br),&(b->br));
}
void agent_print(agent *a) {
 printf("Agent: %i,%i facting:%i, ene:%f, status:%i\n",a->xLoc,a->yLoc,a->facingDirection,a->energy,a->status);
 brain_print(&(a->br));
}
//--------------------
// SAVING AND LOADING
//--------------------
void agent_save(agent *a, FILE *file) {
 fprintf(file,"AG xLoc,%i yLoc,%i energy,%f facingDirection,%i latestDecision,%i age,%lu generation,%lu br,",a->xLoc,a->yLoc,a->energy,a->facingDirection,a->br.latestDecision,a->age,a->generation);
 brain_save(&(a->br),file);
 fprintf(file,"\n");
}

void agent_load(char *str, int strLength, unsigned int worldXMin, unsigned int worldXMax, unsigned int worldYMin, unsigned int worldYMax) {
 agent *a;
 int ptr, namePtr, xLoc, yLoc, facingDirection, age = 0, generation = 0;
 float energy;
 char name[20];
 ptr = 2;
 while(ptr < strLength && str[ptr] != '\n') {
  if(str[ptr] == ' ') { //Begin saving the name
   namePtr = 0;
   ptr++;
  }
  else if(str[ptr] == ',') { //Get the value
   ptr++;
   if(strcmp(name,"xLoc") == 0) 
    xLoc = atoi(str+ptr);
   if(strcmp(name,"yLoc") == 0)
    yLoc = atoi(str+ptr);
   if(strcmp(name,"age") == 0)
    age = atoi(str+ptr);
   if(strcmp(name,"generation") == 0)
    generation = atoi(str+ptr);
   if(strcmp(name,"facingDirection") == 0)
    facingDirection = atoi(str+ptr);
   if(strcmp(name,"energy") == 0)
    energy = atof(str+ptr);
   if(strcmp(name,"br") == 0) { //We should have all the values by now
    if(xLoc == -1 || yLoc == -1 || facingDirection == -1 || energy == -1) {
     printf("World loading: All the agent's parameters aren't here\n");
     //error_handler();  
    }
    a == NULL; //Next, make sure the agent fits in the world somewhere, in case we've resized the world since this save point
    if(xLoc >= worldXMin && xLoc < worldXMax && yLoc >= worldYMin && yLoc < worldYMax)
     a = world_mallocAgent(&sm.w,xLoc,yLoc);
    if(a != NULL) {
     a->energy = energy;
     a->facingDirection = facingDirection;
     a->age = age;
     a->generation = generation;
     brain_load(&(a->br),str+ptr,strLength-ptr);
    }
   }  
   while(str[ptr] != ' ' && str[ptr] != '\n') //Advance the pointer after getting the value
    ptr++;
  } 
  else { //Record the characters of the name
   name[namePtr] = str[ptr];
   name[namePtr+1] = '\0';
   namePtr++;
   ptr++;
  }
 }
}

//----------
// TESTING
//----------
int agent_test_mallocs();
int agent_test_gatherInputs();
int agent_test_actions();
int agent_test() {
 if(agent_test_gatherInputs() == 0)
    return 0;
 if(agent_test_mallocs() == 0)
  return 0; 
 if(agent_test_actions() == 0) 
  return 0;
 return 1;
}

int agent_test_mallocs() {
 int i;
 world_createFromScratch(&sm.w);
 for(i = 0; i < sm.w.numbAgents; i++) {
  if(sm.w.agents[i].energy > 0) {
   return 0; //No agents shoud have anything yet   
  } 
 } 
 for(i = 0; i < sm.w.numbAgents; i++) {
  agent_mallocAgent_fromScratch(i%sm.w.worldSize, (int)(i/sm.w.worldSize), 10);  
 }
 for(i = 0; i < sm.w.numbAgents; i++) {
  if(sm.w.agents[i].energy < 0) {
   return 0; //All agents should have things
  }
 } 
 return 1;
}

int agent_test_gatherInputs() {
 int i,j;
 world *w;
 w = &(sm.w);
 agent as[100];
 agent *a;
 for(i = 0; i < 7; i++) {
  for(j = 0; j < 7; j++) {
   w->locs[i][j].f = 10.0*(float)i+(float)j;
   w->locs[i][j].p = 100+10.0*(float)i+(float)j;
   
   w->locs[i][j].a = &(as[i*7+j]);
   w->locs[i][j].a->facingDirection = DOWN;
   w->locs[i][j].a->energy = 100000.0+10.0*(float)i+(float)j;
   w->locs[i][j].a->xLoc = i;
   w->locs[i][j].a->yLoc = j;
   w->locs[i][j].s[0] = 200.0+10.0*(float)i+(float)j;
   if(AG_SIGNAL_NUMB > 1)
    w->locs[i][j].s[1] = 100.0+10.0*(float)i+(float)j;
  }
 }
 a = w->locs[2][3].a; 
 agent_gatherInputs(a);
 //printf("%i %i %i",-0xFF,-0xFF*0x7F,-0x7FFF);
 for(i = 0; i < AG_INPUTS; i++) {
  if( a->br.inputs[i]/(float)AG_INT_CONVERSION < 0 ) {
    printf("Failed: Agent: The numbers on the input were too low, likely broken\n");
    return 0; //All values should be positive, which they may not be if the input is not handled correctly
  }
 }
 for(i = 0; i < 5; i++) { //Test some foods
   if(a->br.inputs[AG_IN_FOOD+5+i]/AG_INT_CONVERSION != i*1+21) {
     printf("Failed: Agent: food mapping to input was bad. wanted %i, got %i\n",i*10+12,a->br.inputs[AG_IN_FOOD+5+i]/AG_INT_CONVERSION);
     return 0;
  }
 }
 for(i = 0; i < 5; i++) { //Test some movements
   if(a->br.inputs[AG_IN_PASS+i*5+2]/AG_INT_CONVERSION != i*10+113) {
     printf("Failed: Agent: pass mapping to input was bad. wanted %i, got %i\n",i*10+113,a->br.inputs[AG_IN_PASS+i*5+2]/AG_INT_CONVERSION);
     return 0;
  }
 }
 for(i = 0; i < 5; i++) { //Test some energy
   if(a->br.inputs[AG_IN_AGENE+i]/AG_INT_CONVERSION != AG_INPUT_MAX) {
     printf("Failed: Agent: energy mapping to input was bad.\n");
     return 0;
  }
 }
 for(i = 0; i < 5; i++) { //Test some signal
   if(a->br.inputs[AG_IN_SIGNAL+i]/AG_INT_CONVERSION != i+211) {
     printf("Failed: Agent: signal mapping to input was bad.\n");
     return 0;
  }
 }
 if(AG_SIGNAL_NUMB > 1) {
  for(i = 0; i < 5; i++) { //Test some extra signal
   if(a->br.inputs[AG_IN_SIGNAL+AG_INPUT_TYPE_SIZE+i]/AG_INT_CONVERSION != i+111) {
     printf("Failed: Agent: signal mapping to input was bad. Wanted %i but got %i \n",i+111,a->br.inputs[AG_IN_SIGNAL+AG_INPUT_TYPE_SIZE+i]/AG_INT_CONVERSION);
     return 0;
   }
  }
 }
 a->facingDirection = RIGHT; 
 agent_gatherInputs(a);
 for(i = 0; i < 5; i++) { //Test some signal
   if(a->br.inputs[AG_IN_SIGNAL+AG_INPUT_TYPE_SIZE+i+10]/AG_INT_CONVERSION != -10*i+144) {
     printf("Failed: Agent: Facing right, signal mapping to input was bad. Wanted %i but got %i \n",-10*i+144,a->br.inputs[AG_IN_SIGNAL+AG_INPUT_TYPE_SIZE+i+10]/AG_INT_CONVERSION);
     return 0;
  }
 }
 a->facingDirection = LEFT; 
 agent_gatherInputs(a);
 for(i = 0; i < 5; i++) { //Test some signal
   if(a->br.inputs[AG_IN_SIGNAL+AG_INPUT_TYPE_SIZE+i+10]/AG_INT_CONVERSION != 10*i+102) {
     printf("Failed: Agent: Facing right, signal mapping to input was bad. Wanted %i but got %i \n",10*i+102,a->br.inputs[AG_IN_SIGNAL+AG_INPUT_TYPE_SIZE+i+10]/AG_INT_CONVERSION);
     return 0;
  }
 }

/* printf("foods:");
 for(i = 1; i < 6; i++) {
  for(j = 1; j < 6; j++) {
   printf("\t%i,%i:%i",i,j,(int)w->locs[i][j].s[1]);
  }
  printf("\n"); 
 } 
 printf("\n"); 
 printf("Inputs:");
 for(i = 0; i < AG_INPUTS; i++) {
  printf("\t%i:%i",i,w->locs[2][3].a->br.inputs[i]/AG_INT_CONVERSION);
 } 
 printf("\n");*/
 return 1;
}

int agent_test_actions() {
 int i,j,agentsFound;
 float ea,eb;
 agent *a, *b;
 world_createFromScratch(&(sm.w));
 sm.w.worldBorder = 0;
 world_makeRandomTerrain(&(sm.w));
 agent_mallocAgent_fromScratch(5,5,1000);
 agent_mallocAgent_fromScratch(2,3,1000);
 b = sm.w.locs[2][3].a;
 a = sm.w.locs[5][5].a; 
 a->facingDirection = UP;
 b->facingDirection = DOWN;
 agent_M_F(a); //a:4,5
 agent_T_L(a);
 agent_M_R(a); //a:3,5
 agent_M_F(a); //a:3,4
 agent_M_L(b); //b:2,4
 agent_T_R(a); //Now facing B, facing UP
 agent_M_F(a); //No action
 agent_T_L(b); //Facing RIGHT (y+)
 agent_R_F(b); //No action
 agent_R_F(b); //No action
 agent_R_F(b); //No action
 agent_R_F(a); //Should work
 agent_R(b);   //Should work
 if(a->xLoc != 3 || a->yLoc != 4 || b->xLoc != 2 || b->yLoc != 4) {
  printf("Agent: Action test: A and B are not where they should be\n");
  return 0; 
 }
 agentsFound = 0;
 for(i = 0; i < 5; i++) {
  for(j = 0; j < 5; j++) {
   if(sm.w.locs[i][j].a != NULL)
    agentsFound++;
  }
 }
 if(agentsFound != 4) {
  printf("Agent: Action test: didn't find the right number of agents\n");
  return 0;
 }
 //Now do some fighting
 ea = a->energy;
 eb = b->energy; 
 for(i = 0; i < 100; i++) {
  agent_A_F(a);
  if(i == 0 && (ea >= a->energy || eb <= b->energy)) {
   printf("Agent: Action test: Attacking didn't result in energy exchange,%f,%f then %f,%f\n",ea,eb,a->energy,b->energy);
   return 0; 
  } 
 }
 if(sm.w.locs[2][4].a != NULL) {
  printf("Agent: Action test: After many attacks, the agent is not destroyed\n");
  return 0;
 }
 return 1;
}
#endif
