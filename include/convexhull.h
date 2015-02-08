
/**
 * @file   convexhull.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:35:53 2015
 * 
 * @brief  delete the superfluous element in candidate monomial set 
 * 
 * 
 */


#ifndef  CONVEXHULL_INC
#define  CONVEXHULL_INC

#include	"pointlist.h"
#include	"polytype.h"
#ifdef __cplusplus
extern "C" {
#endif

  enum  {
    NOSOS,
    CONVEX_POLY,
    EXACTLY_SOS,
    UNHNOW
  };				/* ----------  end of enum type  ---------- */

  int easyCheck(const SubPoly * poly, PointList *ans);

  int onSameSurf(const SubPoly * poly,const  int * checkPoints, const int size, indice_t * rePoints,   const int limt);
  indice_t * randPointSet(const int length, const int dim, const int domain );
  indice_t * overConvexHull(const indice_t * genSet, const int length, const int dim, int * relength );

  int reduceConvexHull( indice_t* candidateSet, const int length, const int dim, const indice_t * genSet, const int lengthG);



#ifdef __cplusplus 
}
#endif
#endif   /* ----- #ifndef CONVEXHULL_INC  ----- */
