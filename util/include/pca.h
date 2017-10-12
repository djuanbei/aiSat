/**
 * @file   pca.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:50:43 2015
 *
 * @brief  Principal component analysis (PCA)
 *
 *
 */

#ifndef PCA_INC
#define PCA_INC

#include "extenttype.h"
#ifdef __cplusplus
extern "C" {
#endif
int lpca(double *A, const int n, const int m, double *VT, BOOL needCenter);

#ifdef __cplusplus
}
#endif

#endif
