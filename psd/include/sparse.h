
/**
 * @file   sparse.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 13:54:11 2015
 *
 * @brief
 *
 *
 */

#ifndef SPARSE_INC
#define SPARSE_INC

#include "config.h"
//#include "psdtype.h"
#include<vector>
#include<string>
namespace aiSat {
namespace psd {
using std::vector;
using std::string;

typedef float coef_t;

struct sparseRowCol {
  uint32_t row : 16;
  uint32_t col : 16;
};

typedef struct sparseRowCol SparseRowCol;


struct SparseSOSblock {
  vector<coef_t> value;
  vector<int>    index; /* location of sosM */

  void addSparse( const int index, const coef_t value );

  SparseSOSblock() {}
};

/*-----------------------------------------------------------------------------
 *  struct sparse is a special matrix have few same nonzero element
 *-----------------------------------------------------------------------------*/
class ArrangeMatrix {
private:
  coef_t multCF; /* coefficient of this matrix */
  int    rowLength;

  /**
   *   start from 0  all elements of matrix is one or zero
   *                      and data record the coordinate of element if one
   *
   */
  vector<SparseRowCol> data;

public:
  ArrangeMatrix( const int rowLength );

  int size() const { return (int) data.size(); }
  int getRowLength() const { return rowLength; }

  coef_t getCF() const { return multCF; }

  void printS();

  void addSparseElem( int i, int j );

  coef_t getValue( int i, int j );

  void mult( const double a );

  string toString() const;

  const SparseRowCol &operator[]( int i ) const { return data[ i ]; }
};
} // namespace psd
} // namespace aiSat

#endif
