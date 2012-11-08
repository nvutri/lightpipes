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

void int_subst	(FIELD* field, const double* sum, const double smax);
void int_mult	(FIELD* field, const double* sum, const double smax);
void pha_subst	(FILE* fr, FIELD* field, const int bin_ind);
void pha_mult	(FILE* fr, FIELD* field, const int bin_ind);

/**
@param field: the field from begin.c
@param data : the matrix image data from file_pgm
@param c1:    the c1 option (int, pha)
@param c2:    the c2 option (mult, subst)
*/
__declspec(dllexport) void fil_ter(FIELD* field, const char* path, const char* c1, const char* c2){

	/*Variable declaration*/
	int bin_ind = 0;// FILE TYPE: P2 = 0; P5 = 1;
	int imax, max_val;
	char *line;
	FILE* fr;
	double* sum;
	double smax;
	if (( fr = fopen(path,"rb")) == NULL) {
		fprintf(stderr,"error opening file %s, exiting \n", path);
		fflush(stderr);
		return;
	} 

	/* Read file type */
	line = (char *) calloc (110,1);

	read_line(fr, line);
	// Only read PGM file
	if((strstr(line, "P2")) != NULL){
		fprintf(stderr,"Portable ASCII graymap detected \n"); 
		bin_ind = 0;
	}
	else if (strstr(line, "P5") != NULL) {
		fprintf(stderr,"Portable binary graymap detected \n"); 
		bin_ind = 1;
	}
	else {
		fprintf(stderr,"NOT PGM Format");
	}

	/* Read header */
	read_line(fr, line);

	/* Read dimension imax and max_val*/
	fscanf(fr,"%d %d",&imax, &imax);
	fscanf(fr,"%d",&max_val);
	/* Find sum and smax*/
	sum  = (double *) calloc( (field->n_grid)*(field->n_grid), sizeof(double) );
	if (sum == NULL) {
		fprintf(stderr,"fil_ter int subst: Allocation error, exiting\n");
	}

	/*
	Select the right function
	*/

	if ((strstr(c1, "int"))!= NULL) {
		smax = find_sum_smax(fr, field, sum, bin_ind);	
		if ((strstr(c2, "subst"))!=NULL){
			/*  int subst here */
			int_subst(field, sum, smax);
		}
		else { 
			/* int mult here */
			int_mult(field, sum, smax);
		}
	}
	else if ((strstr(c1, "pha"))!= NULL) { 
		if (strstr(c2, "subst")!=NULL){
			/*  pha  subst here */
			pha_subst(fr, field, bin_ind);
		}
		else { 
			/* pha  mult here */
			pha_mult(fr, field, bin_ind);
		}
	}
	else {
		fprintf(stderr, "NOT SELECTED: %s %s", c1, c2);
	}
	fflush(stderr);
	/* Deallocation and close file*/
	fclose(fr);
	free(sum);
	free(line);
}

// Read file type
void read_line(FILE* fr, char* line){
	int i;
	const int MAX_LEN = 100;
	char c_int;
	i=0;
	while((c_int=getc(fr)) != '\n') {
		line[i]=c_int;
		if(i<MAX_LEN){
			++i;
		}
	}
}

/**
* Fill in the sum array
* Find the smax value
*/
double find_sum_smax(FILE *fr, FIELD* field, double *sum, const int bin_ind){
	double fi;
	int ik, i, j;
	double smax=0;
	unsigned char b_in;
	ik=0;
	for (i=1; i<= field->n_grid; ++i){
		for (j=1; j <= field->n_grid; ++j){
			if(bin_ind) {
				if(fread (&b_in, sizeof(unsigned char), 1, fr) != 1){
					fprintf(stderr,"Error reading portable bitmap\n");
					return;
				}
				sum[ik] = (double) b_in;
			}
			else{
				if ((fscanf(fr,"%le",&fi))==EOF){
					fprintf(stderr,"fil_ter int subst: end of input file reached, exiting\n");
					return;
				}
				sum[ik] = fi;
			}
			if(smax < sum[ik]) 
				smax=sum[ik];
			++ik;
		}
	}
	return smax;
}

/**
* int subst helper function
*/

void int_subst(FIELD* field, const double* sum, const double smax){
	long ik;
	double ssum, phi;
	int i,j;

	ik=0;
	for (i=1; i<= field->n_grid; ++i){
		for (j=1; j <= field->n_grid; ++j){
			ssum=sqrt(sum[ik]/smax);
			phi=phase(field->imaginary[ik], field->real[ik]); 
			field->real[ik]=ssum*cos(phi);
			field->imaginary[ik] = ssum*sin(phi);
			ik++;
		}
	}	
}

/**
* int mult helper function
*/

void int_mult(FIELD* field, const double* sum, const double smax){
	long ik;
	double ssum;
	int i,j;

	ik=0;
	for (i=1; i<= field->n_grid; ++i){
		for (j=1; j <= field->n_grid; ++j){
			ssum=sqrt(sum[ik]/smax);

			field->real[ik] *= ssum;
			field->imaginary[ik] *= ssum;
			ik++;
		}
	}
}

/*
pha subst helper function
*/
void pha_subst(FILE* fr, FIELD* field, const int bin_ind){
	long ik;
	double cab, sab, ccc, fi;
	int i,j;
	unsigned char b_in;

	ik=0;
	for (i=1; i<= field->n_grid; ++i)
		for (j=1; j <= field->n_grid; ++j){
			if (bin_ind) {
				if(fread (&b_in, sizeof(unsigned char), 1, fr) != 1){
					fprintf(stderr,"Error reading portable bitmap\n");
					return;
				}
				fi = (double) b_in;
			}
			else{
				if ((fscanf(fr,"%le",&fi))==EOF){
					fprintf(stderr,"fil_ter pha subst: end of input file reached, exiting\n");
					return;
				}
			} 
			cab=cos(fi);
			sab=sin(fi);
			ccc=sqrt(field->real[ik]*field->real[ik]\
				+field->imaginary[ik]*field->imaginary[ik]);
			field->real[ik]= ccc*cab;
			field->imaginary[ik] = ccc*sab;
			ik ++;
		}
}

/* 
pha mult helper function
*/
void pha_mult(FILE* fr, FIELD* field, int bin_ind){
	long ik;
	double cab, sab, cc, fi;
	int i,j;
	unsigned char b_in;

	ik=0;
	for (i=1; i<= field->n_grid; ++i)
		for (j=1; j <= field->n_grid; ++j){
			if(bin_ind) {
				if(fread (&b_in, sizeof(unsigned char), 1, fr) != 1){
					fprintf(stderr,"Error reading portable bitmap\n");
					return;
				}
				fi = (double) b_in;
			}
			else{
				if ((fscanf(fr,"%le",&fi))==EOF){
					fprintf(stderr,"fil_ter int subst: end of input file reached, exiting\n");
					return;
				}
			} 
			cab=cos(fi);
			sab=sin(fi);
			cc	=	field->real[ik]*cab-field->imaginary[ik]*sab;
			field->imaginary[ik]	=	field->real[ik]*sab+field->imaginary[ik]*cab;
			field->real[ik]	=	cc;
			++ik;
		}
}
