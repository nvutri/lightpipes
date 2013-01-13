/*--------------------------------------------------------------*/
/*      (C) Gleb Vdovin 1993-1999                               */
/*      This file is a part of LightPipes package               */
/*      Send bug reports to gleb@okotech.com                    */
/*                                                              */
/*--------------------------------------------------------------*/


#include <math.h>
#include "pipes.h"

/*
fprintf(stderr,"\n%s filters the beam through a gaussian diaphragm\n",arr);
fprintf(stderr,"\nUSAGE:  ");
fprintf(stderr,"%s R [DX DY T], where R is the 1/e (intensity) \n\
radius of the diaphragm in [units you use],\n\
DX and DY are the shifts of the diaphragm  center\n\
T is the intensity transmission in the  maximum (default T=1)\n\n",arr);
void gauss();
*/
__declspec(dllexport) void gauss(FIELD* field, double R, double xs, double ys, double AA){ 
	int i,j,n2;
	long ik;
	double x,x2,y,y2,dx,cc,R2;

	if (AA == 0)
		AA = 1;

	n2=field->n_grid/2;
	dx=field->size/field->n_grid;
	R2=R*R*2.;
	AA=sqrt(fabs(AA));
	ik=0;

	for (i=1;i<=field->n_grid; i++){
		x=(i-n2-1)*dx-xs;
		x2=x*x;
		for (j=1;j<=field->n_grid; j++){
			y=(j-n2-1)*dx-ys;
			y2=y*y;
			cc=AA*exp(-(x2+y2)/R2);
			field->imaginary[ik] *= cc;
			field->real[ik] *= cc;
			ik++;
		}
	}
}
