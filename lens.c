/*--------------------------------------------------------------*/
/*      (C) Gleb Vdovin 1993-1999                               */
/*      This file is a part of LightPipes package               */
/*      Send bug reports to gleb@okotech.com                    */
/*                                                              */
/*--------------------------------------------------------------*/


#include <math.h>
#include "pipes.h"
#include <stdlib.h>
#include <stdio.h>

/**
@param F: F>0 for negative lens
*/
__declspec(dllexport) void lens(FIELD* field, double focal_length, double x_shift, double y_shift){
	double F = -focal_length;
	double xs = x_shift;
	double ys = y_shift;

	int i, j, n2;
	long ik;
	double x, x2, y, dx, pi2, K;


	pi2=3.1415926*2.;
	K=pi2/field->lambda;
	n2=field->n_grid/2;
	dx=field->size/field->n_grid;

	ik=0;

	for (i=1;i<=field->n_grid; i++){
		x=(i-n2-1)*dx-xs;
		x2=x*x;
		for (j=1;j<=field->n_grid; j++){ 
			double cab, sab, fi, cc;
			y	=	(j-n2-1)*dx-ys;
			fi	=	K*(x2+y*y)/(2.*F);
			cab	=	cos(fi);
			sab	=	sin(fi);
			cc	=	field->real[ik]*cab-field->imaginary[ik]*sab;
			field->imaginary[ik]	=	field->real[ik] * sab + field->imaginary[ik] * cab;
			field->real[ik]			=	cc;
			++ik;
		}
	}
}