#ifndef location_h
#define location_h

#include "agent.h"
typedef struct location
{
 agent* a;
 float f; //Food level
 float p; //Passability - More specifically, the cost to traverse (any negative is impassible)
 float s[AG_SIGNAL_NUMB];  
} location; 

void location_save(location *l, int x, int y, FILE *file);
#endif
