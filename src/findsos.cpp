/*
 * =====================================================================================
 *
 *       Filename:  convsos.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/15/2013 11:28:50 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "findsos.h"

#include "polyissos.h"
#include <cmath>
#include "sdpsolver.h"
#include "util.h"
#include "poly.h"
#include "selfassert.h"
#include "polyissos.h"
#include "convexhull.h"
#include "subpoly.h"
#include "sort.h"
#include "search.h"
#include <map>

#include "pointlist.h"
#include "monomial.h"
#include "iostream"


using std::map;
using std::cout;
using std::endl;

extern int csdp_verbose;

struct params{
  double mineignvalue;
  float printMinValue;
  params():mineignvalue(1e-2), printMinValue(1e-2){

  }
};

static bool polyIsSOS( SubPoly * subpoly, PointList *ans, const int verbose=0  );

static params par;
static clock_t start;

static int DIM;


enum  {
  SUCCESS=1,
  FAILURE=2,
  OTHER=3
};				

static void delpoly(void *p ){
  deletePoly((Poly*)p);
}

static void sosPresent(const SubPoly * subpoly, const PointList *sosList, const double printMin){
  printSubPoly(subpoly);
  printf ( "=\n" );
  PointElem *temp=sosList->head;
  while(temp->next!=NULL){
    Poly * tp=(Poly*)temp->value;
    if(tp->size>0){
      printf ( "(" );
      printPolyPrec(tp,printMin);
      temp=temp->next;
      printf ( ")^2+" );
    }
  }

  Poly * tp=(Poly*)temp->value;
  if(tp->size>0){
    printf ( "(" );
    printPolyPrec(tp,printMin);
    printf ( ")^2\n" );
  }
}


static int overConvexChecking(const SubPoly * subpoly, indice_t* indices, const int loc[], const int size, const map<monomial, monomialvalue> &monMap,  PointList *ans );



static int exactConvHull(const SubPoly *subpoly, indice_t * candidateSet, int &candidateLength, const indice_t * genSet, const int genLength, indice_t **SOSM, int &sosLength, PointList *ans){

  int i,j,valueIndex,v, m;
  int k;
  int currCandLen=candidateLength;
  int lastre=-1;
  int size=subpoly->size;
  int *loc=subpoly->locs;
  indice_t* indices=subpoly->poly->indices;

  // uint32_t vertexSet[4];
  // vertexSet[0]=0;
  // vertexSet[1]=0;
  // vertexSet[2]=0;
  // vertexSet[3]=0;
  size_t node_size=DIM*sizeof(indice_t);

  indice_t * key=new indice_t[DIM];
  indice_t * key1=new indice_t[DIM];

  map<monomial, monomialvalue> monMap;
  indice_t *indiceValues=new indice_t[candidateLength*candidateLength*DIM];

  valueIndex=0;
  for ( i = 0; i < genLength; i += 1 ) {
    for ( j = 0; j < DIM; j += 1 ) {
      key[j]=2*genSet[i*DIM+j];
    }
    memcpy(indiceValues+valueIndex*DIM,key,node_size);
    monomialvalue dummy;
    dummy.value=1;
    monMap[monomial(indiceValues+valueIndex*DIM,DIM)]=dummy;
    valueIndex++;
  }

  for ( i = 0; i < currCandLen; i += 1 ) {

    for ( j = i; j < currCandLen; j += 1 ) {

      for ( k = 0; k < DIM; k += 1 ) {
        key[k]=candidateSet[i*DIM+k]+candidateSet[j*DIM+k];
      }
      if(monMap.find(monomial(key,DIM))!=monMap.end()){
        monMap[monomial(key,DIM)].value++;
        //=monMap[key,DIM)]+1;
      }else{
        memcpy(indiceValues+valueIndex*DIM,key,node_size);
        monomialvalue dummy;
        dummy.value=1;
        monMap[monomial(indiceValues+valueIndex*DIM,DIM)]=dummy;
        valueIndex++;
      }

    }
  }

  //	for(open_hash_map<nominal,int>::const_iterator it=monMap.begin();it!=monMap.end();++it){
  //		cout << it.key() << " => " << it.value() << '\n';
  //	}

  /*-----------------------------------------------------------------------------
   *  delete some monomial in monMap which value equal to 1 and the value with relate with this monomial,
   *  itertation delete monomial in monMap
   *-----------------------------------------------------------------------------*/

  while(lastre!=currCandLen){
    lastre=currCandLen;

    for ( i = 0; i < currCandLen; i += 1 ) {

      for ( j = 0; j < DIM; j += 1 ) {
        key[j]=2*candidateSet[i*DIM+j];
      }
      if(monMap[monomial(key,DIM)].value==1){                 /* expect 2*candidateSet[i*DIM+j], no other combines
                                             of monomial has value key*/
        for ( j = 0; j < currCandLen; j += 1 ) {
          for ( k = 0; k < DIM; k += 1 ) {
            key1[k]=candidateSet[i*DIM+k]+candidateSet[j*DIM+k];
          }
          monMap[monomial(key1,DIM)].value--;
          

        }
        monMap.erase(monomial(key,DIM));
        if(i+1<currCandLen)
          memmove(candidateSet+i*DIM, candidateSet+(i+1)*DIM, (currCandLen-i-1)*node_size);

        i--;
        currCandLen--;
      }
    }

  }

  candidateLength=currCandLen;
  if(NULL!=ans)
    cout	<< "Constructing G use time: " << (clock()-start-0.0)/CLOCKS_PER_SEC<<endl;
  
  sosLength=0;


  for(map<monomial,monomialvalue>::const_iterator it=monMap.begin();it!=monMap.end();++it){
    if(it->second.value>0) sosLength++;
  }

  //  open_hash_map<monomial,int>::const_iterator it=monMap.begin();
  // which(it!=monMap.end()){
  //   if(it.value()==0)it= monMap.erase(it.key());
  //   else it++;
  // }
  for(map<monomial,monomialvalue>::iterator it=monMap.begin();it!=monMap.end();++it){
    if(it->second.value==0) monMap.erase(it);
    if(it==monMap.end()) break;
  }

  for ( i = 0; i < genLength; i += 1 ) {

    for ( j = 0; j < DIM; j += 1 ) {
      key[j]=2*genSet[i*DIM+j];
    }
    monMap[monomial(key,DIM)].value--;
    
  }

  k=0;

  for ( j = 0; j < genLength; j += 1 ) {

    for ( i = 0; i < DIM; i += 1 ) {
      key[i]=2*genSet[j*DIM+i];
    }
    v=monMap[monomial(key,DIM)].value;

    if(v==1) {
      for ( i = 0; i < currCandLen; i += 1 ) {

        for ( m = 0; m < DIM; m += 1 ) {
          key1[m]=key[m]/2+candidateSet[i*DIM+m];
        }
        map<monomial,monomialvalue>::iterator it=monMap.find(monomial(key1,DIM));
        
        it->second.add(k);
                                                   
      }


      k++;
      monMap.find(monomial(key,DIM))->second.change=false;
    }

  }
  ASSERT(k<=VERTEX_BOUND,"");
  //  int vertex_num=k;/* the number of vertex */

  bool run=true;

  while(run) {

    run=false;
    for ( i = 0; i < currCandLen; i += 1 ) {

      for ( j = 0; j < DIM; j += 1 ) {
        key[j]=2*candidateSet[i*DIM+j];
      }
      if(monMap.find(monomial(key,DIM))->second.change){
        monMap.find(monomial(key,DIM))->second.change=false;

        for ( j = 0; j < currCandLen; j += 1 ) {

          for ( m = 0; m < DIM; m += 1 ) {
            key1[m]=key[m]/2+candidateSet[j*DIM+m];
          }
          if(!(monMap.find(monomial(key1,DIM))->second.contain(monMap.find(monomial(key,DIM))->second))){
            monMap.find(monomial(key1,DIM))->second.add(monMap.find(monomial(key,DIM))->second);
            run=true;
          }
        }
      }

    }

  }
  delete[] key;
  delete[] key1;


  /*-----------------------------------------------------------------------------
   *  necessary check
   *-----------------------------------------------------------------------------*/
  if(monMap.find(monomial( indices+loc[0]*DIM,DIM))==monMap.end()){
    if(NULL!=ans)
      cout<<  "Corollary 3" <<endl;
    delete[] indiceValues;
    return FAILURE;
  }

  for ( i = 1; i < size; i += 1 ) {

    if(monMap.find(monomial(indices+loc[i]*DIM,DIM))==monMap.end()){
      if(NULL!=ans)
        cout<< "Corollary 3"<<endl;
      delete[] indiceValues;
      return FAILURE;
    }
  }
  int dummy=overConvexChecking(subpoly, indices, loc, size, monMap, ans);
  if(SUCCESS==dummy){
    delete[] indiceValues;
    return SUCCESS;

  }else if(FAILURE==dummy){
    delete[] indiceValues;
    return FAILURE;

  }

  *SOSM=(indice_t*)malloc_d(sosLength*node_size);

  i=0;

  for(map<monomial,monomialvalue>::const_iterator it=monMap.begin();it!=monMap.end();++it){
    if(it->second.value>0) {
      memcpy((*SOSM)+i*DIM, it->first.indice ,node_size);
      i++;
    }
  }

  delete[] indiceValues;
  return OTHER;

}




