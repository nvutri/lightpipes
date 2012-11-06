/*--------------------------------------------------------------*/
/*      (C) Gleb Vdovin 1993-1999                               */
/*      This file is a part of LightPipes package               */
/*      Send bug reports to gleb@okotech.com                    */
/*                                                              */
/*--------------------------------------------------------------*/

#include "pipes.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double Zernike();
double factorial(int n);

__declspec(dllexport) void Zer(FIELD* field, int n, int m, double R, double A){ 
	int i,j,n2;
	long ik;
	double x, y, dx, fi, cab, sab, cc, cc1, rho, phi;

	n2=field->n_grid/2;

	dx=field->size/field->n_grid;

	ik=0;
	/*     fprintf(stderr,"%le %le\n", tx,ty);*/ 
	for (i=1;i<=field->n_grid; i++){
		x=(i-n2-1)*dx;
		for (j=1;j<=field->n_grid; j++){
			y=(j-n2-1)*dx;
			rho=sqrt((x*x+y*y)/(R*R));

			phi=phase(y,x);

			fi= A*Zernike(n,m,rho,phi);
			cab=cos(fi);
			sab=sin(fi);
			cc=field->real[ik]*cab-field->imaginary[ik]*sab;
			cc1=field->real[ik]*sab+field->imaginary[ik]*cab;
			field->real[ik]=cc;
			field->imaginary[ik]=cc1;
			ik++;
		}

	}
}
/***************************************************************/
/* Zernike polynomial 

+-m
R    as in Born and Volf, p. 465, sixth edition, Pergamon
n

The implementation have not been optimized for speed.

*/

double Zernike(int n, int m, double rho, double phi){
	int s, int_sign, mm, ncheck, ind;
	double sum, product;
	if(n<0){
		fprintf(stderr,"Zernike: n must be >0; |m| must be less or equal than n\n\
					   if n is odd then m must be odd,\n\
					   if n is even then m must be even\n");
		return 0;
	}
	ind=0;
	for(ncheck=n; ncheck>=-n; ncheck -= 2){
		if (ncheck == m) ind=1;
	}
	if(ind == 0){
		fprintf(stderr,"Zernike: n must be >0; |m| must be less or equal than n\n\
					   if n is odd then m must be odd,\n\
					   if n is even then m must be even\n");
		return 0;
	}

	mm=(int) fabs( m );
	sum=0;
	int_sign=1;
	for (s=0; s<= (int)((n-mm)/2); s++){
		if(n-2*s != 0) product=pow( rho, (double)(n-2*s) );
		else product =1;
		product *= factorial(n-s)*int_sign;
		product /= factorial(s)*factorial(((n+mm)/2)-s)*factorial(((n-mm)/2)-s);
		sum += product;
		int_sign = -int_sign;
	}

	if(m<=0) 
		return sum*cos(m*phi);
	else 
		return sum*sin(m*phi); 
}

/* Factorial function */
double factorial(int n){
	double product;
	if (n<0) {
		fprintf(stderr,"factorial: argument is negative, exiting \n");
		return 0;
	}
	if (n==0) return 1.;
	else{ 
		product =1;
		while(n>=1){
			product *= n;
			--n;
		}
		return product;
	}
}


/*****************************************************************/
/*****************        END of Zernike     *********************/ 





