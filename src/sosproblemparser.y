%{

#include "sosproblem.h"
#include "poly.h"
#include "vartable.h"
#include "sosproblemparser.h"
#include "sosproblem.h"
  
#include <cstdio>
#include<iostream>
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
  Poly *poly;
  vector<int> *iVec;
  vector<Monomial*> *mVec;
  Monomial * mon;
  polyConstraint* polycons;
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
%nonassoc uminus

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
  
  vector<Monomial*>::iterator it=$4->begin();
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
  $$=createPolyConstraint();
  $$->type=GE;
  $$->supportId= $5;
  problem.polyConsMap[*($1)]=$$;
  delete $1;
}
|
IDENT '=' EQPOLY '(' MONOMIAL ')'
{
  $$=createPolyConstraint();
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
  addVar($3->c_str() );
  
  int varIndex=findVarIndex($3->c_str());
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
  addVar($1->c_str() );
  addVar($3->c_str() );

  $$=new vector<int>();
  int varIndex=findVarIndex($1->c_str());
  $$->push_back(varIndex );
  varIndex=findVarIndex($3->c_str());
  $$->push_back(varIndex);
  delete $1;
  delete $3;
}
|
VAR ',' IDENT {

  addVar($3->c_str() );
  $$=new vector<int>();
  $$->push_back($1 );
  int  varIndex=findVarIndex($3->c_str());
  $$->push_back(varIndex);
  delete $3;
}
|
IDENT ',' VAR {

  addVar($1->c_str() );
  $$=new vector<int>();
  int  varIndex=findVarIndex($1->c_str());
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
  addVar($2->c_str() );
  int varIndex=findVarIndex($2->c_str());
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
  addVar($1->c_str() );
  int varIndex=findVarIndex($1->c_str());
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
  p_add_Poly_assign_del($1,$3); 
  $$=$1;

}
|
poly '-' poly
{
  p_mult_cons_assign($3,-1); 
  p_add_Poly_assign_del($1,$3); 
  $$=$1;

  
}
|
poly '*' poly
{

  p_mult_Poly_assign_del($1,$3);
  $$=$1;


}
| 
poly '^' INTEGER
{
    
  p_pow_assign($1,$3);
  
  $$=$1;

}

|
POLY
{
  $$=copyPoly($1);
}

|

INTEGER
{

  $$=createPoly();
  polyChangeVarId($$,1);
  p_add_cons_assign($$,$1);


}

