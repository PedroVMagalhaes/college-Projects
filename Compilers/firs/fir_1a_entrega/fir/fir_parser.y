%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <algorithm>
#include <memory>
#include <cstring>
#include <cdk/compiler.h>
#include <cdk/types/types.h>
#include "ast/all.h"
#define LINE                         compiler->scanner()->lineno()
#define yylex()                      compiler->scanner()->scan()
#define yyerror(compiler, s)         compiler->scanner()->error(s)
//-- don't change *any* of these --- END!
%}

%parse-param {std::shared_ptr<cdk::compiler> compiler}

%union {
  //--- don't change *any* of these: if you do, you'll break the compiler.
  YYSTYPE() : type(cdk::primitive_type::create(0, cdk::TYPE_VOID)) {}
  ~YYSTYPE() {}
  YYSTYPE(const YYSTYPE &other) { *this = other; }
  YYSTYPE& operator=(const YYSTYPE &other) { type = other.type; return *this; }

  std::shared_ptr<cdk::basic_type> type;        /* expression type */
  //-- don't change *any* of these --- END!

  int                   i;	/* integer value */
  float                f;
  std::string          *s;	/* symbol name or string literal */
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;

  fir::block_node       *block;
};

%token tAND tOR tNE tLE tGE tSIZEOF
%token tINPUT tWRITE tWRITELINE
%token tPUBLIC tPRIVATE tREQUIRE
%token tTYPE_POINTER tTYPE_FLOAT tTYPE_INT tTYPE_STRING tTYPE_VOID tPROCEDURE
%token tWHILE tDO
%token tIF tTHEN tELIF tELSE 
%token tLEAVE tRESTART tRETURN tFINALLY


%token <i> tINTEGER
%token <f> tFLOAT
%token <s> tSTRING tID
%token<expression> tNULLPTR

%type<node> instruction return iffalse restart leave body finally
%type<sequence> file instructions opt_instructions bodys 
%type<sequence> expressions opt_expressions
%type<expression> expression integer real opt_initializer defaultreturn
%type<lvalue> lvalue
%type<block> block prologo epilogo

%type<node>     declaration  argdec vardec fundec fundef
%type<sequence> declarations argdecs vardecs opt_vardecs


%type<s> string
%type<type> data_type void_type

%{
// %type<ids> identifiers  
%}

%nonassoc tIFX
%nonassoc tIF
%nonassoc tTHEN
%nonassoc tELIF tELSE

%right '='
%left tOR
%left tAND
%right '~'
%left tNE tEQ
%left '<' tLE tGE '>'
%left '+' '-'
%left '*' '/' '%'
%right tUMINUS

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%
file         : declarations { compiler->ast($$ = $1); }
             ;

declarations :              declaration { $$ = new cdk::sequence_node(LINE, $1);     }
             | declarations declaration { $$ = new cdk::sequence_node(LINE, $2, $1); }
             ;

declaration  : vardec ';' { $$ = $1; }
             | fundec     { $$ = $1; }
             | fundef     { $$ = $1; }
             ;

vardec       : tREQUIRE data_type  tID                         { $$ = new fir::variable_declaration_node(LINE, tPUBLIC,  $2, *$3, nullptr); }
             | tPUBLIC  data_type  tID         opt_initializer { $$ = new fir::variable_declaration_node(LINE, tPUBLIC,  $2, *$3, $4); }
             |          data_type  tID         opt_initializer { $$ = new fir::variable_declaration_node(LINE, tPRIVATE, $1, *$2, $3); }
             ;

vardecs      : vardec ';'          { $$ = new cdk::sequence_node(LINE, $1);     }
             | vardecs vardec ';' { $$ = new cdk::sequence_node(LINE, $2, $1); }
             ;
             
opt_vardecs  : /* empty */ { $$ = NULL; }
             | vardecs     { $$ = $1; }
             ;
             
/*identifiers  : tID { $$ = new std::vector<std::string>(); $$->push_back(*$1); delete $1; }
             | identifiers ',' tID { $$ = $1; $$->push_back(*$3); delete $3; } 
             ; */
             
data_type    : tTYPE_STRING                     { $$ = cdk::primitive_type::create(4, cdk::TYPE_STRING);  }
             | tTYPE_INT                        { $$ = cdk::primitive_type::create(4, cdk::TYPE_INT);     }
             | tTYPE_FLOAT                       { $$ = cdk::primitive_type::create(8, cdk::TYPE_DOUBLE);  }
             | tTYPE_POINTER '<' data_type '>'  { $$ = cdk::reference_type::create(4, $3); }
             ;
       
