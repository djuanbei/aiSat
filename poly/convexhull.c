/*
 * =====================================================================================
 *
 *       Filename:  convexhull.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/28/2013 10:47:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"convexhull.h"
#include	"selfmemutil.h"
#include	<stdlib.h>
#include	<limits.h>
#include	<math.h>
#include	<string.h>
#include	"vartable.h"
#include	"selfassert.h"
#include	"f2rat.h"
#include	"gcd.h"
#include	"util.h"
#include	"subpoly.h"
#include	"pca.h"
/* DGELSD prototype */
extern void dgelsd_( int* m, int* n, int* nrhs, double* a, int* lda,
                     double* b, int* ldb, double* s, double* rcond, int* rank,
                     double* work, int* lwork, int* iwork, int* info );
/* Auxiliary routines prototypes */
extern void print_matrix_( char* desc, int m, int n, double* a, int lda );

static long int  BOUND=INT_MAX;

static int node_dim;
static size_t node_b_size;


// static BOOL orthogonal(const int dim, const int index, const double *preV, double *vec){
//   int i,j;
//   double dot,norm=0;

//   for ( i = 0; i < index; i += 1 ) {
//     dot=0;
//     for ( j = 0; j < dim; j += 1 ) {
//       dot+=preV[i*dim+j]*vec[j];
//     }
//     for ( j = 0; j < dim; j += 1 ) {
//       vec[j]-=dot*preV[i*dim+j];
//     }
//   }
//   for ( i = 0; i < dim; i += 1 ) {
//     norm+=vec[j]*vec[j];
//   }
//   norm=sqrt(norm);
//   if(norm<1e-6) return FALSE;
//   for ( i = 0; i < dim; i += 1 ) {
//     vec[j]/=norm;
//   }
//   return TRUE;
// }

/*-----------------------------------------------------------------------------
 *  the three points in indices are Different from each other
 *-----------------------------------------------------------------------------*/
static BOOL  checkThreeP(const SubPoly *subpoly, const indice_t * indices, const int *loc, const int dim , const coef_t * coefs, PointList *ans){

  int i;
  int VA,VB,VM;
  indice_t key[dim];
  if(isSameLine(indices,loc,3,dim)){          /* on a same line */
    if(coefs[loc[0]]<0){
      if(coefs[loc[1]]<0||coefs[loc[2]]<0) return NOSOS;
    }else{
      if(coefs[loc[1]]<0&&coefs[loc[2]]<0) return NOSOS;
    }
    i=0;
    while(indices[loc[0]*dim+i]==indices[loc[1]*dim+i]&& indices[loc[1]*dim+i]==indices[loc[2]*dim+i])i++;

    int AB=abs(indices[loc[0]*dim+i]-indices[loc[1]*dim+i]);
    int BC=abs(indices[loc[1]*dim+i]-indices[loc[2]*dim+i]);
    int AC=abs(indices[loc[0]*dim+i]-indices[loc[2]*dim+i]);

    if(AC>AB&&AC>BC ){ /* A C are vertex  */
      VA=0;
      VB=2;
      VM=1;

    }else if (AB>AC && AB> BC) {                             /* A B are vertex  */
      VA=0;
      VB=1;
      VM=2;

    }else{                              /* B C are vertex */
      VA=1;
      VB=2;
      VM=0;
    }

    if(coefs[loc[VA]]<0||coefs[loc[VB]]<0) return NOSOS;
    for ( i = 0; i < dim; i += 1 ) {
      if(indices[loc[VA]*dim+i]&1==1) return NOSOS;
    }

    for ( i = 0; i < dim; i += 1 ) {
      if(indices[loc[VB]*dim+i]&1==1) return NOSOS;
    }

    //		if(fabs(coefs[loc[VM]])< coefs[loc[VA]] && fabs(coefs[loc[VM]]<coefs[loc[VB]])){
    //
    //			return EXACTLY_SOS;
    //		}

    if(coefs[loc[VM]]<0){
      if(fabs(coefs[loc[VM]])> coefs[loc[VA]]+coefs[loc[VB]]) return NOSOS;
    }else{
      for ( i = 0; i < dim; i += 1 ) {
        if(indices[loc[VM]*dim+i]&1==1) break;
      }
      if(i==dim){
        if(NULL!=ans){
          Poly *p=createPoly();
          polyChangeVarId(p, subpoly->poly->varId);

          for ( i = 0; i < dim; i += 1 ) {
            key[i]=indices[loc[0]*dim+i]/2;
          }
          internal_addTerm(p, key, sqrtf(coefs[loc[0]]));
          push_back_L(ans,p);

          Poly *p1=createPoly();
          polyChangeVarId(p1, subpoly->poly->varId);

          for ( i = 0; i < dim; i += 1 ) {
            key[i]=indices[loc[1]*dim+i]/2;
          }
          internal_addTerm(p1, key, sqrtf(coefs[loc[1]]));
          push_back_L(ans,p1);


          Poly *p2=createPoly();
          polyChangeVarId(p2, subpoly->poly->varId);

          for ( i = 0; i < dim; i += 1 ) {
            key[i]=indices[loc[2]*dim+i]/2;
          }
          internal_addTerm(p2, key, sqrtf(coefs[loc[2]]));
          push_back_L(ans,p2);
        }

        return EXACTLY_SOS;
      }
      else if(coefs[loc[VM]] > coefs[loc[VA]]+coefs[loc[VB]]) return NOSOS;

    }
    return UNHNOW;
  }
  else{
    if(coefs[loc[0]]<0 || coefs[loc[1]]< 0||coefs[loc[2]]<0) return NOSOS;
    for ( i = 0; i < dim; i += 1 ) {
      if(indices[loc[0]*dim+i]&1==1) return NOSOS;
    }
    for ( i = 0; i < dim; i += 1 ) {
      if(indices[loc[1]*dim+i]&1==1) return NOSOS;
    }
    for ( i = 0; i < dim; i += 1 ) {
      if(indices[loc[2]*dim+i]&1==1) return NOSOS;
    }
    if(NULL!=ans){
      Poly *p=createPoly();
      polyChangeVarId(p, subpoly->poly->varId);

      for ( i = 0; i < dim; i += 1 ) {
        key[i]=indices[loc[0]*dim+i]/2;
      }
      internal_addTerm(p, key, sqrtf(coefs[loc[i]]));
      push_back_L(ans,p);

      Poly *p1=createPoly();
      polyChangeVarId(p1, subpoly->poly->varId);

      for ( i = 0; i < dim; i += 1 ) {
        key[i]=indices[loc[1]*dim+i]/2;
      }
      internal_addTerm(p1, key, sqrtf(coefs[loc[i]]));
      push_back_L(ans,p1);


      Poly *p2=createPoly();
      polyChangeVarId(p2, subpoly->poly->varId);

      for ( i = 0; i < dim; i += 1 ) {
        key[i]=indices[loc[2]*dim+i]/2;
      }
      internal_addTerm(p2, key, sqrtf(coefs[loc[i]]));
      push_back_L(ans,p2);
    }

    return EXACTLY_SOS;

  }

}



