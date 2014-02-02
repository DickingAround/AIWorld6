#ifndef location_c
#define location_c
#include "location.h"

void location_save(location *l, int x, int y, FILE *file) {
 int k;
 fprintf(file,"%i,%i,%f,%f",x,y,l->p,l->f);
 for(k = 0; k < AG_SIGNAL_NUMB; k++) {
  fprintf(file,",%f",l->s[k]);
 } 
 fprintf(file,"\n");
}

#endif
