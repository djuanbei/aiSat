/*
 * =====================================================================================
 *
 *       Filename:  table.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/07/2012 10:10:52 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include  "support_table.h"
#include  "poly.h"
#include  "util.h"
#include  "sparse.h"
#include "sort.h"       


static Supporttable table={0,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL };

static  void
enlargetable (void  ){

  table.capacity= table.capacity*ENLARGE_RAT+1;
  if(table.values==NULL){
    table.loc=createBimap();
    table.values=(Support** )calloc_d (
        table.capacity,sizeof(Support*) );

    table.sosSup	= (indice_t**)calloc_d (table.capacity, sizeof(indice_t*) );
    table.Gsup= (indice_t**)calloc_d (table.capacity, sizeof(indice_t*) );
    table.sosLength	=(int* ) malloc_d (table.capacity* sizeof(int) );

    table.arrangeM	= (ArrangeMatrix ***)calloc_d ( table.capacity,sizeof(ArrangeMatrix **) );
    table.gLength=(int*) malloc_d(table.capacity*sizeof(int));

  }
  else{ 
    table.values=(Support**) realloc(table.values,
                                     table.capacity*sizeof(Support*));
    memset(table.values+table.size, 0, (table.capacity-table.size)*sizeof(Support*));

    table.sosSup=(indice_t**) realloc_d(table.sosSup,
					table.capacity*sizeof(indice_t*));

    memset(table.sosSup+table.size,0, (table.capacity-table.size)*sizeof(indice_t*));
    table.Gsup=(indice_t**) realloc_d(table.Gsup,
                                      table.capacity*sizeof(indice_t*));

    memset(table.Gsup+table.size,0, (table.capacity-table.size)*sizeof(indice_t*));

    table.sosLength=(int*) realloc_d(table.sosLength,
                                     table.capacity*sizeof(int));

    table.arrangeM=(ArrangeMatrix ***)realloc_d(table.arrangeM,
                                                table.capacity*sizeof(ArrangeMatrix **));
    table.gLength=(int*) realloc_d(table.gLength,
                                   table.capacity*sizeof(int));

  }

}		


//void
// initSupTable(void  ){
//	int i;
//	table.capacity=DEFAULT_COEFMTABLE_SIZE;
//
//	table.loc=createTable();
//	table.values	= (Support**)   malloc_d (table.capacity* sizeof(Support* ) );
//
//	table.sosSup	= (indice_t**)malloc_d (table.capacity*  sizeof(indice_t*) );
//	table.Gsup=(indice_t**)malloc_d (table.capacity*  sizeof(indice_t*) );
//
//
//	table.sosLength	= (int*)malloc_d (table.capacity* sizeof(int) );
//
//	table.gLength=(int*)malloc_d(table.capacity*sizeof(int));
//
//	table.arrangeM	=(ArrangeMatrix ***) malloc_d (table.capacity* sizeof(ArrangeMatrix **) );
//
//	for ( i = 0; i < MAX_DEGREE+1; i += 1 ) {
//		table.values[i]=createSupport(i,1,0,NULL);
//		table.values[i]->id=i;
//		table.sosSup[i]=NULL;
//		table.Gsup[i]=NULL;
//		table.arrangeM[i]=NULL;
//
//	}
//	table.size=MAX_DEGREE+1;
//
//}		/* -----  end of function initcoefMTable  ----- */

static int findSupElem ( const int deg,  const int varId,const int consNum, const int * consIds ){
  int i,j;

  /* 	if(0==deg) return 0; 
   */
  /* when deg==0 the sosM is constant 1 */

  for ( i = 0; i < table.size; i += 1 ) {
    if(table.values[i]->type==NORMAL){

      if(deg==table.values[i]->deg
         &&varId== table.values[i]->varId&& 
         consNum==table.values[i]->constNum){
        j=0;
        while ( j<consNum ){
          if(consIds[j]!=table.values[i]->consId[j]) break;
          j++;
        }
        if(j==consNum){
          return findMapElemByValue(table.loc, i);
        }
      }
    }
  }

  return -1;
}

int findSupByPoly(const SubPoly * subpoly){

  int i=0;
  for ( i = 0; i < table.size; i += 1 ) {
    if(table.values[i]->type==SUB_POLY){
      if(0== memcmp(table.values[i]->md5sum,subpoly->md5sum,DIGEST_SIZE))
        return findMapElemByValue(table.loc,i);
    }
  }
  return -1;
}


static int findSupElemByIndice(const int varId, const indice_t * indices, const int size){
  int i;
  uint8_t md5sum[DIGEST_SIZE];
  md5sumbyIndice(md5sum,varId, indices, size);
  for(i=0; i<table.size;i++){
    if(table.values[i]->type==INDICE && 0==memcmp(table.values[i]->md5sum,md5sum,DIGEST_SIZE )){
      return findMapElemByValue(table.loc,i);
      
    }
  }
  return -1;
  
}

