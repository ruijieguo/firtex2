#include <math.h>
#include "rint.h"

double rint( double x)
// Copyright (C) 2001 Tor M. Aamodt, University of Toronto
// Permisssion to use for all purposes commercial and otherwise granted.
// THIS MATERIAL IS PROVIDED "AS IS" WITHOUT WARRANTY, OR ANY CONDITION OR
// OTHER TERM OF ANY KIND INCLUDING, WITHOUT LIMITATION, ANY WARRANTY
// OF MERCHANTABILITY, SATISFACTORY QUALITY, OR FITNESS FOR A PARTICULAR
// PURPOSE.
{
    if( x > 0 ) {
        __int64 xint = (__int64) (x+0.5);
        if( xint % 2 ) {
            // then we might have an even number...
            double diff = x - (double)xint;
            if( diff == -0.5 ){
                xint = xint-1;
				return (double)xint;
			}
        }
        return (double)xint;
    } else {
        __int64 xint = (__int64) (x-0.5);
        if( xint % 2 ) {
            // then we might have an even number...
            double diff = x - (double)xint;
            if( diff == 0.5 ){
                xint = xint+1;
				return (double)xint;
			}
        }
        return (double)xint;
    }
}