int easyCheck(const SubPoly * subpoly,PointList *ans){

  const int size=subpoly->size;
  int * loc=subpoly->locs;

  indice_t * indices= subpoly->poly->indices;
  coef_t * pcoefs=subpoly->poly->coef;

  int dim=getvarNum(subpoly->poly->varId);

  indice_t key[dim];

  int coefs[dim];
  char convexSurf[size];
  char maxPoint[size];
  int i,j,k;
  int coefDomain=1;
  int maxSum=0;
  long int tempSum;
  int number=0;
  int first=0;
  int second=0;
  double planeSum;
  memset(convexSurf,0,size*sizeof(char));

  if(size==1){
    if(pcoefs[loc[0]]<0) return NOSOS;
    for ( i = 0; i < dim; i += 1 ) {
      if(indices[loc[0]*dim+i]&1==1) return NOSOS;
      key[i]=indices[loc[0]*dim+i]/2;
    }
    if(NULL!=ans){
      Poly * p=createPoly();
      polyChangeVarId(p, subpoly->poly->varId);
      internal_addTerm(p,key, sqrtf(pcoefs[loc[0]]));
      push_back_L(ans, p);
    }

    return EXACTLY_SOS;
  }
  else if(size==2){
    if(pcoefs[loc[0]]<0 || pcoefs[loc[1]]< 0) return NOSOS;
    for ( i = 0; i < dim; i += 1 ) {
      if(indices[loc[0]*dim+i]&1==1) return NOSOS;
    }
    for ( i = 0; i < dim; i += 1 ) {
      if(indices[dim*loc[1]+i]&1==1) return NOSOS;
    }
    if(NULL!=ans){
      Poly * p=createPoly();
      polyChangeVarId(p, subpoly->poly->varId);
      for ( i = 0; i < dim; i += 1 ) {
        key[i]=indices[loc[0]*dim+i]/2;
      }
      internal_addTerm(p,key, sqrtf(pcoefs[loc[0]]));

      push_back_L(ans, p);

      Poly * p1=createPoly();
      polyChangeVarId(p1, subpoly->poly->varId);

      for ( i = 0; i < dim; i += 1 ) {
        key[i]=indices[loc[1]*dim+i]/2;
      }
      internal_addTerm(p1,key, sqrtf(pcoefs[loc[1]]));
      push_back_L(ans, p1);
    }

    return EXACTLY_SOS;
  }
  else if(size==3){
    return checkThreeP(subpoly, indices, loc, dim, pcoefs, ans);
  }

  for ( i = 0; i < size; i += 1 ) {
    if(pcoefs[loc[i]]<0) break; 

    for ( k = 0; k < dim; k += 1 ) {
      if(indices[loc[i]*dim+k]&1==1)break;
    }
    if(k!=dim) break;

  }
  if(i==size){

    if(NULL!=ans)
      for ( i = 0; i < size; i += 1 ) {
        Poly *p=createPoly();
        polyChangeVarId(p, subpoly->poly->varId);

        for ( k = 0; k < dim; k += 1 ) {
          key[k]=indices[loc[i]*dim+k]/2;
        }
        internal_addTerm(p, key, sqrtf(pcoefs[loc[i]]));
        push_back_L(ans,p);
      }

    return EXACTLY_SOS;
  }

  int checkTime=(size)*dim*dim;

  for ( i = 0; i < checkTime; i += 1 ) {
    memset(maxPoint,0,size*sizeof(char));
    number=0;
    coefDomain=sqrt(i)/(2*size)+1;

    maxSum=0;
    while(0==maxSum){
      for ( j = 0; j < dim; j += 1 ) {
        coefs[j]=rand()%(2*coefDomain+1)-coefDomain;
        if(coefs[j]!=0) 
          maxSum=1;
      }
    }
    maxSum=0;

    for ( k = 0; k < dim; k += 1 ) {
      maxSum+=coefs[k]*indices[loc[0]*dim+k];
    }

    maxPoint[0]=1;
    number=1;
    first=0;

    for ( k = 1; k <size; k += 1 ) {
      //			if(1==convexSuf[k])continue;

      tempSum=0;

      for ( j = 0; j < dim; j += 1 ) {
        tempSum+=coefs[j]*indices[loc[k]*dim+j];
      }
      if (tempSum>maxSum){
        maxSum=tempSum;
        memset(maxPoint,0,k*sizeof(char));
        maxPoint[k]=1;
        number=1;
        first=k;
      }else if(tempSum==maxSum){
        maxPoint[k]=1;
        number++;
        second=k;
      }
    }
    if(number==1){
      convexSurf[first]=1;
      if(pcoefs[loc[first]]<-1e-6) return NOSOS;
      for ( j = 0; j < dim; j += 1 ) {
        if(indices[loc[first]*dim+j]&1==1) return NOSOS;
      }
    }
    else if(number==2){
      convexSurf[first]=1;
      if(pcoefs[loc[first]]<-1e-6) return NOSOS;
      for ( j = 0; j < dim; j += 1 ) {
        if(indices[loc[first]*dim+j]&1==1) return NOSOS;
      }
      convexSurf[second]=1;
      if(pcoefs[loc[second]]<-1e-6) return NOSOS;
      for ( j = 0; j < dim; j += 1 ) {
        if(indices[loc[second]*dim+j]&1==1) return NOSOS;
      }
    }
    else{
      planeSum=0;
      for ( k = 0; k < size; k += 1 ) {
        if(1==maxPoint[k]){
          convexSurf[k]=1;
          planeSum+=pcoefs[loc[k]];
        }
      }
      if(planeSum<-1e-6) return NOSOS;
    }

  }
  number=0;
  for ( i = 0; i < size; i += 1 ) {
    if(0==convexSurf[i]) number++;
  }
  double base=0.1;
  if((number+0.0)/size<base)
    return CONVEX_POLY;
  else return UNHNOW;
}


