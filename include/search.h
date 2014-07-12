/*
 * =====================================================================================
 *
 *       Filename:  search.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/26/2013 01:17:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#ifndef  SEARCH_INC
#define  SEARCH_INC

#include	"util.h"

#ifdef __cplusplus
extern "C" {
#endif

	/* 
	 * ===  FUNCTION  ======================================================================
	 *         Name:  findIndex
	 *  Description:  key is the key which want to find the location in map, the length is respect to map , size if with respect to key.
	 * =====================================================================================
	 */

	int findIndex(const indice_t key[], const indice_t *map, const int length,const int size);

	int findLocation( const indice_t key[],const  indice_t *map, const int length, const int size);

	/*-----------------------------------------------------------------------------
	 *  find key in sortList if sccuess find info=0 return is the location of key 
	 *  in sortedList, otherwise -1, the return k  sortedList[k-1]<key<sortedListp[k]
	 *-----------------------------------------------------------------------------*/
	int bisearch(const void * sortedList,const  void *key, const int size , const size_t NSIZE,  int (*com) (const void*,const  void*), int *info);

	/*-----------------------------------------------------------------------------
	 *  find key in sortList if sccuess  return is the location of key 
	 *  in sortedList [0,size) , otherwise -1, the return k  sortedList[k-1]<key<=sortedListp[k]
	 *-----------------------------------------------------------------------------*/
	int bisearchLoc(const void * sortedList, const  void *key, const int size , const size_t NSIZE,  int (*cmp) (const void*, const void*));

	int findlocation(const indice_t * M, const int size, indice_t  key[] ,  indice_t * varMap1, indice_t * varMap2) ;


#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef SEARCH_INC  ----- */
