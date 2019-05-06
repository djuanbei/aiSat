%{
#include "psdproblem.h"
#include "sosproblemparser.h"
#include "psdtype.h"
#include "psdutil.h"
#include <cstdio>
#include<iostream>
  using namespace  aiSat::psd;
  int yydebug=1;   
  using std::cerr;
  using std::endl;

  SOSP problem;

  string unkown_name;

  extern  int lineNum;

  extern int yyerror(const char *);
  extern int yyerror(string);
  extern int yylex();
  extern int yyparse();
  //  extern int yywrap();
  extern FILE* yyin;
  //  extern SOSP problem;
 
  bool err;
  %}

%union{
  
  double dblVal;
  int  dilVal; 
  string * identifier;
  Poly_t *poly;
  vector<int> *iVec;
  vector<SOSMonomial*> *mVec;
  SOSMonomial * mon;
  PolyConstraint* polycons;
  SOSProblem *prob;
 }
%expect 0
%token<dblVal> NUM
%token<identifier> IDENT
%token<dilVal> INTEGER
%token<dilVal> MONOMIAL
%token<dilVal> VAR 
%token<poly>  POLY
%token<iVec> VAR_LIST
%token<polycons> UNHNOW

%token SOSEQ SOSGEQ SOSLEQ SOSGE SOSLE  
%token SOS  EQPOLY  TOTAL   MONO  FINDSOS INTERP

%type<dilVal> monomial_declar
%type<iVec> var_list
%type<iVec> var_list1
%type<iVec> var_list2

%type<iVec> int_list
%type<iVec> int_list1
%type<iVec> int_list2

%type<mVec> monomial_list
%type<mVec> monomial_list1
%type<mVec> monomial_list2
%type<poly> poly
%type<mon> monomial
%type<polycons> unknow_poly
%type<prob> constraint_statement
%type<prob> constraint_left
%type<prob> constraint_right



%left '+' '-'
%left '*' '/'



%right '^'

/* %nonassoc uminus */

%start program

%%

program:  statement_list


;

statement_list:
statement_list declar_statement
|
statement_list constraint_statement
|

declar_statement
;

declar_statement:
var_declar
|
monomial_declar
|
constant_poly
|
unknow_poly
;

var_declar:

'[' var_list  ']'
|
IDENT '=' '[' var_list  ']'
{
  problem.varMap[ *($1) ]=$4;
  delete $1;
}
;

monomial_declar:

IDENT '=' '{'  monomial_list '}'
{

  problem.monoMap[*($1)]=SOSP::addSOSsup( $4);
  delete $1;
  
  vector<SOSMonomial*>::iterator it=$4->begin();
  for(;it!=$4->end();it++){
    delete (*it);
  }
  delete $4;
  
}
|
IDENT '=' MONO '(' VAR_LIST ',' '[' int_list ']'   ')'
{

  problem.monoMap[*($1)]=SOSP::addSOSsup( $5, $8);
  

  delete $1;

  delete $8;
}
;


constant_poly:

IDENT '=' poly

{
  problem.polyMap[ *($1)]= $3 ;
  delete $1;
}
;

unknow_poly:

IDENT '=' SOS '(' MONOMIAL ')'
{
  $$=new PolyConstraint();
  $$->type=GE;
  $$->supportId= $5;
  problem.polyConsMap[*($1)]=$$;
  delete $1;
}
|
IDENT '=' EQPOLY '(' MONOMIAL ')'
{
  $$=new PolyConstraint();
  $$->type=EQ;
  $$->supportId=$5;
  problem.polyConsMap[*($1)]=$$;
  delete $1;
}
;

int_list:

int_list1 {$$=$1; }
|
int_list2 {$$=$1; }
;

int_list1:
int_list1 ',' INTEGER
{ $1->push_back($3);
  $$=$1;
}
|
INTEGER ',' INTEGER
{
  $$=new vector<int>();
  $$->push_back($1);
  $$->push_back($3);
}
;

int_list2:
int_list2 INTEGER
{
  $1->push_back($2);
  $$=$1;
}
|
INTEGER
{
  $$=new vector<int>();
  $$->push_back($1);
}
;


