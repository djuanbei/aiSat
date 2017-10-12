
/**
 * @file   bimap.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:21:47 2015
 *
 * @brief  a map which can search by both key and value
 *
 *
 */

#ifndef BIMAP_INC
#define BIMAP_INC

#include "config.h"
#ifdef __cplusplus
extern "C" {
#endif
struct bimap {
  int capacity;
  int size;
  int *values;  // values[ 2*k ]=value; values[ 2*k+1 ]= key
  int *keys;    // keys[ 2*i ]=key; keys[ 2*i+1 ]=value
};

typedef struct bimap Bimap;

Bimap *createBimap(void);

int addBimapElem(Bimap *, int value);

int findBimapByKey(Bimap *, const int key);

int findBimapByValue(Bimap *, const int value);

void deleteBimap(Bimap *);

void clearBimap(Bimap *);

#ifdef __cplusplus
}
#endif
#endif
