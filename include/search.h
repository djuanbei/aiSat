
/**
 * @file   search.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:54:20 2015
 *
 * @brief
 *
 *
 */

#ifndef SEARCH_INC
#define SEARCH_INC

#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   *
   * @param key an element which been found in map
   * @param map  an ascending array
   * @param length the length of map
   * @param size the size of key
   *
   * @return  loc if loc in [ 0, length ) if map[ loc ]==key
   * otherwise -1
   */

  int findIndex(const indice_t key[], const indice_t *map, const int length,
                const int size);

  /**
   *
   *  bi-search method
   * @param key the element need find
   * @param map an ascending array which used to search
   * @param length the length of map
   * @param size the key
   *
   * @return i if   map[i-1]<=key< map[i]
   * i  if map[i] ==key
   */

  int findLocationBettwen(const indice_t key[], const indice_t *map,
                          const int length, const int size);

  /**
   *
   *
   * @param sortedList  an arascending ray contain the comparable elements
   * @param key an element which need to find the location in sortedList
   * @param size the length of sortetList
   * @param NSIZE the size of key
   * @param cmp the comparable function
   /
   * @param info return 0 if find k such that key==list[ k ], otherwise -1
   *
   * @return  k in [0, size)  the return k  sortedList[k-1]<key<=sortedListp[k],
   otherwise -1.
  */
  int bisearch(const void *sortedList, const void *key, const int size,
               const size_t NSIZE, int (*com)(const void *, const void *),
               int *info);

  /**
   *
   *
   * @param sortedList  an arascending ray contain the comparable elements
   * @param key  an element which need to find the location in sortedList
   * @param size the length of sortetList
   * @param NSIZE the size of key
   * @param cmp the comparable function
   *
   * @return  the return k such that
   *  sortedList[k-1]<key<=sortedListp[k],
   * otherwise -1.
   */

  int bisearchLocBettwen(const void *sortedList, const void *key, const int size,
                         const size_t NSIZE,
                         int (*cmp)(const void *, const void *));

  /**
   *
   *
   * @param M an arascending ray contain the comparable elements
   * @param size the length of M
   * @param key an element which need to find the location in M
   * @param varMap1 the variables of M
   * @param varMap2  the variabvles of key
   *
   * @return k in [ 0,  size) if M[ k ]==key,
   -1 otherwise
  */
  int findlocation(const indice_t *M, const int size, indice_t key[],
                   indice_t *varMap1, indice_t *varMap2);

#ifdef __cplusplus
}
#endif

#endif 