var_list:
var_list1 
{$$=$1;}
|
var_list2
{$$=$1;}
;
var_list1:
var_list1 ',' IDENT
{
  getVarTable<indice_t>().addVar($3);
  
  int varIndex=getVarTable<indice_t>().findVarIndex($3);
  $1->push_back(varIndex);
  delete $3;
  $$=$1;
}
|
var_list1 ',' VAR
{
  $1->push_back($3);
  
  $$=$1;
  
}
|
IDENT ',' IDENT {
  getVarTable<indice_t>().addVar($1);
  getVarTable<indice_t>().addVar($3);


  $$=new vector<int>();
  int varIndex=getVarTable<indice_t>().findVarIndex($1);
  $$->push_back(varIndex );
  varIndex=getVarTable<indice_t>().findVarIndex($3);
  $$->push_back(varIndex);
  delete $1;
  delete $3;
}
|
VAR ',' IDENT {

  getVarTable<indice_t>().addVar($3 );
  $$=new vector<int>();
  $$->push_back($1 );
  int  varIndex=getVarTable<indice_t>().findVarIndex($3);
  $$->push_back(varIndex);
  delete $3;
}
|
IDENT ',' VAR {

  getVarTable<indice_t>().addVar($1 );
  $$=new vector<int>();
  int  varIndex=getVarTable<indice_t>().findVarIndex($1);
  $$->push_back(varIndex);
  $$->push_back($3 );
  delete $1;
}
|
VAR ',' VAR {
  $$=new vector<int>();
  $$->push_back($1);
  $$->push_back($3 );
  
}
;
var_list2:
var_list2   IDENT 
{
  getVarTable<indice_t>().addVar($2 );
  int varIndex=getVarTable<indice_t>().findVarIndex($2);
  $1->push_back(varIndex);
  delete $2;
  $$=$1;
}
|
var_list2   VAR
{
  $1->push_back($2);
  $$=$1;
}
|
IDENT  {
  getVarTable<indice_t>().addVar($1 );
  int varIndex=getVarTable<indice_t>().findVarIndex($1);
  $$=new vector<int>();
  $$->push_back(varIndex);
  delete $1;
}
|
VAR  {

  $$=new vector<int>();
  $$->push_back($1);
  
}
;

poly:

'(' poly ')'
{
  $$=$2;
}

|
poly '+' poly
{
  $1->add_poly($3);
  $$=$1;
  delete $3;

}
|
poly '-' poly
{
  $3->mult(-1);
  $1->add_poly($3);
  $$=$1;
  delete $3;
  
}
|
poly '*' poly
{
  $1->mult_poly($3);

  $$=$1;
  delete $3;

}
| 
poly '^' INTEGER
{
  $1->pow($3);
  $$=$1;

}

|
POLY
{
  $$=new Poly_t(*($1));
}

|

INTEGER
{
  $$=new Poly_t(true);

  $$->add($1);
}

|
NUM{
  $$=new Poly_t(true);
  $$->add($1);


}
|
VAR
{
  $$=new Poly_t(true);

  Poly_t::Term term;
  term.key.push_back(make_pair($1,1));
  term.cf=1;
  $$->add_term(term);

}

;

monomial_list:
monomial_list1
{$$=$1; }
|
monomial_list2
{$$=$1; }
;

monomial_list1:
monomial_list1 ',' monomial
{
  $1->push_back($3);
  $$=$1;
}
|
monomial ',' monomial
{
  $$=new vector<SOSMonomial*>();
  $$->push_back($1);
  $$->push_back($3);
}
;

monomial_list2:
monomial_list2  monomial
{
  $1->push_back($2);
  $$=$1;
}
|
monomial
{
  $$=new vector<SOSMonomial*>();
  $$->push_back($1);

}
;

monomial:
monomial '*' VAR '^' INTEGER
{
  $1->addIndices($3,$5);
  $$=$1;
}
|
monomial '*' VAR
{

  $1->addIndices($3,1);
  $$=$1;
}
|
VAR '^' INTEGER {
  int varNum=getVarTable<indice_t>().getAllVarNum();
  $$=new SOSMonomial(varNum);

  $$->addIndices($1,$3);
}
|
VAR
{ 
  int varNum=getVarTable<indice_t>().getAllVarNum();
  $$=new SOSMonomial(varNum);
  $$->addIndices($1,1);
}
|
INTEGER{
  int varNum=getVarTable<indice_t>().getAllVarNum();
  $$=new SOSMonomial(varNum);
}
;

/*constraint_statement_list:

  constraint_statement_list  constraint_statement

  |

  constraint_statement
  ;
*/

constraint_statement:

