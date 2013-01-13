/*--------------------------------------------------------------*/
/*      (C) Gleb Vdovin 1993-1999                               */
/*      This file is a part of LightPipes package               */
/*      Send bug reports to gleb@okotech.com                    */
/*                                                              */
/*--------------------------------------------------------------*/
    

/* This filter forms the initial grid filled with
   unity field                               
*/

#include <stdlib.h>
#include "pipes.h"

__declspec(dllexport) void freeField(FIELD* field){
	free(field->imaginary);
	free(field->real);	
}




















