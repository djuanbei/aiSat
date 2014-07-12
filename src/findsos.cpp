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
#include <ulib/hash_func.h>
#include <ulib/hash_open.h>
#include <ulib/math_rand_prot.h>
#include "pointlist.h"
#include "monomial.h"
#include "iostream"

using namespace ulib;
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






static int overConvexChecking(const SubPoly * subpoly, indice_t* indices, const int loc[], const int size, const open_hash_map<monomial, int> &monMap, const int vertex_num, PointList *ans );

// static bool separateCheck(indice_t *indices, const int loc[],  const int checkSize, const int check[],  const open_hash_map<monomial, int>  &monMap, const int id){
//   int i;
//   monomial curr=monMap.find(indices+loc[check[id]]*DIM).key();

//   for ( i = 0; i < checkSize; i += 1 ) {
//     monomial temp=monMap.find(indices+loc[check[i]]*DIM).key();
//     if(!curr.contain(temp) && curr.conjunction(temp)){                /* temp's set does not contain in curr's set */
//       return false; /* temp's set does not conjunction with curr's set */
//     }
//   }

//   return true;

// }

// static bool separateCheck(indice_t *indices,const int loc[],  const int checkSize, const int check[],  const open_hash_map<monomial, int>  &monMap, const uint32_t set[]){
//   int i;

//   for ( i = 0; i < checkSize; i += 1 ) {
//     monomial temp=monMap.find(indices+loc[check[i]]*DIM).key();
//     if(!contain(set,temp) && conjunction(set,temp)){                /* temp's set does not contain in curr's set */
//       return false; /* temp's set does not conjunction with curr's set */
//     }
//   }

//   return true;


// }


