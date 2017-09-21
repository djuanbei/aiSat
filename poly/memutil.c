/*
 * =====================================================================================
 *
 *       Filename:  memutil.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  04/07/2013 05:27:41 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "selfmemutil.h"

#include "util.h"


void *malloc_d(const size_t n) {
  void *re;

  re = malloc(n);
  if (re == NULL) {
    PRINT_VALUE("dynamic memory allocation failed");
    fprintf(stderr, "\ndynamic memory allocation failed\n");
    exit(EXIT_FAILURE);
  }

  return re;

}


void *realloc_d(void *start, const size_t n) {
  void *re = realloc(start, n);
  if (re == NULL) {
    PRINT_VALUE("dynamic memory allocation failed");
    fprintf(stderr, "\ndynamic memory allocation failed\n");
    exit(EXIT_FAILURE);
  }

  return re;

}

void *calloc_d(size_t num, size_t size) {
  void *re = calloc(num, size);

  if (re == NULL) {
    PRINT_VALUE("dynamic memory allocation failed");
    fprintf(stderr, "\ndynamic memory allocation failed\n");
    exit(EXIT_FAILURE);
  }

  return re;
}
