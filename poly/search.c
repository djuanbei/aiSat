
/**
 * @file   search.c
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 13:57:07 2015
 * 
 * @brief  
 * 
 * 
 */

#include	"search.h"
#include	"sort.h"

/** 
 * 
 * 
 * @param key an element which been found in map
 * @param map an ascending array 
 * @param length the length of map
 * @param n the size of key
 * 
 * @return  loc if loc in [ 0, length ) if map[ loc ]==key
 * otherwise -1
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



/** 
 * 
 *  bi-search method
 * @param key the element need find
 * @param map an ascending array which used to search
 * @param length the length of map
 * @param size the key
 * 
 * @return i if   map[i-1]<=key< map[i]
 * i  if map[i] ==key
 */
int
findLocationBettwen (const indice_t  key[], const indice_t *map , const int length, const int size  ){

  int low=0;
  int high=length-1;
  int mid;
  
  while ( high>low ) {

    mid=(high+low)/2;
    
    if(compare(key,map+mid*size, size)>0){
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



/** 
 * 
 * 
 * @param sortedList  an arascending ray contain the comparable elements
 * @param key an element which need to find the location in sortedList
 * @param size the length of sortetList
 * @param NSIZE the size of key
 * @param cmp the comparable function
/
 * @param info return 0 if find k such that key==list[ k ], otherwise -1
 * 
 * @return  k in [0, size)  the return k  sortedList[k-1]<key<=sortedListp[k],  otherwise -1.
 */
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


/** 
 * 
 * 
 * @param sortedList  an arascending ray contain the comparable elements
 * @param key  an element which need to find the location in sortedList
 * @param size the length of sortetList
 * @param NSIZE the size of key
 * @param cmp the comparable function
 * 
 * @return  the return k such that
 *  sortedList[k-1]<key<=sortedListp[k],
 * otherwise -1.
 */
int bisearchLocBettwen(const void * sortedList, const  void *key, const int size , const size_t NSIZE,  int (*cmp) (const void*, const void*)){

  int low, high, mid;
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
  
  if(cmp(key, list+(size-1)*NSIZE)>0 )
    return -1;
    
  if(0==mid) return mid;
  /*-----------------------------------------------------------------------------
   *  no match
   *-----------------------------------------------------------------------------*/
  if(cond<0) return mid;
  else  return mid-1;

  return -1;
}



/** 
 * 
 * 
 * @param M an arascending ray contain the comparable elements
 * @param size the length of M
 * @param key an element which need to find the location in M
 * @param varMap1 the variables of M
 * @param varMap2  the variabvles of key
 * 
 * @return k in [ 0,  size) if M[ k ]==key,
 -1 otherwise
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
    else if (comp>0)
      high=mid;
    else
      low=mid+1;
  }
  if(size==low)return -1;

  if(compareM(M+low*varMap1[0], varMap1,key,varMap2)==0)
    return  low;

  return -1;

}	