constraint_left  SOSEQ  poly
{

  $$=$1;
  
  if($$->getRhs().getInit( )){
    
    if(NULL!= $3){
      $$->getRhs().add_poly($3);
      delete $3;
    }
  }else{

    $$->setRhs( $3);
  }
  
  
  if(problem.problem->size()>0){

    problem.solve();

  }
  else{
    yyerror(" constraints error");
  }
  
}
|
constraint_left  SOSEQ  '-' poly
{

  $$=$1;
  Poly_t &rhs=$$->getRhs();

  if(rhs.getInit( )){
    
    if(NULL!= $4){
      $4->mult(-1);

      rhs.add_poly($4);
      delete $4;
    }

  }else{
    $4->mult(-1);
    $$->setRhs( $4);
  }
  
  
  if(problem.problem->size()>0){

    problem.solve();

  }
  else{
    yyerror(" constraints error");
  }
  
}

|
constraint_left  SOSGEQ constraint_right
{
  
  $$=$1;

  Poly_t &lrhs=$$->getRhs();

  Poly_t &rrhs=$3->getRhs();
  
  if(lrhs.getInit( )){
    
    if( rrhs.getInit( )){
      lrhs.add_poly(rrhs);
    }

  }else{
    if(rrhs.getInit( )){
      $$->setRhs(rrhs);
    }
  }
  
  int i;
  int left=problem.left_printMap.size();
  
  for(i=0;i< $3->size(); i++){
    $$->addElem($3->getPoly(i), $3->getPolyConstraint(i) );

    problem.left_printMap[left+i]=problem.right_printMap[i];
  }
  problem.right_printMap.clear();

  delete $3;

  if(problem.problem->size()>0){
    problem.solve();

  }
  else{
    yyerror(" constraints error");
  }
}
|
FINDSOS '(' poly ')'
{
  problem.findSOS($3);
  delete $3;

}
|

INTERP '(' constraint_right ',' constraint_right ')'
{
  int sep= $3->size()+1;
  int i;
  for(i=0; i<$5->size(); i++){
    
    $3->addElem( $5->getPoly(i), $5->getPolyConstraint(i) );
  }
  for(i=0; i<$3->size(); i++){
    $3->getPoly(i).mult(-1);

  }
  
  if(!$3->getRhs().getInit( )){
    $3->setRhs(Poly_t());
  }
  if($5->getRhs().getInit( )){
    $3->getRhs().add_poly($5->getRhs());
    $5->deleteRhs();
  }
  $3->getRhs().add(1);
  $3->getRhs().mult(-1);
  
  problem.interpolant( $3, sep);
  
  delete $5;
  delete $3;


}
;


constraint_left:
 
constraint_left '+' poly '*' UNHNOW
{
  $$=$1;

  PolyConstraint *pcons=new PolyConstraint($5);


  int index=$$-> addElem($3, pcons );
  problem.left_printMap[index]=unkown_name;
}


|
constraint_left '+' UNHNOW '*' poly
{
  $$=$1;
  PolyConstraint *pcons=new PolyConstraint($3);

  int index=$$->addElem($5, pcons );
  problem.left_printMap[index]=unkown_name;
}


|
constraint_left '-' poly '*' UNHNOW
{
  $$=$1;

  PolyConstraint *pcons=new PolyConstraint($5);

  $3->mult(-1);

  int index=$$->addElem($3, pcons );
  problem.left_printMap[index]=unkown_name;
}


|


constraint_left '-' UNHNOW '*' poly
{
  $$=$1;
  $5->mult(-1);

  PolyConstraint *pcons=new PolyConstraint($3);

  int index=$$->addElem( $5, pcons );
  problem.left_printMap[index]=unkown_name;
}
|

constraint_left '+' poly
{
  $$=$1;
  //  Poly_t *poly=copyPoly($3);
  $3->mult(-1);

  if(!$$->getRhs().getInit( )){
    $$->setRhs($3);
  }else{
    $$->getRhs().add_poly($3);

  }
}
|
constraint_left '-' poly
{
  $$=$1;

  if(!($$->getRhs().getInit( ))){
    $$->setRhs($3);
  }else{
    $$->getRhs().add_poly($3);

  }
}
|

constraint_left '+' UNHNOW
{
  $$=$1;
  Poly_t * poly=new Poly_t();
  poly->add(1);

  PolyConstraint *pcons=new PolyConstraint( $3);

  int index=$$->addElem(poly, pcons );
  problem.left_printMap[index]=unkown_name;
}
|
constraint_left '-' UNHNOW
{
  $$=$1;
  Poly_t * poly= new Poly_t();
  poly->add(-1);

  PolyConstraint *pcons=new PolyConstraint( $3);

  int index=$$->addElem(poly, pcons );
  problem.left_printMap[index]=unkown_name;
  
}

|
'(' constraint_left ')'
{  $$=$2; }

