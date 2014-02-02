#ifndef world_h
#define world_h

#include "location.h"
#include "agent.h"
typedef struct world
{
 int worldSize;
 int worldBorder;
 int numbAgents;
 agent agents[AG_TOTAL_ALLOWED];
 location locs[WORLD_SIZE][WORLD_SIZE];
 char whichFileToUse;//This is used to output to either file A or B so that anything reading while it's running sees a full file instead of a partially written one
} world; 

void world_setupAgentList(world *w);
agent* world_mallocAgent(world *w, int x, int y);
void world_deleteAgent(world *w, agent *a);

#endif
