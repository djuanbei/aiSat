/*
 * =====================================================================================
 *
 *       Filename:  pointstack.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  01/05/2013 03:00:25 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "pointstack.h"

#include <stdio.h>
#include <stdlib.h>
#include "selfmemutil.h"

PointStack *createStack(void (*del)(void *)) {
  PointStack *re;

  re = (PointStack *)malloc_d(sizeof(PointStack));

  re->capacity = DEFAULT_CAP;

  re->data = (void **)malloc_d(DEFAULT_CAP * sizeof(void *));

  re->size = -1;
  re->delFun = del;

  return re;
}

void reScale_S(PointStack *p) {
  p->capacity *= ENLARGE_RAT;
  p->capacity += 2;
  p->data = (void **)realloc(p->data, DEFAULT_CAP * sizeof(void *));
  if (p->data == NULL) {
    fprintf(stderr, "\ndynamic memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
}

void delStack(PointStack *p) {
  int i;
  for (i = 0; i < p->size; i += 1) {
    p->delFun(p->data[i]);
  }
  free(p->data);
  free(p);
}

int isEmpty_S(PointStack *p) { return p->size < 0; }

void push_S(PointStack *p, void *elem) {
  if (p->size == p->capacity) reScale_S(p);

  p->size++;
  p->data[p->size] = elem;
}

void *top_S(PointStack *p) {
  if (p->size < 0)
    return NULL;
  else
    return p->data[p->size];
}

void pop_S(PointStack *p) {
  if (p->size >= 0) p->size--;
}
