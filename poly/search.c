/*
 * =====================================================================================
 *
 *       Filename:  search.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/26/2013 01:22:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"search.h"
#include	"sort.h"
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  findIndex
 *  Description:  bi-search method
 *  [0,length) the key are ordered
 *  return -1 denote the key doesn't in map.
 * =====================================================================================
 */
int
findIndex (const indice_t key[],const  indice_t * map, const int length, const int n )
{
  if(length<=0) return -1;

  int low=0;
  int high=length-1;
  int mid;
  int comp;
  
  while ( high>=low ) {

    mid=(high+low)/2;

    if((comp=compare(key, map+mid*n, n))<0){
      high=mid-1;
    }
    else if(comp>0){
      low=mid+1;
    }
    else {
      return mid;
    }

  }

  return -1;

}	

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  findLocation
 *  Description:  
 *  bi-search method
 *  [0,length)
 *  key is not in ascending map
 *  return i if   map[i-1]<key< map[i]
 *  
 * =====================================================================================
 */
int
findLocation (const indice_t  key[], const indice_t *map , const int length, const int size  ){

  int low=0;
  int high=length-1;
  int mid;
  int comp;
  
  while ( high>low ) {

    mid=(high+low)/2;

    
    if((comp=compare(key,map+mid*size, size))>0){
      low=mid+1;
    }
    else {
      high=mid-1;
    }
    
  }
  
  //  if(low==length) return low;

  if ( compare(map+low*size,key,size)>0 ) {
    return low;
  }else
    return low+1;

}	

/*-----------------------------------------------------------------------------
 *  find key in sortList if sccuess find info=0 return is the location of key 
 *  in ascending sortedList, [0, size)  otherwise -1, the return k  sortedList[k-1]<key<sortedListp[k]
 *-----------------------------------------------------------------------------*/
int bisearch(const void * sortedList, const  void *key, const int size , const size_t NSIZE,  int (*cmp) (const void*, const void*), int *info){

  int low,high, mid;
  low=0;
  int cond=0;
  high=size-1;
  *info=0;
  char *list=(char*) sortedList;

  while(low<=high){
    mid=(low+high)/2;
    if((cond=cmp(key, list+mid*NSIZE))<0 )
      high=mid-1;
    else if(cond>0 )
      low=mid+1;
    else return mid;
  }
  *info=-1;
  if(0==mid) return mid;
  /*-----------------------------------------------------------------------------
   *  no match
   *-----------------------------------------------------------------------------*/
  if(cond<0) return mid;
  else  return mid-1;

}

/*-----------------------------------------------------------------------------
 *  find key in sortList if sccuess  return is the location of key 
 *  in  ascending list sortedList [0,size) , otherwise -1, the return k such that
 *  ssortedList[k-1]<key<=sortedListp[k]
 *-----------------------------------------------------------------------------*/
int bisearchLoc(const void * sortedList, const  void *key, const int size , const size_t NSIZE,  int (*cmp) (const void*, const void*)){

  int low,high, mid;
  low=0;
  int cond=0;
  high=size-1;
  int *list=(int*) sortedList;
  mid=-1;
  while(low<=high){
    mid=(low+high)/2;
    if((cond=cmp(key, list+mid*NSIZE))<0 )
      high=mid-1;
    else if(cond>0 )
      low=mid+1;
    else {
      return mid;
    }
  }
  
  if(cmp(key,list+(size-1)*NSIZE)>0 )
    return -1;
    
  if(0==mid) return mid;
  /*-----------------------------------------------------------------------------
   *  no match
   *-----------------------------------------------------------------------------*/
  if(cond<0) return mid;
  else  return mid-1;


  return -1;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  contain
 *  Description: if key is not in M return -1
 *  else return the location of key in M
 *  [0, size)
 * =====================================================================================
 */
int
findlocation (const indice_t * M, const int size, indice_t key[], indice_t * varMap1,  indice_t * varMap2 ){

  if(0==size) return -1;

  int low=0;
  int high=size;
  int mid;
  int comp;

  while(high>low){

    mid=(high+low)/2;
    comp=compareM( M+mid*varMap1[0], varMap1, key, varMap2);

    if(comp==0) 
      return mid;
    else if (comp==1)
      high=mid;
    else
      low=mid+1;
  }
  if(size==low)return -1;

  if(compareM(M+low*varMap1[0], varMap1,key,varMap2)==0)
    return  low;

  return -1;

}	
