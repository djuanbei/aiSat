/*
 * =====================================================================================
 *
 *       Filename:  table.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/22/2013 07:29:51 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"bimap.h"
#include "search.h"

#include	"selfmemutil.h"

Bimap * createBimap(void){
  Bimap *re=malloc_d(sizeof(Bimap));
  re->capacity=DEFAULT_CAP;
  re->values=malloc_d(2*re->capacity*sizeof(int));
  re->keys=malloc_d(2*re->capacity*sizeof(int));
  re->size=0;
  return re;
}
static int compfun( const void* lhs, const void*rhs){
  return ((int *) lhs)[0] -((int *) rhs)[0];
}

static void enlargeBimap(Bimap * table){
  table->capacity= table->capacity*ENLARGE_RAT+1;
  table->values=(int*) realloc_d(table->values,2*table->capacity* sizeof(int));
}

int addMapElemValue(Bimap *table , int value){


  if(table->size+2>=table->capacity)
    enlargeBimap(table);


  if(0==table->size){
    table->values[0]=value;
    table->values[1]=table->size;
    table->keys[0]=0;
    table->keys[1]=value;
    table->size++;
    return 0;

  }
  int loc=bisearchLoc(table->values, &value,table->size,2,compfun);
  if(loc<0){

    if(table->values[0]>value){
      memmove(table->values+2,table->values,2*table->size*sizeof(int));
      table->values[0]=value;
      table->values[1]=table->size;

    }else{
      table->values[2*table->size]=value;
      table->values[2*table->size+1]=table->size;
    }

  }else if (table->values[2*loc]==value){
    return table->values[2*loc+1];
  }else{
    memmove(table->values+2*loc+2,table->values+2*loc,2*(table->size-loc)*sizeof(int));
    table->values[2*loc]=value;
    table->values[2*loc+1]=table->size;

  }

  table->keys[2*table->size]=table->size;
  table->keys[2*table->size+1]=value;
  table->size++;
  return table->size-1;

}
/*
 * find return >=0
 * otherwise -1
 */

int findMapElemByKey(Bimap *table, const int key){
  int loc=bisearchLoc(table->keys, &key,table->size,2,compfun);
  if(loc<0) return -1;
  if(table->keys[2*loc]!=key) return -1;
  return table->keys[2*loc];

}

int findMapElemByValue(Bimap *table, const int value){

  int loc=bisearchLoc(table->values, &value,table->size,2,compfun);
  if(loc<0 || table->values[2*loc]!=value) return -1;
  return table->values[2*loc+1];

}


void deleteBimap(Bimap *table){
  if(NULL!=table){
    free(table->values);
    free(table->keys);
    free(table);
    table=NULL;
  }
}
void clearBimap(Bimap *table){
  table->size=0;
}

