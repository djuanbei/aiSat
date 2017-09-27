/*
 * =====================================================================================
 *
 *       Filename:  sparse.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012年05月11日 22时24分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "sparse.h"
#include <stdio.h>
#include <stdlib.h>

#include "polytype.h"
#include "selfmemutil.h"

#include <assert.h>

ArrangeMatrix *createSparse(const int rowLength) {
  ArrangeMatrix *re = (ArrangeMatrix *)malloc_d(sizeof(ArrangeMatrix));
  re->rowLength = rowLength;

  re->mult = 1;

  re->data = (SparseRowCol *)malloc_d(DEFAULT_NUM * sizeof(SparseRowCol));

  re->capacity = DEFAULT_NUM;

  re->size = 0;

  return re;
}

void printS(ArrangeMatrix *s) {
  int i;

  for (i = 0; i < s->size; i += 1) {
    printf("%d %d %f\n", s->data[i].row, s->data[i].col, s->mult);
  }
  printf("\n");

  return;
}

void deleteSparse(ArrangeMatrix *s) {
  free(s->data);
  free(s);
}

void addSparseElem(ArrangeMatrix *const s, int row, int col) {
  if (s->size == s->capacity) reScale(s);

  s->data[(s->size)].row = row; /* start from 0 */
  s->data[(s->size)].col = col;
  s->size++;
}

/**
 * @brief  s is the sparse matirx row is row and col is col
 *  s's contain sort by row first
 *
 * @param s
 * @param row
 * @param col
 *
 * @return
 */
coef_t getValue(ArrangeMatrix const *s, int row, int col) {
  int start = 0;
  int end = s->size;
  int mid;

  if (row < col) {  /* unit form */
    int temp = row; /* row>=col */
    row = col;
    col = temp;
  }

  while (start < end) {
    mid = (start + end) / 2;

    if (s->data[mid].row > row) {
      end = mid;
    } else if (s->data[mid].row < row) {
      start = mid + 1;
    } else if (s->data[mid].col > col) {
      end = mid;
    } else if (s->data[mid].col < col) {
      start = mid + 1;
    } else
      return s->mult;
  }

  if (s->data[start].row == row && s->data[start].col == col) {
    return s->mult;
  }

  return 0;
}

void reScale(ArrangeMatrix *const s) {
  s->capacity *= ENLARGE_RAT;

  s->data =
      (SparseRowCol *)realloc_d(s->data, (s->capacity) * sizeof(SparseRowCol));
}

void mult(ArrangeMatrix *const s, const double a) { s->mult = a; }

void print(const ArrangeMatrix *s) {
  int i;

  for (i = 0; i < s->size; i += 1) {
    printf("(%d , %d)\n", s->data[i].row, s->data[i].col);
  }
  return;
}
