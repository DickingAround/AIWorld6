#ifndef agent_h
#define agent_h
#include "brain.h"
typedef struct agent
{
 int xLoc,yLoc; //Agent's location
// int id; //The location in the overall agent array, used only by world
 int status;  
 int facingDirection; //Agent's facing direction
 //float signals[AG_SIGNAL_NUMB];
 float energy; 
 brain br;
 unsigned long age;
 unsigned long generation; 
} agent; 

void agent_makeDecision(agent *ag);
void agent_mallocAgent_fromScratch(int x, int y, float e);
void agent_mallocAgent_fromAsex(agent *a);
void agent_mallocAgent_fromSex(agent *a, agent *b);
void agent_M_F(agent *ag);
void agent_M_L(agent *ag);
void agent_M_R(agent *ag);
void agent_T_L(agent *ag);
void agent_T_R(agent *ag);
void agent_A_F(agent *ag);
void agent_R(agent *ag);
void agent_R_F(agent *ag);
void agent_GROW(agent *ag);

void agent_save(agent *a, FILE *file);
void agent_load(char *str, int strLength);
void agent_print(agent *a);
#endif