opt_initializer  : /* empty */         { $$ = nullptr; /* must be nullptr, not NIL */ }
                 | '=' expression      { $$ = $2; }
                 ;
       
void_type   : tPROCEDURE { $$ = cdk::primitive_type::create(0, cdk::TYPE_VOID);   }
            ;
             
fundec   :          data_type  tID '(' argdecs ')' { $$ = new fir::function_declaration_node(LINE, tPRIVATE, $1, *$2, $4); }
         | tREQUIRE data_type  tID '(' argdecs ')' { $$ = new fir::function_declaration_node(LINE, tPUBLIC,  $2, *$3, $5); }
         | tPUBLIC  data_type  tID '(' argdecs ')' { $$ = new fir::function_declaration_node(LINE, tPUBLIC,  $2, *$3, $5); }
         |          void_type  tID '(' argdecs ')' { $$ = new fir::function_declaration_node(LINE, tPRIVATE, $1, *$2, $4); }
         | tREQUIRE void_type  tID '(' argdecs ')' { $$ = new fir::function_declaration_node(LINE, tPUBLIC,  $2, *$3, $5); }
         | tPUBLIC  void_type  tID '(' argdecs ')' { $$ = new fir::function_declaration_node(LINE, tPUBLIC,  $2, *$3, $5); }
         ;

fundef   :         data_type  tID '(' argdecs ')' defaultreturn bodys { $$ = new fir::function_definition_node(LINE, tPRIVATE, $1, *$2, $4, $7); }
         | tPUBLIC data_type  tID '(' argdecs ')' defaultreturn bodys { $$ = new fir::function_definition_node(LINE, tPUBLIC,  $2, *$3, $5, $8); }
         |         void_type  tID '(' argdecs ')' defaultreturn bodys { $$ = new fir::function_definition_node(LINE, tPRIVATE, $1, *$2, $4, $7); }
         | tPUBLIC void_type  tID '(' argdecs ')' defaultreturn bodys { $$ = new fir::function_definition_node(LINE, tPUBLIC,  $2, *$3, $5, $8); }
         ;

argdecs  : /* empty */         { $$ = new cdk::sequence_node(LINE);  }
         |             argdec  { $$ = new cdk::sequence_node(LINE, $1);     }
         | argdecs ',' argdec  { $$ = new cdk::sequence_node(LINE, $3, $1); }
         ;

argdec   : data_type tID { $$ = new fir::variable_declaration_node(LINE, tPRIVATE, $1, *$2, nullptr); }
         ;

bodys    :body                   { $$ = new cdk::sequence_node(LINE, $1); }
         |bodys body            { $$ = new cdk::sequence_node(LINE, $2, $1); }  
         ;

body     : prologo                              { $$ = $1; }
         | block                                { $$ = $1; }
         | epilogo                              { $$ = $1; }
         ;

prologo  : '@' block       { $$ = $2; }
         ;

epilogo  : '>''>' block       { $$ = $3; }
         ;                  

block    : '{' opt_vardecs opt_instructions '}' { $$ = new fir::block_node(LINE, $2, $3); }
         ;

defaultreturn : '-''>' integer                       { $$ = $3; }
              | '-''>' real                          { $$ = $3; }
              | '-''>' string                        { $$ = new cdk::string_node(LINE, $3); }
              | '-''>' tNULLPTR                      { $$ = new fir::null_node(LINE); }
              ;
              

return          : tRETURN                    { $$ = new fir::return_node(LINE, nullptr); }
                ;

leave           : tLEAVE tINTEGER   ';' { $$ = new fir::leave_node(LINE, $2);  }
                ;

restart         : tRESTART tINTEGER   ';' { $$ = new fir::restart_node(LINE, $2); }
                ; 

finally         : tFINALLY instruction   {  $$ = $2; }
                ;                               

instructions    : instruction                { $$ = new cdk::sequence_node(LINE, $1);     }
                | instructions instruction   { $$ = new cdk::sequence_node(LINE, $2, $1); }
                ;

opt_instructions: /* empty */  { $$ = new cdk::sequence_node(LINE); }
                | instructions { $$ = $1; }
                ;

instruction     : tIF expression tTHEN instruction                                          { $$ = new fir::if_node(LINE, $2, $4); }
                | tIF expression tTHEN instruction iffalse                                  { $$ = new fir::if_else_node(LINE, $2, $4, $5); }
                | tWHILE '(' expression ')' instruction finally                              { $$ = new fir::while_finally_node(LINE, $3, $5, $6); }  
                | tWHILE '(' expression ')' instruction                                    { $$ = new fir::while_node(LINE, $3, $5); }    
                | expression ';'                                                            { $$ = new fir::evaluation_node(LINE, $1); }
                | leave                                                                     { $$ = $1;  }
                | restart                                                                  { $$ = $1; }
                | return                                                                     { $$ = $1; }
                | block                                                                     { $$ = $1; }
                ;

