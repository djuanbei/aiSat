/*
 * =====================================================================================
 *
 *       Filename:  table.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/07/2012 04:58:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"mono_const.h"
#include	"monoconst_table.h"
#include	"selfmemutil.h"
#include	"vartable.h"
#include	"poly.h"
static MonomialConstTable table;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  enlargetable
 *  Description:  
 * =====================================================================================
 */
static void
enlargetable (void  ){
	table.capacity=table.capacity*ENLARGE_RAT+1;
	table.values=(MonomialConstraint**) realloc_d( table.values , table.capacity*
				sizeof ( MonomialConstraint* ));
	
}		/* -----  end of function enlargetable  ----- */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  initconsTable
 *  Description:  
 * =====================================================================================
 */
void
initconsTable (void  ){
	table.capacity= DEFAULT_CONS_SIZE;

	table.values	= (MonomialConstraint **) malloc_d ( table.capacity *sizeof(MonomialConstraint *) );
	table.last=0;

}		/* -----  end of function initconsTable  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  addconsElem
 *  Description:  
 * =====================================================================================
 */
int
addconsElem ( const int deg, const int varId,const int* linCoef,
			int minValue,const int consFunId  ){

	int re=findconsElem( deg, varId, linCoef, minValue, consFunId);
	if(re>-1) return re;

	else if (table.last==table.capacity)
	  enlargetable();

	table.values[table.last]= createConstraint();
	table.values[table.last]->deg=deg;
	table.values[table.last]->varId=varId;
	table.values[table.last]->minValue=minValue;
	table.values[table.last]->consFunId=consFunId;
	table.values[table.last]->id=re;

	table.last++;

	return  re;
}		/* -----  end of function addconsElem  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  findconsElem
 *  Description:  
 * =====================================================================================
 */
int
findconsElem ( const int deg, const int varId,const int* linCoef,
			int minValue,const int consFunId ){

	int i,j,num;

	for ( i = 0; i < table.last; i += 1 ) {
		if(deg==table.values[i]->deg&& varId== table.values[i]->varId 
					&& minValue==table.values[i]->minValue&&
					consFunId==table.values[i]->consFunId) {
			if(linCoef==NULL) return i;
			num= getvarNum(varId);

			for ( j = 0; j <num  ; j += 1 ) {
				if( linCoef[j]!=table.values[i]->linCoefs[j]) break;
			}
			if(j==num) return i;

		}
	}
	return -1;
}		/* -----  end of function findconsElem  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getconsElem
 *  Description:  
 * =====================================================================================
 */
MonomialConstraint*
getconsElem ( const int id ){

	if(id>table.last-1) return NULL;
	return table.values[id];
}		/* -----  end of function getconsElem  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  deleteconsTable
 *  Description:  
 * =====================================================================================
 */
void
deleteconsTable ( void ){

	int i;
	for ( i = 0; i < table.last; i += 1 ) {
		deleteConstraint(table.values[i]); 
	}
	table.capacity=0;
	table.last=0;
	free(table.values);
	table.values=NULL;

}		/* -----  end of function deleteconsTable  ----- */