|
NUM{

  $$=createPoly();
  polyChangeVarId($$,1);
  p_add_cons_assign($$,$1);

}
|
VAR
{
  $$=createPoly();
  polyChangeVarId($$,1);
  int varNum=getAllVarNum();//varNum(1);

  indice_t *key=new indice_t[varNum];
  int i;	
  for(i=0;i<varNum;i++)
    key[i]=0;
  key[$1]=1;
  internal_addTerm($$,key,1);
  delete[  ] key;

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
  $$=new vector<Monomial*>();
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
  $$=new vector<Monomial*>();
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
  int varNum=getAllVarNum();//getvarNum(1);
  $$=new Monomial(varNum);

  $$->addIndices($1,$3);
}
|
VAR
{ 
  int varNum=getAllVarNum();//varNum(1);
  $$=new Monomial(varNum);
  $$->addIndices($1,1);
}
|
INTEGER{
  int varNum=getAllVarNum();//varNum(1);
  $$=new Monomial(varNum);
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

  
  if(NULL!=$$->rhs){
    
    if(NULL!= $3)
      p_add_Poly_assign_del($$->rhs, $3 );
  }else{

      setSOSRhs($$, $3);
  }
  
  
  if(problem.problem->size>0){

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

    if(NULL!=$$->rhs){
    
    if(NULL!= $4){
      p_mult_cons_assign($4,-1); 
      p_add_Poly_assign_del($$->rhs, $4 );
      
    }

  }else{
      p_mult_cons_assign($4,-1); 
      setSOSRhs($$, $4);
  }
  
  
  if(problem.problem->size>0){

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
    
  if(NULL!=$$->rhs){
    
    if(NULL!= $3->rhs)
      p_add_Poly_assign($$->rhs, $3->rhs );
  }else{
    if(NULL!=$3->rhs){
      setSOSRhs($$, $3->rhs);
    }
  }
  
  int i;
  int left=problem.left_printMap.size();
  
  for(i=0;i< $3->size; i++){
    addConstraint($$,$3->polys[i], $3->polyConstraints[i] );
    problem.left_printMap[left+i]=problem.right_printMap[i];
  }
  problem.right_printMap.clear();
  free($3->polys );
  free($3->polyConstraints );
  free($3);

  if(problem.problem->size>0){
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
  deletePoly($3);
}
|

INTERP '(' constraint_right ',' constraint_right ')'
{
  int sep= $3->size+1;
  int i;
  for(i=0; i<$5->size; i++){
    addConstraint($3, $5->polys[i], $5->polyConstraints[i] );
  }
  for(i=0; i<$3->size; i++){
    p_mult_cons_assign($3->polys[i],-1);
  }
  if(NULL==$3->rhs){
    $3->rhs=createPoly();
  }
  if(NULL!=$5->rhs)
    p_add_Poly_assign_del($3->rhs, $5->rhs);
  //  p_mult_cons_assign($3->rhs,-1);
  
  problem.interpolant( $3, sep);
  
  free($5->polys );
  free($5->polyConstraints );
  free($5);
  deleteSOSProblem($3);
  
  /* free($3->polys ); */
  /* free($3->polyConstraints ); */
  /* free($3); */
}
;


constraint_left:
 
constraint_left '+' poly '*' UNHNOW
{
  $$=$1;

  PolyConstraint *pcons=copyPolyConstraint($5);
  int index=addConstraint($$,$3, pcons );
  problem.left_printMap[index]=unkown_name;
}


|
constraint_left '+' UNHNOW '*' poly
{
  $$=$1;
  PolyConstraint *pcons=copyPolyConstraint($3);

  int index=addConstraint($$,$5, pcons );
  problem.left_printMap[index]=unkown_name;
}


|
constraint_left '-' poly '*' UNHNOW
{
  $$=$1;

  PolyConstraint *pcons=copyPolyConstraint($5);
  
  p_mult_cons_assign($3,-1  );
  int index=addConstraint($$,$3, pcons );
  problem.left_printMap[index]=unkown_name;
}


|


constraint_left '-' UNHNOW '*' poly
{
  $$=$1;

  p_mult_cons_assign($5,-1  );
  PolyConstraint *pcons=copyPolyConstraint($3);
  int index=addConstraint($$, $5, pcons );
  problem.left_printMap[index]=unkown_name;
}
|

constraint_left '+' poly
{
  $$=$1;
  //  Poly *poly=copyPoly($3);
  p_mult_cons_assign($3,-1  );
  if(NULL==$$->rhs){
    setSOSRhs($$,$3);
  }else{
    p_add_Poly_assign($$->rhs,$3 );
  }
}
|
constraint_left '-' poly
{
  $$=$1;
  //  Poly *poly=copyPoly($3);
  //  p_mult_cons_assign($3,-1  );
  if(NULL==$$->rhs){
    setSOSRhs($$,$3);
  }else{
    p_add_Poly_assign($$->rhs,$3 );
  }
}
|

constraint_left '+' UNHNOW
{
  $$=$1;
  Poly * poly=createPoly();
  resetOne(poly);
  PolyConstraint *pcons=copyPolyConstraint($3);
  int index=addConstraint($$,poly, pcons );
  problem.left_printMap[index]=unkown_name;
}
|
constraint_left '-' UNHNOW
{
  $$=$1;
  Poly * poly=createPoly();
  resetOne(poly);
  p_mult_cons_assign(poly,-1);
  PolyConstraint *pcons=copyPolyConstraint($3);
  int index=addConstraint($$,poly, pcons );
  problem.left_printMap[index]=unkown_name;
  
}

|
'(' constraint_left ')'
{  $$=$2; }

|
poly '*' UNHNOW
{
  $$=problem.problem;
  PolyConstraint *pcons=copyPolyConstraint($3);
  int index=addConstraint($$,$1, pcons );
  problem.left_printMap[index]=unkown_name;
  
}
|
UNHNOW '*' poly
{
  $$=problem.problem;
  PolyConstraint *pcons=copyPolyConstraint($1);
  int index=addConstraint($$,$3, pcons );
  problem.left_printMap[index]=unkown_name;
  
}


|
UNHNOW
{
  $$ =problem.problem;
  Poly *poly=createPoly();
  resetOne(poly);
  
  PolyConstraint *pcons=copyPolyConstraint($1);
  int index=addConstraint($$,poly, pcons );
  problem.left_printMap[index]=unkown_name;
}
;

constraint_right:


constraint_right '+' poly  '*' UNHNOW
{
  $$=$1;

  p_mult_cons_assign($3,-1);
  PolyConstraint *pcons=copyPolyConstraint($5);
  int index=addConstraint($$,$3, pcons );
  problem.right_printMap[index]=unkown_name;
}
|

constraint_right '+' UNHNOW '*' poly
{

  p_mult_cons_assign($5,-1);
  PolyConstraint *pcons=copyPolyConstraint($3);
  $$=$1;
  int index=addConstraint($$,$5, pcons );
  problem.right_printMap[index]=unkown_name;
}

|
constraint_right '-' poly '*' UNHNOW
{
  $$=$1;

  PolyConstraint *pcons=copyPolyConstraint($5);

  int index=addConstraint($$,$3, pcons );
  problem.right_printMap[index]=unkown_name;
}

|


constraint_right '-' UNHNOW '*' poly
{
  $$=$1;

  PolyConstraint *pcons=copyPolyConstraint($3);
  int index=addConstraint($$, $5, pcons );
  problem.right_printMap[index]=unkown_name;

}

|

constraint_right '+' poly
{
  $$=$1;

  if(NULL==$$->rhs){
    setSOSRhs($$,$3);
  }else{
    p_add_Poly_assign($$->rhs,$3 );
  }
}
|
constraint_right '-' poly
{
  $$=$1;
  p_mult_cons_assign($3,-1  );
  if(NULL==$$->rhs){
    setSOSRhs($$,$3);
  }else{
    p_add_Poly_assign($$->rhs,$3 );
  }
}
|
constraint_right '+' UNHNOW
{
  $$=$1;
  Poly * poly=createPoly();
  resetOne(poly);
  p_mult_cons_assign(poly,-1);
  PolyConstraint *pcons=copyPolyConstraint($3);
  int index=addConstraint($$,poly, pcons );
  problem.right_printMap[index]=unkown_name;
}
|
constraint_right '-' UNHNOW
{
  $$=$1;
  Poly * poly=createPoly();
  resetOne(poly);

  PolyConstraint *pcons=copyPolyConstraint($3);
  int index=addConstraint($$,poly, pcons );
  problem.right_printMap[index]=unkown_name;
}
|
'(' constraint_right ')'
{  $$=$2; }

|
poly '*' UNHNOW{

  $$ =createSOSProblem();
  PolyConstraint *pcons=copyPolyConstraint($3);
  p_mult_cons_assign($1,-1);
  int index=addConstraint($$,$1, pcons );
  
  problem.right_printMap[index]=unkown_name;    
}
|
UNHNOW '*' poly{
  $$ =createSOSProblem();
  PolyConstraint *pcons=copyPolyConstraint($1);
  p_mult_cons_assign($3,-1);
  int index=addConstraint($$,$3, pcons );
  
  problem.right_printMap[index]=unkown_name;    
  
}


|
UNHNOW
{
  $$ =createSOSProblem();
  
  Poly *poly=createPoly();
  resetOne(poly);
  p_mult_cons_assign(poly,-1);
  PolyConstraint *pcons=copyPolyConstraint($1);
  int index=addConstraint($$,poly, pcons );
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
