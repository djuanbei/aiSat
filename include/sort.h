/**
 * @file   sort.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 13:53:08 2015
 * 
 * @brief  
 * 
 * 
 */


#ifndef  SORT_INC
#define  SORT_INC

#include	"polytype.h"
#ifdef __cplusplus
extern "C" {
#endif

  /** 
   * swap the content of loc i and j in array Z
   * 
   * @param Z an array
   * @param i 
   * @param j 
   * @param n size of one element in Z
   */
  void  swap ( indice_t * Z, const int i, const int j, const int n  );




  /** 
   * sort tht elements in [ left, right ] array Z by ascending order
   * 
   * @param Z an array 
   * @param n the size of one element in Z
   * @param left 
   * @param right 
   * @param comp comparat funtion
   */
  void qsortM( indice_t *, const int n ,const  int left, const int right, int (*comp)(const indice_t *, const indice_t *,const int)); 



  /** 
   * 
   * sort an array Z[ left,right ] sort by ascending order. Be care that the element between C and Z is one by one. Thus, after sort the location relation
   * between   C and Z shold keep.
   * @param C 
   * @param Z an array sort by ascending order.
   * @param n the size of one element in Z
   * @param left 
   * @param right 
   * @param comp the comparable function
   */
  void qsortT(coef_t *, indice_t *, const int n ,const  int left, const int right, int (*comp)(const indice_t *, const indice_t *,const int)); 



  /** 
   * sort arrary vars by ascending order
   * 
   * @param vars 
   * @param n 
   */
  void sortVar ( indice_t * const  vars, const  int n );

  /** 
   * 
   * 
   * @param key1 an exponent of one monomial
   * @param varMap1 the variables corresponding key1
   * @param key2 an exponent of one monomial
   * @param varMap2 the variables corresponding key1
   * 
   * @return 1 if key1> key2 by alphabeta order
   -1 if key1< key2 by alphabeta order
   0 otherwise
  */
  int compareM(const indice_t *   key1, const indice_t * varMap1, const  indice_t *   key2, const indice_t *varMap2);


#ifdef __cplusplus
}
#endif
#endif   /* ----- #ifndef SORT_INC  ----- */