iffalse         : tELSE instruction                             { $$ = $2; }
                | tELIF expression tTHEN instruction            { $$ = new fir::if_node(LINE, $2, $4); }
                | tELIF expression tTHEN instruction iffalse    { $$ = new fir::if_else_node(LINE, $2, $4, $5); }
                ;

lvalue          : tID                                            { $$ = new cdk::variable_node(LINE, *$1); delete $1; }
                | lvalue             '[' expression ']'          { $$ = new fir::index_node(LINE, new cdk::rvalue_node(LINE, $1), $3); }
                | '(' expression ')' '[' expression ']'          { $$ = new fir::index_node(LINE, $2, $5); }
                | tID '(' opt_expressions ')' '[' expression ']' { $$ = new fir::index_node(LINE, new fir::function_call_node(LINE, *$1, $3), $6); }
                ;

expression      : integer                       { $$ = $1; }
                | real                          { $$ = $1; }
                | string                        { $$ = new cdk::string_node(LINE, $1); }
                | tNULLPTR                      { $$ = new fir::null_node(LINE); }
                /* LEFT VALUES */
                | lvalue                        { $$ = new cdk::rvalue_node(LINE, $1); }
                /* ASSIGNMENTS */
                | lvalue '=' expression         { $$ = new cdk::assignment_node(LINE, $1, $3); }
                /* ARITHMETIC EXPRESSIONS */
                | expression '+' expression    { $$ = new cdk::add_node(LINE, $1, $3); }
                | expression '-' expression    { $$ = new cdk::sub_node(LINE, $1, $3); }
                | expression '*' expression    { $$ = new cdk::mul_node(LINE, $1, $3); }
                | expression '/' expression    { $$ = new cdk::div_node(LINE, $1, $3); }
                | expression '%' expression    { $$ = new cdk::mod_node(LINE, $1, $3); }
                /* LOGICAL EXPRESSIONS */
                | expression  '<' expression    { $$ = new cdk::lt_node(LINE, $1, $3); }
                | expression tLE  expression    { $$ = new cdk::le_node(LINE, $1, $3); }
                | expression tEQ  expression    { $$ = new cdk::eq_node(LINE, $1, $3); }
                | expression tGE  expression    { $$ = new cdk::ge_node(LINE, $1, $3); }
                | expression  '>' expression    { $$ = new cdk::gt_node(LINE, $1, $3); }
                | expression tNE  expression    { $$ = new cdk::ne_node(LINE, $1, $3); }
                /* LOGICAL EXPRESSIONS */
                | expression tAND  expression    { $$ = new cdk::and_node(LINE, $1, $3); }
                | expression tOR   expression    { $$ = new cdk::or_node (LINE, $1, $3); }
                /* UNARY EXPRESSION */
                | '-' expression %prec tUMINUS  { $$ = new cdk::neg_node(LINE, $2); }
                | '+' expression %prec tUMINUS  { $$ = $2; }
                | '~' expression                { $$ = new cdk::not_node(LINE, $2); }
                /* OTHER EXPRESSION */
                | tID '(' opt_expressions ')'   { $$ = new fir::function_call_node(LINE, *$1, $3); delete $1; }
                | tSIZEOF '(' expression ')'   { $$ = new fir::sizeof_node(LINE, $3); } 
                /* OTHER EXPRESSION */
                | '(' expression ')'            { $$ = $2; }
                | '[' expression ']'            { $$ = new fir::stack_alloc_node(LINE, $2); }
                | lvalue '?'                    { $$ = new fir::adress_node(LINE, $1); } 
                ;

expressions     : expression                     { $$ = new cdk::sequence_node(LINE, $1);     }
                | expressions ',' expression     { $$ = new cdk::sequence_node(LINE, $3, $1); }
                ;

opt_expressions : /* empty */         { $$ = new cdk::sequence_node(LINE); }
                | expressions         { $$ = $1; }
                ;

integer         : tINTEGER                      { $$ = new cdk::integer_node(LINE, $1); };
real            : tFLOAT                         { $$ = new cdk::double_node(LINE, $1); };
string          : tSTRING                       { $$ = $1; }
                | string tSTRING                { $$ = $1; $$->append(*$2); delete $2; }
                ;

%%