/*
 * =====================================================================================
 *
 *       Filename:  util.c
 *
 *    Description:  
 *
 *        Version:  1.0    
 *        Created:  2012年05月11日 17时19分58秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dai liyun (pku), dailiyun2009@163.com
 *        Company:  
 *
 * =====================================================================================
 */


#include	<stdlib.h>
#include	<stdio.h>
#include	"polytype.h"
#include	<stdarg.h>
#include	"selfassert.h"
#include	"util.h"
#include	<string.h>
#include	"monoconst_table.h"
#include	"mono_const.h"
#include	"support_table.h"
#include	"pconst_table.h"
#include	"vartable.h"
#include	"search.h"


extern void qsortM( indice_t *, const int n ,const  int left, const int right, int (*comp)(const indice_t *, const indice_t *,const int)); 
BOOL isSameLine(const indice_t points[], const int * loc, const int n , const int dim ){

  if(dim==1||n<3) return TRUE;
  int i,j,blockIndex;
  int base[dim];
  int current,next;
  for ( i = 0; i < dim; i += 1 ) {
    base[i]=points[loc[1]*dim+i]-points[loc[0]*dim+i];
    if(base[i]!=0) blockIndex=i;
  }
  for ( i = 2; i < n; i += 1 ) {
    current=points[loc[i]*dim+blockIndex]-points[loc[0]*dim+blockIndex];
    for ( j = 1; j < dim-1; j += 1 ) {
      next=points[loc[i]*dim+j]-points[loc[0]*dim+j];
      if(current*base[j]!=next*base[blockIndex]) return FALSE;
    }
  }
  return TRUE;
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  nchoosek
 *  Description:  n and d can not too large
 * =====================================================================================
 */
int 
nchoosek (const  int n,const int blockIndex )
{
  if(n<blockIndex) return 0;

  if ( n<blockIndex||n<0||blockIndex<0 ) {
    printf ( "input error\n" );
    exit(1);
  }


  int i=0;
  long double re;
  re=1;
  for(i=0;i<blockIndex;i++){
    re*=(n-i);
    re/=(blockIndex-i);
  }

  return (int) (re+0.5);
}		/* -----  end of function nchoosek  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  enLargeZ
 *  Description:  enlarge the capacity of Z, [0,start) has elements  
 * =====================================================================================
 */
void enLargeZ (indice_t *Z,const int n , int *const cap  )
{

  *cap=(*cap)+(*cap)*ENLARGE_RAT+1;
  Z= realloc_d(Z,n*(*cap)*sizeof(indice_t));



}		/* -----  end of function enLargeZ  ----- */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getAllMonByTd
 *  Description: n (n>=1) is the number of variables of polynomial and d is the degree of polynomial  
 * enumerate all monomials whos total degree less or equal than d.
 * Useing Lexicographic Order x1>x2>..>xn
 * restraintNum is the number of restraints 
 * output all the monomials which satsfy these restraints
 *
 * =====================================================================================
 */

indice_t *
getAllMonByTd (const  int supportId, int *const length )
{
  int i,j,blockIndex;
  int sum=1;
  int size=0;
  int cap;
  int nch;
  int n,d;
  indice_t *Z;

  Support * sup=getSupElem(supportId);
  
  if(sup==NULL) return NULL;

  n=getvarNum(sup->varId);
  if(0==n){
    Z=malloc_d(1*sizeof(indice_t));
    Z[0]=0;
    *length=1;
    return Z;
  }

  d=sup->deg;

  if(sup->constNum==0){

    *length=nchoosek(n+d,d);
    Z=(indice_t *) malloc_d((*length)*n*sizeof(indice_t ));

    /*-----------------------------------------------------------------------------
     *  The first row denote const 1, and second ros denote monomial x1
     *-----------------------------------------------------------------------------*/
    for ( i = 0; i < n; i += 1 ) {              /* initial the first two rows (monomial) to zero  */
      Z[i]=0;
      Z[n+i]=0;
    }

    Z[2*n-1]=1;
    i=2;

    while (i<(*length)  ) {
      /*memcpy(Z[i],Z[i-1],n*sizeof(int));*/

      for ( j=0;j<n ;j++ ) {
        Z[i*n+j]=Z[(i-1)*n+j];
      }

      if ( sum<d ) {
        Z[i*n+n-1]++;
        sum++;
      }
      else{

        blockIndex=n-1;
        while (Z[i*n+blockIndex]==0 )blockIndex--;
        sum-=(Z[i*n+blockIndex]-1);
        Z[i*n+blockIndex]=0;
        Z[i*n+blockIndex-1]++;
      }
      i++;
    }
  }
  else{

    nch=nchoosek(n+d,d);
    cap=nch/((sup->constNum+1)*n);

    Z	=(indice_t *) malloc_d ( n*cap*sizeof(indice_t ) );


    indice_t temp[n];

    for ( i = 0; i < n; i += 1 ) {
      temp[i]=0;                          /* constant term */
    }
    /*-----------------------------------------------------------------------------
     *  satsfies restraints 
     *-----------------------------------------------------------------------------*/
    if(criteria( sup , temp )) {
      addZ(Z,&cap,&size,n,temp);
    }

    temp[n-1]=1;

    if(criteria(sup, temp)) {
      addZ(Z,&cap,&size,n,temp);
    }
    i=2;
    while (i<nch ) {

      if(sum<d){
        temp[n-1]++;
        sum++;
      }
      else{
        blockIndex=n-1;
        while(temp[blockIndex]==0) blockIndex--;
        sum-=(temp[blockIndex]-1);
        temp[blockIndex]=0;
        temp[blockIndex-1]++;
      }
      if(criteria(sup, temp)) {
        addZ(Z,&cap,&size,n,temp);
      }

      i++;

    }
    *length=size;
  }
  return Z;
}


void getAllMonHomoTd(const int varNum, const int deg, const int length, indice_t* indices ){
  ASSERT(varNum>0,"variable number must be positive number");
  if(varNum==1){
    indices[0]=deg;
    return;
  }
  int n=varNum-1;

  indice_t *Z=malloc_d(length*n*sizeof(indice_t));
  int sum=1;
  int i,j,blockIndex;
  for(i=0;i<n;i++){
    Z[i]=0;
    Z[n+i]=0;
  }
  Z[2*n-1]=1;
  i=2;
  while (i<length) {
    for (j=0; j<n; j++) {
      Z[i*n+j]=Z[(i-1)*n+j];
    }
    if(sum<deg){
      Z[i*n+n-1]++;
      sum++;
    }else{
      
      blockIndex=n-1;
      while (Z[i*n+blockIndex]==0 )blockIndex--;
      sum-=(Z[i*n+blockIndex]-1);
      Z[i*n+blockIndex]=0;
      Z[i*n+blockIndex-1]++;
    }
    i++;
  }
  for(i=0;i<length;i++){
    sum=0;
    for (j=0; j<n; j++) {
      indices[i*varNum+j]=Z[i*n+j];
      sum+=Z[i*n+j];
    }
    indices[i*varNum+j]=deg-sum;//homogenous
  }
  free( Z);
}


void
addZ (indice_t * const  Z  , int * const cap , int * const size, const int n, indice_t *key)
{
  int i;
  if(*size==*cap){
    enLargeZ(Z, n, cap);
  }

  for ( i = 0; i < n; i += 1 ) {
    Z[(*size)*n+i ]=key[i];
  }
  (*size)++;

}




BOOL
criteria (const Support * S,  indice_t * key  )
{
  int i;
  int resNum=S->constNum;

  for ( i = 0; i < resNum; i += 1 ) {
    if(! consEval(key, S->varId, getconsElem(S->consId[i]))) return FALSE;

  }

  return TRUE;
}		/* -----  end of function criteria  ----- */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getZ
 *  Description:  n (n>=1) is the number of variables of polynomial and d
 *  if the total degree of polynomial. 
 *  enumerate all monomials whos total degree less or equal to d.
 *  Using Monomial order comp
 *  restraintNum is the number of the restraints
 * =====================================================================================
 */
// indice_t *
// getZ ( const int n, int *varMap, const int d, int * const length, int (*comp) (const indice_t *,const  indice_t *,const  int), const int restraintNum,  MonomialConstraint* restraints  )
// {
//   indice_t *Z;


//   Z=getAllMonByTd( d,length );

//   qsortM(Z,n,0,(*length)-1,comp);

//   return Z;
// }		/* -----  end of function getZ  ----- */



void
deleteZ ( indice_t **Z, const int length )
{
  int i;

  for ( i = 0; i < length; i += 1 ) {
    free(Z[i]);
  }
  free(Z);
}		/* -----  end of function deleteZ  ----- */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  compare ">"
 *  Description: x>y is and only if exists blockIndex x(blockIndex)>y(blockIndex) and when i<blockIndex x(i)=y(i) 
 *  return 1 if key1> key2
 *  if key1==key2 return 0
 *  otherwise -1 
 * =====================================================================================
 */
int
compare (const  indice_t * key1, const indice_t* key2,const int size )
{
  int i=0;

  for ( i = 0; i < size ; i += 1 ) {        

    if(key1[i]>key2[i]) return 1;
    else if (key1[i]<key2[i]) return -1;

  }

  return 0;

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getW
 *  Description:
 *  For every monomial c of SOSM  W_c is  a  nchoosek(n+d/2,d/2) * nchoosek(n+d/2,d/2) size symmetric matrix
 *  all element of W_c is one identity or zero. It means the location of c occurs in Z^T * Z 
 *  where Z=[1,x_1,...,x_n,x_1x_2,...,x_n^(d/2)]
 *  n is the number of variable and d is the max total degree of SOSM.
 *  d is a even integer
 *  return is a lengthM (usually nchoosek(n+d,d)) length array of matrix of size nchoosek(n+d/2,d/2) * nchoosek(n+d/2,d/2) 
 * =====================================================================================
 */
ArrangeMatrix**
createArrangeM(const int supportId, indice_t const *SOSM ,  int *const blockSize ,int const lengthM)
{
  int i,j,blockIndex,index;
  Support *sup=getSupElem(supportId);
  const int n=getvarNum(sup->varId);

  indice_t  temp[n+1]; 	
  indice_t *Z= getGsup(supportId, blockSize);
  if(NULL==Z){
    sup->deg/=2;
    Z=getAllMonByTd(supportId, blockSize); /* degree squared */
    setGsup(supportId, *blockSize, Z);
    sup->deg*=2;
  }

  ArrangeMatrix **re=(ArrangeMatrix **) malloc_d(lengthM*sizeof(ArrangeMatrix *));

  for ( i=0;i<lengthM;i++ ) {
    re[i]=createSparse(*blockSize);
  }

  /*-----------------------------------------------------------------------------
   *  |  a00 a10 a20  |
   *  |  a10 a11 a12  |
   *  |  a20 a21 a22  |
   *  As (Z^T) Z is a systematic matrix, we only need to compute the lower triangle 
   *  of it.
   *-----------------------------------------------------------------------------*/
  for ( i=0;i<(*blockSize) ;i++ ) {
    for ( j = 0; j <= i; j += 1 ) {
      for ( blockIndex = 0; blockIndex < n; blockIndex += 1 ) {
        temp[blockIndex]=Z[i*n+blockIndex]+Z[j*n+blockIndex];      /* monomial add */
      }
      index=findIndex(temp,SOSM,lengthM,n);  
      ASSERT(index>=0,"some thing wrong");
      addSparseElem(re[index],i,j);
    }
  }

  //	free(Z);

  return re ;
}		/* -----  end of function getW  ----- */


void
deleteW ( ArrangeMatrix **W[], const int size, const int len[])
{
  int i,j;

  for ( i = 0; i < size; i += 1 ) {

    for ( j = 0; j < len[i]; j += 1 ) {

      deleteSparse(W[i][j]);
    }
    free(W[i]);
  }

}



void
deleteSparseA (ArrangeMatrix **s, const int len ){
  int i;
  for ( i = 0; i < len; i += 1 ) {
    deleteSparse(s[i]);
  }
  free(s);

}	

void
catch_memory_alllocate_error ( int a )
{
  printf ( "cannot allow memory %d\n",a );
  fprintf ( stderr, "\ndynamic memory allocation failed\n" );
  exit (EXIT_FAILURE);
}	

void
config (void  )
{
  initcoefTable();

}

Poly* sosConvertPoly (Blockmatrix *const X,  const int blockIndex, const int blockSize, const int supportId ){

  Poly *re;
  coef_t cf;
  int i,j,row,col;
  re	= (Poly*)malloc_d ( sizeof(Poly) );

  re->varId=getSupElem(supportId)->varId;

  const int varSize=getvarNum(re->varId);
  
  const int length=getsosSLength(supportId);

  re->capacity=length;

  re->size=re->capacity;


  re->coef= (coef_t*) malloc_d ( re->size*sizeof(coef_t ) );

  //  memset(re->coef,0, re->size*sizeof(coef_t));

  re->indices	=(indice_t*) malloc_d ( re->size*varSize*sizeof(indice_t) );
  
  int dummy;
  memcpy(re->indices, getsosSup(supportId,&dummy), re->size*varSize*sizeof(indice_t));

  ArrangeMatrix ** S=getAMIndex(supportId,&dummy);

  for ( i = 0; i < length; i += 1 ) {
    cf=0;
    for ( j = 0; j < S[i]->size; j += 1 ) {

      row=S[i]->data[j].row+1;
      col=S[i]->data[j].col+1;
      if(row==col){
        cf+=X->blocks[blockIndex+1].data.mat[ijtok(row,col,blockSize)];

      }else{

        cf+=2*(X->blocks[blockIndex+1].data.mat[ijtok(row,col,blockSize)]);
      }

    }
    re->coef[i]=cf;
  }

  return re ;
}	
