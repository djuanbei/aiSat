%{

#include <stdlib.h>
#include <string.h>
#include<iostream>
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
"^" {return '^';}
"-" {return '-';}
"(" {return '(';}
")" {return ')';}
";" {return ';';}

{integer} {
  yylval.dilVal=atoi(yytext);  return INTEGER; };

{number}  {  yylval.dblVal=atof(yytext) ;

  return NUM;  }

{whitespace} {}

{ident} {
  string *temp=new string(yytext);

  yylval.varname=temp;
  return VAR;
}

.  { printf("Unknown Character in line %d : %s -- Ignored\n", lineNum, yytext); }
%%
