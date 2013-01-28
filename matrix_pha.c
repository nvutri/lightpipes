/*--------------------------------------------------------------*/
/*      (C) Gleb Vdovin 1993-1999                               */
/*      This file is a part of LightPipes package               */
/*      Send bug reports to gleb@okotech.com                    */
/*                                                              */
/*--------------------------------------------------------------*/

#include "pipes.h"
#include <stdio.h>

/*Writes phase of the field  into a matrix
*/

__declspec(dllexport) void matrix_pha(FIELD* field, double matrix[]){
	int i,j;
	double im, re, p_res;
	long ik1;
	int N = field->n_grid;
	/* writing the phase    */
	for (i=1; i<= N; ++i){
		for (j=1; j<= N; ++j){				
			ik1	=	N*(i-1) + j - 1;
			re	=	field->real[ik1];
			im	=	field->imaginary[ik1];
			p_res = phase(im, re) ;
			matrix[ik1] = p_res;
		}
	}
}