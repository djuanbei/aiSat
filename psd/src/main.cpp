#include "findsos.h"
#include "polyparser.h"
#include "psdenvironment.h"
using namespace aiSat::poly;
using namespace aiSat::psd;
typedef Poly<coef_t, indice_t> Poly_t;

void example1( ){
  PSDEnvironment environment;
  
  string X="x";
  int xid=environment.addVar( X);
  string Y="y";
  int yid=environment.addVar( Y);
  string Z="z";
  int zid=environment.addVar( Z);
  int vNum=zid+1;
  vector<Power> ps;

      
  Power one(vNum);
  ps.push_back( one);
  Power Xp( vNum);
  Xp.setValue(xid, 1 );
  ps.push_back( Xp);
  
  Power Yp( vNum);
  Yp.setValue(yid, 1 );
  ps.push_back( Yp);
  
  Power X2( vNum);
  X2.setValue(xid, 2 );
  ps.push_back( X2);
  
  Power X2y( vNum);
  X2y.setValue(xid, 2 );
  X2y.setValue(yid, 1 );
  ps.push_back( X2y);

  Power Y2( vNum);
  Y2.setValue(yid, 2 );
  ps.push_back( Y2);
  
  Power X3( vNum);
  X3.setValue(xid, 3 );
  ps.push_back( X3);

  int supportID=  environment.addSupport( ps);
  

  Poly_t f1 = parse("4*x^2-y^2");
    
  Poly_t g = parse("x-y");

  PolyConstraint s;
  s.supportId=supportID;
  s.type=GE;
    
  PolyConstraint cg;
  cg.supportId=supportID;
  cg.type=EQ;
  
  SOSProblem problem;
  problem.setRhs( f1);
  problem.addElem(g, cg );
  Poly_t onep = parse("1");
  problem.addElem(onep, s );
  vector<Poly_t> resP;
  problem.sdp_solver(resP, "pro.txt", "result.txt");
  g=g*resP[0]+resP[1];
  g.simplify();
  cout<<g<<endl<<endl;
  
  
  
      
      
  
}
int main() {
  example1( );
  return 0;
  
  Poly_t p = parse("(x+2)^2+y^2+3");

  SOSChecker checker(p);
  if (checker.checksos()) {
    cout << "ok" << endl;
  }

  return EXIT_SUCCESS;
}
