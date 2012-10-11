/*--------------------------------------------------------------*/
/*      (C) Gleb Vdovin 1993-1999                               */
/*      This file is a part of LightPipes package               */
/*      Send bug reports to gleb@okotech.com                    */
/*                                                              */
/*--------------------------------------------------------------*/


/* This filter forms the initial grid filled with
unity field                               
*/

#include "pipes.h"
#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include <string.h> 

/* Global Variable */
FIELD field;

void main(int argc, char *argv[]){
	/*Function Declaration*/
	void errorprint(char* arr );
	/*Variable */
	double size_grid, lambda;
	int n_grid=256;
	double radius = 0.0025;
	/* Processing the command line argument  */	
	if (argc < 3 || argc >4) {
		errorprint(argv[0]);
	}

	sscanf(argv[1],"%le",&size_grid);
	sscanf(argv[2],"%le",&lambda);
	/* reading the data from a command line */

	if(argc>3) 
		sscanf(argv[3],"%d",&n_grid);

	/*Create a new field and generate image for it*/
	begin(n_grid, size_grid, lambda, &field);
	//circ_ap(radius, 0, 0, &field);
	//file_pgm(0, 0, &field);
	fil_ter(&field, "out.pgm", "int", "subst");
}

/*
Printing error message
*/
void errorprint(char* arr){
	fprintf(stderr,"\n%s forms the initial data structure for use \
				   by all following programs\n", arr);
	fprintf(stderr,"\nUSAGE: begin B C [A],  where:                       \n\n");
	fprintf(stderr,"A is the grid dimension, grid of AxA \
				   points will be formed (if omitted, A=256) \n");
	fprintf(stderr,"B is the grid size, the real size of the grid will\
				   be put as BxB [units you use]\n");
	fprintf(stderr,"C is the wavelength in [units you use]\n\n");
	exit(1);
}


/* 
Newfield allocates memory 
@return new FIELD
*/
void newfield(FIELD* field, int n_grid, double size, double lambda, 
			  int int1, int int2, int int3, 
			  double doub1, double doub2, double doub3){
				  field->n_grid=n_grid;
				  field->size=size;
				  field->lambda=lambda;
				  field->int1=int1;
				  field->int2=int2;
				  field->int3=int3;
				  field->double1=doub1;
				  field->double2=doub2;
				  field->double3=doub3;

				  field->real = (double *) calloc( (n_grid)*(n_grid), sizeof(double) );
				  if(field->real == NULL){
					  fprintf(stderr,"Allocation error, Real\n");
					  exit(1);
				  }

				  field->imaginary = (double *) calloc( (n_grid)*(n_grid), sizeof(double) );
				  if (field->imaginary == NULL){
					  fprintf(stderr,"Allocation error, Imaginary\n");
					  exit(1);
				  }
}

/*
Allocate a new field. And Initialize it.
*/
__declspec(dllexport) void begin(int n_grid, double size_grid, double lambda, FIELD* field){
	int i,j;
	long ik;   

	newfield(field, n_grid, size_grid, lambda,0,0,0,0.,0.,0.);
	/*  Here the initial field is formed   */
	// printf("%d %f %f", n_grid, lambda,size_grid);
	ik=0;
	for (i=1; i<=field->n_grid ; i++)
		for (j=1; j<=field->n_grid ; j++){
			field->real[ik]=1.;
			field->imaginary[ik]=0.;
			ik++;
		}
}


/* Debugging
_TCHAR szBuffer[100];
if ((field->real == NULL) || (field->imaginary == NULL)) {
fprintf(stderr,"NULL Pointers Error");
_stprintf(szBuffer, _T("NULL Pointers Error" ));
MessageBox(NULL, szBuffer, _T("wat"), MB_OK);
fflush(stderr);
}
*/
