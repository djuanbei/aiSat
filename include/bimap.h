/*
 * =====================================================================================
 *
 *       Filename:  table.h
 *
 *    Description:   bisearch map key<->value 
 *
 *        Version:  1.0
 *        Created:  12/22/2013 07:25:17 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#ifndef  BIMAP_INC
#define  BIMAP_INC


#include	"config.h"
struct bimap {
	int capacity;
	int size;
	int *values;
	int *keys;

};				/* ----------  end of struct table  ---------- */

typedef struct bimap Bimap;

Bimap * createBimap(void);

int addMapElemValue(Bimap * , int value);

int findMapElemByKey(Bimap *, const int key);

int findMapElemByValue(Bimap *, const int value);

void deleteBimap(Bimap *);

void clearBimap(Bimap *);


#endif   /* ----- #ifndef BIMAP_INC  ----- */
