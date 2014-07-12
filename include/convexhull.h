/*
 * =====================================================================================
 *
 *       Filename:  convexhull.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/28/2013 10:40:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
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
