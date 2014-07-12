/*
 * =====================================================================================
 *
 *       Filename:  order.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/04/2012 10:06:40 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"order.h"



BOOL
indiceEqual (coef_t *a, int aSize, int *avarMap, coef_t *b, int bSize, int *bvarMap  )
{
	int i,j;
	i=j=1;
                                                /* The first index of a or b is the id location */
	while(i<=aSize &&j<=bSize){

		if(avarMap[i]<bvarMap[j]){
			if(a[i]>0) return FALSE;
			i++;

		}else if (avarMap[i]>bvarMap[j]){
			if(b[j]>0) return FALSE;
			j++;

		}
		else {
			 if(a[i]>b[j]) return FALSE;
			 else if(a[i]<b[j]) return FALSE;
			 i++;
			 j++;
		}
	}

	while(i<=aSize) if(a[i++]>0) return FALSE;
	while(j<=bSize) if(b[j++]>0) return FALSE;

	return TRUE ;


}	
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  lex
 *  Description:  Lexicographic Order
 *
 *  Input:
 *  a is the indices (a1,...,an) b is other indices (b1,...,bn)
 *  aSize if length of a and bSize is the length of b
 *  avarMap is the map of a's index to variables
 *  bvarMap is the map of b's index to variables
 *
 *  Output:
 * (a1,..an) > (b1,...,bn) if the leftmost  nozero of (a1,...,an) - (b1,...,bn) 
 * is positive return YES.
 * if (a1,...,an) == (b1,...,bn) return YES
 * otherwise return false;
 * =====================================================================================
 */
BOOL
lex (coef_t *a, int aSize, int * avarMap, coef_t *b, int bSize, int *bvarMap     )
{
	int i,j;
	i=j=1;
	
	while(i<=aSize &&j<=bSize){

		if(avarMap[i]<bvarMap[j]){
			if(a[i]>0) return TRUE;
			i++;

		}else if (avarMap[i]>bvarMap[j]){
			if(b[j]>0) return FALSE;
			j++;

		}
		else {
			 if(a[i]>b[j]) return TRUE;
			 else if(a[i]<b[j]) return FALSE;
			 i++;
			 j++;
		}
	}

	while(i<=aSize) if(a[i++]>0) return TRUE;
	while(j<=bSize) if(b[j++]>0) return FALSE;

	return TRUE ;
}	

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  grlex
 *  Description:  Graded Lex Order
 *   Input:
 *  a is the indices (a1,...,an) b is other indices (b1,...,bn)
 *  aSize if length of a and bSize is the length of b
 *  avarMap is the map of a's index to variables
 *  bvarMap is the map of b's index to variables
 *  
 *  Output:
 *  if Sum(a1,...,an)> Sum(b1,...,bn) return YES
 *  if Sum(a1,...,an) < Sum(b1,...,bn) return NO
 *  otherwise return lex(a,b)
 * =====================================================================================
 */
BOOL
grlex ( coef_t *a, int aSize, int * avarMap, coef_t *b, int bSize, int *bvarMap   )
{
	int i,j,sumA,sumB;
	sumA=sumB=0;
    i=j=1;
	for ( i = 1; i <=aSize; i += 1 ) {
		sumA+=a[i];
	}

	for ( j = 1; j <= bSize; j += 1 ) {
		sumB+=b[i];
	}
	if(sumA>sumB) return TRUE;
	else if (sumA<sumB) return FALSE;

	return lex(a, aSize, avarMap, b, bSize, bvarMap);
}	


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  grevlex
 *  Description:  Graded Reverse Lex Order
 *
 *  Input:
 *  a is the indices (a1,...,an) b is other indices (b1,...,bn)
 *  aSize if length of a and bSize is the length of b
 *  avarMap is the map of a's index to variables
 *  bvarMap is the map of b's index to variables
 *
 * Output:
 *
 *  if Sum(a1,...,an)> Sum(b1,...,bn) return YES
 *  if Sum(a1,...,an) < Sum(b1,...,bn) return NO
 *  
 *  if the rightmose nonzero enry of a-b is negative return YES
 *   otherwise return NO 
 * =====================================================================================
 */
BOOL
grevlex ( coef_t *a, int aSize, int * avarMap, coef_t *b, int bSize, int *bvarMap  )
{
	int i,j,sumA, sumB;
	sumA=sumB=0;
	i=j=1;
	for ( i = 1; i <= aSize; i += 1 ) {
		sumA+=a[i];
	}

	for ( j = 1; j <= bSize; j += 1 ) {
		sumB+=b[j];
	}
	
	if(sumA>sumB) return TRUE;
	else if (sumA<sumB) return FALSE;
	

	while(i> 0&&j>0){

		if(avarMap[i]>bvarMap[j]){
			if(a[i]>0) return TRUE;
			i--;
		}else if (avarMap[i]<bvarMap[j]){
			if(b[j]>0) return FALSE;
			j--;
		}else{
			if(a[i]>b[j]) return TRUE;
			else if (a[i]<b[j]) return FALSE;
			i--;
			j--;
		}

	}

	while(i>0) if(a[i--]>0) return TRUE;
	while(j>0) if (b[j--]>0) return FALSE;
	return TRUE;

}	
