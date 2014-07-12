/*
 * =====================================================================================
 *
 *       Filename:  selfmemutil.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/07/2013 05:26:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  SELFMEMUTIL_INC
#define  SELFMEMUTIL_INC

#include	<stdio.h>
#include	<stdlib.h>
/*
 ** Make sure we can call this stuff from C++.
 */
#ifdef __cplusplus
extern "C" {
#endif

	void* malloc_d(const size_t );
	void* calloc_d(const size_t, const size_t );
	void* realloc_d(void *, const size_t );

#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif

#endif   /* ----- #ifndef MEMUTIL_INC  ----- */
