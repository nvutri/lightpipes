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

/* Shared functions */
double phase(double y, double x);
void fft3(FIELD* field, int ind);

/*Exporting to DLL functions*/
__declspec(dllexport) void begin(int n_grid, double size_grid, double lambda, FIELD* field);
__declspec(dllexport) void circ_ap(double radius, double y_shift, double x_shift, FIELD* field);
__declspec(dllexport) void fil_ter(FIELD *field, const char* path, const char* c1, const char* c2);
__declspec(dllexport) void Zer(FIELD *field, int n, int m, double R, double A);
__declspec(dllexport) void forvard(FIELD *field, double distance);
__declspec(dllexport) void lens_forvard (FIELD *field, double focal, double distance);
__declspec(dllexport) void lens(FIELD *field, double focal_length, double x_shift, double y_shift);
__declspec(dllexport) void convert(FIELD *field);

__declspec(dllexport) void file_ps (FIELD *field );
__declspec(dllexport) void file_pgm(FIELD *field, const char* path, double gamma, int max_val);
__declspec(dllexport) void file_int(FIELD *field, const char* path, int number_of_points);
__declspec(dllexport) void file_pha(FIELD *field, const char* path, int number_of_points, double level_of_intensity);
__declspec(dllexport) void gauss(FIELD *field, double R, double xs, double ys, double AA );

//Added to Lightpipes
__declspec(dllexport) void matrix_int(FIELD *field, double matrix[]);
__declspec(dllexport) void matrix_pha(FIELD *field, double matrix[]);
__declspec(dllexport) void fil_ter_matrix(FIELD* field, const char* c1, const char* c2, double matrix[]);
__declspec(dllexport) void free_field(FIELD *field);

#endif // !_PIPES_H