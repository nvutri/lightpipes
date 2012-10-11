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
void int_subst(FIELD* field, const double* sum, const double smax);
void int_mult(FIELD* field, const char *data);
void pha_subst(FIELD* field, const char *data);
void pha_mult(FIELD* field, const char *data);
void read_line(FILE* fr, char* line);
double find_sum_smax(FILE *fr, FIELD* field, double *sum);

/**
@param field: the field from begin.c
@param data : the matrix image data from file_pgm
@param c1:    the c1 option (int, pha)
@param c2:    the c2 option (mult, subst)
*/
__declspec(dllexport) void fil_ter(FIELD* field, const char* path, const char* c1, const char* c2){

	/*Variable declaration*/
	int imax, max_val;
	char* line;
	FILE* fr;
	double* sum;
	double smax;
	//fprintf(stderr, "%s", path);
	fflush(stderr);
	if (( fr = fopen(path,"r")) == NULL) {
		fprintf(stderr,"error opening file %s, exiting \n", path);
		fflush(stderr);
	} 

	/* Read file type */
	line = (char *) calloc (110,1);
	read_line(fr, line);
	// Only read PGM file
	if((strstr(line, "P2")) == NULL ) {
		return ;
	}
		
	/* Read header */
	read_line(fr, line);
	//fprintf(stderr, "%s\n", line); 

	/* Read dimension imax and max_val*/
	fscanf(fr,"%d %d",&imax, &imax);
	fscanf(fr,"%d",&max_val);
	//fprintf(stderr, "%d %d\n", imax, max_val);
	//fprintf(stderr, "%s %s \n", c1, c2);
	fflush(stderr);
	/* Find sum and smax*/
	sum  = (double *) calloc( (field->n_grid)*(field->n_grid), sizeof(double) );
	if (sum == NULL) {
		fprintf(stderr,"fil_ter int subst: Allocation error, exiting\n");
		exit(1);
	}
	smax = find_sum_smax(fr, field, sum);	

	/*
	Select the right function
	*/

	if ((strstr(c1, "int"))!= NULL) {
		if ((strstr(c2, "subst"))!=NULL){
			/*  int subst here */
			int_subst(field, sum, smax);
		}
		else { 
			/* int mult here */
			//int_mult(field);
		}
	}
	if ((strstr(c1, "pha"))!= NULL) { 
		if (strstr(c2, "subst")!=NULL){
			/*  pha  subst here */
			//pha_subst(field);
		}
		else { 
			/* pha  mult here */
			//pha_mult(field);
		}
	}
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
		if(i<MAX_LEN)
			++i;
	}

}

double phase(double y, double x){    
	double pp=0.; 
	if(x==0.){
		if (y>0) pp= 0.5*Pi;
		if (y==0)pp=0.;
		if (y<0) pp= -0.5*Pi;
	}
	else {
		if(y !=0) pp= atan2 (y,x);
		else pp=0.;
	}
	return pp;
}
/* Fill in the sum array
	Find the smax value
*/
double find_sum_smax(FILE *fr, FIELD* field, double *sum){
	double fi;
	int ik, i, j;
	double smax=0;
	ik=0;
	for (i=1; i<= field->n_grid; ++i){
		for (j=1; j <= field->n_grid; ++j){
			if (fscanf(fr,"%le",&fi)!= EOF){
				sum[ik]= fi;
				if(smax < sum[ik]) 
					smax = sum[ik];
			}
			ik++;
		}
	}
	return smax;
}

/*
int subst helper method
*/

