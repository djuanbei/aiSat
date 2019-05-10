/**
 * @file   findsos.cpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon Oct  9 20:02:27 2017
 *
 * @brief  polynomial semindefinite programming
 *
 *
 */

#include "findsos.h"

#include "convexhull.h"
#include <cmath>
#include <map>

#include "iostream"
#include "psdutil.h"
#include "search.h"
#include "selfassert.h"
#include "sort.h"
#include "sosproblem.h"
//#include "subpoly.hpp"
#include "util.h"

#include "pointlist.h"

extern "C" {
void dsyev_( char *jobz, char *uplo, int *n, double *a, int *lda, double *w,
             double *work, int *lwork, int *info );
}

extern int csdp_verbose;

namespace aiSat {
namespace psd {

using std::cout;
using std::endl;
using std::map;

static clock_t start;

static void delpoly( void *p ) { delete ( (Poly_t *) p ); }

bool SOSChecker::sosrepresent( PointList *sosList, double *X,
                               const int blockSize, const int sosMid ) {
  int     i, j, l, lda = blockSize;
  double *w     = (double *) malloc_d( blockSize * sizeof( double ) );
  int     lwork = blockSize * blockSize * 2;
  double *work;
  int     info;
  int     n = blockSize;
  lwork     = -1;
  double workopt;
  char   V[ 2 ], U[ 2 ];
  V[ 0 ] = 'V';
  U[ 0 ] = 'U';
  V[ 1 ] = U[ 1 ] = '\0';
  /**
   * try get best work space
   *
   */
  dsyev_( V, U, &n, X, &lda, w, &workopt, &lwork, &info );
  if ( 0 != info ) {
    return false;
  }
  lwork = (int) workopt;
  work  = (double *) malloc_d( lwork * sizeof( double ) );

  dsyev_( V, U, &n, X, &lda, w, work, &lwork, &info );
  if ( 0 != info ) {
    return false;
  }

  const int varNum = getVarTable<indice_t>().getVarNum(
      SUPPORT_TABLE.getSupElem( sosMid )->varId );

  indice_t *Z = SUPPORT_TABLE.getGsup( sosMid, &l );

  for ( i = 0; i < blockSize; i += 1 ) {
    if ( w[ i ] > para.mineignvalue ) {
      Poly_t *p1 = new Poly_t();
      p1->changeVarId( SUPPORT_TABLE.getSupElem( sosMid )->varId );
      vector<indice_t> vars;
      getVarTable<indice_t>().getVarVec(
          SUPPORT_TABLE.getSupElem( sosMid )->varId, vars );

      for ( j = 0; j < blockSize; j += 1 ) {
        Poly_t::Term temp_term;
        for ( size_t k = 0; k < vars.size(); k++ ) {
          temp_term.key.push_back(
              make_pair( vars[ k ], Z[ j * varNum + k ] ) );
        }
        temp_term.cf = X[ ijtok( j + 1, i + 1, blockSize ) ];

        p1->add_term( temp_term );
      }

      p1->mult( sqrtf( w[ i ] ) );

      push_back_L( sosList, p1 );
    }
  }

  free( w );
  free( work );
  return true;
}

void SOSChecker::sosPresent( Poly_t &poly, const PointList *sosList ) {
  cout << poly << "=" << endl;

  PointElem *temp = sosList->head;

  while ( temp->next != NULL ) {
    Poly_t *tp = (Poly_t *) temp->value;
    if ( tp->getSize() > 0 ) {
      if ( tp->isAllCoefNegative() ) {
        tp->mult( -1 );
      }
      cout << "(";
      string str = tp->toString( para.mineignvalue, para.printPrec );
      cout << str;

      temp = temp->next;
      cout << ")^2+";
    }
  }

  Poly_t *tp = (Poly_t *) temp->value;
  if ( tp->getSize() > 0 ) {
    if ( tp->isAllCoefNegative() ) {
      tp->mult( -1 );
    }
    cout << "(";
    string str = tp->toString( para.mineignvalue, para.printPrec );
    cout << str;

    cout << ")^2" << endl;
  }
}

int SOSChecker::exactConvHull( const Poly_t &poly, indice_t *candidateSet,
                               int &candidateLength, const indice_t *genSet,
                               const int genLength, indice_t **SOSM,
                               int &sosLength, PointList *ans ) {
  int                i, j, valueIndex, v, m;
  int                k;
  int                currCandLen = candidateLength;
  int                lastre      = -1;
  int                size        = poly.coef.size();


  const indice_t *indices = &( poly.indices[ 0 ] );

  size_t           node_size = dim * sizeof( indice_t );
  vector<indice_t> key( dim );
  vector<indice_t> key1( dim );

  map<Monomial, Monomialvalue> monMap;
  indice_t *                   indiceValues =
      new indice_t[ candidateLength * candidateLength * dim ];

  valueIndex = 0;
  for ( i = 0; i < genLength; i += 1 ) {
    for ( j = 0; j < dim; j += 1 ) {
      key[ j ] = 2 * genSet[ i * dim + j ];
    }
    memcpy( indiceValues + valueIndex * dim, &key[ 0 ], node_size );
    Monomialvalue dummy;
    dummy.value                                                = 1;
    monMap[ Monomial( indiceValues + valueIndex * dim, dim ) ] = dummy;
    valueIndex++;
  }

  for ( i = 0; i < currCandLen; i += 1 ) {
    for ( j = i; j < currCandLen; j += 1 ) {
      for ( k = 0; k < dim; k += 1 ) {
        key[ k ] = candidateSet[ i * dim + k ] + candidateSet[ j * dim + k ];
      }
      if ( monMap.find( Monomial( &key[ 0 ], dim ) ) != monMap.end() ) {
        monMap[ Monomial( &key[ 0 ], dim ) ].value++;

      } else {
        memcpy( indiceValues + valueIndex * dim, &key[ 0 ], node_size );
        Monomialvalue dummy;
        dummy.value                                                = 1;
        monMap[ Monomial( indiceValues + valueIndex * dim, dim ) ] = dummy;
        valueIndex++;
      }
    }
  }

  /*-----------------------------------------------------------------------------
   *  delete some Monomial in monMap which value equal to 1 and the value with
   *relate with this Monomial,
   *  itertation delete Monomial in monMap
   *-----------------------------------------------------------------------------*/

  while ( lastre != currCandLen ) {
    lastre = currCandLen;

    for ( i = 0; i < currCandLen; i += 1 ) {
      for ( j = 0; j < dim; j += 1 ) {
        key[ j ] = 2 * candidateSet[ i * dim + j ];
      }
      if ( monMap[ Monomial( &key[ 0 ], dim ) ].value ==
           1 ) { /* expect 2*candidateSet[i*dim+j], no other combines
                    of Monomial has value key*/
        for ( j = 0; j < currCandLen; j += 1 ) {
          for ( k = 0; k < dim; k += 1 ) {
            key1[ k ] =
                candidateSet[ i * dim + k ] + candidateSet[ j * dim + k ];
          }
          monMap[ Monomial( &key1[ 0 ], dim ) ].value--;
        }
        monMap.erase( Monomial( &key[ 0 ], dim ) );
        if ( i + 1 < currCandLen )
          memmove( candidateSet + i * dim, candidateSet + ( i + 1 ) * dim,
                   ( currCandLen - i - 1 ) * node_size );

        i--;
        currCandLen--;
      }
    }
  }

  candidateLength = currCandLen;
  if ( NULL != ans )
    cout << "Constructing G use time: "
         << ( clock() - start - 0.0 ) / CLOCKS_PER_SEC << endl;

  sosLength = 0;

  for ( map<Monomial, Monomialvalue>::const_iterator it = monMap.begin();
        it != monMap.end(); ++it ) {
    if ( it->second.value > 0 ) sosLength++;
  }

  for ( map<Monomial, Monomialvalue>::iterator it = monMap.begin();
        it != monMap.end(); ++it ) {
    if ( it->second.value == 0 ) {
      monMap.erase( it );
    }
    if ( it == monMap.end() ) {
      break;
    }
  }

  for ( i = 0; i < genLength; i += 1 ) {
    for ( j = 0; j < dim; j += 1 ) {
      key[ j ] = 2 * genSet[ i * dim + j ];
    }
    monMap[ Monomial( &key[ 0 ], dim ) ].value--;
  }

  k = 0;

  for ( j = 0; j < genLength; j += 1 ) {
    for ( i = 0; i < dim; i += 1 ) {
      key[ i ] = 2 * genSet[ j * dim + i ];
    }
    v = monMap[ Monomial( &key[ 0 ], dim ) ].value;

    if ( v == 1 ) {
      for ( i = 0; i < currCandLen; i += 1 ) {
        for ( m = 0; m < dim; m += 1 ) {
          key1[ m ] = key[ m ] / 2 + candidateSet[ i * dim + m ];
        }
        map<Monomial, Monomialvalue>::iterator it =
            monMap.find( Monomial( &key1[ 0 ], dim ) );

        it->second.add( k );
      }

      k++;
      monMap.find( Monomial( &key[ 0 ], dim ) )->second.change = false;
    }
  }
  ASSERT( k <= VERTEX_BOUND, "" );
  //  int vertex_num=k;/* the number of vertex */

  bool run = true;

  while ( run ) {
    run = false;
    for ( i = 0; i < currCandLen; i += 1 ) {
      for ( j = 0; j < dim; j += 1 ) {
        key[ j ] = 2 * candidateSet[ i * dim + j ];
      }
      if ( monMap.find( Monomial( &key[ 0 ], dim ) )->second.change ) {
        monMap.find( Monomial( &key[ 0 ], dim ) )->second.change = false;

        for ( j = 0; j < currCandLen; j += 1 ) {
          for ( m = 0; m < dim; m += 1 ) {
            key1[ m ] = key[ m ] / 2 + candidateSet[ j * dim + m ];
          }
          if ( !( monMap.find( Monomial( &key1[ 0 ], dim ) )
                      ->second.contain(
                          monMap.find( Monomial( &key[ 0 ], dim ) )
                              ->second ) ) ) {
            monMap.find( Monomial( &key1[ 0 ], dim ) )
                ->second.add(
                    monMap.find( Monomial( &key[ 0 ], dim ) )->second );
            run = true;
          }
        }
      }
    }
  }

  /*-----------------------------------------------------------------------------
   *  necessary check
   *-----------------------------------------------------------------------------*/
  if ( monMap.find( Monomial( indices , dim ) ) ==
       monMap.end() ) {
    if ( NULL != ans ) cout << "Corollary 3" << endl;
    delete[] indiceValues;
    return FAILURE;
  }

  for ( i = 1; i < size; i += 1 ) {
    if ( monMap.find( Monomial( indices +  i  * dim, dim ) ) ==
         monMap.end() ) {
      if ( NULL != ans ) cout << "Corollary 3" << endl;
      delete[] indiceValues;
      return FAILURE;
    }
  }
  int dummy = overConvexChecking( poly, indices,  monMap, ans );
  if ( SUCCESS == dummy ) {
    delete[] indiceValues;
    return SUCCESS;

  } else if ( FAILURE == dummy ) {
    delete[] indiceValues;
    return FAILURE;
  }

  *SOSM = (indice_t *) malloc_d( sosLength * node_size );

  i = 0;

  for ( map<Monomial, Monomialvalue>::const_iterator it = monMap.begin();
        it != monMap.end(); ++it ) {
    if ( it->second.value > 0 ) {
      memcpy( ( *SOSM ) + i * dim, it->first.indice, node_size );
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
 *  <numofCons>: is the number of monomials whose total degree less or equal to
 *poly's
 *  total degree
 *  <blockSize>: the nchoosek(td(poly)/2+varnums(poly),varnums(poly))
 *  <b>: is the array of coefs of poly.
 *-----------------------------------------------------------------------------*/

Constraintmatrix *SOSChecker::createConstraintMatrix(
    const Poly_t &poly, const int sosId, double **b, const indice_t *SOSM,
    const int sosLength, const int gLength ) {
  int       i, k, tempI;
  const int varNum = getVarTable<indice_t>().getVarNum( poly.varId );
  //	const int totalDegree=poly_getTotalDegree(poly);
  int                   size    = poly.coef.size();

  const indice_t *      indices = &( poly.indices[ 0 ] );
  const vector<coef_t> &coef    = poly.coef;

  Sparseblock *blockptr;

  printf( "Matrix's size %d,  Constraints size: %d,  G size:  %d\n",
          gLength * gLength, sosLength, gLength );

  int temploc;
  b[ 0 ] = (double *) calloc_d( ( sosLength + 1 ), sizeof( double ) );

  for ( i = 0; i < size; i += 1 ) {
    temploc = findIndex( indices +  i * varNum, SOSM, sosLength, varNum );

    if ( temploc < 0 ) {
      return NULL;
    }
    /* b index start form 1 */
    b[ 0 ][ temploc + 1 ] = coef[ i ];
    /* printf(" %d %f",loc, poly->coef[i]);  */
  }

  ArrangeMatrix **AM = SUPPORT_TABLE.getAMIndex( sosId, &temploc );
  if ( NULL == AM ) {
    AM = createArrangeM( sosId, SOSM, &temploc, sosLength );
  }

  Constraintmatrix *re = createNconstraintmatrix(
      sosLength ); /* every Monomial w.r.t. to a constraint */

  /*-----------------------------------------------------------------------------
   *  use block to  construct  constraints
   *-----------------------------------------------------------------------------*/

  for ( i = 0; i < sosLength; i += 1 ) {
    blockptr = createSparseblock( 1, gLength, i + 1, AM[ i ]->size() );
    tempI    = 1;
    for ( k = 0; k < AM[ i ]->size(); k += 1 ) {
      blockptr->iindices[ tempI ] = ( *( AM[ i ] ) )[ k ].row + 1;
      blockptr->jindices[ tempI ] = ( *( AM[ i ] ) )[ k ].col + 1;
      blockptr->entries[ tempI ]  = 1;
      tempI++;
    }
    frontInsertBlock( re + i + 1, blockptr );
  }

  return re;
}

int SOSChecker::overConvexChecking( const Poly_t &  poly,
                                    const indice_t *   indices,
                                    const map<Monomial, Monomialvalue> &monMap,
                                    PointList *                         ans ) {
  int i, j, num, k;
  int size=poly.coef.size( );
  vector<char> check( size, 0 );
  int          checkSize = size;
  vector<int>  choose( size );
  vector<char> checked( size, 0 );
  vector<int>  tempLoc( size );

  uint32_t set[ 4 ];

  for ( i = 0; i < checkSize; i += 1 ) {
    check[ i ] = i;
  }
  i = 0;
  while ( i < checkSize ) {
    if ( !checked[ check[ i ] ] ) {
      set[ 0 ] = 0u;
      set[ 1 ] = 0u;
      set[ 2 ] = 0u;
      set[ 3 ] = 0u;
      fill( choose.begin(), choose.end(), 0 );
      Monomialvalue curr =
          monMap.find( Monomial( indices + check[ i ] * dim, dim ) )
              ->second;
      num = 0;
      Monomialvalue::add( set, curr );
      checked[ check[ i ] ] = 1;
      tempLoc[ num++ ]      = check[ i ] ;
      choose[ i ]           = 1;
      int tempNum           = 0;
      while ( tempNum != num ) {
        tempNum = num;
        for ( j = 0; j < checkSize; j += 1 ) {
          Monomialvalue temp =
              monMap.find( Monomial( indices +  check[ j  ] * dim, dim ) )
                  ->second;
          if ( Monomialvalue::conjunction( set, temp ) && !choose[ j ] ) {
            Monomialvalue::add( set, temp );
            checked[ check[ j ] ] = 1;
            tempLoc[ num++ ]      =  check[ j ] ;
            choose[ j ]           = 1;
          }
        }
      }

      if ( num < size ) {
        if ( NULL != ans ) {
          cout << "found a split poly" << endl;
        }
        Poly_t tempSub( poly, num, tempLoc );
        int       preAnsLength = 0;
        if ( NULL != ans ) preAnsLength = size_L( ans );
        if ( !polyIsSOS( tempSub, ans ) ) { /* necessary condition */
          if ( NULL != ans ) {
            delListStart( ans, preAnsLength );
          }

          return FAILURE;
        }

        k = 0;

        for ( j = 0; j < checkSize; j += 1 ) {
          if ( 0 == choose[ j ] ) {
            check[ k++ ] = check[ j ];
          }
        }
        checkSize = k;
        i         = -1;
      }
    }
    i++;
  }

  if ( 0 == checkSize ) {
    return SUCCESS;
  }

  if ( checkSize < size ) {
    for ( i = 0; i < checkSize; i += 1 ) {
      tempLoc[ i ] =  check[ i ] ;
    }
    int preAnsLength = 0;
    if ( NULL != ans ) preAnsLength = size_L( ans );

    Poly_t tempSub( poly, checkSize, tempLoc );
    if ( !polyIsSOS( tempSub, ans ) ) {
      if ( NULL != ans ) delListStart( ans, preAnsLength );
      return FAILURE;
    } else {
      return SUCCESS;
    }
  }

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

bool SOSChecker::polyIsSOS( Poly_t &poly, PointList *ans,
                            const int verbose ) {
  int             i, k, j, sosId, sosLength, gLength, genLength, tempLength = 0;
  bool            even;
  indice_t *      SOSM = NULL;
  indice_t *      GSUP = NULL;
  ConvexGenerator generator( poly );
  int             check = generator.easyCheck( ans );

  if ( CONVEX_POLY == check ) {
  } else if ( NOSOS == check ) {
    return false;
  } else if ( EXACTLY_SOS == check ) {
    return true;
  }

  sosId                      = SUPPORT_TABLE.findSupByPoly( poly );
  const indice_t *   indices = &( poly.indices[ 0 ] );

  int                size    =poly.coef.size();

  if ( sosId > -1 ) {
    SOSM = SUPPORT_TABLE.getSOSsup( sosId, &sosLength );
    GSUP = SUPPORT_TABLE.getGsup( sosId, &gLength );
  }
  if ( NULL == SOSM || NULL == GSUP ) {
    start          = clock();
    sosId          = SUPPORT_TABLE.addconvexsosSup( poly );
    indice_t *temp = new indice_t[ size * dim ];
    j              = 0;

    for ( i = 0; i < size; i += 1 ) {
      even = true;
      for ( k = 0; k < dim; k += 1 ) {
        if ( ( indices[  i  * dim + k ] & 1 ) == 1 ) {
          even = false;
          break;
        }
      }
      if ( even ) {
        for ( k = 0; k < dim; k += 1 ) {
          temp[ j * dim + k ] = indices[  i  * dim + k ] / 2;
        }
        j++;
      }
    }
    genLength = j;

    indice_t *candidate = overConvexHull( temp, genLength, dim, &tempLength );

    int dummy = exactConvHull( poly, candidate, tempLength, temp, genLength,
                               &SOSM, sosLength, ans );

    if ( SUCCESS == dummy ) {
      delete[] temp;
      free( SOSM );
      free( candidate );
      return true;
    } else if ( FAILURE == dummy ) {
      delete[] temp;
      free( SOSM );
      free( candidate );
      return false;
    }

    GSUP = (indice_t *) realloc_d( candidate,
                                   tempLength * dim * sizeof( indice_t ) );
    qsortKElem( GSUP, dim, 0, tempLength - 1, compare );
    gLength = tempLength;

    qsortKElem( SOSM, dim, 0, sosLength - 1, compare );
    SUPPORT_TABLE.setGsup( sosId, gLength, GSUP );
    SUPPORT_TABLE.setSOSsup( sosId, sosLength, SOSM );
    delete[] temp;
  }

  Blockmatrix *C = (Blockmatrix *) malloc_d( sizeof( Blockmatrix ) );

  double *b[ 1 ]; // = (double **)malloc_d(sizeof(double *));

  Constraintmatrix *constraints;
  Blockmatrix       X, Z;
  double *          y;
  double            pobj, dobj;

  /*-----------------------------------------------------------------------------
   *  For every term of poly whose total degree less or equal to total degree of
   *poly
   *  which contain the term whose coefficient eqaul to zero. There is a
   *constraint
   *  related it.
   *-----------------------------------------------------------------------------*/

  constraints =
      createConstraintMatrix( poly, sosId, b, SOSM, sosLength, gLength );

  if ( constraints != NULL ) {
    int bsize[ 1 ];

    bsize[ 0 ] = gLength;
    /* printf(" bb size %d", blockSize); */

    createBlockMatrixC( bsize, 1, C );

    //		int numofblock=blockSize;

    initsoln( gLength, sosLength, *C, *b, constraints, &X, &y, &Z );
    csdp_verbose = verbose;

    if ( 0 == easy_sdp( gLength, sosLength, *C, *b, constraints, 0.0, &X, &y,
                        &Z, &pobj, &dobj ) ) {
      if ( NULL != ans ) {
        if ( sosrepresent( ans, X.blocks[ 1 ].data.mat, gLength, sosId ) ) {
          free_prob( gLength, sosLength, *C, *b, constraints, X, y, Z );
          free( C );
          return true;
        }

        free_prob( gLength, sosLength, *C, *b, constraints, X, y, Z );
        free( C );

        return false;
      }

    } else {
      free_prob( gLength, sosLength, *C, *b, constraints, X, y, Z );
      free( C );

      return false;
    }
  } else {
    free( *b );
    return false;
  }
  return false;
}

bool SOSChecker::checksos( bool print ) {
  if ( p.getTotalDegree() <= 0 ) {
    if ( p.isZero() || p.isPositive() ) {
      return true;
    } else {
      return false;
    }
  }

  dim = getVarTable<indice_t>().getVarNum( p.varId );

  Poly_t subp( p );

  PointList *ans = createList( delpoly );

  bool re = polyIsSOS( subp, ans, 1 );

  if ( re && print ) {
    sosPresent( subp, ans );
  }
  delList( ans );

  return re;
}

bool SOSChecker::findSOS( vector<Poly_t> &polyvec ) {
  if ( p.getTotalDegree() <= 0 ) {
    if ( p.isZero() || p.isPositive() ) {
      return true;
    } else {
      return false;
    }
  }

  dim = getVarTable<indice_t>().getVarNum( p.varId );

  Poly_t subp( p );

  PointList *ans = createList( delpoly );

  bool re = polyIsSOS( subp, ans, 1 );
  if ( re ) {
    PointElem *temp = ans->head;
    while ( temp != NULL ) {
      polyvec.push_back( *( (Poly_t *) temp->value ) );
      temp = temp->next;
    }
  }
  delList( ans );
  return re;
}
} // namespace psd
} // namespace aiSat
