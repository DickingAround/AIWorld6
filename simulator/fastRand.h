#ifndef fastrand_h
#define fastrand_h

typedef struct fastRand {
 unsigned int ptr;
 //See the fastRand.c for documentation on why the 'LEN' is different from 'ARRAY_LEN'
 unsigned char rands[FAST_RAND_ARRAY_LEN];
} fastRand;

void fastRand_init(fastRand *r);
unsigned char fastRand_getRand(fastRand *r); 

#endif
