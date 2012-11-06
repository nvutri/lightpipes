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
__declspec(dllexport) void lens_forvard (FIELD* field, double focal, double distance ){
	int i,j;
	long ik;
	double z1,  f1, ampl_scale, *fi1, *fr1;
	double z = distance;
	double f = focal;

	f1=0.;
	if (field->double1 !=0. ) f1=1./field->double1;
	else f1=10000000.* field->size*field->size/field->lambda;
	if( (f+f1) != 0.) f=(f*f1)/(f+f1);
	else f=10000000.* field->size*field->size/field->lambda;
	z1=-z*f/(z-f);

	forvard(field, z1); 

	ampl_scale=(f-z)/f;
	field->size *= ampl_scale;
	field->double1= -1./(z-f);

	if (z1 >= 0.){
		ik=0;
		for (i=1;i<=field->n_grid; i++){
			for (j=1;j<=field->n_grid; j++){
				field->real[ik] = field->real[ik]/ampl_scale;
				field->imaginary[ik] =field->imaginary[ik]/ampl_scale; 
				ik++;
			}
		}
	}
	else{
		fr1 = (double *) calloc( (field->n_grid)*(field->n_grid), sizeof(double) );
		if(fr1 == NULL){
			fprintf(stderr,"Allocation error, fr1 in lens_forvard\n");
			return;
		}

		fi1 = (double *) calloc( (field->n_grid)*(field->n_grid), sizeof(double) );
		if (field->imaginary == NULL){
			fprintf(stderr,"Allocation error, fi1 in lens_forvard\n");
			return;
		}
		for (i=1;i<=field->n_grid; i++){
			for (j=1;j<=field->n_grid; j++){
				int i_i, j_i;
				long ik1;
				i_i=field->n_grid-i+1;
				j_i=field->n_grid-j+1;
				ik1 = (i_i-1)*field->n_grid + j_i -1;
				ik=(i-1)*field->n_grid +j-1;
				fr1[ik] = field->real[ik1]/ampl_scale;
				fi1[ik] =field->imaginary[ik1]/ampl_scale; 
			}
		}
		for (i=1;i<=field->n_grid; i++){
			for (j=1;j<=field->n_grid; j++){
				
				ik=(i-1)*field->n_grid +j-1;
				field->real[ik]= fr1[ik];
				field->imaginary[ik] = fi1[ik];

			}
		}
		free(fi1);
		free(fr1);
	}
	/*
	fprintf(stderr,"%e %e %e %e %e\n",f1,f,  z1, field->size,1./field->double1);
	*/
}