/*-----------------------------------------------------------------------------
 *  varId is the var id of polynomial we want to write as
 *  sums of  sos polynomials.
 *  td is the total degree of poly we want check
 *
 *  <numofCons>: is the number of monomials whose total degree less or equal to poly's 
 *  total degree
 *  <blockSize>: the nchoosek(td(poly)/2+varnums(poly),varnums(poly))
 *  <b>: is the array of coefs of poly.
 *-----------------------------------------------------------------------------*/

static Constraintmatrix * createConstraintMatrix(const SubPoly *subpoly, const int sosId,  double ** b, const  indice_t* SOSM, const int sosLength,  const int gLength ){

  int i,k,tempI;
  const int varNum=getvarNum(subpoly->poly->varId);
  //	const int totalDegree=poly_getTotalDegree(poly);
  int size=subpoly->size;
  int *loc=subpoly->locs;
  indice_t * indices= subpoly->poly->indices;
  coef_t * coef=subpoly->poly->coef;

  Sparseblock *blockptr;

  printf ( "Matrix's size %d,  Constraints size: %d,  G size:  %d\n",gLength*gLength, sosLength,gLength );

  int temploc;
  *b=(double*)calloc_d((sosLength+1),sizeof(double));

  /* 	for ( i = 0; i < *numofCons+1; i += 1 ) {
   * 		(*b)[i]=0;
   * 	}
   */

  //	indice_t key[MAX_VAR_NUM];
  for ( i = 0; i < size; i += 1 ) {

    //		cout	<< "vertex num %" <<VERTEX_NUM<<" "<<monMap.find(indices+loc[i]*varNum).key().size()<< endl;

    //		memcpy(key,indices+loc[i]*varNum,varNum);
    //		for ( j = 0; j < varNum; j += 1 ) {
    //			key[j]=indices[loc[i]*varNum+j];
    //		}
    temploc=findIndex(indices+loc[i]*varNum,SOSM, sosLength, varNum);

    //		ASSERT(temploc>-1,"");
    if(temploc<0) {
      return NULL;
    }
    /* b index start form 1 */
    (*b)[temploc+1]=coef[loc[i]];
    /* printf(" %d %f",loc, poly->coef[i]);  */
  }


  ArrangeMatrix **AM=getAMIndex(sosId, &temploc);
  if(NULL==AM){ 
    AM=createArrangeM(sosId, SOSM, &temploc ,sosLength);
    //    setArrangeM(sosId, AM, temploc );
  }


  Constraintmatrix *re=createNconstraintmatrix(sosLength); /* every monomial w.r.t. to a constraint */

  /*     block=(SparseSOSblock**) malloc_d ((*numofCons)*sizeof(SparseSOSblock *));
   * 
   *     for ( i = 0; i < *numofCons; i += 1 ) {
   *         block[i]=createSparseSOSblock();
   *     }
   * 
   *     for ( j = 0; j < lengthM; j += 1 ) {
   *         addSparse(block[j],j,1);
   *     }
   */

  /*-----------------------------------------------------------------------------
   *  use block to  construct  constraints
   *-----------------------------------------------------------------------------*/

  for ( i = 0; i < sosLength; i += 1 ) {

    /* 		sum=0;
     * 
     * 		for ( k = 0; k < block[i]->size; k += 1 ) {
     * 			sum+=W[block[i]->index[k]]->size;
     * 		}
     */
    /* assert(sum==1); */

    /* printf("b size %d",*blockSize);; */

    blockptr=createSparseblock(1,gLength,i+1,AM[i]->size);
    tempI=1;
    for ( k = 0; k < AM[i]->size; k += 1 ) {
      blockptr->iindices[tempI]=AM[i]->data[k].row+1;
      blockptr->jindices[tempI]=AM[i]->data[k].col+1;
      blockptr->entries[tempI]=1;
      tempI++;
    }
    frontInsertBlock(re+i+1,blockptr);

  }
  /* 
   * 	for ( i = 0; i < *numofCons; i += 1 ) {
   * 
   * 		deleteSparseSOSblock(block[i]);
   * 	}
   * 	free(block);
   */

  return re;
}