int addsosSup ( const int deg,  const int varId, const int consNum,  int * consIds ){

  int re=findSupElem(deg,  varId, consNum, consIds);

  if(re >-1) return re;

  else if (table.size==table.capacity)
    enlargetable();

  table.values[table.size]=createSupport(deg, varId, consNum, consIds);

  table.sosSup[table.size]=NULL;
  table.Gsup[table.size]=NULL;
  table.arrangeM[table.size]=NULL;
  re= table.size;

  table.size++;
  return addMapElemValue(table.loc, re);
}

int addsosSupByIndice(const int varId, indice_t * indices, const int size){

  qsortM(indices, getvarNum(varId) ,0 , size-1, compare);
  
  int re=findSupElemByIndice(varId, indices, size);
  
  if(re>-1) return re;
  else if(table.size==table.capacity)
    enlargetable();
  table.values[table.size]=createSupBySup(varId, indices, size);
  //  int varNum=getvarNum(varId);
  
  table.sosSup[table.size]=NULL;
  // (indice_t*)malloc_d(size*varNum*sizeof(indice_t) );
  
  // memcpy(table.sosSup[table.size], indices,size*varNum*sizeof(indice_t));
  
  table.Gsup[table.size]=NULL;

  table.arrangeM[table.size]=NULL;
  re=table.size;
  table.size++;
    
  return addMapElemValue(table.loc, re);
  
  // int glength=0;
  
  // ArrangeMatrix **AM=createArrangeM(re, table.sosSup[table.size], &glength ,size);
  
  // setArrangeM(re,AM,glength );
  
  //  return re;
  
  
}

Support* getSupElem (  const int id ){

  int  index=findMapElemByKey(table.loc,id);

  if(index<0) return NULL;
  return table.values[index];

}

indice_t* getsosSup ( const int id , int *length ){

  int index =findMapElemByKey(table.loc, id);
  if(index<0) return NULL;
  *length=table.sosLength[index];
  return table.sosSup[index];

}	

indice_t* getGsup ( const int id, int *length ){
  int index=findMapElemByKey(table.loc, id);
  if(index<0) return NULL;
  *length=table.gLength[index];
  return table.Gsup[index];
}		



void setsosSup (const int id,const int len, indice_t *value  ){

  int index=findMapElemByKey(table.loc, id);

  ASSERT(index>-1,"There are some bugs");
  if(index<0) return ;
  table.sosLength[index]=len;
  table.sosSup[index]=value;

}	

void setGsup ( const int id, const int len, indice_t * value ){

  int index=findMapElemByKey(table.loc,id);
  ASSERT(index>-1,"There are some bugs");
  if(index<0) return;
  table.gLength[index]=len;
  table.Gsup[index]=value;

}


int addconvexsosSup( SubPoly * poly ){

  int re=findSupByPoly(poly);
  if (re>-1) return re;
  if(table.size==table.capacity)
    enlargetable();

  table.values[table.size]=createSupByPoly(poly);
  
  
  table.sosSup[table.size]=NULL;
  table.Gsup[table.size]=NULL;
  table.arrangeM[table.size]=NULL;
  re= table.size;
  table.size++;
  return  addMapElemValue(table.loc, re);
}

int
getsosSLength ( const int id ){
  int index=findMapElemByKey(table.loc, id);
  if(index<0) return -1;
  return table.sosLength[index];
}	
void
setArrangeM( const int id, ArrangeMatrix **value, const int gLength ){

  int index= findMapElemByKey(table.loc, id);
  ASSERT(index>-1,"There are some bugs");
  if(index<0) return;
  table.gLength[index]=gLength;
  table.arrangeM[index]=value;
}	

ArrangeMatrix**
getAMIndex ( const int id,int *gLength  ){
  int index=findMapElemByKey(table.loc,id);
  if(index<0) return NULL;

  *gLength=table.gLength[index];
  return table.arrangeM[index];

}	


void clearSupportTable(void){
  if(table.values!=NULL){
    int i;
    for ( i = 0; i < table.size; i += 1 ) {
      deleteSupport(table.values[i]);
      if(table.sosSup[i]!=NULL) 
        free(table.sosSup[i]);
      if(table.Gsup[i]!=NULL)
        free(table.Gsup[i]);
      if(table.arrangeM[i]!=NULL){
        deleteSparseA(table.arrangeM[i],table.sosLength[i]);
      }
    }
    clearBimap(table.loc);
  }

  table.size=0;

}

void
deleteSupportTable(void  ){

  int i;
  for ( i = 0; i < table.size; i += 1 ) {
    deleteSupport(table.values[i]);
    if(table.sosSup[i]!=NULL) 
      free(table.sosSup[i]);
    if(table.Gsup[i]!=NULL)
      free(table.Gsup[i]);
    if(table.arrangeM[i]!=NULL){
      deleteSparseA(table.arrangeM[i],table.sosLength[i]);
    }

  }

  deleteBimap(table.loc);
  free(table.sosSup);
  free(table.Gsup);
  free(table.arrangeM);
  free(table.sosLength);
  table.capacity=0;
  table.size=0;
  free(table.values);
  table.values=NULL;

}		
