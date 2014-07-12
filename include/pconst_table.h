/*
 * =====================================================================================
 *
 *       Filename:  coeftable.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/06/2012 02:59:57 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */




#ifndef  PCONST_TABLE_INC
#define  PCONST_TABLE_INC

#include	"config.h"
#include	"polytype.h"
#ifdef __cplusplus
extern "C" {
#endif

struct polyConstraintTable {
	int capacity;
	int last;
	PolyConstraint ** values;

};				/* ----------  end of struct coefTable  ---------- */

typedef struct polyConstraintTable PolyConstraintTable;

void initcoefTable(void);

int addcoefElem(const PolyConstraintType type , const int mId );   

int findcoefElem(const PolyConstraintType type , const int mId );



#ifdef __cplusplus 
}
#endif

#endif   /* ----- #ifndef PCONST_TABLE_INC  ----- */
