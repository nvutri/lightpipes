/*--------------------------------------------------------------*/
/*      (C) Gleb Vdovin 1993-1999                               */
/*      This file is a part of LightPipes package               */
/*      Send bug reports to gleb@okotech.com                    */
/*                                                              */
/*--------------------------------------------------------------*/

#include "pipes.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Function declaration
*/
void read_line(FILE* fr, char* line);
double find_sum_smax(FILE *fr, FIELD* field, double *sum, const int bin_ind);

void int_subst_matrix(FIELD* field, double sum[]);
void int_mult_matrix(FIELD* field, double sum[]);
void pha_subst_matrix(FIELD* field, double sum[]);
void pha_mult_matrix(FIELD* field, double sum[]);

/**
@param field: the field from begin.c
@param data : the matrix image data from file_pgm
@param c1:    the c1 option (int, pha)
@param c2:    the c2 option (mult, subst)
*/
__declspec(dllexport) void fil_ter_matrix(FIELD* field, const char* c1, const char* c2, double matrix[]){
	/*
	Select the right function
	*/

	if ((strstr(c1, "int"))!= NULL) {
		if ((strstr(c2, "subst"))!=NULL){
			/*  int subst here */
			int_subst_matrix(field, matrix);
		}
		else { 
			/* int mult here */
			int_mult_matrix(field, matrix);
		}
	}
	else if ((strstr(c1, "pha"))!= NULL) { 
		if (strstr(c2, "subst")!=NULL){
			/*  pha  subst here */
			pha_subst_matrix(field, matrix);
		}
		else { 
			/* pha  mult here */
			pha_mult_matrix(field, matrix);
		}
	}
}

/**
* int subst helper function
*/

void int_subst_matrix(FIELD* field, double sum[]){
	long ik;
	double ssum, phi;
	int i,j;

	ik=0;
	for (i=1; i<= field->n_grid; ++i){
		for (j=1; j <= field->n_grid; ++j){
			ssum = sqrt(sum[ik]);
			phi=phase(field->imaginary[ik], field->real[ik]); 
			field->real[ik]		 = ssum*cos(phi);
			field->imaginary[ik] = ssum*sin(phi);
			ik++;
		}
	}	
}

/**
* int mult helper function
*/

void int_mult_matrix(FIELD* field, double sum[]){
	long ik;
	double ssum;
	int i,j;

	ik=0;
	for (i=1; i<= field->n_grid; ++i){
		for (j=1; j <= field->n_grid; ++j){
			ssum=sqrt(sum[ik]);
			field->real[ik] *= ssum;
			field->imaginary[ik] *= ssum;
			ik++;
		}
	}
}

/*
pha subst helper function
*/
void pha_subst_matrix(FIELD* field, double sum[]){
	long ik;
	double cab, sab, ccc, fi;
	int i,j;

	ik=0;
	for (i=1; i<= field->n_grid; ++i)
		for (j=1; j <= field->n_grid; ++j){
			fi = sum[ik];
			cab=cos(fi);
			sab=sin(fi);
			ccc=sqrt(field->real[ik]*field->real[ik]\
				+field->imaginary[ik]*field->imaginary[ik]);
			
			field->real[ik]		 = ccc*cab;
			field->imaginary[ik] = ccc*sab;			
			++ik;
		}
}

/* 
pha mult helper function
*/
void pha_mult_matrix(FIELD* field, double sum[]){
	long ik;
	double cab, sab, cc, fi;
	int i,j;

	ik=0;
	for (i=1; i<= field->n_grid; ++i)
		for (j=1; j <= field->n_grid; ++j){
			fi=sum[ik];
			cab=cos(fi);
			sab=sin(fi);
			cc	=	field->real[ik]*cab-field->imaginary[ik]*sab;
			field->imaginary[ik]	=	field->real[ik]*sab+field->imaginary[ik]*cab;
			field->real[ik]	=	cc;
			++ik;
		}
}
