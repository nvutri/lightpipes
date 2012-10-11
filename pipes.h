
/* 
The structure FIELD contains the characteristics
of the light beam: number of points along the side 
of a  square grid, wavelength and side length of the
square grid, then two huge arrays of Re and Im data 
*/

#ifndef GAMMA
#define GAMMA 2.0
#endif

#ifndef Pi
#define Pi 3.14159265358979323844
#endif

#ifndef Pi2
#define Pi2 6.28318530717958647688
#endif

#ifndef _PIPES_H
#define _PIPES_H

typedef struct{ 
	int n_grid;
	double size; 	double lambda;
	int int1;  int int2; 	int int3;
	double double1; double double2; double double3;
	double *real; double *imaginary; 
} 
FIELD;


/*Exporting to DLL functions*/
__declspec(dllexport) void begin(int n_grid, double size_grid, double lambda, FIELD* field);
__declspec(dllexport) void circ_ap(double radius, double y_shift, double x_shift, FIELD* field);
__declspec(dllexport) void file_ps (FIELD* field );
__declspec(dllexport) void file_pgm(double gamma, int max_val, FIELD* field);
__declspec(dllexport) void fil_ter(FIELD* field, const char* path, const char* c1, const char* c2);

#endif // !_PIPES_H