static int overConvexChecking(const SubPoly * subpoly, indice_t* indices, const int loc[], const int size, const map<monomial, monomialvalue> &monMap, PointList *ans){

  //			printSubPoly(tempSub);
  int i,j,num,k;

  int *check =new int[size];
  int checkSize=size;
  int *choose=new int[size];
  int *checked=new int[size];
  int *tempLoc=new int[size];
  //  const int randTime=vertex_num+DIM;
  uint32_t set[4];

  for ( i = 0; i < checkSize; i += 1 ) {
    check[i]=i;
  }
  i=0;
  memset(checked,0,size*sizeof(int));
  while(i<checkSize){
    if(!checked[check[i]]){
      set[0]=0u;
      set[1]=0u;
      set[2]=0u;
      set[3]=0u;
      memset(choose,0, checkSize*sizeof(int));
      monomialvalue curr=monMap.find(monomial(indices+loc[check[i]]*DIM,DIM))->second;
      num=0;
      monomialvalue::add(set,curr);
      checked[check[i]]=1;
      tempLoc[num++]=loc[check[i]];
      choose[i]=1;
      int tempNum=0;
      while(tempNum!=num){
        tempNum=num;
        for ( j = 0; j < checkSize; j += 1 ) {
          monomialvalue temp=monMap.find(monomial( indices+loc[check[j]]*DIM,DIM))->second;
          if(monomialvalue::conjunction(set,temp)&& !choose[j]){
            monomialvalue::add(set,temp);
            checked[check[j]]=1;
            tempLoc[num++]=loc[check[j]];
            choose[j]=1;
          }
        }
      }

      if(num<size){
        if(NULL!=ans)
          cout	<< "found a split poly" << endl;
        SubPoly * tempSub=createSubPoly(subpoly->poly,  num, tempLoc);
        int preAnsLength=0;
        if(NULL!=ans)
          preAnsLength=size_L(ans);
        if(!polyIsSOS(tempSub, ans)){            /* necessary condition */
          if(NULL!=ans)
            delListStart(ans, preAnsLength);
          delete [] check;
          delete [] checked;
          delete [] choose;
          delete [] tempLoc;
          deleteSubPoly(tempSub);
          return FAILURE;
        }

        deleteSubPoly(tempSub);

        k=0;

        for ( j = 0; j < checkSize; j += 1 ) {
          if(0==choose[j]){
            check[k++]=check[j];
          }
        }
        checkSize=k;
        i=-1;
      }
    }
    i++;

  }



  if(0==checkSize) {
    delete [] check;
    delete [] checked;
    delete [] choose;
    delete [] tempLoc;
    return SUCCESS;
  }

  if(checkSize<size){
    for ( i = 0; i < checkSize; i += 1 ) {
      tempLoc[i]=loc[check[i]];
    }
    int preAnsLength=0;
    if(NULL!=ans)
      preAnsLength=size_L(ans);
    SubPoly * tempSub=createSubPoly(subpoly->poly, checkSize, tempLoc);
    if(!polyIsSOS(tempSub,ans)){
      if(NULL!=ans)
        delListStart(ans,preAnsLength);
      delete [] check;
      delete [] checked;
      delete [] choose;
      delete [] tempLoc;
      deleteSubPoly(tempSub);
      return FAILURE;

    }else{
      delete [] check;
      delete [] choose;
      delete [] checked;
      delete [] tempLoc;
      deleteSubPoly(tempSub);
      return SUCCESS;

    }
  }

  delete [] check;
  delete [] choose;
  delete [] tempLoc;
  return OTHER;
}



