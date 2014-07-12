/*
 * =====================================================================================
 *
 *       Filename:  coeftable.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/06/2012 09:06:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#include	"pconst_table.h"
#include	"selfmemutil.h"
#include	"poly.h"
#include	"config.h"
static PolyConstraintTable table;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  initcoefTable
 *  Description:  
 * =====================================================================================
 */
void
initcoefTable ( void ){

	table.capacity=DEFAULT_COEFTABLE_SIZE;

	table.values	= malloc_d ( table.capacity*sizeof(PolyConstraint*) );

}		/* -----  end of function initcoefTable  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  enlargecoefTable
 *  Description:  
 * =====================================================================================
 */
static void
enlargecoefTable (void  ){

	table.capacity= table.capacity*ENLARGE_RAT+1;
	if (table.values==NULL){
		table.values	=(PolyConstraint**)  malloc_d (table.capacity*sizeof(PolyConstraint*) );
		
	}else{

		table.values=(PolyConstraint**) realloc_d(table.values,table.capacity* sizeof(PolyConstraint*));

	}

}		/* -----  end of function enlargecoefTable  ----- */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  addcoefElem
 *  Description:  
 * =====================================================================================
 */
int
addcoefElem ( const PolyConstraintType type, const int mId  ){

	int re;
	re=findcoefElem(type,mId);
	if(re>-1) return re;
	else if (table.capacity==table.last)
	  enlargecoefTable();

	table.values[table.last]=createPolyConstraint();
	table.values[table.last]->type=type;
    table.values[table.last]->supportId=mId;
	re=table.last;
    table.last++;
	return re;

}		/* -----  end of function addcoefElem  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  findcoefElem
 *  Description:  
 * =====================================================================================
 */
int
findcoefElem ( const PolyConstraintType type, const int mId  ){

	int i;
	for ( i = 0; i <table.last; i += 1 ) {
		if(table.values[i]->type==type&& table.values[i]->supportId==mId)
		  return i;
	}

	return -1 ;
}		/* -----  end of function findcoefElem  ----- */