void int_subst(FIELD* field, const double* sum, const double smax){
	long ik;
	double ssum, phi, fi;
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

/*
int mult helper method


void int_mult(FIELD* field){
long ik;
double *sum, cc, smax, ssum, fi, cab,sab,phi,ccc;
int i,j, bin_ind, ind;
char * first_string, c_int;
unsigned char b_in;

sum  = (double *) calloc( (field->n_grid)*(field->n_grid), sizeof(double) );
if(sum == NULL){fprintf(stderr,"fil_ter int subst: Allocation error, exiting\n");
exit(1);}

smax=0;
ik=0;
for (i=1;i<= field->n_grid;i += 1){
for (j=1;j <= field->n_grid;j += 1){

if(bin_ind) {
if(fread (&b_in, sizeof(unsigned char), 1, fr) != 1){
fprintf(stderr,"Error reading portable bitmap\n");
exit(1);}
sum[ik]= (double) b_in;
}
else{
if ((fscanf(fr,"%le",&fi))==EOF){
fprintf(stderr,"fil_ter int subst: end of input file reached, exiting\n");
exit(1);}
sum[ik]=fi;
} 

if(sum[ik] < 0 && ind == 0){
fprintf(stderr,"fil_ter int mult  warning: the\
intensity is negative\n"); ind = 1;
}
if(smax < sum[ik]) smax=sum[ik];
ik++;
}
}

if (argc != 5) smax=1.;

ik=0;
for (i=1;i<= field->n_grid;i += 1){
for (j=1;j <= field->n_grid;j += 1){
ssum=sqrt(sum[ik]/smax);

field->real[ik] *= ssum;
field->imaginary[ik] *= ssum;
ik++;
}
}
free(sum);
}

/*
pha_mult
*/
/*
void pha_subst(FIELD* field){
long ik;
double *sum, cc, smax, ssum, fi, cab,sab,phi,ccc;
int i,j, bin_ind, ind;
char * first_string, c_int;
unsigned char b_in;

ik=0;
for (i=1;i<= field->n_grid;i += 1){
for (j=1;j <= field->n_grid;j += 1){

if(bin_ind) {
if(fread (&b_in, sizeof(unsigned char), 1, fr) != 1){
fprintf(stderr,"Error reading portable bitmap\n");
exit(1);}
fi = (double) b_in;
}
else{
if ((fscanf(fr,"%le",&fi))==EOF){
fprintf(stderr,"fil_ter pha subst: end of input file reached, exiting\n");
exit(1);}

} 
/*          if ((fscanf(fr,"%le",&fi))==EOF){
fprintf(stderr,"fil_ter pha subst: end of input file reached, exiting\n");
exit(1);}
*/
/*
cab=cos(fi);
sab=sin(fi);
ccc=sqrt(field->real[ik]*field->real[ik]\
+field->imaginary[ik]*field->imaginary[ik]);
field->real[ik]= ccc*cab;
field->imaginary[ik] = ccc*sab;
ik ++;
}
}
}

void pha_mult(FIELD* field){
long ik;
double *sum, cc, smax, ssum, fi, cab,sab,phi,ccc;
int i,j, bin_ind, ind;
char * first_string, c_int;
unsigned char b_in;

ik=0;
for (i=1;i<= field->n_grid;i += 1){
for (j=1;j <= field->n_grid;j += 1){ 
if(bin_ind) {
if(fread (&b_in, sizeof(unsigned char), 1, fr) != 1){
fprintf(stderr,"Error reading portable bitmap\n");
exit(1);}
fi = (double) b_in;
}
else{
if ((fscanf(fr,"%le",&fi))==EOF){
fprintf(stderr,"fil_ter int subst: end of input file reached, exiting\n");
exit(1);}

} 
/*  if ((fscanf(fr,"%le",&fi))==EOF){
fprintf(stderr,"fil_ter pha mult: end of input file reached, exiting\n");
exit(1);}
*/
/*
cab=cos(fi);
sab=sin(fi);
cc=field->real[ik]*cab-field->imaginary[ik]*sab;
field->imaginary[ik]=field->real[ik]*sab+field->imaginary[ik]*cab;
field->real[ik]=cc;
ik++;

}
}
}
*/



/*	reading the author header
do {
i=0;
while((c_int=getc(fr)) != '\n') {
first_string[i]=c_int;
i++;
if(i>100) i=100;
}
//  fprintf(stderr, "%s\n", first_string); 
} 
while (first_string[0] == '#');

do{ 
i=0;
while((c_int=getc(fr)) != '\n') {
first_string[i]=c_int;
i++;
if(i>100) i=100;}
//  fprintf(stderr, "%s\n", first_string); 
} 
while (first_string[0] == '#');
*/
