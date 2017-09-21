
/**
 * @file   subpoly.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 13:54:28 2015
 *
 * @brief
 *
 *
 */

#ifndef SUBPOLY_INC
#define SUBPOLY_INC
#include "polytype.h"

#ifdef __cplusplus
extern "C" {
#endif

  SubPoly *createSubPolyByPoly(Poly *poly);

  SubPoly *createSubPoly(Poly *poly, const int size, int *loc);

  void printSubPoly(const SubPoly *subpoly);
  void deleteSubPoly(SubPoly *poly);

#ifdef __cplusplus
}
#endif

#endif 
