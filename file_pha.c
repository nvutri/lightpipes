/*--------------------------------------------------------------*/
/*      (C) Gleb Vdovin 1993-1999                               */
/*      This file is a part of LightPipes package               */
/*      Send bug reports to gleb@okotech.com                    */
/*                                                              */
/*--------------------------------------------------------------*/

#include "pipes.h"
#include <stdio.h>

/*Writes phase of the field  into file F
USAGE: %s F [N L], where F is the filename
optional parameter N is the number of points in the grid,
N=64 if omitted in command line,
L is the level of intensity at which the phase is put out as zero\n\
default L=0.01
The phase can be plotted with gnuplot splot command
*/

__declspec(dllexport) void file_pha(FIELD* field, const char* path, int number_of_points, double level_of_intensity){
	FILE* fr;
	int i,j,imax, istep, ii, jj;
	double al, im, re, p_res;
	long ik1;

	//Open files
	if (( fr = fopen(path,"wb")) == NULL) {
		fprintf(stderr,"error opening file %s, exiting \n", path);
	}
	//Default values
	imax	=	64;
	al		=	0.01;
	//Given values 
	if (number_of_points != 0){
		imax = number_of_points;
	}
	if (level_of_intensity != 0.0){
		al = level_of_intensity;
	}

	istep=1;
	if (imax>field->n_grid) 
		imax=field->n_grid;

	if (field->n_grid/imax >1) {
		istep	=	field->n_grid/imax;
		imax	=	field->n_grid/istep;
	}

	/* writing the phase    */
	if(istep != 1){
		for (i=1;i < field->n_grid;i+=istep){
			for (j=1;j< field->n_grid;j+=istep){
				re = im = 0.;
				for(ii=i; ii<i+istep; ii++){
					for(jj=j; jj<j+istep; jj++){
						ik1	=	(ii-1)*field->n_grid+jj-1;
						re	+=	field->real[ik1];
						im	+=	field->imaginary[ik1];
					}
				}
				p_res = phase(im,re);
				fprintf(fr,"%0.3f\n",p_res);
			}
			fprintf(fr,"\n");
		}
	}
	else { 
		for (i=1; i<= field->n_grid; i+=istep){
			for (j=1; j<= field->n_grid; j+=istep){
				re	=	im  =  0.;
				ik1	=	(i-1)*field->n_grid+j-1;
				re	=	field->real[ik1];
				im	=	field->imaginary[ik1];
				p_res = phase(im, re);
				fprintf(fr,"%0.3f\n", p_res);
			}
			fprintf(fr,"\n");
		}
	}

	fclose(fr);
}