
/**
 * @file   pointstack.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:52:39 2015
 *
 * @brief
 *
 *
 */

#ifndef POINTSTACK_INC
#define POINTSTACK_INC

#include "config.h"

struct pointStack {
  int capacity;
  int size;
  void **data;
  void (*delFun)(void *);
};

typedef struct pointStack PointStack;

PointStack *createStack(void (*)(void *));

void reScale_S(PointStack *);

void delStack(PointStack *);
int isEmpty_S(PointStack *);

void push_S(PointStack *, void *);

void *top_S(PointStack *);
void pop_S(PointStack *);

#endif
