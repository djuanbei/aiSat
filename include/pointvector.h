/*
 * =====================================================================================
 *
 *       Filename:  rootvector.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/05/2013 09:51:29 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"config.h"
#include	"polytype.h"
#ifndef  POINTVECTOR_INC
#define  POINTVECTOR_INC

struct pointVector {

  pointer_t* data;
  size_t capacity;
  size_t size;
  void (*delfun)(void *);
	
};				/* ----------  end of struct rootVector  ---------- */

typedef struct pointVector PointVector;

PointVector* vector_new(void (*)(void *));

PointVector* vector_newByL( void (*)(void *) , unsigned length);

void reScale_V(PointVector *);

void vector_free(PointVector *);

void push_back_V(PointVector* , void * );

void sortVector ( PointVector * p, int (*com)(void *, void*) ) ;

size_t vector_length(const PointVector *);

void vector_set(PointVector *p, unsigned idx, void *);
void* vector_get(const PointVector *,const int n );

void insertBack_V(PointVector* lhs,const  PointVector* rhs);

void clear_V(PointVector *);

#endif   /* ----- #ifndef POINTVECTOR_INC  ----- */
