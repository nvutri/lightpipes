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
//#include "begin.h"

void errorprint();

void main(int argc, char *argv[]){
	int n_grid,i,j;
    long ik;
    double size_grid, lambda;

	/* Processing the command line argument  */

    if (argc < 3 || argc >4) {
		errorprint(argv[0]);
    }
	begin(argc, argv);

	 /*    return (0);
			*/
}

//Printing error message
void errorprint(char *arr){
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