/*--------------------------------------------------------------*/
/*      (C) Gleb Vdovin 1993-1999                               */
/*      This file is a part of LightPipes package               */
/*      Send bug reports to gleb@okotech.com                    */
/*                                                              */
/*--------------------------------------------------------------*/

#include "pipes.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/**
Writes intensity distribution into a matrix
*/

__declspec(dllexport) void matrix_int(FIELD *field, double matrix[]){
	int i,j, ik1;
	int N = field->n_grid;
	double sum;
	/* put the intensity in to the matrix*/
	ik1 = 0;
	for (i=1; i <= N; ++i){
		for (j=1; j <= N; ++j){ 
			sum = field->real[ik1]*field->real[ik1] + field->imaginary[ik1]*field->imaginary[ik1];
			matrix[ik1] = sum;
			++ik1;
		}
	}
}