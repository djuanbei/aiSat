/*
 * =====================================================================================
 *
 *       Filename:  varmap.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  12/05/2012 08:13:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include    "selfmemutil.h"
#include    <stdlib.h>
#include    <stdio.h>
#include    "vartable.h"
#include    "selfassert.h"
#include     <string.h>
static VarTable table={0,NULL,0,0,0,NULL,NULL,0,0};
static int INIT=FALSE;

static const int var_size=MAX_VAR_NAME_LENGTH+1;


static void enlargevarTable (void  ){

  table.capacity= table.capacity *ENLARGE_RAT+1;
  table.mapCap=table.mapCap*ENLARGE_RAT+1;
  
  if(table.values==NULL){

    table.values    =(indice_t*) malloc_d ( table.capacity*sizeof(indice_t) );
    table.locMap= (int*) malloc_d(table.mapCap *sizeof(int) );
  }
  else{
    table.values=(indice_t*) realloc_d(table.values,(size_t)table.capacity*sizeof(indice_t));
    table.locMap=(int*) realloc_d (table.locMap, (size_t) table.mapCap*sizeof(int));
  }

}

void defaultVarTable(void ){
  
  int n=DEFAULT_VARTABLE_NUM;

  if(FALSE==INIT){
    INIT=TRUE;
  }else{
    //		deletevarTable();
    if (table.varCap<n)
      resetvarTable(n);
    return ;
  }

  table.varNum=0;
  table.varCap=n;

  table.varName   =(char*) malloc_d (n*var_size*sizeof(char) );
  table.capacity=DEFAULT_VARTABLE_SIZE+n;

  table.values  = (indice_t *)malloc_d (table.capacity*sizeof(indice_t ) );

  table.mapCap=DEFAULT_VARTABLE_MAP_CAP+2;
  table.locMap=(int*) malloc_d(table.mapCap*sizeof(int) );

  // int i;

  // for ( i = 0; i < n; i += 1 ) {

  //   table.varName[i*var_size]='a'+i%26;
  //   table.varName[i*var_size+1]='a'+(i/26)%26;
  //   table.varName[i*var_size+2]='\0';
  //   table.values[i+2]=i;
  // }



  table.values[0]=0;                              /* const indices */
  table.values[1]=0;
  
  table.last=2;
  

  table.locMap[0]=0;
  table.locMap[1]=1;
  
  table.mapLast=2;
}


void initvarTable ( const int k ){
  
  int n=1.2*k+10;

  if(FALSE==INIT){
    INIT=TRUE;
  }else{
    //		deletevarTable();
    if (table.varCap<n)
      resetvarTable(n);
    return ;
  }
  
  int i;

  table.varNum=k;
  table.varCap=n;

  table.varName   =(char*) malloc_d (n*var_size*sizeof(char) );

  table.capacity=DEFAULT_VARTABLE_SIZE+n;

  table.values  = (indice_t *)malloc_d (table.capacity*sizeof(indice_t ) );

  table.mapCap=DEFAULT_VARTABLE_MAP_CAP+2;
  table.locMap=(int*) malloc_d(table.mapCap*sizeof(int) );


  table.values[0]=0;                              /* const indices */

  table.values[1]=k;

  for ( i = 0; i < k; i += 1 ) {
    table.varName[i*var_size]='a'+i%26;
    table.varName[i*var_size+1]='a'+(i/26)%26;
    table.varName[i*var_size+2]='@'+(i/26)%26;
    table.varName[i*var_size+3]='\0';
    
    table.values[i+2]=i;
  }
  
  table.last=2+k;
  
  table.locMap[0]=0;
  table.locMap[1]=1;
  table.mapLast=2;
}

BOOL addVar(const char *name  ){
  
  int i;
  for(i=0;i<table.varNum;i++){
    if(strcmp(name,table.varName+i*var_size)==0) return FALSE;
  }

  if(table.varNum>=table.varCap){
    table.varCap*=ENLARGE_RAT;
    table.varCap++;
    table.varName=(char*)realloc_d(table.varName,table.varCap*var_size*sizeof(char));
  }

  memcpy(table.varName+table.varNum*var_size, name, (strlen(name)+1)*sizeof(char));
  table.varNum++;
  
  while (table.last +2>= table.capacity || table.mapLast>= table.mapCap) 
    enlargevarTable();
  
  memmove(table.values+table.varNum+2,table.values+table.varNum+1,table.last-table.varNum-1);
  
  for(i=2;i<table.mapLast;i++){
    table.locMap[i]++;
  }
  table.values[1]=table.varNum;
  table.values[table.varNum+1]=table.varNum-1;
  table.last++;
  
  return TRUE;
    

}

int findVarIndex(const char *name ){
  int i;
  for(i=0;i<table.varNum;i++){
    if(strcmp(name,table.varName+i*var_size)==0) return i;//start from 0
  }
  return -1;
  
}


void resetvarTable ( const int n ){

  if(TRUE==INIT){
    int i;

    clearvarTable();

    if(n>table.varCap){
      table.varName=(char *) realloc_d(table.varName, n*var_size*sizeof(char));
      //      memset(table.varName+table.varNum*var_size ,0, (n-table.varNum)*var_size*sizeof(char));
      table.capacity+=n;
      table.values=(indice_t*) realloc_d(table.values, table.capacity*sizeof(indice_t));

    }
    table.varNum=n;
    table.values[0]=0;                              /* const indices */
    table.values[1]=n;

    for ( i = 0; i < n; i += 1 ) {
      table.varName[i*var_size]='a'+i%26;
      table.varName[i*var_size+1]='a'+(i/26)%26;
      table.varName[i*var_size+2]='@'+(i/26)%26;
      table.varName[i*var_size+3]='\0';
 
      table.values[i+2]=i;
    }
    
    table.last=2+n;
    table.mapLast=2;
    table.locMap[0]=0;
    table.locMap[1]=1;
    table.mapLast=2;

  }else{
    initvarTable(n);

  }
}  


