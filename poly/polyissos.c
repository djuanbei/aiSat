
#include     <math.h>
#include    "sdpsolver.h"
#include    "util.h"
#include    "poly.h"
#include    <assert.h>
#include    "polyissos.h"


extern void dsyev_( char* jobz, char* uplo, int* n, double* a, int* lda,
                    double* w, double* work, int* lwork, int* info );


void sosrepresent(PointList * sosList, double * X, const int  blockSize , const int sosMid, const float minV ){
  int i,j,l,lda=blockSize;
  double* w=malloc_d(blockSize*sizeof(double));
  int lwork=blockSize*blockSize*2;
  double* work;
  int info;
  int n=blockSize;
  lwork=-1;
  double workopt;


  dsyev_("V", "U", &n, X, &lda, w, &workopt, &lwork,&info);
  lwork=(int)workopt;
  work=(double*) malloc_d(lwork*sizeof(double));

  dsyev_("V", "U", &n, X, &lda, w, work, &lwork,&info);

  const  int varNum=getvarNum(getSupElem(sosMid)->varId);

  indice_t * Z=getGsup(sosMid, &l);

  //	for ( i = 0; i < blockSize; i += 1 ) {
  //		printf ( "  %f\n",w[i] );
  //	}
  //
  //	for ( i = 0; i < blockSize; i += 1 ) {
  //
  //		for ( j = 0; j < blockSize; j += 1 ) {
  //			printf ( "     %6.5f",X[ijtok(i+1,j+1,blockSize)] );
  //		}
  //		printf ( "\n" );
  //	}

  for ( i = 0; i < blockSize; i += 1 ) {
    if (w[i]>minV){

      Poly *p1 =createPoly();
      polyChangeVarId(p1,getSupElem(sosMid)->varId);

      for ( j = 0; j < blockSize; j += 1 ) {
        internal_addTerm(p1, Z+j*varNum,X[ijtok(j+1,i+1,blockSize)] );
      }
      p_mult_cons_assign(p1, sqrtf(w[i]));
      push_back_L(sosList, p1);
      //			printPoly(p1);
      //			deletePoly(p1);
    }
  }

  free(w);
  free(work);

}

