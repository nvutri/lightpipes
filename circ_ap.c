/*--------------------------------------------------------------*/
/*      (C) Gleb Vdovin 1993-1999                               */
/*      This file is a part of LightPipes package               */
/*      Send bug reports to gleb@okotech.com                    */
/*                                                              */
/*--------------------------------------------------------------*/

#include "pipes.h"
#include <stdio.h> 
#include <stdlib.h>
#include <tchar.h>
#include <Windows.h>

__declspec(dllexport) void circ_ap(double radius, double y_shift, double x_shift, FIELD* field){
	int i,j,i2;
	double dx,x,y,rr;
	long ik1;

	rr=radius*radius;
	dx =field->size/(field->n_grid);
	i2=field->n_grid/2;

	/* Cuttitng the aperture */     

	for (i=1;i<=field->n_grid; i++){
		x=(i-i2-1)*dx-x_shift;
		for (j=1;j<=field->n_grid; j++){

			y=(j-i2-1)*dx-y_shift;
			ik1=(i-1)*field->n_grid+j-1; 

			if(x*x+y*y > rr) {
				field->real[ik1]=0.;
				field->imaginary[ik1]=0.; 
			}
		}
	}
}
