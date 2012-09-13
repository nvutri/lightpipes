#include "pipes.h"
void begin(int, char*);

void begin(int argc, char* argv[]){
	int n_grid,i,j;
    long ik;
    double size_grid, lambda;

    /* reading the data from a command line */
    sscanf(argv[1],"%le",&size_grid);
    sscanf(argv[2],"%le",&lambda);
    n_grid=256;
    if(argc>3) 
		sscanf(argv[3],"%d",&n_grid);
	
	newfield(n_grid, size_grid, lambda,0,0,0,0.,0.,0.);

    /*  Here the initial field is formed   */
    ik=0;
    for (i=1; i<=field.number ; i++)
		for (j=1; j<=field.number ; j++){
			field.real[ik]=1.;
			field.imaginary[ik]=0.;
			ik++;
		}

    write_field(); 
}