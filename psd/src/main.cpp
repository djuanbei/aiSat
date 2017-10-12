#include "findsos.h"
#include "polyparser.h"
using namespace aiSat::poly;
using namespace aiSat::psd;
typedef Poly<coef_t, indice_t> Poly_t;


int main() {
  Poly_t  p = parse("(x+2)^2+y^2+3");
  
  SOSChecker checker(p);
  if(checker.easychecksos()){
    cout<<"ok"<<endl;
  }
  
  return EXIT_SUCCESS;
}
