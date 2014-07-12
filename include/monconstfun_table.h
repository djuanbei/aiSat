/*
 * =====================================================================================
 *
 *       Filename:  consfuntable.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/07/2012 06:33:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */



#ifndef  MONCONSTFUN_TABLE_INC
#define  MONCONSTFUN_TABLE_INC


#include	"mono_const.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef  BOOL (*fn_ptr_t)(indice_t * a, const int varId , const int cdeg , 
			const int cvarId, const int *clinCoefs, const int cminValue ) ;

fn_ptr_t getconsFunById (const int id);


#ifdef __cplusplus 
}
#endif

#endif   /* ----- #ifndef MONCONSTFUN_TABLE_INC  ----- */

