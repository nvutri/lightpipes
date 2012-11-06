/*--------------------------------------------------------------*/
/*      (C) Gleb Vdovin 1993-1999                               */
/*      This file is a part of LightPipes package               */
/*      Send bug reports to gleb@okotech.com                    */
/*                                                              */
/*--------------------------------------------------------------*/

#include "pipes.h"
#include <math.h>
#include <stdio.h>


/**
Writes intensity distribution into file F
USAGE: %s F [N], where F is the output filename,\n\
optional parameter N is the number of points in the plotting grid, \n\
N=64 if omitted in command line\n\
N equals to grid sampling if you pass the word \"same\" \n",arr);
The intensity can be plotted with gnuplot splot command\n\n");
*/

__declspec(dllexport) void file_int(FIELD* field, const char* path, int number_of_points){
	FILE* fr;
	int i,j, istep;
	double dx,dx2;
	long ik1;
	int imax = 64;

	if (( fr = fopen(path,"wb")) == NULL) {
		fprintf(stderr,"error opening file %s, exiting \n", path);
	}
	if (number_of_points != 0){
		imax = number_of_points;
	}

	dx	= (field->size) / (field->n_grid-1.);
	dx2	=	dx * dx;

	istep=1;
	if (imax > field->n_grid)
		imax = field->n_grid;
	if ( (field->n_grid / imax) >1) {
		istep	=	field->n_grid/imax;
		imax	=	field->n_grid/istep;
	}

	/* writing the intensity     */

	for (i=1; i <= field->n_grid; i += istep){
		for (j=1; j <= field->n_grid; j += istep){ 
			double sum;
			sum	=	0;
			ik1	=	(i-1)*field->n_grid + j - 1; 
			sum += field->real[ik1]			* field->real[ik1] 
			+  field->imaginary[ik1]	* field->imaginary[ik1];
			fprintf(fr, "%0.3f\n", sum );
		}
		fprintf(fr, "\n");
	}

	fclose(fr);
}