///*-----------------------------------------------------------------------------
// *  varId is the var id of polynomial we want to write as
// *  sums of  sos polynomials.
// *  td is the total degree of poly we want check
// *
// *  <numofCons>: is the number of monomials whose total degree less or equal to poly's 
// *  total degree
// *  <blockSize>: the nchoosek(td(poly)/2+varnums(poly),varnums(poly))
// *  <b>: is the array of coefs of poly.
// *-----------------------------------------------------------------------------*/
//
//static Constraintmatrix *
//createConstraintMatrix(const Poly *poly,  int * numofCons, int * sosId, int * blockSize, double ** b){
//
//    int i,j,h,tempI;
//    const int varNum=getvarNum(poly->varId);
//    const int totalDegree=poly_getTotalDegree(poly);
//    int lengthM=0;
//
//    Sparseblock *blockptr;
//
////	*sosId=addconvexsosSup(poly);
//
//    *sosId =addsosSup(totalDegree, poly->varId,0,NULL);
//
//    indice_t *SOSM=getsosSup(*sosId, &lengthM);
//
//    if(NULL==SOSM){
//        SOSM=getAllMonByTd(*sosId, &lengthM);        /* get all the moninal whose total degree less or equal to totalDegree */
//        setsosSup(*sosId, lengthM, SOSM);
//    }
//
///*     SparseSOSblock **block;
// */
//
//    ArrangeMatrix **W=getArrangeM(*sosId, SOSM, blockSize ,lengthM);
//
//    setArrangeM(*sosId, W,lengthM);
//
//    *numofCons=lengthM;
//
//    Constraintmatrix *re=createNconstraintmatrix(*numofCons); /* every monomial w.r.t. to a constraint */
//
///*     block=(SparseSOSblock**) malloc_d ((*numofCons)*sizeof(SparseSOSblock *));
// * 
// *     for ( i = 0; i < *numofCons; i += 1 ) {
// *         block[i]=createSparseSOSblock();
// *     }
// * 
// *     for ( j = 0; j < lengthM; j += 1 ) {
// *         addSparse(block[j],j,1);
// *     }
// */
//
//    /*-----------------------------------------------------------------------------
//     *  use block to  construct  constraints
//     *-----------------------------------------------------------------------------*/
//
//	for ( i = 0; i < *numofCons; i += 1 ) {
//
//		/* 		sum=0;
//		 * 
//		 * 		for ( k = 0; k < block[i]->size; k += 1 ) {
//		 * 			sum+=W[block[i]->index[k]]->size;
//		 * 		}
//		 */
//		/* assert(sum==1); */
//
//		/* printf("b size %d",*blockSize);; */
//
//		blockptr=createSparseblock(1,*blockSize,i+1,W[i]->size);
//		tempI=1;
//		for ( h = 0; h < W[i]->size; h += 1 ) {
//			blockptr->iindices[tempI]=W[i]->data[h].row+1;
//			blockptr->jindices[tempI]=W[i]->data[h].col+1;
//			blockptr->entries[tempI]=1;
//			tempI++;
//		}
//		frontInsertBlock(re+i+1,blockptr);
//
//	}
///* 
// * 	for ( i = 0; i < *numofCons; i += 1 ) {
// * 
// * 		deleteSparseSOSblock(block[i]);
// * 	}
// * 	free(block);
// */
//
//	int loc;
//	*b=(double*)calloc_d((*numofCons+1),sizeof(double));
//
///* 	for ( i = 0; i < *numofCons+1; i += 1 ) {
// * 		(*b)[i]=0;
// * 	}
// */
//
//	indice_t key[varNum];
//	for ( i = 0; i < poly->size; i += 1 ) {
//
//		for ( j = 0; j < varNum; j += 1 ) {
//			key[j]=poly->indices[i*varNum+j];
//		}
//		loc=findIndex(key,SOSM, lengthM, varNum);
//		assert(loc>-1);
//		(*b)[loc+1]=poly->coef[i];
//		/* printf(" %d %f",loc, poly->coef[i]);  */
//	}
//
//	return re;
//}
//
//
///*
//   poly is a mult polynomial
//   this function checking wether poly can be
//   write as sum of  n sos polynomials'
//
//*/
//
//BOOL polyIsSOS(const Poly * poly){
//
//	/*
//	 * The semidefinite programming need data.
//	 *
//	 *     s.t. <(*constraints),X > (in dot) = b
//	 *      X>=0 is a semidefinite matrix.
//	 *      b is the array of the coefs of poly
//	 *
//	 *      X is the matrix [x^ax^b_{a,b}]
//	 *
//	 */
//
//	/*
//	   poly is a mult polynomial
//	   this function checking wether poly can be
//	   write as sums of polynomials'
//	   */
//
//	int i, numofCons, blockSize,sosId;
//
//	Blockmatrix C;
//
//	double **b= (double**) malloc_d(sizeof(double *));
//
//	Constraintmatrix  * constraints;
//
//	Blockmatrix X,Z;
//
//	double *y;
//
//	double pobj,dobj;
//
//	/*-----------------------------------------------------------------------------
//	 *  For every term of poly whose total degree less or equal to total degree of poly
//	 *  which contain the term whose coefficient eqaul to zero. There is a constraint 
//	 *  related it.
//	 *-----------------------------------------------------------------------------*/
//
//	constraints=createConstraintMatrix(poly, &numofCons, &sosId, &blockSize, b );
//
//	int bsize[1];
//
//	for ( i = 0; i < 1; i += 1 ) {
//		bsize[i]=blockSize;
//	}
//	/* printf(" bb size %d", blockSize); */
//
//
//	C=*createBlockMatrixC(bsize, 1);
//
//	int numofblock=1*blockSize;
//
//	initsoln(numofblock,numofCons,C,*b,constraints,&X,&y,&Z);
//
//	if(0==easy_sdp(numofblock, numofCons, C,*b, constraints, 0.0, &X,&y,&Z, &pobj, &dobj)){
//		sosrepresent( X.blocks[1].data.mat, blockSize,sosId);
//		free_prob(numofblock,numofCons,C,*b,constraints,X,y,Z);
//		free(b);
//		return TRUE;
//	}else{
//
//		free_prob(numofblock,numofCons,C,*b,constraints,X,y,Z);
//		free(b);
//		return FALSE;
//	}
//
//}