|
poly '*' UNHNOW
{
  $$=problem.problem;
  PolyConstraint *pcons=new  PolyConstraint( $3);

  int index=$$->addElem($1, pcons);
  problem.left_printMap[index]=unkown_name;
  
}
|
UNHNOW '*' poly
{
  $$=problem.problem;
  PolyConstraint *pcons=new PolyConstraint($1);

  int index=$$->addElem($3, pcons );
  problem.left_printMap[index]=unkown_name;
  
}


|
UNHNOW
{
  $$ =problem.problem;
  Poly_t *poly=new Poly_t();
  poly->add(1);

  
  PolyConstraint *pcons=new PolyConstraint($1);

  int index=$$->addElem(poly, pcons );
  problem.left_printMap[index]=unkown_name;
}
;

constraint_right:


constraint_right '+' poly  '*' UNHNOW
{
  $$=$1;
  $3->mult(-1);

  PolyConstraint *pcons=new PolyConstraint($5);

  int index=$$->addElem($3, pcons );
  problem.right_printMap[index]=unkown_name;
}
|

constraint_right '+' UNHNOW '*' poly
{
  $5->mult(-1);

  PolyConstraint *pcons=new PolyConstraint($3);

  $$=$1;
  int index=$$->addElem($5, pcons );
  problem.right_printMap[index]=unkown_name;
}

|
constraint_right '-' poly '*' UNHNOW
{
  $$=$1;

  PolyConstraint *pcons=new PolyConstraint($5);

  int index=$$->addElem($3, pcons );
  problem.right_printMap[index]=unkown_name;
}

|

constraint_right '-' UNHNOW '*' poly
{
  $$=$1;

  PolyConstraint *pcons= new PolyConstraint($3);

  int index=$$->addElem( $5, pcons );
  problem.right_printMap[index]=unkown_name;

}

|

constraint_right '+' poly
{
  $$=$1;

  if(!$$->getRhs().getInit( )){
    $$->setRhs($3);
  }else{
    $$->getRhs().add_poly($3);
  }
}
|
constraint_right '-' poly
{
  $$=$1;
  $3->mult(-1);

  if($$->getRhs().getInit( )){
    $$->setRhs($3);
  }else{
    $$->getRhs().add_poly($3);

  }
}
|
constraint_right '+' UNHNOW
{
  $$=$1;
  Poly_t * poly= new Poly_t();
  poly->add(-1);

  PolyConstraint *pcons=new PolyConstraint($3);

  int index=$$->addElem(poly, pcons );
  problem.right_printMap[index]=unkown_name;
}
|
constraint_right '-' UNHNOW
{
  $$=$1;
  Poly_t * poly=new Poly_t();
  poly->add(1);

  PolyConstraint *pcons=new PolyConstraint($3);
  int index=$$->addElem(poly, pcons );
  problem.right_printMap[index]=unkown_name;
}
|
'(' constraint_right ')'
{  $$=$2; }

|
poly '*' UNHNOW{

  $$ =new SOSProblem();
  PolyConstraint *pcons=new PolyConstraint($3);
  $1->mult(-1);

  int index=$$->addElem($1, pcons );
  
  problem.right_printMap[index]=unkown_name;    
}
|
UNHNOW '*' poly{
  $$ =new SOSProblem();
  PolyConstraint *pcons=new PolyConstraint($1);
  $3->mult(-1);

  int index=$$->addElem($3, pcons );
  
  problem.right_printMap[index]=unkown_name;    
  
}

|
UNHNOW
{
  $$ =new SOSProblem();
  
  Poly_t *poly=new Poly_t();
  poly->add(-1);

  PolyConstraint *pcons=new PolyConstraint($1);
  int index=$$->addElem(poly, pcons );
  problem.right_printMap[index]=unkown_name;
}
;


%%
#include "sosproblemscan.h"

int yywrap(void){
  return 1;
}


int yyerror(const char * what)
{
  fprintf(stderr, "Error line %d: %s\n", lineNum,what);
  err=true;
  return 1;
}

int yyerror(string what)
{
  cerr << "Error line "<<lineNum<<" "<<what<<endl;
  err=true;
  return 1;
}

void parseProblem(const char * filename){
  lineNum=1;
  static int init=0;
  if(init==0){
    init=1;
  }
  else{
    problem.clear();
  }
  yyin=fopen(filename,"r");
  yyparse();
  fclose(yyin);
}
void parseProblem(){
  std::cout<<"aiSat start:"<<std::endl;
  lineNum=1;
  static int init=0;
  if(init==0){
    init=1;
  }
  else{
    problem.clear();
  }

  yyparse();
  fclose(yyin);
}
