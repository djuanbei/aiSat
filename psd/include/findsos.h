
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

enum { SUCCESS = 1, FAILURE = 2, OTHER = 3 };
struct Params {
  double mineignvalue;
  int printPrec;
  Params() : mineignvalue(1e-6), printPrec(6) {}
};

class SOSChecker {
 private:
  int dim;
  Params para;
  Poly_t &p;

  bool polyIsSOS(Subpoly_t &subpoly, PointList *ans, const int verbose = 0);

  /**
   *
   *
   * @param sosList
   * @param X
   * @param blockSize
   * @param sosMid
   *
   * @return  true if success find a sos presentation for given polynomial,
   * flase otherwise
   */
  bool sosrepresent(PointList *sosList, double *X, const int blockSize,
                    const int sosMid);

  int overConvexChecking(const Subpoly_t &subpoly, const indice_t *indices,
                         const vector<int> &loc, const int size,
                         const map<Monomial, Monomialvalue> &monMap,
                         PointList *ans);

  void sosPresent(Subpoly_t &subpoly, const PointList *sosList);

  int exactConvHull(const Subpoly_t &subpoly, indice_t *candidateSet,
                    int &candidateLength, const indice_t *genSet,
                    const int genLength, indice_t **SOSM, int &sosLength,
                    PointList *ans);

  Constraintmatrix *createConstraintMatrix(const Subpoly_t &subpoly,
                                           const int sosId, double **b,
                                           const indice_t *SOSM,
                                           const int sosLength,
                                           const int gLength);

 public:
  SOSChecker(Poly_t &poly) : p(poly) {}
  void setPara(const Params &p) { para = p; }
  /**
   *
   *
   * @param polyvec the sos representaion polynomials
   *
   * @return true if find SOS presentation for a given polynomial, otherwise,
   * false
   */
  bool checksos(bool print = true);
  /**
   *
   *
   * @param polyvec the sos representaion polynomials
   *
   * @return true if find SOS presentation for a given polynomial, otherwise,
   * false
   */
  bool findSOS(vector<Poly_t> &polyvec);
};
}
}

#endif
