
/**
 * @file   bitmap.h
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
  struct bitmap {
  int  capacity;
  int  size;
  int *values; // values[ 2*k ]=value; values[ 2*k+1 ]= key
  int *keys;   // keys[ 2*i ]=key; keys[ 2*i+1 ]=value
};

typedef struct bitmap Bitmap;

Bitmap *createBitmap( void );

int addBitmapElem( Bitmap *, int value );

int findBitmapByKey( Bitmap *, const int key );

int findBitmapByValue( Bitmap *, const int value );

void deleteBitmap( Bitmap * );

void clearBitmap( Bitmap * );

#ifdef __cplusplus
}
#endif
#endif
