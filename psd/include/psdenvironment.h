/**
 * @file   sosproblem.h
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 13:53:32 2015
 *
 * @brief
 *
 *
 */

#ifndef SOSPROBLEM_INC
#define SOSPROBLEM_INC
#include "psdtype.h"
#include "sosproblem.h"
#include "util.h"
#include <algorithm>
#include <map>
#include <string>
#include <vector>

namespace aiSat {
namespace psd {

using std::map;
using std::string;
using std::vector;

class Power {
public:
  Power() {}
  Power( int size )
      : indices( size, 0 ) {}
  ~Power() { indices.clear(); }

  void setValue( int id, int value ) { indices[ id ] = value; }

  void addIndices( const int loc, const int v ) { indices[ loc ] += v; }
  void times( const int loc, const int t ) { indices[ loc ] *= t; }

  friend class PSDEnvironment;

private:
  vector<int> indices;
};

class PSDEnvironment {
public:
  PSDEnvironment()
      : sos_num( 0 ) {
    getVarTable<indice_t>().clear();
  }
  ~PSDEnvironment() { clear(); }

  /**
   * add monomial list
   *
   * @param vindices
   *
   * @return
   */
  int addSupport( const vector<Power> &vindices );

  /**
   * add [ x, y] times [ degrees]
   *
   * @param varVec
   * @param degVec
   *
   * @return
   */
  int addSupport( const vector<int> &varVec, const vector<int> &degVec );

  void solve( const SOSProblem &problem );

  void findSOS( const Poly_t &poly );

  void interpolant( const SOSProblem &sys, const int sep );

  void title();

  void clear();

  int addVar( const string &var );

  bool addVarVec( const string &ID, const vector<int> &value );

  /**
   *
   *
   * @param str
   * @param value
   *
   * @return
   */
  bool addMonoElem( const string &ID, const int monomialListID );

  bool addPolyElem( const string &ID, const Poly_t &poly );

  bool addPolyConsElem( const string &ID, const PolyConstraint &polyCons );

private:
  int                         sos_num;
  map<string, vector<int>>    varMap;
  map<string, int>            monoMap;
  map<string, Poly_t>         polyMap;
  map<string, PolyConstraint> polyConsMap;
  map<int, string>            left_printMap;
  map<int, string>            right_printMap;
};
} // namespace psd
} // namespace aiSat

#endif
