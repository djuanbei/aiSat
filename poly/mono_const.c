/*
 * =====================================================================================
 *
 *       Filename:  constraint.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/04/2012 10:04:01 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#include	"mono_const.h"
#include	"vartable.h"
#include	<stdarg.h>
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  cons
 *  Description:  
 * =====================================================================================
 */
BOOL
consEval ( indice_t *a ,  const int varId, MonomialConstraint *C )
{
  fn_ptr_t fun= getconsFunById(C->consFunId) ;
  return fun(a, varId, C->deg, C->varId , C->linCoefs, 
             C->minValue);
}		/* -----  end of function cons  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  homog
 *  Description:  
 *  Input:
 *   (a1,...,an)
 *  if Sum(a1,...,an)==deg return YES
 *  otherwise return false;
 * =====================================================================================
 */
BOOL homog (indice_t * a, const int varId , const int cdeg , const int cvarId,   const int *clinCoefs, const int cminValue      )
{
  int i=0;
  int sum=0;
  const int size=getvarNum(varId);

  for ( i = 0; i < size; i += 1 ) {
    sum+=a[i];
  }
  if(sum==cdeg) return TRUE;
  else return  FALSE;
}




BOOL Khomog (indice_t * a, const int varId , const int cdeg , const int cvarId,    const int *clinCoefs, const int cminValue    ) {
  
  int i,j,sum;
	

  const indice_t * varMap=getvarElem(varId);
  const indice_t * KvarMap = getvarElem(cvarId) ;
  const int size=(int)varMap[0] ;
  const int KvarSize=getvarNum(cvarId);
  sum=0;
  i=j=1;
  while(i<=size&&j<=KvarSize){
    if(varMap[i]<KvarMap[j]) i++;
    else if  (varMap[i] > KvarMap[j]) j++;
    else{

      sum+=a[i];
      i++;
      j++;
    }

  }
  if (cdeg==sum) return TRUE;
  else return FALSE;
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  symm
 *  Description:   every variable contain in symvarMap   
 *  has same degree
 * =====================================================================================
 */
BOOL symm (indice_t * a, const int varId , const int cdeg , const int cvarId,  const int *clinCoefs, const int cminValue ) {
  
  int i,j, value;

  const indice_t * varMap=getvarElem(varId);
  const indice_t * symvarMap=getvarElem(cvarId) ;
  const int size=(int) varMap[0];
  const int symSize= (int )symvarMap[0];

  value=-1;
  i=j=1;
  while(i<=size && j<=symSize){
    if(varMap[i]<symvarMap[j])i++;
		
    else if (varMap[i]>symvarMap[j]){

      if(-1==value)value=0;
      else if (0!=value) return FALSE;
      j++;

    }else{
      if(-1==value)value= a[i];
      else if (value!=a[i]) {
        return FALSE;
      }
      i++;
      j++;

    }
  }
  if(j!=symSize+1){
    if(-1!=value|| 0!=value )
      return FALSE;
  }	
  return TRUE ;
}


BOOL linCons (indice_t * a, const int varId , const int cdeg , const int cvarId,  const int *clinCoefs, const int cminValue  ) { 
  int i,j;
  int sum=0;
  const indice_t * varMap=getvarElem(varId);
  const indice_t * linvarMap=getvarElem(cvarId);
  const int size=(int)varMap[0];
  const int linSize=(int)linvarMap[0];

  i=j=1;    
  while(i<=size&&j<=linSize){
    if(varMap[i]<linvarMap[j]) i++;
    else if (varMap[i]>linvarMap[j]){
      j++;
    }
    else {
      sum+=clinCoefs[j]*a[i];
      i++;
      j++;
    }
  }
  /*-----------------------------------------------------------------------------
   *  sum>= minValue or sum > minValue need further consider.
   *-----------------------------------------------------------------------------*/
  if (sum>=cminValue) return TRUE;
  else return FALSE;

}
