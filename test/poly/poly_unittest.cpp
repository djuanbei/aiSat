#include "poly.hpp"
#include "gtest/gtest.h"
#include "polyparser.h"
namespace {
using namespace aiSat;
using namespace poly;
typedef Poly<coef_t, indice_t> Poly_t;

TEST(Poly,init){
  VarTable<indice_t>& table= getVarTable<indice_t>();
  
  Poly_t p=parse("x+1");
  Poly_t p1=parse("y+1");

  EXPECT_EQ(table.getVarNum(p.getVarId()) ,1);
  EXPECT_EQ(table.getVarNum(p1.getVarId()) ,1);
  p1.mult_poly(p);
  EXPECT_EQ(table.getVarNum(p1.getVarId()) ,2);
  p1.mult_poly(p);
  EXPECT_EQ(table.getVarNum(p1.getVarId()) ,2);
  p1=parse("x*y*z+1+h^3");
  EXPECT_EQ(table.getVarNum(p1.getVarId()) ,4);
}


}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

