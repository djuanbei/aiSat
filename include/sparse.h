/*
 * =====================================================================================
 *
 *       Filename:  sparse.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月11日 22时05分03秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dai liyun (pku), dailiyun2009@163.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  SPARSE_INC
#define  SPARSE_INC


#include	"config.h"
#include	"polytype.h"
#ifdef __cplusplus
extern "C" {
#endif


/*-----------------------------------------------------------------------------
 *  struct sparse is a special matrix have few same nonzero element
 *-----------------------------------------------------------------------------*/
struct arrangeMatrix {
	coef_t mult;                                /* coefficient of this matrix */
	int rowLength;
	int size;
	SparseRowCol *data;                                  /* start from 0  all elements of matrix is one of zero */ 
	int capacity;

};				/* ----------  end of struct sparse  ---------- */

typedef struct arrangeMatrix ArrangeMatrix;

 ArrangeMatrix* createSparse(const int rowLength);

void printS(ArrangeMatrix* s);

void deleteSparse(ArrangeMatrix *s);

void addSparseElem(ArrangeMatrix * const s, int i, int j);

coef_t getValue(ArrangeMatrix const * s,int i, int j);

void reScale(ArrangeMatrix  * const S);

void mult(ArrangeMatrix  * const S ,const double a);

void print(const ArrangeMatrix *  S);




#ifdef __cplusplus
}
#endif


#endif   /* ----- #ifndef SPARSE_INC  ----- */
