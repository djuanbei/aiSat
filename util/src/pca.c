/*
 * =====================================================================================
 *
 *       Filename:  pca.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  12/26/2013 07:47:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "pca.h"
#include "index.h"

#include "selfmemutil.h"
/* DGESVD prototype */
extern void dgesvd_(char* jobu, char* jobvt, int* m, int* n, double* a,
                    int* lda, double* s, double* u, int* ldu, double* vt,
                    int* ldvt, double* work, int* lwork, int* info);

/*-----------------------------------------------------------------------------
 *  the matrix A is M*N whose colum Dominant
 *  return the most lestest deominant vector
 *-----------------------------------------------------------------------------*/
int lpca(double* a, const int M, const int N, double* VT, BOOL needCenter) {
  int i, j;
  if (needCenter) {
    double mean[N];
    for (i = 0; i < N; i += 1) {
      mean[i] = a[ijtok(1, i + 1, M)];
    }
    for (j = 0; j < N; j += 1) {
      for (i = 1; i < M; i += 1) {
        mean[j] += a[ijtok(i + 1, j + 1, M)];
      }
    }
    for (i = 0; i < N; i += 1) {
      mean[i] /= M;
    }

    for (j = 0; j < N; j += 1) {
      for (i = 0; i < M; i += 1) {
        a[ijtok(i + 1, j + 1, M)] -= mean[j];
      }
    }
  }
  //	printf ( "M %d, N %d\n",M,N );

  /* Locals */
  int LDU = M, LDVT = N;
  int LDA = M;

  int m = M, n = N, lda = LDA, ldu = LDU, ldvt = LDVT, info, lwork;
  double wkopt;
  double* work;
  /* Local arrays */
  double s[N < M ? N : M], u[LDU * M];

  /* Executable statements */
  /* Query and allocate the optimal workspace */
  lwork = -1;
  dgesvd_("N", "A", &m, &n, a, &lda, s, u, &ldu, VT, &ldvt, &wkopt, &lwork,
          &info);
  lwork = (int)wkopt;
  work = (double*)malloc_d(lwork * sizeof(double));
  /* Compute SVD */
  dgesvd_("N", "A", &m, &n, a, &lda, s, u, &ldu, VT, &ldvt, work, &lwork,
          &info);
  /* Check for convergence */
  if (info > 0) {
    return -1;
  }
  //	for ( i = 0; i < N; i += 1 ) {
  //		eigvector[i]=vt[ijtok(N,i+1,ldvt)];
  //	}
  free((void*)work);
  return 0;
}
