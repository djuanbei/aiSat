/*
 * =====================================================================================
 *
 *       Filename:  myassert.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  04/22/2013 11:34:27 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "selfassert.h"

#include <stdio.h>

#include <stdlib.h>

#if defined(__WIN32)
void print_stack_trace(int fd) {}
#else
#include <execinfo.h>

void print_stack_trace(int fd) {
  void *array[256];
  size_t size;
  size = backtrace(array, 256);
  backtrace_symbols_fd(array, size, fd);
}
#endif
