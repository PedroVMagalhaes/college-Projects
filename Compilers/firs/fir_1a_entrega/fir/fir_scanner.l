%option c++ prefix="fir_scanner_" outfile="fir_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "fir_parser.tab.h"
static std::ostringstream strlit;

// don't change this
#define yyerror LexerError
%}

SPACE     ([ \t\r]|\n)
INT       [[:digit:]]+

LETTER  [A-Za-z_]
IDENTIFIER   {LETTER}({LETTER}|[[:digit:]])*

%x X_COMMENT
%x X_STRING  X_TIL   X_CHAR    X_CHAR_END
%x X_OCTAL   X_HEXADECIMAL X_OCTAL_INT X_BIN_INT

%%


  /* ====================================================================== */
  /* ====[                      2.2 - COMMENTS                        ]==== */
  /* ====================================================================== */

"!!".*$                /* match the whole line (it is ignored) */

"(*"                   yy_push_state(X_COMMENT);
<X_COMMENT>"*)"        yy_pop_state();
<X_COMMENT>.|"\n"      ;  /* ignore inside comments */


  /* ====================================================================== */
  /* ====[                  1.1   - Tipos de dados                    ]==== */
  /* ====[                  1.1   - Null pointers                     ]==== */
  /* ====[                  1.1   - Procedures                        ]==== */
  /* ====================================================================== */

"int"                    return tTYPE_INT;
"float"                  return tTYPE_FLOAT;
"string"                 return tTYPE_STRING;
"void"					 return tTYPE_VOID;
"sizeof"    			 return tSIZEOF;
"null"                   return tNULLPTR;


  /* ====================================================================== */
  /* ====[                  3.4.2 - Símbolos globais                  ]==== */
  /* ====================================================================== */

"*"                 return tPUBLIC;
"?"                return tREQUIRE;

  /* ====================================================================== */
  /* ====[                 5.5 - Instrução condicional                ]==== */
  /* ====[                 5.5 - Ciclos                               ]==== */
  /* ====[                 5.5 - Begin End Print Read                 ]==== */
  /* ====================================================================== */

"while"                return tWHILE; 
"do"                   return tDO;
"finally"			   return tFINALLY;
"leave"				   return tLEAVE;			
"restart"			   return tRESTART;
"if"                   return tIF; 
"else"                 return tELSE;

  /* ====================================================================== */
  /* ====[                 6 - Operadores de expressões               ]==== */
  /* ====================================================================== */

[\[\](),?]             return *yytext;
[-+*/%~<>?=]          return *yytext;
"=="                   return tEQ;
"!="                   return tNE;
"<="                   return tLE;
">="                   return tGE;
"&&"                   return tAND;
"||"                   return tOR;
"@"                    return tINPUT;


  /* ====================================================================== */
  /* ====[              3   - Delimitadores e separadores             ]==== */
  /* ====[              4.3 - Corpo (de função)                       ]==== */
  /* ====[                 5.3 - Instrução de retorno                 ]==== */
  /* ====================================================================== */


[#\[\](){};:!,]                return *yytext;
"return"               return tRETURN; 
"write"                return tWRITE; 
"writeln"              return tWRITELINE;


  /* ====================================================================== */
  /* ====[                   2.7.1 - inteiros                         ]==== */
  /* ====================================================================== */


"0"                   yy_push_state(X_OCTAL_INT);

<X_OCTAL_INT>[[:xdigit:]]+      { yylval.i = strtoul(yytext, NULL, 16); yy_pop_state(); return tINTEGER; }
<X_OCTAL_INT>.|\n               { yylval.i = strtoul(yytext, NULL, 10); yy_pop_state(); return tINTEGER; }

{INT}                  yylval.i = strtoul(yytext, NULL, 10); return tINTEGER;

 
  /* ====================================================================== */
  /* ====[                   2.7.2 - Floats                           ]==== */
  /* ====================================================================== */


([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)([eE][-+]?[0-9]+)? { yylval.f = strtod(yytext, NULL); return tFLOAT; }

 
  /* ====================================================================== */
  /* ====[                2.6 - Identificadores (nomes)               ]==== */
  /* ====================================================================== */


{IDENTIFIER}  yylval.s = new std::string(yytext); return tID;


  /* ====================================================================== */
  /* ====[              2.7.3 - Cadeias de caracteres                 ]==== */
  /* ====================================================================== */


"'"                            yy_push_state(X_STRING);
<X_STRING>"~"                  yy_push_state(X_TIL);
<X_STRING>\'                   {
                                 yylval.s = new std::string(strlit.str());
                                 strlit.str("");
                                 yy_pop_state();
                                 return tSTRING;
                               }
<X_STRING>.            		   strlit << *yytext;


<X_TIL>n                 strlit << '\n'; yy_pop_state();
<X_TIL>r                 strlit << '\r'; yy_pop_state();
<X_TIL>t                 strlit << '\t'; yy_pop_state();
<X_TIL>~           strlit << '\\'; yy_pop_state();

<X_TIL>[[:xdigit:]]{1,2} {
                                 strlit << (char)(unsigned char)strtoul(yytext, NULL, 16);
                                 yy_pop_state();
                               }
<X_TIL>.                 strlit << *yytext; yy_pop_state();


  /* ====================================================================== */
  /* ====[                    (other characters)                      ]==== */
  /* ====================================================================== */

[[:space:]]         /* ignore remaining white space inside the program */
.                      yyerror(yytext);    /* all other chars: error! */



%%
