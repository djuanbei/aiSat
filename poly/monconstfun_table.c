/*
 * =====================================================================================
 *
 *       Filename:  consfuntable.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/07/2012 06:38:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#include	"monconstfun_table.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getconsFunById
 *  Description:  
 * =====================================================================================
 */

fn_ptr_t
getconsFunById ( const int id ){

	switch(id){
		case 1:
			return homog ;
		case 2:
			return  Khomog;
		case 3:
			return symm ;
		case 4:
			return linCons;

	}
	return NULL;

}		/* -----  end of function getconsFunById  ----- */
