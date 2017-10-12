
/**
 * @file   pointvector.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:53:03 2015
 *
 * @brief
 *
 *
 */

#ifndef POINTVECTOR_INC
#define POINTVECTOR_INC
#include "config.h"
#include "extenttype.h"

struct pointVector {
  pointer_t *data;
  size_t capacity;
  size_t size;
  void (*delfun)(void *);
};

typedef struct pointVector PointVector;

PointVector *vector_new(void (*)(void *));

PointVector *vector_newByL(void (*)(void *), unsigned length);

void reScale_V(PointVector *);

void vector_free(PointVector *);

void push_back_V(PointVector *, void *);

void sortVector(PointVector *p, int (*com)(void *, void *));

size_t vector_length(const PointVector *);

void vector_set(PointVector *p, unsigned idx, void *);
void *vector_get(const PointVector *, const int n);

void insertBack_V(PointVector *lhs, const PointVector *rhs);

void clear_V(PointVector *);

#endif
