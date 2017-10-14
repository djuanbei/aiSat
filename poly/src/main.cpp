
/**
 * @file   main.cpp
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Mon Oct  9 19:45:03 2017
 * 
 * @brief  
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include "poly.hpp"
#include "subpoly.hpp"
#include "polyparser.h"

using namespace aiSat::poly;

typedef Poly<coef_t, indice_t> Poly_t;

typedef Subpoly<coef_t, indice_t> Subpoly_t;

int main(int argc, char* argv[]) {
  Poly_t  p=  parse("x^2+y^2+1+(x+23)^2");

  cout<<p.getCF("x^2")<<endl;
  
  // Poly_t  p = parse("(x+1)*(y+1)+x^2+x^2*y+2*j+33*z");
  
  vector<int>locs1,lcos2;
  locs1.push_back(1);
  locs1.push_back(2);
  locs1.push_back(3);
  Subpoly_t sp1(p, locs1);

  Subpoly_t sp2(p, locs1);

  if(sp1==sp2){
    cout<<"sss"<<endl;
  }


  // Poly_t  p = parse("(x+1)*(y+1)+x^2+x^2*y+2*j+33*z");
  // VarTable<indice_t>& table = getVarTable<indice_t>();
  // int x=table.addVar(VAR(x));
  // int y=table.addVar(VAR(y));  
  // vector<Poly_t::assignElem> value1, value2;
  // Poly_t::assignElem tempx, tempy;

  // tempx.var=x;
  // tempx.value=0;//rand()%20;
    
  // tempy.var=y;
  // tempy.value=0;//rand()%20;
  
  // value1.push_back(tempx);
  // value1.push_back(tempy);

  // value2.push_back(tempy);
  // value2.push_back(tempx);
    
  // Poly_t p1= p.eval(value1);
    
  // Poly_t p2= p.eval(value2);
  // cout<<p1<<endl;
  // cout<<p2<<endl;
  
  // if(p1==p2){
  //   cout<<"right"<<endl;
  // }


  
  // VarTable<indice_t> table= getVarTable<indice_t>();
  
  // table.addVar(VAR(x));
  
  
  

  // int length = 10;
  // const int dim = 7;
  // const int domain = 5;
  // indice_t *points = randPointSet(length, dim, domain);

  // int relength;
  // indice_t *cand = overConvexHull(points, length, dim, &relength);
  // printf("%d\n %s", relength, cand);
  // printf(" argc %d file %s\n", argc, argv[0]);

  // Poly_t p = parse("(x+2)*(y+1)");
  // string str = p.toString();
  // cout << str << endl;
  // int size = p.getSize();

  // cout << p.toString() << endl;
  // Poly_t p1 = parse("y+1");

  // VarTable<indice_t>& table = getVarTable<indice_t>();
  // // table.clear();
  // cout << table.getVarNum(p.getVarId()) << endl;
  // cout << table.getVarNum(p1.getVarId()) << endl;
  // Poly<coef_t, indice_t> p=parse("x+1");
  // string str=p.toString();
  // cout<<str<<endl;

  // Poly<double, indice_t> p1, p2;
  // Poly<double, indice_t> p3 = p1 * p2;
  return EXIT_SUCCESS;
}
