

/**
 * @file   monconstfun_table.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 12:41:06 2015
 * 
 * @brief  
 * 
 * 
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