static int exactConvHull(const SubPoly *subpoly, indice_t * candidateSet, int &candidateLength, const indice_t * genSet, const int genLength, indice_t **SOSM, int &sosLength, PointList *ans){

  int i,j,k,valueIndex,v, m;
  int currCandLen=candidateLength;
  int lastre=-1;
  int size=subpoly->size;
  int *loc=subpoly->locs;
  indice_t* indices=subpoly->poly->indices;

  uint32_t vertexSet[4];
  vertexSet[0]=0;
  vertexSet[1]=0;
  vertexSet[2]=0;
  vertexSet[3]=0;
  size_t node_size=DIM*sizeof(indice_t);

  indice_t * key=new indice_t[DIM];
  indice_t * key1=new indice_t[DIM];

  open_hash_map<monomial, int> monMap;
  indice_t *indiceValues=new indice_t[candidateLength*candidateLength*DIM];

  valueIndex=0;
  for ( i = 0; i < genLength; i += 1 ) {
    for ( j = 0; j < DIM; j += 1 ) {
      key[j]=2*genSet[i*DIM+j];
    }
    memcpy(indiceValues+valueIndex*DIM,key,node_size);
    monMap[monomial(indiceValues+valueIndex*DIM,DIM)]=1;
    valueIndex++;
  }

  for ( i = 0; i < currCandLen; i += 1 ) {

    for ( j = i; j < currCandLen; j += 1 ) {

      for ( k = 0; k < DIM; k += 1 ) {
        key[k]=candidateSet[i*DIM+k]+candidateSet[j*DIM+k];
      }
      if(monMap.contain(monomial(key,DIM))){
        monMap[monomial(key,DIM)]=monMap[monomial(key,DIM)]+1;
      }else{
        memcpy(indiceValues+valueIndex*DIM,key,node_size);
        monMap[monomial(indiceValues+valueIndex*DIM,DIM)]=1;
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
      if(monMap[monomial(key,DIM)]==1){                 /* expect 2*candidateSet[i*DIM+j], no other combines
                                             of monomial has value key*/
        for ( j = 0; j < currCandLen; j += 1 ) {
          for ( k = 0; k < DIM; k += 1 ) {
            key1[k]=candidateSet[i*DIM+k]+candidateSet[j*DIM+k];
          }
          monMap[monomial(key1,DIM)]=monMap[monomial(key1,DIM)]-1;

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


  for(open_hash_map<monomial,int>::const_iterator it=monMap.begin();it!=monMap.end();++it){
    if(it.value()>0) sosLength++;
  }

  //  open_hash_map<monomial,int>::const_iterator it=monMap.begin();
  // which(it!=monMap.end()){
  //   if(it.value()==0)it= monMap.erase(it.key());
  //   else it++;
  // }
  for(open_hash_map<monomial,int>::const_iterator it=monMap.begin();it!=monMap.end();++it){
    if(it.value()==0) monMap.erase(it.key());
    if(it==monMap.end()) break;
  }

  for ( i = 0; i < genLength; i += 1 ) {

    for ( j = 0; j < DIM; j += 1 ) {
      key[j]=2*genSet[i*DIM+j];
    }
    monMap[monomial(key,DIM)]=monMap[monomial(key,DIM)]-1;
  }

  k=0;

  for ( j = 0; j < genLength; j += 1 ) {

    for ( i = 0; i < DIM; i += 1 ) {
      key[i]=2*genSet[j*DIM+i];
    }
    v=monMap[monomial(key,DIM)];

    if(v==1) {
      for ( i = 0; i < currCandLen; i += 1 ) {

        for ( m = 0; m < DIM; m += 1 ) {
          key1[m]=key[m]/2+candidateSet[i*DIM+m];
        }
        monMap.find(monomial(key1,DIM)).key().add(k);
      }
      monomial::add(vertexSet,k);

      k++;
      monMap.find(monomial(key,DIM)).key().change=false;
    }

  }
  ASSERT(k<=VERTEX_BOUND,"");
  int vertex_num=k;/* the number of vertex */

  bool run=true;

  while(run) {

    run=false;
    for ( i = 0; i < currCandLen; i += 1 ) {

      for ( j = 0; j < DIM; j += 1 ) {
        key[j]=2*candidateSet[i*DIM+j];
      }
      if(monMap.find(monomial(key,DIM)).key().change){
        monMap.find(monomial(key,DIM)).key().change=false;

        for ( j = 0; j < currCandLen; j += 1 ) {

          for ( m = 0; m < DIM; m += 1 ) {
            key1[m]=key[m]/2+candidateSet[j*DIM+m];
          }
          if(!(monMap.find(monomial(key1,DIM)).key().contain((monMap.find(monomial(key,DIM)).key())))){
            monMap.find(monomial(key1,DIM)).key().add(monMap.find(monomial(key,DIM)).key());
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
  if(!monMap.contain(monomial( indices+loc[0]*DIM,DIM))){
    if(NULL!=ans)
      cout<<  "Corollary 3" <<endl;
    delete[] indiceValues;
    return FAILURE;
  }

  for ( i = 1; i < size; i += 1 ) {

    if(!monMap.contain(monomial(indices+loc[i]*DIM,DIM))){
      if(NULL!=ans)
        cout<< "Corollary 3"<<endl;
      delete[] indiceValues;
      return FAILURE;
    }
  }
  int dummy=overConvexChecking(subpoly, indices, loc, size, monMap, vertex_num, ans);
  if(SUCCESS==dummy){
    delete[] indiceValues;
    return SUCCESS;

  }else if(FAILURE==dummy){
    delete[] indiceValues;
    return FAILURE;

  }

  *SOSM=(indice_t*)malloc_d(sosLength*node_size);

  i=0;

  for(open_hash_map<monomial,int>::const_iterator it=monMap.begin();it!=monMap.end();++it){
    if(it.value()>0) {
      memcpy((*SOSM)+i*DIM, it.key().indice ,node_size);
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

static Constraintmatrix *
createConstraintMatrix(const SubPoly *subpoly, const int sosId,  double ** b, const  indice_t* SOSM, const int sosLength,  const int gLength ){

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
    setArrangeM(sosId, AM, temploc );
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


static int overConvexChecking(const SubPoly * subpoly, indice_t* indices, const int loc[], const int size, const open_hash_map<monomial, int> &monMap, const int vertex_num, PointList *ans){

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
      monomial curr=monMap.find(monomial(indices+loc[check[i]]*DIM,DIM)).key();
      num=0;
      monomial::add(set,curr);
      checked[check[i]]=1;
      tempLoc[num++]=loc[check[i]];
      choose[i]=1;
      int tempNum=0;
      while(tempNum!=num){
        tempNum=num;
        for ( j = 0; j < checkSize; j += 1 ) {
          monomial temp=monMap.find(monomial( indices+loc[check[j]]*DIM,DIM)).key();
          if(monomial::conjunction(set,temp)&& !choose[j]){
            monomial::add(set,temp);
            checked[check[j]]=1;
            tempLoc[num++]=loc[check[j]];
            choose[j]=1;
          }
        }
      }

      if(num<size){
        if(NULL!=ans)
          cout	<< "found a split poly" << endl;
        SubPoly * tempSub=createSubPoly1(subpoly->poly,  num, tempLoc);
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


  //	for ( i = 0; i < checkSize; i += 1 ) {
  //
  //		if((monMap.find(indices+loc[check[i]]*DIM).key().size())<vertex_num && separateCheck(indices, loc, checkSize, check, monMap, i)){
  //			if(NULL!=ans)
  //			  cout	<< "found a split poly" << endl;
  //
  //			memset(choose,0, checkSize*sizeof(int));
  //			monomial curr=monMap.find(indices+loc[check[i]]*DIM).key();
  //
  //			num=0;
  //			for ( j = 0; j < checkSize; j += 1 ) {
  //				monomial temp=monMap.find(indices+loc[check[j]]*DIM).key();
  //				if(curr.contain(temp)){
  //					tempLoc[num++]=loc[check[j]];
  //					choose[j]=1;
  //				}
  //			}
  //			SubPoly * tempSub=createSubPoly1(subpoly->poly,  num, tempLoc);
  //			int preAnsLength=0;
  //			if(NULL!=ans)
  //			  preAnsLength=size_L(ans);
  //			if(!polyIsSOS(tempSub, ans)){            /* necessary condition */
  //				if(NULL!=ans)
  //				  delListStart(ans, preAnsLength);
  //				delete [] check;
  //				delete [] choose;
  //				delete [] dummyInd;
  //				delete [] tempLoc;
  //				deleteSubPoly(tempSub);
  //				return FAILURE;
  //			}
  //
  //			deleteSubPoly(tempSub);
  //
  //			k=0;
  //
  //			for ( j = 0; j < checkSize; j += 1 ) {
  //				if(0==choose[j]){
  //					check[k++]=check[j];
  //				}
  //			}
  //			checkSize=k;
  //			i=0;
  //		}
  //	}
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
    SubPoly * tempSub=createSubPoly1(subpoly->poly, checkSize, tempLoc);
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
  //
  //	for ( i = 0; i < randTime; i += 1 ) {
  //
  //		dummMon.clear();
  //		for ( j = 0; j < vertex_num/2; j += 1 ) {
  //			dummMon.add(rand()%vertex_num);
  //		}
  //		num=0;
  //		for ( j = 0; j < size; j += 1 ) {
  //			monomial temp=monMap.find(indices+loc[j]*DIM).key();
  //			if(dummMon.contain(temp)){
  //				tempLoc[num++]=loc[j];
  //			}
  //		}
  //		SubPoly * tempSub=createSubPoly1(subpoly->poly,  num, tempLoc);
  //		if(!polyIsSOS(tempSub, NULL)){            /* necessary condition */
  //			delete [] dummyInd;
  //			delete [] check;
  //			delete [] choose;
  //			delete [] tempLoc;
  //			deleteSubPoly(tempSub);
  //			return FAILURE;
  //		}
  //		deleteSubPoly(tempSub);
  //
  //	}


  delete [] check;
  delete [] choose;
  delete [] tempLoc;
  return OTHER;
}

/*
  poly is a mult polynomial
  this function checking wether poly can be
  write as sum of  n sos polynomials'

*/

bool polyIsSOS (SubPoly * subpoly, PointList * ans, const int verbose ){
  /*
   * The semidefinite programming need data.
   *
   *     s.t. <(*constraints),X > (in dot) = b
   *      X>=0 is a semidefinite matrix.
   *      b is the array of the coefs of poly
   *
   *      X is the matrix [x^ax^b_{a,b}]
   *
   */

  /*
    poly is a mult polynomial
    this function checking wether poly can be
    write as sums of polynomials'
  */

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
    SOSM=getsosSup(sosId,  &sosLength);
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
    setsosSup(sosId,sosLength, SOSM);
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


// bool easychecksos(const CanonicalForm &p){
//   //
//   //	int nVar=getNumVars(p);
//   //	CanonicalForm allvars=getVars(p);
//   //	int maxV=p.mvar();
//   //	ASSERT(maxV>=0,"");
//   //
//   //	/*-----------------------------------------------------------------------------
//   //	 *  obtain all the variables which occor in p
//   //	 *-----------------------------------------------------------------------------*/
//   //	int vars[nVar];
//   //	char *varName;
//   //	int i=nVar-1;
//   //	while(i>=0){
//   //		int currVar=allvars.mvar();
//   //		if(currVar>maxV) maxV=currVar;
//   //		vars[i]=currVar;
//   //		CFMap mapp(currVar,CanonicalForm(1));
//   //		allvars=mapp(allvars);
//   //		i--;
//   //	}
//   //	initvarTable(maxV);
//   //
//   //	allvars=getVars(p);
//   //	int i=nVar-1;
//   //	while(i>=0){
//   //		varName=(char*)malloc_d(2*sizeof(char));
//   //		varName[1]='\0';
//   //		int currVar=allvars.mvar();
//   //		ASSERT(currVar>=0,"");
//   //		vars[i]=currVar;
//   //		varName[0]=name(currVar);
//   //		setvarName(currVar,varName);
//   //		CFMap mapp(currVar,CanonicalForm(1));
//   //		allvars=mapp(allvars);
//   //		i--;
//   //	}
//   //	if(p.level()<0)
//   if(p.degree()<=0){
//     if(p.isZero() ||p>CanonicalForm(0)) return true;
//     else return false;
//   }

//   DIM=getNumVars(p);
//   Poly *poly=convertCanonToPoly(p);
//   SubPoly * subp=createSubPoly(poly);

//   PointList *ans=createList(delpoly);
//   bool re=polyIsSOS(subp,ans,1);

//   if(re){
//     sosPresent(subp, ans, par.printMinValue);
//   }
//   delList(ans);

//   deleteSubPoly(subp);
//   //	printPoly(poly);
//   //	bool re=polyIsSOS(poly);
//   //	int check[poly->size];
//   //
//   //	for ( i = 0; i < poly->size; i += 1 ) {
//   //		check[i]=i;
//   //	}
//   //	indice_t rep[poly->size];
//   //	int same=onSameSurf(poly,check,poly->size, rep,40);
//   //	printf ( "size %d, %d\n",poly->size, same );
//   //
//   //	int re=easyCheck(poly);
//   //	if(re==NOSOS) return false;
//   //	else if(CONVEX_POLY==re){
//   //		printf ( " convex poly%d\n",re );
//   //	}
//   deletePoly(poly);
//   clearSupportTable();
//   //	clearvarTable();
//   return re;
// }

bool easychecksos( Poly *p){

  if(poly_getTotalDegree(p)<=0){
    if(isZero(p)  ||isPositive(p)) return true;
    else return false;
  }

  DIM=getvarNum(p->varId);

  Poly *poly=copyPoly(p);
  
  SubPoly * subp=createSubPoly(poly);

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
