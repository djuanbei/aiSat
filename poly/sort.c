/*
 * =====================================================================================
 *
 *       Filename:  sort.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/26/2013 01:40:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"sort.h"
void swap ( indice_t * Z, const int i, const int j, const int n  )
{
  int k;
  int temp;
  int iindex=i*n;
  int jindex=j*n;

  for ( k = 0; k < n; k += 1 ) {

    temp=Z[iindex+k];
    Z[iindex+k]=Z[jindex+k];
    Z[jindex+k]=temp;
  }
}

/*
  [ left, right ]
*/
void qsortM ( indice_t *Z, const int n,const  int left, const int right , int (*comp)(const  indice_t *, const indice_t *,const  int)  )
{
  int i ,last;

  /*-----------------------------------------------------------------------------
   * do nothing fewer than two elements 
   *-----------------------------------------------------------------------------*/
  if (  left>=right ) {
    return;
  }

  swap(Z,left, (left+right)/2,n);

  last=left;
  for ( i = left+1; i <= right; i += 1 ) {

    if(comp(Z+i*n,Z+left*n,n)<0){
      swap(Z,++last,i,n);
    }
  }

  swap(Z,left,last,n);
  qsortM( Z,n,left,last-1,comp);
  qsortM( Z,n,last+1,right,comp);


}		


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  qsort compate term by the indice
 *  Description:  Using comp to sort Z from left to right
 *  sort [left, right] Z[right] with C is in the array 
 * =====================================================================================
 */
void
qsortT (coef_t * C, indice_t *Z, const int n, const  int left, const int right , int (*comp)(const  indice_t *, const indice_t *,const  int)  )
{
  int i ,last;
  coef_t tempC;

  /*-----------------------------------------------------------------------------
   * do nothing fewer than two elements 
   *-----------------------------------------------------------------------------*/
  if (  left>=right ) {
    return;
  }

  swap(Z,left, (left+right)/2,n);
  tempC=C[(left+right)/2];
  C[(left+right)/2]= C[left];
  C[left]=tempC;

  last=left;

  for ( i = left+1; i <= right; i += 1 ) {

    if((comp)(Z+i*n,Z +left*n,n)<0){

      swap(Z,++last,i,n);
      tempC=C[last];
      C[last]=C[i];
      C[i]=tempC;
    }
  }

  swap(Z,left,last,n);
  tempC=C[left];
  C[left]=C[last];
  C[last]=tempC;

  qsortT(C, Z,n,left,last-1,comp);
  qsortT(C, Z,n,last+1,right,comp);


}	

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sortVar
 *  Description:  ascending order
 * =====================================================================================
 */
void
sortVar ( indice_t * const  vars, const  int n ){
  int i,j,temp;

  for ( i = 0; i < n-1; i += 1 ) {
    for ( j = i+1; j < n; j += 1 ) {

      if( vars[j]<vars[i]){
        temp=vars[j];
        vars[j]=vars[i];
        vars[i]=temp;
      }
    }

  }
}		
int
compareM ( const indice_t *  key1 , const indice_t *varMap1,
           const  indice_t * key2, const indice_t *varMap2 ){

  int i,j,s1,s2;
  s1=(int)varMap1[0];
  s2=(int)varMap2[0];
  i=j=0;
  while(i<s1&&j<s2){
    if(varMap1[i+1]<varMap2[j+1]){
      if(key1[i]>0) return 1;
      i++;
    }
    else if (varMap1[i+1]> varMap2[j+1]){
      if(key2[j]>0) return -1;
      j++;

    }
    else{
      if(key1[i]>key2[j])
        return 1;
      else if (key1[i]<key2[j])
        return -1;
      else{
        i++;
        j++;
      }
    }
  }

  while(i<s1) if (key1[i++]>0) return 1;
  while(j<s2) if (key2[j++]>0) return -1;
  return 0;

}	