/** 
 * 
 * test whether a given subpoly is a SOS polynomial

 * The semidefinite programming need data.
 *
 *     s.t. <(*constraints),X > (in dot) = b
 *      X>=0 is a semidefinite matrix.
 *      b is the array of the coefs of poly
 *
 *      X is the matrix [x^ax^b_{a,b}] 
 *

 * @param subpoly a mult-variable polynomial
 * @param ans if subpoly is a SOS polynomial the subpoly=\sum_{
 i=0}^s ans[ i ]^2
 }
 * @param verbose print information
 * 
 * @return true if find a SOS representation
 * false otherwise
 */

bool polyIsSOS (SubPoly * subpoly, PointList * ans, const int verbose ){


  int i,k,j,sosId,sosLength,gLength,genLength,tempLength=0;
  bool even;
  indice_t *SOSM=NULL;
  indice_t* GSUP=NULL;

  int check=easyCheck(subpoly,ans);

  if(CONVEX_POLY==check){

  }else if(NOSOS==check){
    return false;
  }else if (EXACTLY_SOS==check)
    return true;


  sosId=findSupByPoly(subpoly);
  indice_t * indices= subpoly->poly->indices;
  int *loc=subpoly->locs;
  int size=subpoly->size;

  if(sosId>-1){
    SOSM=getSOSsup(sosId,  &sosLength);
    GSUP=getGsup(sosId, &gLength);

  }	
  if(NULL==SOSM||NULL==GSUP) {

    start =clock();
    sosId=addconvexsosSup(subpoly);
    indice_t *temp =new indice_t[size*DIM];
    j=0;

    for ( i = 0; i < size; i += 1 ) {
      even=true;
      for ( k = 0; k < DIM; k += 1 ) {
        if((indices[loc[i]*DIM+k]&1)==1){
          even=false;
          break;
        }
      }
      if(even){
        for ( k = 0; k < DIM; k += 1 ) {
          temp[j*DIM+k]=indices[loc[i]*DIM+k]/2;
        }
        j++;
      }
    }
    genLength=j;


    indice_t *candidate =overConvexHull(temp, genLength,DIM,  &tempLength);

    int dummy=exactConvHull(subpoly,candidate, tempLength, temp,genLength,&SOSM, sosLength ,ans);
    
    if(SUCCESS==dummy){
      delete[] temp;
      free(SOSM);
      free(candidate);
      return true;
    }else if(FAILURE==dummy){
      delete[] temp;
      free(SOSM);
      free(candidate);
      return false;
    }

    GSUP=(indice_t*)realloc_d(candidate,tempLength*DIM*sizeof(indice_t));
    qsortM(GSUP,DIM,0,tempLength-1,compare);
    gLength=tempLength;

    qsortM(SOSM, DIM,0,sosLength-1,compare);
    setGsup(sosId, gLength, GSUP);
    setSOSsup(sosId,sosLength, SOSM);
    delete[] temp;
  }

  Blockmatrix *C;

  double **b= (double**) malloc_d(sizeof(double *));

  Constraintmatrix  * constraints;
  Blockmatrix X,Z;
  double *y;
  double pobj,dobj;

  /*-----------------------------------------------------------------------------
   *  For every term of poly whose total degree less or equal to total degree of poly
   *  which contain the term whose coefficient eqaul to zero. There is a constraint 
   *  related it.
   *-----------------------------------------------------------------------------*/

  constraints=createConstraintMatrix(subpoly, sosId, b, SOSM, sosLength,  gLength );

  if (constraints!=NULL){
    int bsize[1];

    bsize[0]=gLength;
    /* printf(" bb size %d", blockSize); */

    C=createBlockMatrixC(bsize, 1);

    //		int numofblock=blockSize;

    initsoln(gLength,sosLength,*C,*b,constraints,&X,&y,&Z);
    csdp_verbose=verbose;

    if(0==easy_sdp(gLength, sosLength, *C,*b, constraints, 0.0, &X,&y,&Z, &pobj, &dobj)){
      if(NULL!=ans)
        sosrepresent(ans, X.blocks[1].data.mat, gLength,sosId, 1e-2);
      free_prob(gLength,sosLength,*C,*b,constraints,X,y,Z);
      free(C);
      free(b);
      return true;
    }else{
      free_prob(gLength,sosLength,*C,*b,constraints,X,y,Z);
      free(C);
      free(b);
      return false;
    }
  }else{
    free(*b);
    free(b);
    return false;
  }

}



bool easychecksos( Poly *p){

  if(poly_getTotalDegree(p)<=0){
    if(isZero(p)  ||isPositive(p)) return true;
    else return false;
  }

  DIM=getvarNum(p->varId);

  Poly *poly=copyPoly(p);
  
  SubPoly * subp=createSubPolyByPoly(poly);

  PointList *ans=createList(delpoly);
  
  bool re=polyIsSOS(subp,ans,1);

  if(re){
    sosPresent(subp, ans, par.printMinValue);
  }
  delList(ans);

  deleteSubPoly(subp);

  deletePoly(poly);
  //  clearSupportTable();
  //	clearvarTable();
  return re;
}
