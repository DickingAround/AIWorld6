#ifndef brain_h
#define brain_h
/*  STRUCTURE
lvl 1 : inputs
lvl 2 : mids
lvl 3 : outputs
*/
typedef struct brain
{
 //Values
 int inputs[AG_INPUTS]; //Multiply by AG_INT_CONVERSION to make these into ints on the way in
 int mids[AG_MID_NODES];
 int outputs[AG_OUTPUTS];
 //Connections
 unsigned char inL1[AG_CONNS_L1];
 unsigned char outL1[AG_CONNS_L1];
 unsigned char inL2[AG_CONNS_L2];
 unsigned char outL2[AG_CONNS_L2];
 float multL1[AG_CONNS_L1];
 float multL2[AG_CONNS_L2];
 float mutationRate;
 unsigned char latestDecision;
 unsigned int speciesHash;
} brain; 

void brain_computeHash(brain *b);
void brain_makeFromScratch(brain *newB);
void brain_makeFromAsex(brain *newB, brain *b);
void brain_makeFromSex(brain *newB, brain *b, brain *c);
//Seeding
void brain_makeConnLvlFromScratch(unsigned char *in, unsigned char inMax, unsigned char *out, unsigned char outMax, float *mult, float mutationRate, int connMax);
void brain_fillRestWithNoOps(unsigned char *in, unsigned char *out, int connMax, int i);
void brain_makeConnLvlFromScratch(unsigned char *in, unsigned char inMax, unsigned char *out, unsigned char outMax, float *mult, float mutationRate, int connMax);
//Saving and loading
void brain_save(brain *b, FILE *file);

#endif
