
/**
 * @file   findsos.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:39:58 2015
 *
 * @brief  check whether a given polynomial  has a sos representation
 *
 *
 */

#ifndef FINDSOS_INC
#define FINDSOS_INC
#include "monomial.h"
#include "psdtype.h"
#include "sdpsolver.h"

namespace aiSat {
namespace psd {
class SOSChecker {
 private:
  Poly_t *p;

  bool polyIsSOS(Subpoly_t *subpoly, PointList *ans, const int verbose = 0);

  void sosrepresent(PointList *sosList, double *X, const int blockSize,
                    const int sosMid, const float minv);

  int overConvexChecking(const Subpoly_t *subpoly, const indice_t *indices,
                         const vector<int> &loc, const int size,
                         const map<monomial, monomialvalue> &monMap,
                         PointList *ans);

  void sosPresent(Subpoly_t *subpoly, const PointList *sosList,
                  const double printMin);

  int exactConvHull(const Subpoly_t *subpoly, indice_t *candidateSet,
                    int &candidateLength, const indice_t *genSet,
                    const int genLength, indice_t **SOSM, int &sosLength,
                    PointList *ans);

  Constraintmatrix *createConstraintMatrix(const Subpoly_t *subpoly,
                                           const int sosId, double **b,
                                           const indice_t *SOSM,
                                           const int sosLength,
                                           const int gLength);

 public:
  SOSChecker(Poly_t *poly) : p(poly) {}

  bool easychecksos();
};
}
}

#endif