/*-----------------------------------------------------------------------------
 *  checkPoints denote this term has not been checked.
 *  limit it the least number of term in one surface
 *-----------------------------------------------------------------------------*/
int onSameSurf(const SubPoly * subpoly, const  int * checkPoints, const int size, indice_t * rePoints,   const int limit){

  ASSERT(limit>=1&& size>=limit,"candidate must greater or equal to limit");

  indice_t * indices=subpoly->poly->indices;
  int dim=getvarNum(subpoly->poly->varId);
  int *loc=subpoly->locs;
  int i,j,m, k,maxSum, tempSum;
  int checkTime=dim*limit*size;
  int number=0;
  int choose[limit];
  long int numBound;
  int coefsNum[dim];
  int coefsDen[dim];
  double A[size*dim];
  double dummy;
  int info;
  int DUM_DOMAIN=1,bound=1;

  for ( i = 0; i < dim; i += 1 ) {
    for ( j = 0; j < size; j += 1 ) {
      A[ijtok(j+1,i+1,size)]=indices[loc[checkPoints[j]]*dim+i];
    }
  }

  double VT[dim*dim];

  double eigenvector[dim];

  info=lpca(A, size,dim, VT,TRUE);

  if(info!=-1){

    for ( k = 0; k < dim; k += 1 ) {

      for ( i = 0; i < dim; i += 1 ) {
        eigenvector[i]=VT[ijtok(k+1,i+1,dim)];
      }

      DUM_DOMAIN=0;

      for ( m = 0; m < EIGN_CEF_BOUND; m += 1 ) {

        memset(rePoints,0,size*sizeof(indice_t));

        DUM_DOMAIN++;

        for ( i = 0; i < dim; i += 1 ) {
          f2rat(eigenvector[i], DUM_DOMAIN, coefsDen+i, coefsNum+i );
        }

        bound=1;

        for ( i = 0; i < dim; i += 1 ) {
          if(coefsNum[i]!=0) {
            dummy=bound*coefsNum[i];
            if(fabs(dummy)>LONG_MAX/2){
              bound=0;
              break;
            }
            bound*=coefsNum[i];
          }
        }

        ASSERT(0!=bound,"");

        if(0==bound)  continue;

        for ( i = 0; i < dim; i += 1 ) {
          if(coefsNum[i]!=0){
            coefsDen[i]=(bound/coefsNum[i]);
          }else{
            coefsDen[i]=0;
          }
        }
        number=0;
        maxSum=0;

        for ( i = 0; i < dim; i += 1 ) {
          maxSum+=coefsDen[i]*indices[loc[checkPoints[0]]*dim+i];
        }
        number=1;
        rePoints[0]=1;

        for ( i = 1; i < size; i += 1 ) {
          tempSum=0;

          for ( j = 0; j < dim; j += 1 ) {
            tempSum+=coefsDen[j]*indices[loc[checkPoints[i]]*dim+j];
          }
          if(tempSum>maxSum){
            maxSum=tempSum;
            memset(rePoints,0,i*sizeof(indice_t));
            rePoints[i]=1;
            number=1;
          }else if(tempSum==maxSum){
            number++;
            rePoints[i]=1;
          }

        }
        if(number>=limit){
          return number;
        }
      }

    }

  }

  int M=limit;
  int N=dim;
  int NRHS=1;
  int LDA=limit;
  int LDB=dim>limit?dim:limit;
  m = M;
  int  n = N, nrhs = NRHS, lda = LDA, ldb = LDB,  lwork, rank;
  /* Negative rcond means using default (machine precision) value */
  double rcond = -1.0;
  double wkopt;
  double* work;
  /* Local arrays */
  /* iwork dimension should be at least 3*min(m,n)*nlvl + 11*min(m,n),
     where nlvl = max( 0, int( log_2( min(m,n)/(smlsiz+1) ) )+1 )
     and smlsiz = 25 */
  int iwork[3*M*1+11*M];
  double s[M<N?M:N];
  double a[LDA*N];
  //	double dummy;

  double b[LDB*NRHS];

  for ( i = 0; i < checkTime; i += 1 ) {
    memset(rePoints,0,size*sizeof(indice_t));
    number=0; j=0; k=0;
    while(1){
      k++;
      if(k==size)k=0;
      if(rePoints[k]) continue;
      if(rand()%size<limit){ 
        rePoints[k]=1;
        choose[number++]=k;
        if(number>=limit) break;
      }
    }

    numBound=dim*dim;

    for ( k = 0; k < dim; k += 1 ) {
      numBound*=(indices[loc[checkPoints[choose[0]]]*dim+k]+1);
    }

    for ( j = 0; j < limit; j += 1 ) {
      b[j]=1;
      for ( k = 0; k < dim; k += 1 ) {
        a[k*limit+j]= indices[loc[checkPoints[choose[j]]]*dim+k];
      }
    }

    /* Query and allocate the optimal workspace */
    lwork=-1;
    dgelsd_( &m, &n, &nrhs, a, &lda, b, &ldb, s, &rcond, &rank, &wkopt, &lwork,
             iwork, &info );
    lwork = (int)wkopt;
    work = (double*)malloc( lwork*sizeof(double) );
    /* Solve the equations A*X = B */
    dgelsd_( &m, &n, &nrhs, a, &lda, b, &ldb, s, &rcond, &rank, work, &lwork,
             iwork, &info );
    if(info==0){                            /* success */
      for ( k = 0; k < dim; k += 1 ) {
        f2rat(b[k],numBound,coefsDen+k ,coefsNum+k);
      }
      numBound=1;

      for ( k = 0; k < dim; k += 1 ) {
        if (coefsNum[k]!=0){
          dummy=numBound*coefsNum[k];
          if(fabs(dummy)>LONG_MAX/2){
            numBound=0;
            break;
          }
          numBound*=coefsNum[k];
        }
      }
      if(0==numBound) continue;

      for ( k = 0; k < dim; k += 1 ) {

        if(coefsNum[k]!=0){
          coefsDen[k]*=(numBound/coefsNum[k]);
        }else coefsDen[k]=0;
      }

      for ( k = 0; k < limit; k += 1 ) {
        tempSum=0;
        for ( j = 0; j < dim; j += 1 ) {
          tempSum+=coefsDen[j]*indices[loc[checkPoints[choose[k]]]*dim+j];
        }
        if(tempSum!=numBound)break;
      }

      if(tempSum!=numBound)continue;

      k=0;
      number=limit;
      while(k<size){
        while(1==rePoints[k]&&k<size)k++;
        if(k==size) return number;
        tempSum=0;
        for ( j = 0; j < dim; j += 1 ) {
          tempSum+=coefsDen[j]*indices[loc[checkPoints[k]]*dim+j];
        }
        k++;
        if(tempSum==numBound){
          number++;
          rePoints[k-1]=1;

        }else break;

      }
      if(k==size) return number;

      if(tempSum>numBound){

        for ( ; k < size; k += 1 ) {
          if(0==rePoints[k]){
            tempSum=0;
            for ( j = 0; j < dim; j += 1 ) {
              tempSum+=coefsDen[j]*indices[loc[checkPoints[k]]*dim+j];
            }
            if(tempSum<numBound) break;
            else if(tempSum==numBound){
              rePoints[k]=1;
              number++;
            }

          }
        }
        if(tempSum<numBound) continue;

        return number;

      }
      else if (tempSum<numBound){

        for ( ; k < size; k += 1 ) {
          if(0==rePoints[k]){
            tempSum=0;
            for ( j = 0; j < dim; j += 1 ) {
              tempSum+=coefsDen[j]*indices[loc[checkPoints[k]]*dim+j];
            }
            if(tempSum>numBound) break;
            else if(tempSum==numBound){
              rePoints[k]=1;
              number++;
            }

          }
        }
        if(tempSum>numBound) continue;

        return number;
      }
    }

    free( (void*)work );

  }
  return -1;

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  randPointSet
 *  Description:  
 * =====================================================================================
 */
indice_t *
randPointSet ( const int length, const int dim, const int domain ){
  indice_t *re=malloc_d(length*dim*sizeof(indice_t));
  int i,j;

  for ( i = 0; i < length; i += 1 ) {
    for ( j = 0; j < dim; j += 1 ) {
      re[i*dim+j]=rand()%domain;
    }
  }
  return re;

}		/* -----  end of function randPointSet  ----- */

static int reduceByPlane1(indice_t* candidateSet, const int length, const int dim,  const int * coefs, int maxSum){
  int i;
  int tempSum=0;
  int len=length;

  for ( i = 0; i < len; i += 1 ) {
    tempSum=coefs[0]*candidateSet[i*dim+0];
    if(tempSum>maxSum){
      memmove(candidateSet+i*dim, candidateSet+(i+1)*dim, (len-i-1)*node_b_size);
      len--;
      i--;
    }
  }
  return len;

}
static int reduceByPlane2(indice_t* candidateSet, const int length, const int dim,  const int * coefs, int maxSum){
  int i;
  int tempSum=0;
  int len=length;

  for ( i = 0; i < len; i += 1 ) {
    tempSum=coefs[0]*candidateSet[i*dim+0];
    tempSum+=coefs[1]*candidateSet[i*dim+1];


    if(tempSum>maxSum){
      memmove(candidateSet+i*dim, candidateSet+(i+1)*dim, (len-i-1)*node_b_size);
      len--;
      i--;
    }
  }
  return len;

}

static int reduceByPlane3(indice_t* candidateSet, const int length, const int dim,  const int * coefs, int maxSum){
  int i;
  int tempSum=0;
  int len=length;

  for ( i = 0; i < len; i += 1 ) {
    tempSum=coefs[0]*candidateSet[i*dim+0];
    tempSum+=coefs[1]*candidateSet[i*dim+1];
    tempSum+=coefs[2]*candidateSet[i*dim+2];


    if(tempSum>maxSum){
      memmove(candidateSet+i*dim, candidateSet+(i+1)*dim, (len-i-1)*node_b_size);
      len--;
      i--;
    }
  }
  return len;

}

static int reduceByPlane4(indice_t* candidateSet, const int length, const int dim,  const int * coefs, int maxSum){
  int i;
  int tempSum=0;
  int len=length;

  for ( i = 0; i < len; i += 1 ) {
    tempSum=coefs[0]*candidateSet[i*dim+0];
    tempSum+=coefs[1]*candidateSet[i*dim+1];
    tempSum+=coefs[2]*candidateSet[i*dim+2];
    tempSum+=coefs[3]*candidateSet[i*dim+3];


    if(tempSum>maxSum){
      memmove(candidateSet+i*dim, candidateSet+(i+1)*dim, (len-i-1)*node_b_size);
      len--;
      i--;
    }
  }
  return len;

}

static int reduceByPlane5(indice_t* candidateSet, const int length, const int dim,  const int * coefs, int maxSum){
  int i;
  int tempSum=0;
  int len=length;

  for ( i = 0; i < len; i += 1 ) {
    tempSum=coefs[0]*candidateSet[i*dim+0];
    tempSum+=coefs[1]*candidateSet[i*dim+1];
    tempSum+=coefs[2]*candidateSet[i*dim+2];
    tempSum+=coefs[3]*candidateSet[i*dim+3];
    tempSum+=coefs[4]*candidateSet[i*dim+4];


    if(tempSum>maxSum){
      memmove(candidateSet+i*dim, candidateSet+(i+1)*dim, (len-i-1)*node_b_size);
      len--;
      i--;
    }
  }
  return len;

}

static int reduceByPlane6(indice_t* candidateSet, const int length, const int dim,  const int * coefs, int maxSum){
  int i;
  int tempSum=0;
  int len=length;

  for ( i = 0; i < len; i += 1 ) {
    tempSum=coefs[0]*candidateSet[i*dim+0];
    tempSum+=coefs[1]*candidateSet[i*dim+1];
    tempSum+=coefs[2]*candidateSet[i*dim+2];
    tempSum+=coefs[3]*candidateSet[i*dim+3];
    tempSum+=coefs[4]*candidateSet[i*dim+4];
    tempSum+=coefs[5]*candidateSet[i*dim+5];


    if(tempSum>maxSum){
      memmove(candidateSet+i*dim, candidateSet+(i+1)*dim, (len-i-1)*node_b_size);
      len--;
      i--;
    }
  }
  return len;

}

static int reduceByPlane7(indice_t* candidateSet, const int length, const int dim,  const int * coefs, int maxSum){
  int i;
  int tempSum=0;
  int len=length;

  for ( i = 0; i < len; i += 1 ) {
    tempSum=coefs[0]*candidateSet[i*dim+0];
    tempSum+=coefs[1]*candidateSet[i*dim+1];
    tempSum+=coefs[2]*candidateSet[i*dim+2];
    tempSum+=coefs[3]*candidateSet[i*dim+3];
    tempSum+=coefs[4]*candidateSet[i*dim+4];
    tempSum+=coefs[5]*candidateSet[i*dim+5];
    tempSum+=coefs[6]*candidateSet[i*dim+6];

    if(tempSum>maxSum){
      memmove(candidateSet+i*dim, candidateSet+(i+1)*dim, (len-i-1)*node_b_size);
      len--;
      i--;
    }
  }
  return len;

}

static int reduceByPlane8(indice_t* candidateSet, const int length, const int dim,  const int * coefs, int maxSum){
  int i;
  int tempSum=0;
  int len=length;

  for ( i = 0; i < len; i += 1 ) {
    tempSum=coefs[0]*candidateSet[i*dim+0];
    tempSum+=coefs[1]*candidateSet[i*dim+1];
    tempSum+=coefs[2]*candidateSet[i*dim+2];
    tempSum+=coefs[3]*candidateSet[i*dim+3];
    tempSum+=coefs[4]*candidateSet[i*dim+4];
    tempSum+=coefs[5]*candidateSet[i*dim+5];
    tempSum+=coefs[6]*candidateSet[i*dim+6];
    tempSum+=coefs[7]*candidateSet[i*dim+7];

    if(tempSum>maxSum){
      memmove(candidateSet+i*dim, candidateSet+(i+1)*dim, (len-i-1)*node_b_size);
      len--;
      i--;
    }
  }
  return len;

}
static int reduceByPlane9(indice_t* candidateSet, const int length, const int dim,  const int * coefs, int maxSum){
  int i;
  int tempSum=0;
  int len=length;

  for ( i = 0; i < len; i += 1 ) {
    tempSum=coefs[0]*candidateSet[i*dim+0];
    tempSum+=coefs[1]*candidateSet[i*dim+1];
    tempSum+=coefs[2]*candidateSet[i*dim+2];
    tempSum+=coefs[3]*candidateSet[i*dim+3];
    tempSum+=coefs[4]*candidateSet[i*dim+4];
    tempSum+=coefs[5]*candidateSet[i*dim+5];
    tempSum+=coefs[6]*candidateSet[i*dim+6];
    tempSum+=coefs[7]*candidateSet[i*dim+7];
    tempSum+=coefs[8]*candidateSet[i*dim+8];

    if(tempSum>maxSum){
      memmove(candidateSet+i*dim, candidateSet+(i+1)*dim, (len-i-1)*node_b_size);
      len--;
      i--;
    }
  }
  return len;

}

static int reduceByPlane10(indice_t* candidateSet, const int length, const int dim,  const int * coefs, int maxSum){
  int i;
  int tempSum=0;
  int len=length;

  for ( i = 0; i < len; i += 1 ) {
    tempSum=coefs[0]*candidateSet[i*dim+0];
    tempSum+=coefs[1]*candidateSet[i*dim+1];
    tempSum+=coefs[2]*candidateSet[i*dim+2];
    tempSum+=coefs[3]*candidateSet[i*dim+3];
    tempSum+=coefs[4]*candidateSet[i*dim+4];
    tempSum+=coefs[5]*candidateSet[i*dim+5];
    tempSum+=coefs[6]*candidateSet[i*dim+6];
    tempSum+=coefs[7]*candidateSet[i*dim+7];
    tempSum+=coefs[8]*candidateSet[i*dim+8];
    tempSum+=coefs[9]*candidateSet[i*dim+9];
    if(tempSum>maxSum){
      memmove(candidateSet+i*dim, candidateSet+(i+1)*dim, (len-i-1)*node_b_size);
      len--;
      i--;
    }
  }
  return len;

}

/*-----------------------------------------------------------------------------
 *  reduce the point in candidateSet whose sum of index less or equal to maxSum
 *-----------------------------------------------------------------------------*/

static int reduceByPlane(indice_t* candidateSet, const int length, const int dim,  const int * coefs, int maxSum){
  int i,j;
  int tempSum=0;
  int len=length;

  for ( i = 0; i < len; i += 1 ) {
    tempSum=0;

    for ( j = 0; j < dim; j += 1 ) {
      tempSum+=coefs[j]*candidateSet[i*dim+j];
    }

    if(tempSum>maxSum){
      memmove(candidateSet+i*dim, candidateSet+(i+1)*dim, (len-i-1)*node_b_size);
      len--;
      i--;
    }
  }
  return len;

}

// static int reduceByRandPlane(indice_t * candidateSet, const int candLength, const int dim,const indice_t * genSet, const int genLength, const int max[] ){

//   BOOL modify=FALSE;

//   int randTime=RANDTIME*RANDTIME;
//   int coefDomain=2;
//   int coefs[dim];
//   double dummy;
//   long int maxSum=0;
//   long int minSum=0;
//   int tempSum=0;
//   int re=candLength;
//   int i,j,k,n;
//   double latestLength=candLength;
//   int coefB=pow(INT_MAX,1.0/(dim));

//   //	double vecs[dim*dim];
//   //	int v_index=0;

//   //	int lastLength=candLength;
//   //	int time=0;
//   //	int timeBase=sqrt(genLength/dim);
//   n=0;
//   i=0;
//   while (1) {
//     n++;
//     if(COEFDOMAIN_INCA*i>randTime &&coefDomain<coefB ){
//       coefDomain++;
//       //			lastLength=re;
//     }
//     BOOL state=TRUE;
//     if(modify){
//       k=rand()%dim;
//       if(rand()%2==1)
//         coefs[k]++;
//       else coefs[k]--;
//     }else{
//       while(state){
//         for ( k = 0; k < dim; k += 1 ) {
//           coefs[k]=rand()%(2*coefDomain+1)-coefDomain;
//           if(coefs[k]!=0)
//             state=FALSE;
//         }
//       }

//     }
//     dummy=0;
//     for ( k = 0; k < dim; k += 1 ) {
//       dummy+=abs(coefs[k])*max[k];
//       ASSERT(dummy<BOUND,"");
//     }
//     if(dummy<BOUND){

//       maxSum=0;

//       for ( k = 0; k < dim; k += 1 ) {
//         maxSum+=coefs[k]*genSet[k];
//       }
//       minSum=maxSum;

//       for ( k = 1; k < genLength; k += 1 ) {
//         tempSum=0;
//         for ( j = 0; j < dim; j += 1 ) {
//           tempSum+=coefs[j]*genSet[k*dim+j];
//         }
//         if(tempSum>maxSum){
//           maxSum=tempSum;
//         }else if (tempSum==maxSum){
//         }else if(tempSum<minSum){
//           minSum=tempSum;
//         }
//       }

//       re=reduceByPlane(candidateSet, re, dim, coefs, maxSum);

//       for ( j = 0; j < dim; j += 1 ) {
//         coefs[j]*=-1;
//       }
//       re=reduceByPlane(candidateSet, re, dim, coefs, -minSum);
//     }

//     ASSERT(re>=genLength,"must some bugs in this code");
//     if(re==genLength) return re;
//     if(re==latestLength){
//       modify=FALSE;
//       i++;
//       randTime=dim*(sqrtf(candLength/(re+0.0))+20);
//       if(i>coefDomain*dim*randTime)
//         break;
//     }
//     else{
//       modify=TRUE;
//       i=0;
//       latestLength=re;
//     }
//   }

//   return re;
// }

int reduceByLestEignV(indice_t * candidateSet, const int candLength, const int dim,const indice_t * genSet, const int genLength, const int max[]){

  long int bound;
  int maxSum=0;
  int minSum;
  double dummy;
  int  temp;
  int re=candLength;
  int cutNum=dim;
  int j,i,n,m,r,k=0;
  int origLength=candLength;

  int DUM_DOMAIN=1;
  int coefNum[dim];
  int coefDen[dim];
  double a[genLength*dim];

  int (*reduce)(indice_t*, const int, const int, const int* , int);
  if(dim>10)
    reduce=reduceByPlane;
  else {
    switch(dim){
      case 1:
        reduce=reduceByPlane1;
        break;
      case 2:
        reduce=reduceByPlane2;
        break;
      case 3:
        reduce=reduceByPlane3;
        break;
      case 4:
        reduce=reduceByPlane4;
        break;
      case 5:
        reduce=reduceByPlane5;
        break;
      case 6:
        reduce=reduceByPlane6;
        break;

      case 7:
        reduce=reduceByPlane7;
        break;

      case 8:
        reduce=reduceByPlane8;
        break;

      case 9:
        reduce=reduceByPlane9;
        break;

      case 10:
        reduce=reduceByPlane10;
        break;
    }
  }

  for ( i = 0; i < dim; i += 1 ) {
    for ( j = 0; j < genLength; j += 1 ) {
      a[ijtok(j+1,i+1,genLength)]=genSet[j*dim+i];
    }
  }
  double VT1[dim*dim];
  double eigenvector[dim];
  int info=lpca(a, genLength,dim, VT1,TRUE);

  for ( r = 0; r < 1+RANDTIME; r += 1 ) {

    if(info!=-1){
      for ( k = 0; k < dim; k += 1 ) {
        for ( i = 0; i < dim; i += 1 ) {
          eigenvector[i]=VT1[ijtok(dim-k,i+1,dim)];
        }
        dummy=0;

        for ( i = 0; i < dim; i += 1 ) {
          dummy+=eigenvector[i]*eigenvector[i];
        }
        dummy=sqrt(dummy);
        dummy/=16;
        if(dummy<1e-6) continue;        /* the norm2 of eigenvector is too small */
        DUM_DOMAIN=1;

        for ( m = 0; m < DUN_DOMAIN_BOUND; m += 1 ) {

          for ( i = 0; i < dim; i += 1 ) {
            f2rat(eigenvector[i]/dummy, DUM_DOMAIN, coefDen+i, coefNum+i );
          }
          bound=1;

          for ( i = 0; i < dim; i += 1 ) {
            if(coefNum[i]!=0) {
              dummy=bound*coefNum[i];
              if(fabs(dummy)>BOUND){ /* the all norm of the coefs must be in a bound */
                bound=0;
                break;
              }
              bound*=coefNum[i];
            }
          }

          if(0==bound) break;/* the all norm of the coefs must be in a bound */

          for ( i = 0; i < dim; i += 1 ) {
            if(coefNum[i]!=0){
              coefDen[i]*=bound/coefNum[i];
            }else
              coefDen[i]=0;
          }


          for ( n = 0; n < 10*cutNum*sqrt(re); n += 1 ) {

            dummy=0;
            bound=0;
            for ( i = 0; i < dim; i += 1 ) {
              dummy+=abs(coefDen[i])*max[i];
              bound+=abs(coefDen[i])*max[i];
            }
            /* the all norm of the coefs must be in a bound */

            if(dummy>BOUND||bound>BOUND || fabs(dummy-bound)>BOUND/10) break;

            maxSum=0;

            for ( i = 0; i < dim; i += 1 ) {
              maxSum+=coefDen[i]*genSet[i];
            }
            minSum=maxSum;

            for ( i = 1; i < genLength; i += 1 ) {
              temp=0;
              for ( j = 0; j < dim; j += 1 ) {
                temp+=coefDen[j]*genSet[i*dim+j];
              }
              if(temp>maxSum){
                maxSum=temp;
              }else if (temp<minSum){
                minSum=temp;
              }
            }

            re=reduce(candidateSet, re, dim, coefDen, maxSum);

            for ( i = 0; i < dim; i += 1 ) {
              coefDen[i]*=-1;
            }

            re=reduce(candidateSet, re, dim, coefDen, -minSum);
            if(re==origLength) break;
            origLength=re;

            i=rand()%dim;
            if(rand()%2==1){
              coefDen[i]++;
            }else coefDen[i]--;
          }
          if(re==genLength) return re;

          DUM_DOMAIN++;

        }

      }
    }

    for ( i = 0; i < dim; i += 1 ) {
      for ( j = 0; j < genLength; j += 1 ) {
        a[ijtok(j+1,i+1,genLength)]=candidateSet[(rand()%re)*dim+i];
      }
    }


    info=lpca(a,genLength,dim, VT1,TRUE);

  }


  return re;

}

// static int refine(indice_t * candidateSet, const int candLength, const int dim,const indice_t * genSet, const int genLength, const int max[]){

//   int i,j,k,m,n,p;
//   BOOL check=FALSE;
//   double a[genLength*dim];
//   double VT[dim*dim];
//   int info;
//   long int maxSum,minSum,temp;
//   int DUM_DOMAIN=1;
//   int coefNum[dim];
//   int coefDen[dim];
//   double eigenvector[dim];
//   double dummy;
//   BOOL delete=FALSE;
//   int len=candLength;
//   long int bound=1;
//   for ( i = 0; i < len; i += 1 ) {
//     check=TRUE;
//     for ( j = 0; j < genLength; j += 1 ) {
//       if(memcmp(candidateSet+i*node_dim,genSet+j*node_dim,node_b_size)==0){
//         check=FALSE;
//         break;
//       }
//     }
//     if(check){
//       delete=FALSE;
//       for ( j = 0; j < genLength; j += 1 ) {
//         for ( k = 0; k < dim; k += 1 ) {
//           a[ijtok(j+1,k+1,genLength)]=genSet[j*dim+k]-candidateSet[i*dim+k];
//         }
//       }
//       info=lpca(a,genLength,dim,VT,FALSE);
//       if(info==-1) continue;

//       for ( p = 0; p < dim; p += 1 ) {

//         for ( j = 0; j < dim; j += 1 ) {
//           eigenvector[j]=VT[ijtok(p+1,j+1,dim)];
//         }
//         dummy=0;

//         for ( j = 0; j < dim; j += 1 ) {
//           dummy+=eigenvector[j]*eigenvector[j];
//         }
//         dummy=sqrt(dummy);
//         dummy/=16;
//         if(dummy<1e-6) continue;



//         DUM_DOMAIN=3;

//         for ( m = 0; m < DUN_DOMAIN_BOUND; m += 1 ) {

//           for ( j = 0; j < dim; j += 1 ) {
//             f2rat(eigenvector[j]/dummy, DUM_DOMAIN, coefDen+j, coefNum+j );
//           }

//           bound=1;

//           for ( j = 0; j < dim; j += 1 ) {
//             if(coefNum[j]!=0) {
//               dummy=bound*coefNum[j];
//               if(fabs(dummy)>BOUND){
//                 bound=0;
//                 break;
//               }
//               bound*=coefNum[j];
//             }
//           }

//           if(0==bound) break;

//           ASSERT(0!=bound,"");
//           for ( j = 0; j < dim; j += 1 ) {
//             if(coefNum[j]!=0){
//               coefDen[j]=(bound/coefNum[j]);
//             }else{
//               coefDen[j]=0;
//             }
//           }

//           for ( n = 0; n < 10*dim*sqrt(len); n += 1 ) {
//             dummy=0;
//             for ( j = 0; j < dim; j += 1 ) {
//               dummy+=abs(coefDen[j])*max[j];
//             }
//             if(dummy>BOUND) break;

//             maxSum=0;

//             for ( j = 0; j < dim; j += 1 ) {
//               maxSum+=coefDen[j]*genSet[j];
//             }
//             minSum=maxSum;

//             for ( k = 1; k < genLength; k += 1 ) {
//               temp=0;
//               for ( j = 0; j < dim; j += 1 ) {
//                 temp+=coefDen[j]*genSet[k*dim+j];
//               }
//               if(temp>maxSum){
//                 maxSum=temp;
//               }else if (temp<minSum){
//                 minSum=temp;
//               }
//             }
//             temp=0;	

//             for ( j = 0; j < dim; j += 1 ) {
//               temp+=coefDen[j]*candidateSet[i*dim+j];
//             }
//             if(temp>maxSum||temp<minSum){
//               memmove(candidateSet+i*node_dim,candidateSet+(i+1)*node_dim,(len-i)*node_b_size);
//               len--;
//               i--;
//               delete=TRUE;
//               break;
//             }

//             j=rand()%dim;
//             if(rand()%2==1){
//               coefDen[j]++;
//             }else coefDen[j]--;
//           }
//           if(delete) break;

//           DUM_DOMAIN++;

//         }
//         if(delete) break;
//       }
//     }
//   }


//   return len;
// }


indice_t*
overConvexHull(const indice_t *genSet, const int genLength, const int dim, int *reLength){

  ASSERT(genLength>0,"at least have one point");
  int i,j,k;
  node_dim=dim;
  node_b_size=dim*sizeof(indice_t);
  int max[dim];
  int min[dim];
  int temp[dim];
  int maxSum=0;
  int minSum=0;
  int tempsum=0;
  int index;
  int tempLength=0;
  int pairNum=(dim*(dim-1))/2;
  int threeNum=(dim*(dim-1)*(dim-2))/6;
  int pair[pairNum*2];
  int pairMax[pairNum];
  int pairMin[pairNum];

  int three[threeNum*3];
  int threeMax[threeNum];
  int threeMin[threeNum];
  //	printf ( "genLength %d\n",genLength );

  k=0;
  for ( i = 0; i < dim; i += 1 ) {
    for ( j = i+1; j < dim; j += 1 ) {
      pair[2*k]=i;
      pairMax[k]=genSet[pair[2*k]];
      pair[2*k+1]=j;
      pairMax[k]+=genSet[pair[2*k+1]];
      pairMin[k]=pairMax[k];
      k++;
    }
  }
  index=0;

  for ( i = 0; i < dim; i += 1 ) {
    for ( j = i+1; j < dim; j += 1 ) {
      for ( k = j+1; k < dim; k += 1 ) {
        three[3*index]=i;
        threeMax[index]=genSet[i];
        three[3*index+1]=j;
        threeMax[index]+=genSet[j];
        three[3*index+2]=k;
        threeMax[index]+=genSet[k];
        threeMin[index]=threeMax[index];
        index++;
      }
    }
  }


  for ( i = 0; i < dim; i += 1 ) {
    max[i]=genSet[i];
    min[i]=genSet[i];
    maxSum+=genSet[i];
    minSum+=genSet[i];
  }

  for ( i = 1; i < genLength; i += 1 ) {
    tempsum=0;
    for ( j = 0; j < dim; j += 1 ) {
      tempsum+=genSet[i*dim+j];
      if(genSet[i*dim+j]>max[j]){
        max[j]=genSet[i*dim+j];
      }
      else if (genSet[i*dim+j]<min[j]){
        min[j]=genSet[i*dim+j];
      }
    }
    if(tempsum>maxSum)
      maxSum=tempsum;
    else if (tempsum<minSum)
      minSum=tempsum;


    for ( j = 0; j < pairNum; j += 1 ) {
      tempsum=genSet[i*dim+pair[2*j]];
      tempsum+=genSet[i*dim+pair[2*j+1]];
      if(tempsum>pairMax[j])
        pairMax[j]=tempsum;
      else if(tempsum<pairMin[j])
        pairMin[j]=tempsum;
    }

    for ( j = 0; j < threeNum; j += 1 ) {
      tempsum=genSet[i*dim+three[3*j]];
      tempsum+=genSet[i*dim+three[3*j+1]];
      tempsum+=genSet[i*dim+three[3*j+2]];
      if(tempsum>threeMax[j])
        threeMax[j]=tempsum;
      else if(tempsum<threeMin[j])
        threeMin[j]=tempsum;
    }
  }
  memcpy(temp,min,sizeof(int)*dim);

  j=0;
  index=0;

  while (1) {
    if(temp[0]>max[0]){
      j=1;
      while(j<dim && temp[j]==max[j])j++;
      if(j==dim){
        break;
      }
      temp[j]++;
      memcpy(temp,min,j*sizeof(int));
    }

    tempsum=0;

    for ( k = 0; k < dim; k += 1 ) {
      tempsum+=temp[k];
    }
    if(tempsum<=maxSum&& tempsum>= minSum){
      for ( k = 0; k < pairNum; k += 1 ) {
        tempsum=temp[pair[2*k]];
        tempsum+=temp[pair[2*k+1]];
        if(tempsum>pairMax[k])
          break;
        else if(tempsum<pairMin[k])
          break;
      }
      if(k==pairNum){
        for ( k = 0; k < threeNum; k += 1 ) {
          tempsum=temp[three[3*k]];
          tempsum+=temp[three[3*k+1]];
          tempsum+=temp[three[3*k+2]];
          if(tempsum>threeMax[k]){
            break;
          }else if(tempsum<threeMin[k])
            break;
        }
        if(k==threeNum)
          index++;
      }
    }

    temp[0]++;

  }

  ASSERT(index>=genLength,"there some bugs occor in your this code");
  tempLength=index;

  indice_t * candidateSet=malloc_d(tempLength*node_b_size);
  memcpy(temp, min,dim*sizeof(int));

  j=0;
  index=0;

  while ( 1 ) {
    if(temp[0]>max[0]){
      j=1;
      while(j<dim && temp[j]==max[j])j++;
      if(j==dim){
        break;
      }
      temp[j]++;
      memcpy(temp,min,j*sizeof(int));

    }

    tempsum=0;

    for ( k = 0; k < dim; k += 1 ) {
      tempsum+=temp[k];
    }

    if(tempsum<=maxSum&& tempsum>= minSum){
      for ( k = 0; k < pairNum; k += 1 ) {
        tempsum=temp[pair[2*k]];
        tempsum+=temp[pair[2*k+1]];
        if(tempsum>pairMax[k])
          break;
        else if(tempsum<pairMin[k])
          break;
      }
      if(k==pairNum){
        for ( k = 0; k < threeNum; k += 1 ) {
          tempsum=temp[three[3*k]];
          tempsum+=temp[three[3*k+1]];
          tempsum+=temp[three[3*k+2]];
          if(tempsum>threeMax[k]){
            break;
          }else if(tempsum<threeMin[k])
            break;
        }
        if(k==threeNum){

          for ( k = 0; k < dim; k += 1 ) {
            candidateSet[index*dim+k]=temp[k];
          }
          index++;
        }
      }
    }
    temp[0]++;

  }
  if(tempLength==genLength){
    *reLength=genLength;

  }else{
    //		printf ( "%d\n",tempLength );
    tempLength=reduceByLestEignV(candidateSet, tempLength, dim, genSet, genLength,max);

    //		printf ( "%d\n",tempLength );
    if(tempLength==genLength){
      *reLength=tempLength;
      return candidateSet;

    }
    //		tempLength=reduceByRandPlane(candidateSet, tempLength, dim, genSet, genLength,max);
    //		if(tempLength==genLength){
    //			*reLength=tempLength;
    //			return candidateSet;
    //		}
    //
    *reLength=tempLength;
    //		*reLength=refine(candidateSet,tempLength,dim,genSet, genLength,max);


  }
  return candidateSet;


}		/* -----  end of function overConvexHull  ----- */
