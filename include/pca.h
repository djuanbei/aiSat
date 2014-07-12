/*
 * =====================================================================================
 *
 *       Filename:  pca.h
 *
 *    Description:   Principal component analysis
 *
 *        Version:  1.0
 *        Created:  12/26/2013 07:45:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  PCA_INC
#define  PCA_INC

#include	"polytype.h"
#ifdef __cplusplus
extern "C" {
#endif
int lpca(double * A,  const int n, const int m,  double *VT, BOOL needCenter);

#ifdef __cplusplus 
}
#endif

#endif   /* ----- #ifndef PCA_INC  ----- */
