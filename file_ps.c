/*--------------------------------------------------------------*/
/*      (C) Gleb Vdovin 1993-1999                               */
/*      This file is a part of LightPipes package               */
/*      Send bug reports to gleb@okotech.com                    */
/*                                                              */
/*--------------------------------------------------------------*/

#include "pipes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <io.h>

__declspec(dllexport) void file_ps (FIELD* field ){
	int i,j,ii,jj,imax, istep,i0;
	long ik1;
	float max_int=0, gamma;
	double sum = 0;

	setmode(fileno(stderr), O_BINARY);
	if (field->n_grid > 0){
		//Initialize
		imax=field->n_grid;
		gamma = GAMMA;
		istep=1;
		if(imax>field->n_grid)
			imax=field->n_grid;

		if(field->n_grid/imax >1){
			istep= field->n_grid/imax;
			imax=(int)ceil ((float)field->n_grid/(float) istep);
		};

		/* header of the postscript file */
		fprintf(stderr, "%%!PS-Adobe-2.0 EPSF-2.0\n");
		fprintf(stderr, "%%%%BoundingBox: 0 0 256 256  \n");
		fprintf(stderr, "%%%%Creator: LightPipes (C) 1993 Gleb Vdovin, beta version 1995 \n");

		/*   //fprintf(stderr, "%s", "%%%%Pages: 1\n");*/
		fprintf(stderr, "%%%%EndComments\n");
		fprintf(stderr, "%%%%EndProlog\n\n");
		/*    //fprintf(stderr, "%s", "%%%%Page: 1 1\n\n");*/

		fprintf(stderr, "/origstate save def\n20 dict begin\n");
		fprintf(stderr, "/picstr %d string def\n", (imax-1));
		fprintf(stderr, "256  256   scale\n");
		fprintf(stderr, "%d %d  8\n", (imax-1), (imax-1));
		fprintf(stderr, "[ %d 0 0 %d 0 %d]\n", (imax-1), -(imax-1), (imax-1));
		fprintf(stderr, "{currentfile\npicstr readhexstring pop}\nimage\n ");    
		fflush(stderr);
		/* Print the image */

		for (i=1; i<= field->n_grid-istep; i +=  istep){
			for (j=1; j <= field->n_grid-istep; j += istep){
				double sum;
				sum=0;
				for (ii=i; ii<=i+istep; ii++)
					for(jj=j; jj<=j+istep; jj++){
						ik1=(ii-1)*field->n_grid +jj- 1;
						sum += field->real[ik1] *field->real[ik1]+ field->imaginary[ik1] *field->imaginary[ik1];
					}
					sum=sum/(istep*istep);
					if(sum>max_int) 
						max_int=sum;
			}
		}

		for (i = 1; i<= field->n_grid-istep; i +=  istep){
			for (j=1;j <= field->n_grid-istep;j += istep){ 
				sum = 0;
				for (ii=i; ii<=i+istep; ii++){
					for(jj=j; jj<=j+istep; jj++){
						ik1=(ii-1)*field->n_grid +jj- 1;
						sum += field->real[ik1] * field->real[ik1] + 
							field->imaginary[ik1] * field->imaginary[ik1];
					}
				}
				sum=sum/(istep*istep);
				i0=(int) floor( pow( (sum/max_int), 
					(1. / (gamma+0.00001)) )
					*255);
				fprintf(stderr, "%02x", i0);					
			}		
			fflush(stderr);
		}
		fprintf(stderr, "\n\nshowpage\n%%%%Trailer\n");
	}
	fflush(stderr);
}

/*Debugging 
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
