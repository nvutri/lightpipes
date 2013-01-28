/*--------------------------------------------------------------*/
/*      (C) Gleb Vdovin 1993-1999                               */
/*      This file is a part of LightPipes package               */
/*      Send bug reports to gleb@okotech.com                    */
/*                                                              */
/*--------------------------------------------------------------*/

#include "pipes.h"
#include <stdio.h>
#include <stdlib.h>

/**
Writes intensity distribution into a matrix
*/

void read_line(FILE *fr, char *line);

__declspec(dllexport) void image_to_matrix(const char *path, int matrixSize, double matrix[]){
	/*Variable declaration*/
	char *line;
	FILE* fr;
	int N = matrixSize;
	int i,j, ik1, imax, max_val;
	if (( fr = fopen(path,"rb")) == NULL) {
		fprintf(stderr,"error opening file %s, exiting \n", path);
		fflush(stderr);
		return;
	} 

	/* Read file type */
	line = (char *) calloc (110,1);

	read_line(fr, line);
	// Only read PGM file
	if( (strstr(line, "P2") != NULL) || (strstr(line, "P5") != NULL) ){
		fprintf(stderr,"PGM detected \n");
		/* Read header */
		read_line(fr, line);

		/* Read dimension imax and max_val*/
		fscanf(fr,"%d %d",&imax, &imax);
		fscanf(fr,"%d",&max_val);
	}
	else {
		fprintf(stderr,"NOT PGM Format \n");
		/* Re-open File to read straig*/
		fprintf(stderr,"Re Openning file \n");
		rewind(fr);
	}

	/* put the data in to the matrix*/
	ik1 = 0;
	for (i=1; i <= N; ++i){
		for (j=1; j <= N; ++j){ 
			double x;
			fscanf(fr, "%le", &x);
			matrix[ik1] = x;
			++ik1;
		}
	}
	fclose(fr);
	free(line);
}