%{

#include <stdlib.h>
#include <string.h>


  int lineNum = 1;

%}

delim [ \t\r]
line [\n]
whitespace {delim}+
digit [0-9]
integer 0|[1-9]{digit}*
letter [a-zA-Z_]
ident {letter}({letter}|{digit})*
number (({digit}+)|({digit}*"."{digit}*)|({digit}+"e"(({digit}+)|("-"{digit}+)))|({digit}*"."{digit}*"e"(({digit}+)|("-"{digit}+)))|({digit}*"."{digit}*"e"("+"({digit}+)|("-"{digit}+))))

%%
{delim}   { ; }
{line}      { lineNum ++;}

"+" {return '+';}
"*" {return '*';}
"-" {return '-';}
"^" {return '^';}
"=" { return '=';}
"," {return ',';}

"==" { return SOSEQ;}
">=" {return SOSGEQ;}
"<=" {return SOSLEQ;}
">" {return SOSGE;}
"<" {return SOSLE;}


"(" {return '(';}
")" {return ')';}
"[" {return '[';}
"]" {return ']';}
"{" {return '{';}
"}" {return '}';}

"findsos" {return FINDSOS; }

"interp" {return INTERP; }

"sos" {return SOS;}

"poly" { return EQPOLY;}

"total" {return TOTAL;}

"monomial" {  return MONO;}


{integer} {  yylval.dilVal=atoi(yytext);  return INTEGER; };

{number}  {  yylval.dblVal=atof(yytext) ; return NUM;  }

{ident} { 

  if(problem.polyMap.find(string(yytext) )!=problem.polyMap.end() ){

    yylval.poly=problem.polyMap[string(yytext)];

    return POLY;

  }else if(problem.polyConsMap.find(string(yytext))!=problem.polyConsMap.end()){

    unkown_name=string(yytext);
    yylval.polycons=problem.polyConsMap[unkown_name];
    
    return UNHNOW;
  }else if(problem.monoMap.find(string(yytext))!=problem.monoMap.end() ){

    yylval.dilVal=problem.monoMap[string(yytext)];
    return MONOMIAL;
  }else if(findVarIndex(yytext)>-1){
    yylval.dilVal=findVarIndex(yytext);
    return VAR;
  }else if (problem.varMap.find(string(yytext))!=problem.varMap.end() ){

    yylval.iVec=problem.varMap[string(yytext)];
    return VAR_LIST;
  }
  

  yylval.identifier= new string(yytext);
  return IDENT;
	
}

{whitespace} 

"#" {
  /* Comment line */
  int c;
  c = yyinput();
  while(c!='\n' && c!=EOF)
  {
    c = yyinput();
  }
	
  if(c == '\n')
  {
    ++lineNum;
  }
}

.  { printf("Unknown Character in line %d : %s -- Ignored\n", lineNum, yytext); }


%%
