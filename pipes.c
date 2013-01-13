/**
Contains library functions
*/

#include "pipes.h"
#include "fftn.h"
#include <math.h>
#include <stdlib.h>

double phase(double y, double x){    
	double pp=0.; 
	if(x==0.){
		if (y>0)	pp= 0.5*Pi;
		if (y==0)	pp= 0.;
		if (y<0)	pp= -0.5*Pi;
	}
	else {
		if(y !=0) pp = atan2 (y,x);
		else pp=0.;
	}
	return pp;
}

void fft3(FIELD* field, int ind){
	int *dims =  (int*) malloc (2*sizeof(int));
	dims[0]=dims[1]=field->n_grid;
	/* fprintf(stderr,"%d %d \n", dims[0], dims[1]); */
	fftn(2, dims, field->real, field->imaginary, ind, (double) field->n_grid);
	free( dims );
}