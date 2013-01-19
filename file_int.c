/*--------------------------------------------------------------*/
/*      (C) Gleb Vdovin 1993-1999                               */
/*      This file is a part of LightPipes package               */
/*      Send bug reports to gleb@okotech.com                    */
/*                                                              */
/*--------------------------------------------------------------*/

#include "pipes.h"
#include <stdio.h>
#include <math.h>

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
	int imax = field->n_grid;
	const int _MAX_VAL = 255;
	//Open the file
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

	//fprintf(fr, "%s\n", "P2");
	//fprintf(fr, "%s\n", "#Beam Intensity Image");
	//fprintf(fr, "%d %d\n", imax, imax);
	//fprintf(fr, "255\n");

	/* writing the intensity */
	ik1 = 0;
	for (i=1; i <= field->n_grid; i += istep){
		for (j=1; j <= field->n_grid; j += istep){ 
			double sum;
			sum = field->real[ik1]*field->real[ik1] + field->imaginary[ik1]*field->imaginary[ik1];
			if (j< field->n_grid)
				fprintf(fr, "%le ", sum );
			else
				fprintf(fr, "%le\n", sum );
			++ik1;
		}
	}
	//Close the file
	fclose(fr);
}