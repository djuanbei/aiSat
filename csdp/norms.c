/*
 * Calls the blas routine to compute a norm of a vector.
 */

#include <math.h>
#include "declarations.h"

double norm2(n,x)
int n;
double *x;
{
    int incx=1;
    return MY_DNRM2(&n,x,&incx);
   
}

double norm1(n,x)
int n;
double *x;
{
    int incx=1;
    return MY_DASUM(&n,x,&incx);
}

double norminf(n,x)
int n;
double *x;
{

    int i;
    int incx=1;
    i= MY_IDAMAX(&n,x,&incx);
    return fabs(x[i-1]);
      
} 



