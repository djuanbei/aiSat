#include "poly.hpp"
#include "polyparser.h"
#include "gtest/gtest.h"
namespace {
using namespace aiSat;
using namespace poly;
typedef Poly<coef_t, indice_t> Poly_t;

TEST( Poly, init ) {
  VarTable<indice_t> &table = getVarTable<indice_t>();

  Poly_t p  = parse( "x+1" );
  Poly_t p1 = parse( "y+1" );

  EXPECT_EQ( table.getVarNum( p.getVarId() ), 1 );
  EXPECT_EQ( table.getVarNum( p1.getVarId() ), 1 );
  p1.mult_poly( p );
  EXPECT_EQ( table.getVarNum( p1.getVarId() ), 2 );
  p1.mult_poly( p );
  EXPECT_EQ( table.getVarNum( p1.getVarId() ), 2 );
  p1 = parse( "x*y*z+1+h^3" );
  EXPECT_EQ( table.getVarNum( p1.getVarId() ), 4 );
  Poly_t p3( p1.getVarId(), 1000, 10, 4 );
  EXPECT_EQ( p3.getVarNum(), 4 );
}
TEST( Poly, getSize ) {
  Poly_t p = parse( "x+1" );
  EXPECT_EQ( p.getSize(), 2 );
  p = parse( "(x+1)*(y+1)" );
  EXPECT_EQ( p.getSize(), 4 );
}
TEST( Poly, getVarId ) {
  Poly_t p( 1 );
  EXPECT_EQ( p.getVarId(), 1 );
}
TEST( Poly, getVarNum ) {
  Poly_t p = parse( "(x+1)*(y+1)" );
  EXPECT_EQ( p.getVarNum(), 2 );
  Poly_t p1 = parse( "(z+100)" );
  p.mult_poly( p1 );
  EXPECT_EQ( p.getVarNum(), 3 );
}
TEST( Poly, getCF ) {
  VarTable<indice_t> &             table = getVarTable<indice_t>();
  Poly_t                           p     = parse( "(x+1)*(y+1)+x^2+x^2*y^3" );
  int                              x     = table.addVar( VAR( x ) );
  int                              y     = table.addVar( VAR( y ) );
  Poly_t::Term                     term;
  vector<pair<indice_t, indice_t>> key; // var, power
  key.push_back( make_pair( x, 2 ) );
  term.key = key;
  EXPECT_EQ( 1, p.getCF( term ) );
  key.push_back( make_pair( y, 3 ) );
  term.key = key;
  EXPECT_EQ( 1, p.getCF( term ) );
}

TEST( Poly, eval ) {
  VarTable<indice_t> &table = getVarTable<indice_t>();
  Poly_t              p     = parse( "(x+1)*(y+1)+x^2+x^2*y^3" );
  int                 x     = table.addVar( VAR( x ) );
  int                 y     = table.addVar( VAR( y ) );

  vector<Poly_t::assignElem> value;
  Poly_t::assignElem         temp;
  temp.var   = x;
  temp.value = 1;
  value.push_back( temp );

  temp.var   = y;
  temp.value = 1;
  value.push_back( temp );

  Poly_t p1 = p.eval( value );
  EXPECT_TRUE( p1.isConstant() );

  for ( int i = 0; i < 20; i++ ) {
    vector<Poly_t::assignElem> value1, value2;
    Poly_t::assignElem         tempx, tempy;

    tempx.var   = x;
    tempx.value = rand() % 20;

    tempy.var   = y;
    tempy.value = rand() % 20;
    value1.push_back( tempx );
    Poly_t p3 = p.eval( value1 );
    EXPECT_FALSE( p3.isConstant() );
    value1.push_back( tempy );

    value2.push_back( tempy );
    value2.push_back( tempx );

    Poly_t p1 = p.eval( value1 );

    Poly_t p2 = p.eval( value2 );

    EXPECT_EQ( p1.getConstant(), p2.getConstant() );
  }
}

TEST( Poly, equal ) {
  VarTable<indice_t> &table = getVarTable<indice_t>();

  Poly_t p = parse( "(x+1)*(y+1)+x^2+x^2*y+2*j+33*z" );

  // VarTable<indice_t>& table = getVarTable<indice_t>();
  int x = table.addVar( VAR( x ) );
  int y = table.addVar( VAR( y ) );

  vector<Poly_t::assignElem> value1, value2;
  Poly_t::assignElem         tempx, tempy;

  tempx.var   = x;
  tempx.value = rand() % 20;

  tempy.var   = y;
  tempy.value = rand() % 20;

  value1.push_back( tempx );
  value1.push_back( tempy );

  value2.push_back( tempy );
  value2.push_back( tempx );

  Poly_t p1 = p.eval( value1 );

  Poly_t p2 = p.eval( value2 );

  EXPECT_TRUE( p1 == p2 );
}
} // namespace

int main( int argc, char **argv ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}
