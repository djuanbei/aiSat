%{
#include "polyparser.h"
  
#include<ctype.h>
#include<set>
#include<map>
#include <cstdio>
#include<iostream>
  using namespace aiSat;
  using namespace poly;
  
  int yydebug=1;   
  using std::cerr;
  using std::endl;
  using std::set;
  using std::vector;
  using std::map;

  typedef    Poly<coef_t, indice_t> Poly_t;

  string unkown_name;

  extern  int lineNum;

  extern int yyerror(const char *);
  extern int yyerror(string);
  extern int yylex();
  extern int yyparse();

  extern FILE* yyin;
 
  bool err;
  int varId;
  Poly_t gloabl_poly;
  %}

%union{
  
  double dblVal;
  int  dilVal; 
  string * varname;
  Poly_t *poly;
  vector<indice_t> *iVec;

 }

// %expect 0

%token<dilVal> INTEGER
%token<dblVal> NUM
%token<varname> VAR


%type<iVec> TERM
%type<poly> POLY


%left '+' '-'
%left '*' 
%left '^' 

%start REPOLY

%%

REPOLY:
POLY 
{
  gloabl_poly= Poly_t(*($1));
  delete $1;
};

POLY:
'(' POLY ')'
{
  $$=$2;
}
|

POLY  '*'   POLY
{
  $1->mult_poly(*($3));
  $$=$1;
  delete $3;
}

|
POLY '-' POLY
{
  $3->mult(-1);
  $1->add_poly(*($3));
  $$=$1;
  delete $3;
}
|
POLY '+' POLY
{

  $1->add_poly(*($3));

  $$=$1;
  delete $3;
}

|
TERM
{

  $$=new Poly_t(varId);
  
  map<int, int> keyMap;
  
  VarTable<indice_t> &table=getVarTable<indice_t>();

  vector<indice_t> vars;
  table.getVarElem(varId, vars);
  for(size_t i=0; i< vars.size(); i++){
    keyMap[vars[i]]=i;
  }
  fill(vars.begin(), vars.end(), 0);

  for(size_t i=0; i< $1->size(); i+=2){
    vars[ keyMap[(*$1)[i]]]+=(*$1)[i+1];
  }
  
  $$->add_term(vars, 1);
  
  delete $1;
}
|
NUM
{

  $$=new Poly_t(varId);
  $$->add($1);
}
|
INTEGER
{

  $$=new Poly_t(varId);
  $$->add($1);
}
;

TERM:

VAR '^' INTEGER
{
  VarTable<indice_t> &table=getVarTable<indice_t>();
  int index=table.findVarIndex(*($1));
  $$=new  vector<indice_t> ();
  $$->push_back(index);
  $$->push_back($3);
  delete  $1;
}
|
VAR
{

  VarTable<indice_t> &table=getVarTable<indice_t>();
  int index=table.findVarIndex(*($1));
  $$=new  vector<indice_t> ();
  $$->push_back(index);
  $$->push_back(1);
  delete  $1;
}
;

%%
#include "polyscan.h"

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

Poly_t aiSat::poly::parse(const string &str){
  
  char buffer[1024];
  int k=0;
  int size=str.size();
  set<int>  vars;
  VarTable<indice_t> &table=getVarTable<indice_t>();
  for(int i=0; i< size; i++){
    k=0;
    while(k<size && isalpha(str[i]) ){
      buffer[k++]=str[i++];
    }
    
    if(k>0){
      buffer[k]='\0';
      string vname(buffer);

      table.addVar(vname);
      vars.insert(table.findVarIndex(vname));
    }
  }
  vector<indice_t> varsids(vars.begin(), vars.end());

  varId=table.addVarElem(varsids);
  yyin=tmpfile();
  fputs( str.c_str(), yyin);
  rewind(yyin);
  yyparse();
  fclose(yyin);

  return gloabl_poly;
  
}
