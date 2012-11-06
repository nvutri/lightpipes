/*--------------------------------------------------------------*/
/*      (C) Gleb Vdovin 1993-1999                               */
/*      This file is a part of LightPipes package               */
/*      Send bug reports to gleb@okotech.com                    */
/*                                                              */
/*--------------------------------------------------------------*/
    

#include <math.h>
#include "pipes.h"

/**
Converts the field from spherical variable
coordinate system into normal coordinate system",arr)
@param Y, where Y is dummy parameter preventing printing of this message
*/
__declspec(dllexport) void convert(FIELD* field){
    if (field->double1 !=0.) {
        lens (field, (-1.)/ (field->double1), 0. , 0.);
        field->double1=0;
    }
}

