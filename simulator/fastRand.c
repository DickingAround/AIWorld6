#ifndef fastrand_c
#define fastrand_c
#include "fastRand.h"

void fastRand_init(fastRand *r) {
 int i;
 for( i = 0; i < FAST_RAND_ARRAY_LEN; i++) {
  r->rands[i] = (rand() / (float)RAND_MAX) * AG_INT_CONVERSION;
 }
 r->ptr = 0;
}
//WARNING: This is a piece of code that will be run in parallel threads and it's clearly not multi-threading safe
// This is on purpose, we don't need to be consistent with it. We just need to leave the system in an eternally consistent state.
// Note the we've added extra space to the rand list, this way if we ever walk off the end of the list we're still not off the end of the array
// We need only to add extra space equal to the number of threads, which we've done
unsigned char fastRand_getRand(fastRand *r) {
 unsigned char tmp = r->rands[r->ptr];
 r->ptr += 1;
 if(r->ptr >= FAST_RAND_LEN) {
  r->ptr = 0;
 } 
 return tmp;
}

#endif

