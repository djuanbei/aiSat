
/**
 * @file   convexhull.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:35:53 2015
 *
 * @brief  delete the superfluous element in candidate monomial set
 *
 *
 */

#ifndef CONVEXHULL_INC
#define CONVEXHULL_INC

#include "pointlist.h"
#include "polytype.h"
#include "psdtype.h"

namespace aiSat {
namespace psd {

enum POLY_SOS_T { NOSOS, CONVEX_POLY, EXACTLY_SOS, UNHNOW };


indice_t* overConvexHull(const indice_t* genSet, const int length,
                           const int dim, int* relength);

int reduceByLestEignV(indice_t* candidateSet, const int candLength,
                        const int dim, const indice_t* genSet,
                        const int genLength, const int max[]);


int reduceByPlane(indice_t* candidateSet, const int length, const int dim,
                    const int* coefs, int maxSum);

class ConvexGenerator {
 private:
  const Subpoly_t& subpoly;

  POLY_SOS_T checkThreeP(const int dim, PointList* ans);


 public:
  ConvexGenerator(const Subpoly_t& subp) : subpoly(subp) {}

  POLY_SOS_T easyCheck(PointList* ans);

  int onSameSurf(const int* checkPoints, const int size, indice_t* rePoints,
                 const int limt);

  indice_t* randPointSet(const int length, const int dim, const int domain);



  int reduceConvexHull(indice_t* candidateSet, const int length, const int dim,
                       const indice_t* genSet, const int lengthG);


};
}
}

#endif
