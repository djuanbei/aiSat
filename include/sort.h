/*
 * =====================================================================================
 *
 *       Filename:  sort.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/26/2013 01:37:17 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  SORT_INC
#define  SORT_INC

#include	"polytype.h"
#ifdef __cplusplus
extern "C" {
#endif
  void
  swap ( indice_t * Z, const int i, const int j, const int n  );

  void qsortT(coef_t *, indice_t *, const int n ,const  int left, const int right, int (*comp)(const indice_t *, const indice_t *,const int)); 

  void qsortM( indice_t *, const int n ,const  int left, const int right, int (*comp)(const indice_t *, const indice_t *,const int)); 

  /*-----------------------------------------------------------------------------
   *  ascending order
   *-----------------------------------------------------------------------------*/
  void sortVar ( indice_t * const  vars, const  int n );


  int compareM(const indice_t *   key1, const indice_t * varMap1, const  indice_t *   key2, const indice_t *varMap2);


#ifdef __cplusplus
}
#endif
#endif   /* ----- #ifndef SORT_INC  ----- */
