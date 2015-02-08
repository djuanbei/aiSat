
/**
 * @file   support_table.c
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 15:35:45 2015
 * 
 * @brief   Since every polynomial constrain is parametric polynomial
 * when the coefficients of the template constraint is concrete the 
 * constraint is definited. And there are different polynomial contraints may share monomial base such as polynomial contraints c00+c01x+c10y+c11xy+c02x^2+c20y^2 >=0  and b00+b01x+b10y+b11xy+b02x^2+b20y^2 >=0
 * where c**, b** are unknown parameters and set {1, x, y, xy, x^2, y^2 } is the monomial base.
 * 
 *
 * f= G^T M G=<G G^T,M > = \sum_{i=1}^s g_i^2
 * G called half monomial set 
 * all the elements occouring in GG^T called all monomial set
 * we define an ArrangeMatrix by the location   x^a  occurring in  for every element x^a
 * in  GG^T. For example G=[ 1 x y z ] 
 * G G^T=[ 1  x    y    z ]
 *       [ x  x^2  xy   xz]
 *       [ y  xy   y^2  yz]
 *       [ z  xz   yz  z^2], then the ArrangeMatrix corresponding monomial xy is
 *  [0  0  0  0]
 *  [0  0  1  0]
 *  [0  1  0  0]                                                     
 *  [0  0  0  0]
 * 
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

    table.SOSsup	= (indice_t**)calloc_d (table.capacity, sizeof(indice_t*) );
    table.Gsup= (indice_t**)calloc_d (table.capacity, sizeof(indice_t*) );
    table.sosLength	=(int* ) malloc_d (table.capacity* sizeof(int) );

    table.arrangeM	= (ArrangeMatrix ***)calloc_d ( table.capacity,sizeof(ArrangeMatrix **) );
    table.gLength=(int*) malloc_d(table.capacity*sizeof(int));

  }
  else{ 
    table.values=(Support**) realloc(table.values,
                                     table.capacity*sizeof(Support*));
    memset(table.values+table.size, 0, (table.capacity-table.size)*sizeof(Support*));

    table.SOSsup=(indice_t**) realloc_d(table.SOSsup,
					table.capacity*sizeof(indice_t*));

    memset(table.SOSsup+table.size,0, (table.capacity-table.size)*sizeof(indice_t*));
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
//	table..SOSsup	= (indice_t**)malloc_d (table.capacity*  sizeof(indice_t*) );
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
//		table..SOSsup[i]=NULL;
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
          return findBimapByValue(table.loc, i);
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
        return findBimapByValue(table.loc,i);
    }
  }
  return -1;
}


static int findSupElemByIndice(const int varId, const indice_t * indices, const int size){
  int i;
  uint8_t md5sum[DIGEST_SIZE];
  md5sumbyIndice(md5sum,varId, indices, size);
  for(i=0; i<table.size;i++){
    if(table.values[i]->type==INDICE && 0==memcmp(table.values[i]->md5sum, md5sum,DIGEST_SIZE )){
      return findBimapByValue(table.loc,i);
      
    }
  }
  return -1;
  
}

int addSOSup ( const int deg,  const int varId, const int consNum,  int * consIds ){

  int re=findSupElem(deg,  varId, consNum, consIds);

  if(re >-1) return re;

  else if (table.size==table.capacity)
    enlargetable();

  table.values[table.size]=createSupport(deg, varId, consNum, consIds);

  table.SOSsup[table.size]=NULL;
  table.Gsup[table.size]=NULL;
  table.arrangeM[table.size]=NULL;
  re= table.size;

  table.size++;
  return addBimapElem(table.loc, re);
}

int addSOSsupByIndice(const int varId, indice_t * indices, const int size){

  qsortM(indices, getvarNum(varId) ,0 , size-1, compare);
  
  int re=findSupElemByIndice(varId, indices, size);
  
  if(re>-1) return re;
  else if(table.size==table.capacity)
    enlargetable();
  table.values[table.size]=createSupBySup(varId, indices, size);
  //  int varNum=getvarNum(varId);
  
  table.SOSsup[table.size]=NULL;
  // (indice_t*)malloc_d(size*varNum*sizeof(indice_t) );
  
  // memcpy(table..SOSsup[table.size], indices,size*varNum*sizeof(indice_t));
  
  table.Gsup[table.size]=NULL;

  table.arrangeM[table.size]=NULL;
  re=table.size;
  table.size++;
    
  return addBimapElem(table.loc, re);
  
  // int glength=0;
  
  // ArrangeMatrix **AM=createArrangeM(re, table..SOSsup[table.size], &glength ,size);
  
  // setArrangeM(re,AM,glength );
  
  //  return re;
  
  
}

Support* getSupElem (  const int id ){

  int  index=findBimapByKey(table.loc,id);

  if(index<0) return NULL;
  return table.values[index];

}

indice_t* getSOSsup ( const int id , int *length ){

  int index =findBimapByKey(table.loc, id);
  if(index<0) return NULL;
  *length=table.sosLength[index];

  return table.SOSsup[index];

}	

indice_t* getGsup ( const int id, int *length ){
  int index=findBimapByKey(table.loc, id);
  if(index<0) return NULL;
  
  *length=table.gLength[index];
  return table.Gsup[index];
}		



void setSOSsup (const int id,const int len, indice_t *value  ){

  int index=findBimapByKey(table.loc, id);

  ASSERT(index>-1,"There are some bugs");
  if(index<0) return ;
  table.sosLength[index]=len;
  table.SOSsup[index]=value;

}	

void setGsup ( const int id, const int len, indice_t * value ){

  int index=findBimapByKey(table.loc,id);
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
  
  
  table.SOSsup[table.size]=NULL;
  table.Gsup[table.size]=NULL;
  table.arrangeM[table.size]=NULL;
  re= table.size;
  table.size++;
  return  addBimapElem(table.loc, re);
}

int
getsosSLength ( const int id ){
  int index=findBimapByKey(table.loc, id);
  if(index<0) return -1;
  return table.sosLength[index];
}	
void
setArrangeM( const int id, ArrangeMatrix **value, const int gLength ){

  int index= findBimapByKey(table.loc, id);
  ASSERT(index>-1,"There are some bugs");
  if(index<0) return;
  table.gLength[index]=gLength;
  table.arrangeM[index]=value;
}	

ArrangeMatrix**
getAMIndex ( const int id,int *gLength  ){
  int index=findBimapByKey(table.loc,id);
  if(index<0) return NULL;

  *gLength=table.gLength[index];
  return table.arrangeM[index];

}	


void clearSupportTable(void){
  if(table.values!=NULL){
    int i;
    for ( i = 0; i < table.size; i += 1 ) {
      deleteSupport(table.values[i]);
      if(table.SOSsup[i]!=NULL) 
        free(table.SOSsup[i]);
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
    if(table.SOSsup[i]!=NULL) 
      free(table.SOSsup[i]);
    if(table.Gsup[i]!=NULL)
      free(table.Gsup[i]);
    if(table.arrangeM[i]!=NULL){
      deleteSparseA(table.arrangeM[i],table.sosLength[i]);
    }

  }

  deleteBimap(table.loc);
  free(table.SOSsup);
  free(table.Gsup);
  free(table.arrangeM);
  free(table.sosLength);
  table.capacity=0;
  table.size=0;
  free(table.values);
  table.values=NULL;

}		