void setvarName (const int index, const char *name  ){

  ASSERT(index<table.varNum,"overflow");
  ASSERT(strlen(name)<=MAX_VAR_NAME_LENGTH,"the length of variable is too large");
  memcpy(table.varName+index*var_size,name,(strlen(name)+1)*sizeof(char));


}  

char* getvarName (const  int  index){

  ASSERT(index<table.varNum,"" );

  return table.varName+index*var_size ;
} 

int findvarElem (const indice_t * vars, const int  n ){

  int i;
  int j;
  int k;
  int step;

  i=0;
  while(i<table.mapLast){
    k= table.locMap[i];
    step=(int) table.values[k];
    if(n==step){
      for ( j = 0; j < n; j += 1 ) {

        if(table.values[i+j+1]!=vars[j]) break;
      }
      if(j==n) return i;
    }
    i++;        
    
  }

  // while (  i < table.last  ) {

  //   step=(int)table.values[i];

  //   if(n==step){
  //     for ( j = 0; j < n; j += 1 ) {

  //       if(table.values[i+j+1]!=vars[j]) break;
  //     }
  //     if(j==n) return i;
  //   }
  //   i+=step+1;
  // }
  
  return -1;
} 


static int
varContain ( const int sId, const int bId ){

  int re=0;
  int i,j;
  indice_t * svars=getvarElem(sId);
  indice_t * bvars=getvarElem(bId);

  if(svars[0]<=bvars[0]) {
    i=j=0;
    while(i<(int)svars[0]&&j<(int)bvars[0]){
      if(bvars[j+1]<svars[i+1]) j++;
      else if ( bvars[j+1]==svars[i+1]){
        j++;
        i++;
      }
      else return re;
    }
    if(i<(int)svars[0]) return re;          /* svars has more variables than bvars */

  }

  return 1 ;
}  

  

int addvarElem ( const indice_t * vars, const int n ){

  if(0==n) return 0;
  ASSERT(n<=table.varNum,"");
  //  ASSERT(vars[0]>0,"");
  int re;
  int i;

  re=findvarElem(vars, n);
  if(re>-1) return re;

  while(table.last+n+2>=table.capacity || table.mapLast>=table.mapCap)
    enlargevarTable();
  
  table.values[table.last]=n;

  for ( i = 0; i < n; i += 1 ) {
    ASSERT(vars[i]<table.varNum,"");
    table.values[table.last+i+1]=vars[i];
  }

  table.locMap[table.mapLast++]=table.last;
  table.last+=(n+1);
  
  return table.mapLast-1;
} 


indice_t *
getvarElem ( const int id ){
  if(id>=table.mapLast) return NULL;

  //  if(id>table.last-2) return NULL;             /* an easy method to chech id extend bound */
  
  return table.values+table.locMap[id];

}  


int
mergeVar (const int id1, const int id2  ){

  int i,j,k;
  i=j=1;
  k=0;
  if(id1==id2) return id1;

  const indice_t * var1=getvarElem(id1);
  const indice_t *var2=getvarElem(id2);
  indice_t var[var1[0]+var2[0]];


  while(i<=var1[0]&&j<=var2[0]){
    if(var1[i]<var2[j]) var[k]=var1[i++];
    else if (var1[i]>var2[j]) var[k]=var2[j++];
    else {
      var[k]=var1[i];
      i++;
      j++;

    }
    k++;
  }
  while(i<=var1[0]) var[k++]=var1[i++];
  while(j<=var2[0]) var[k++]=var2[j++];

  return  addvarElem(var, k) ;
}  
void
getconvertMap ( const int oId, const int nId, int map[]  ){


  if(oId!=0)ASSERT(varContain(oId,nId),"" );

  int i,j;

  indice_t* ovars=getvarElem(oId);
  indice_t* nvars=getvarElem(nId);
  const int oSize=(int) ovars[0];
  const int nSize=(int) nvars[0];
  if(oId==nId){

    for ( i = 0; i < oSize; i += 1 ) {
      map[i]=i;
    }
    return;
  }

  i=j=1;

  while(i<=oSize&& j<=nSize){

    if(ovars[i]<nvars[j]){
      ASSERT(0,"every variable contain in ovars must aslo contain in novars ");

      map[i-1]=-1;
      i++;
    }

    else if(ovars[i]>nvars[j]) j++;

    else{
      map[i-1]=j-1;
      i++;
      j++;
    }
  }
  ASSERT(i>oSize,"");
} 

int getvarNum ( const int id ){
  if(id>=table.mapLast) return -1;

  //  if(id>table.last-2) return -1;             /* an easy method to chech id extend bound */
  return (int)table.values[table.locMap[id]] ;
} 

int getAllVarNum (void  ){
  return table.varNum;
}   

void deletevarTable ( void ){

  if(table.values!=NULL) free(table.values);
  if(table.varName!=NULL) free(table.varName);
  if(table.locMap!=NULL) free(table.locMap);
  table.varNum=0;
  table.values=NULL;
  table.capacity=0;
  table.last=0;
  table.mapLast=0;
  table.mapCap=0;
}  

void clearvarTable (void  ){
  table.last=0;
  table.mapLast=0;
}    
