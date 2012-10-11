/*--------------------------------------------------------------*/
/*      (C) Gleb Vdovin 1993-1999                               */
/*      This file is a part of LightPipes package               */
/*      Send bug reports to gleb@okotech.com                    */
/*                                                              */
/*--------------------------------------------------------------*/


#include "pipes.h"
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <tchar.h>
#include <Windows.h>
/*
file_pgm F [N, G, MAX]
N: grid_size from *field
G: @param double gamma
MAX: @param int max_val
*/
__declspec(dllexport) void file_pgm(double gamma, int max_val, FIELD* field){

	/*Varible declaration*/
	int i,j,ii,jj, istep,i0, i_i;
	int imax;
	long ik1;
	float max_int=0;

	//Initialization
	imax = field->n_grid;
	if (gamma==0)
		gamma=GAMMA;
	if (max_val==0)
		max_val=255;

	/*    if (((double) field->n_grid)/((double) imax) != field->n_grid/imax) imax=field->n_grid;
	*/

	istep=1;
	setmode(fileno(stderr), O_BINARY);
	/*    fprintf(stderr,"%d\n", istep); */
	//Header Information
	fprintf(stderr,"P2\n");
	fprintf(stderr,"#Creator: LightPipes (C) 1993-1996, Gleb Vdovin\n");
	fprintf(stderr,"%d %d\n", imax, imax);
	fprintf(stderr,"%d\n", max_val);
	fflush(stderr);
	if( istep != 1){

		for (i=1 ; i<= field->n_grid-istep; i +=  istep){
			for (j=1;j <= field->n_grid-istep;j += istep){
				double sum;
				sum=0;
				for (ii=i; ii<=i+istep; ii++)
					for(jj=j; jj<=j+istep; jj++){
						ik1=(ii-1)*field->n_grid +jj- 1;
						sum += field->real[ik1] *field->real[ik1]+ field->imaginary[ik1] *field->imaginary[ik1];
					}
					sum=sum/(istep*istep);
					if (sum>max_int) 
						max_int=sum;
			}
		}

		i_i=1;
		for (i=1; i<= field->n_grid-istep; i += istep){
			for (j=1;j <= field->n_grid-istep;j += istep){ 
				double sum;
				sum=0;
				for (ii=i; ii<=i+istep; ii++)
					for(jj=j; jj<=j+istep; jj++){
						ik1=(ii-1)*field->n_grid +jj- 1;
						sum +=field->real[ik1] *field->real[ik1]+ field->imaginary[ik1] *field->imaginary[ik1];
					}
					sum=sum/(istep*istep);
					i0=(int) floor(pow((sum/max_int),1./(gamma+0.0001))*max_val);
					/* i0=(int)  (sum/max_int)*255;*/
					fprintf(stderr,"%d ", i0);
					i_i++;
					if (i_i == 40){
						fprintf(stderr,"\n");
						i_i=1;
					}
			}
			fflush(stderr);
		}
	}
	else{
		for (i=1; i<= field->n_grid; i++){
			for (j=1 ;j <= field->n_grid; j++ ){
				double sum;
				ik1=(i-1)*field->n_grid +j- 1;
				sum =field->real[ik1] *field->real[ik1]+ field->imaginary[ik1] *field->imaginary[ik1];
				if (sum>max_int) 
					max_int=sum;
			}
		}
		i_i=1;
		for (i=1; i<= field->n_grid; i++ ){
			for (j=1; j <= field->n_grid; j++ ){
				double sum;
				ik1=(i-1)*field->n_grid +j- 1;
				sum = field->real[ik1] *field->real[ik1]+ field->imaginary[ik1] * (field->imaginary[ik1]);
				i0=(int) floor(pow((sum/max_int),1./(gamma+0.00001))*max_val);
				fprintf(stderr,"%d ", i0);
				i_i++;
				if (i_i == 40){
					fprintf(stderr,"\n");
					i_i=1;
				}
			}
			fflush(stderr);
		}
	}
	fflush(stderr);
}
/*
_TCHAR szBuffer[100];
_stprintf(szBuffer, _T("begin %d"), field->n_grid);
MessageBox(NULL, szBuffer, _T("wat"), MB_OK);
if ((field->real == NULL) || (field->imaginary == NULL)) {
fprintf(stderr,"NULL Pointers Error");
_stprintf(szBuffer, _T("NULL Pointers Error" ));
MessageBox(NULL, szBuffer, _T("wat"), MB_OK);
fflush(stderr);
}
*/














