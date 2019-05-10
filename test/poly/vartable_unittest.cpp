#include "poly.hpp"
#include "gtest/gtest.h"

#include <algorithm>
#include <random>
namespace {
using namespace aiSat;
using namespace poly;
using namespace std;

TEST( VarTable, init ) {
  VarTable<int> &table = getVarTable<int>();

  EXPECT_EQ( table.getAllVarNum(), 0 );

  table.addVar( "x" );
  EXPECT_EQ( table.getAllVarNum(), 1 );

  table.addVar( "y" );
  EXPECT_EQ( table.getAllVarNum(), 2 );
}

TEST( VarTable, diffTable ) {
  VarTable<int> &table = getVarTable<int>();
  table.clear();

  VarTable<int> &table1 = getVarTable<int>();
  EXPECT_EQ( table1.getAllVarNum(), 0 );

  table.addVar( "x" );
  EXPECT_EQ( table1.getAllVarNum(), 1 );

  table.addVar( "y" );
  EXPECT_EQ( table1.getAllVarNum(), 2 );

  VarTable<char> &table2 = getVarTable<char>();
  table2.addVar( "y" );
  EXPECT_EQ( table2.getAllVarNum(), 1 );
}

TEST( VarTable, findVarIndex ) {
  VarTable<int> &table = getVarTable<int>();
  table.clear();

  string vname = "xyxxy";
  table.addVar( vname );
  EXPECT_EQ( table.getAllVarNum(), 1 );
  EXPECT_EQ( table.findVarIndex( vname ), 0 );
  table.addVar( vname );
  EXPECT_EQ( table.getAllVarNum(), 1 );
}

TEST( VarTable, setVarName ) {
  VarTable<int> &table = getVarTable<int>();
  table.clear();
  string vname = "xyxxy";
  table.addVar( vname );
  EXPECT_EQ( table.getVarName( 0 ), vname );
  string nname = "new";

  table.setVarName( 0, nname );
  EXPECT_EQ( table.getVarName( 0 ), nname );
}

TEST( VarTable, addVarElem ) {
  VarTable<int> &table = getVarTable<int>();
  table.clear();
  int num = 48;
  for ( int i = 0; i < num; i++ ) {
    string name = "a";
    name[ 0 ]   = 'a' + i;
    table.addVar( name );
  }

  EXPECT_EQ( table.getAllVarNum(), num );

  EXPECT_EQ( table.getVarNum( 0 ), 0 );

  vector<int> vars;

  vars.push_back( 1 );
  vars.push_back( 2 );
  vars.push_back( 10 );

  int varId = table.addVarElem( vars );

  EXPECT_EQ( table.getVarNum( varId ), vars.size() );
  vars.push_back( 13 );
  varId = table.addVarElem( vars );
  EXPECT_EQ( table.getVarNum( varId ), vars.size() );

  vars.push_back( 22 );
  varId = table.addVarElem( vars );
  EXPECT_EQ( table.getVarNum( varId ), vars.size() );
  table.addVar( "xyz" );
  table.addVar( "xy" );
  EXPECT_EQ( table.getVarNum( varId ), vars.size() );
}

TEST( VarTable, findVarElem ) {
  VarTable<int> &table = getVarTable<int>();
  table.clear();
  int num = 20;
  table.reset( num );
  EXPECT_EQ( table.getAllVarNum(), num );
  EXPECT_EQ( table.getVarNum( 0 ), 0 );

  vector<int> vars;

  vars.push_back( 1 );
  vars.push_back( 2 );
  vars.push_back( 10 );
  int id = table.addVarElem( vars );

  EXPECT_EQ( table.findVarElem( vars ), id );
}

TEST( VarTable, getVarElem ) {
  VarTable<int> &table = getVarTable<int>();
  table.clear();
  int num = 20;
  table.reset( num );
  vector<int> allvars;
  for ( int i = 0; i < num; i++ ) {
    allvars.push_back( i );
  }
  shuffle( allvars.begin(), allvars.end(), std::default_random_engine() );

  vector<int> vars( allvars.begin(), allvars.begin() + 3 );
  int         vid = table.addVarElem( vars );
  vector<int> revars;
  table.getVarElem( vid, revars );
  EXPECT_EQ( vars.size(), revars.size() );
}

TEST( VarTable, getVarNum ) {
  VarTable<int> &table = getVarTable<int>();
  table.clear();
  int num = 20;
  table.reset( num );

  vector<int> allvars;
  for ( int i = 0; i < num; i++ ) {
    allvars.push_back( i );
  }
  for ( int i = 0; i < 10; i++ ) {
    int         len = rand() % num;
    vector<int> vars( allvars.begin(), allvars.begin() + len );
    int         vid = table.addVarElem( vars );
    EXPECT_EQ( table.getVarNum( vid ), len );
  }
}

TEST( VarTable, mergeVar ) {
  VarTable<int> &table = getVarTable<int>();
  table.clear();
  int num = 20;
  table.reset( num );

  vector<int> allvars;
  for ( int i = 0; i < num; i++ ) {
    allvars.push_back( i );
  }
  for ( int i = 0; i < 20; i++ ) {
    int         len = rand() % num;
    vector<int> vars( allvars.begin(), allvars.begin() + len );
    int         len1 = rand() % num;
    vector<int> vars1( allvars.begin(), allvars.begin() + len1 );
    int         vid  = table.addVarElem( vars );
    int         vid1 = table.addVarElem( vars1 );
    int         mid  = table.mergeVar( vid, vid1 );
    set<int>    temp( vars.begin(), vars.end() );
    temp.insert( vars1.begin(), vars1.end() );

    vector<int> mvars;
    table.getVarElem( mid, mvars );
    EXPECT_EQ( mvars.size(), temp.size() );
    for ( vector<int>::iterator it = mvars.begin(); it != mvars.end(); it++ ) {
      EXPECT_TRUE( temp.find( *it ) != temp.end() );
    }
  }
}

TEST( VarTable, getConvertMap ) {
  VarTable<int> &table = getVarTable<int>();
  table.clear();
  int num = 20;
  table.reset( num );

  vector<int> allvars;
  for ( int i = 0; i < num; i++ ) {
    allvars.push_back( i );
  }
  for ( int i = 0; i < 20; i++ ) {
    int         len = rand() % num;
    vector<int> vars( allvars.begin(), allvars.begin() + len );
    int         len1 = rand() % num;
    vector<int> vars1( allvars.begin(), allvars.begin() + len1 );
    int         vid  = table.addVarElem( vars );
    int         vid1 = table.addVarElem( vars1 );
    int         mid  = table.mergeVar( vid, vid1 );
    set<int>    temp( vars.begin(), vars.end() );
    temp.insert( vars1.begin(), vars1.end() );
    vector<int> mapIndex( vars.size() );
    table.getConvertMap( vid, mid, mapIndex );
    vector<int> mvars;
    table.getVarElem( mid, mvars );
    for ( int j = 0; j < vars.size(); j++ ) {
      EXPECT_EQ( vars[ j ], mvars[ mapIndex[ j ] ] );
    }
  }
}

TEST( VarTable, clear ) {
  VarTable<int> &table = getVarTable<int>();
  table.clear();
  EXPECT_EQ( table.getAllVarNum(), 0 );
  EXPECT_EQ( table.getVarNum( 0 ), 0 );
}
} // namespace

int main( int argc, char **argv ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}
