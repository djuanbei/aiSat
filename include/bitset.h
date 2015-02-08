
/**
 * @file   bitset.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:24:40 2015
 * 
 * @brief  use unset int to encapsulation bit function
 * 
 * 
 */


#ifndef  BITSET_INC
#define  BITSET_INC
#include	<stdint.h>
#include	"config.h"
#include	"util.h"

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif
struct bitSet {

	uint32_t * value; 
	unsigned long length;
	unsigned cap;
	
};				/* ----------  end of struct bitSet  ---------- */

typedef struct bitSet BitSet;

BitSet * bitSet_new(void);

BitSet * bitSet_newByL(const unsigned length);

BitSet * bitSet_copy(const BitSet *);

uscalar_t bitSet_length(const BitSet *);

void bitSet_free(BitSet * );

void bitSet_clear(BitSet *);
void bitSet_add(BitSet *, const unsigned int  loc);

void bitSet_set(BitSet * , unsigned int loc, BOOL v);

BOOL bitSet_get(const BitSet *, const unsigned int loc);


#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif
#endif   /* ----- #ifndef BITSET_INC  ----- */
