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
matrix pgm Put a matrix in to pgm file
N: grid_size from *field
G: @param double gamma
MAX: @param int max_val
*/
__declspec(dllexport) void matrix_pgm(const char *path, double matrix[], int gamma, int normalize, int matrixSize){
	FILE* fr;
	/*Varible declaration*/
	int i,j, max_int;
	long ik1;
	const int N = matrixSize;
	const int max_val = 255;
	//Initialization
	gamma = gamma ? gamma : GAMMA;
	normalize = normalize ? normalize : 0;

	//Open the file
	if (( fr = fopen(path,"wb")) == NULL) {
		fprintf(stderr,"error opening file %s, exiting \n", path);
	}

	//Header Information
	fprintf(fr,"P2\n");
	fprintf(fr,"#Creator: LightPipes (C) 1993-1996, Gleb Vdovin\n");
	fprintf(fr,"%d %d\n", N, N);
	fprintf(fr,"%d\n", max_val);

	//Normalization
	if (normalize){
		for (i=1; i<= N; i++){
			for (j=1 ;j <= N; j++ ){
				double sum;
				ik1=(i-1)*N +j- 1;
				sum = matrix[ik1];
				if (sum>max_int) 
					max_int=sum;
			}
		}
	}

	for (i=1; i<= N; ++i ){
		for (j=1; j <= N; ++j ){
			double sum;
			ik1 = N * (i - 1) + j - 1;
			sum = matrix[ik1];
			if (normalize){
				int i0=(int) floor(pow((sum/max_int),1./(gamma+0.00001))*max_val);
				fprintf(fr, "%d ", i0);
			}
			else {
				fprintf(fr, "%le ", sum);
			}
		}
		fprintf(fr,"\n");
	}
	fclose(fr);
}