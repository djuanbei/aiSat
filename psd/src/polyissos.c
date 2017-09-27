
#include "polyissos.h"
#include <assert.h>
#include <math.h>
#include "poly.h"
#include "sdpsolver.h"
#include "util.h"

extern void dsyev_(char* jobz, char* uplo, int* n, double* a, int* lda,
                   double* w, double* work, int* lwork, int* info);

void sosrepresent(PointList* sosList, double* X, const int blockSize,
                  const int sosMid, const float minV) {
  int i, j, l, lda = blockSize;
  double* w = malloc_d(blockSize * sizeof(double));
  int lwork = blockSize * blockSize * 2;
  double* work;
  int info;
  int n = blockSize;
  lwork = -1;
  double workopt;

  dsyev_("V", "U", &n, X, &lda, w, &workopt, &lwork, &info);
  lwork = (int)workopt;
  work = (double*)malloc_d(lwork * sizeof(double));

  dsyev_("V", "U", &n, X, &lda, w, work, &lwork, &info);

  const int varNum = getvarNum(getSupElem(sosMid)->varId);

  indice_t* Z = getGsup(sosMid, &l);

  //	for ( i = 0; i < blockSize; i += 1 ) {
  //		printf ( "  %f\n",w[i] );
  //	}
  //
  //	for ( i = 0; i < blockSize; i += 1 ) {
  //
  //		for ( j = 0; j < blockSize; j += 1 ) {
  //			printf ( "     %6.5f",X[ijtok(i+1,j+1,blockSize)] );
  //		}
  //		printf ( "\n" );
  //	}

  for (i = 0; i < blockSize; i += 1) {
    if (w[i] > minV) {
      Poly* p1 = createPoly();
      polyChangeVarId(p1, getSupElem(sosMid)->varId);

      for (j = 0; j < blockSize; j += 1) {
        internal_addTerm(p1, Z + j * varNum, X[ijtok(j + 1, i + 1, blockSize)]);
      }
      p_mult_cons_assign(p1, sqrtf(w[i]));
      push_back_L(sosList, p1);
      //			printPoly(p1);
      //			deletePoly(p1);
    }
  }

  free(w);
  free(work);
}
