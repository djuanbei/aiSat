/*
 * =====================================================================================
 *
 *       Filename:  f2rat.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  12/23/2013 06:38:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "f2rat.h"
#include <math.h>
void f2rat( const double orig, const int bound, int *den, int *num ) {
  double intpart   = floor( orig );
  double floatpart = orig - intpart;
  double temp;
  double h2 = 0;
  double h1 = 1;
  double k2 = 1;
  double k1 = 0;
  double h0 = intpart * h1 + h2;
  double k0 = intpart * k1 + k2;
  while ( k0 <= bound ) {
    //		if((h1+0.0)/k1==orig)
    //		  break;
    if ( fabs( floatpart ) < 1e-10 ) break;

    temp = 1.0 / floatpart;
    //		printf ( "%.20f\n",temp );

    intpart   = floor( temp );
    floatpart = temp - intpart;
    h2        = h1;
    h1        = h0;
    k2        = k1;
    k1        = k0;
    h0        = intpart * h1 + h2;
    k0        = intpart * k1 + k2;
  }
  *num = k0;
  *den = h0;
}
