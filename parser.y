%{
    //Nome: Nicolas Paris - Cartao: 302650
    #include <stdio.h>
    #include <stdlib.h>
    #include "ast.h"
    #include "decompiler.h"
    extern FILE *yyout;
    int yyerror();
    int yylex();
    int yyparse();
    extern int getLineNumber();
%}

%union {
    HashNode *symbol;
    Ast *ast;
}

%start program

%token KW_CHAR      
%token KW_INT       
%token KW_FLOAT     

%token KW_IF        
%token KW_ELSE      
%token KW_WHILE     
%token KW_READ      
%token KW_PRINT     
%token KW_RETURN    

%token ASSIGNMENT   
%token OPERATOR_LE  
%token OPERATOR_GE  
%token OPERATOR_EQ  
%token OPERATOR_DIF 

%token<symbol> TK_IDENTIFIER

%token<symbol> LIT_INTEGER  
%token<symbol> LIT_FLOAT    
%token<symbol> LIT_CHAR     
%token<symbol> LIT_STRING   

%token TOKEN_ERROR

%type<ast> program
%type<ast> declarationList
%type<ast> variableDeclaration
%type<ast> functionDeclaration
%type<ast> parameterList
%type<ast> parameter
%type<ast> block
%type<ast> commandList
%type<ast> command
%type<ast> expressionList
%type<ast> expression
%type<ast> read
%type<ast> printList
%type<ast> print
%type<ast> return
%type<ast> while
%type<ast> if
%type<ast> ifElse
%type<ast> type
%type<ast> identifier
%type<ast> functionCall
%type<ast> literalList
%type<ast> literal

%left '<' '>' OPERATOR_DIF OPERATOR_EQ OPERATOR_GE OPERATOR_LE
%right '~'
%left '&'
%left '|'
%left '+' '-'
%left '.' '/'

%%

program:
    declarationList     {
                            $$ = $1;
                            FILE *treeFile;
                            if ((treeFile = fopen("tree.txt", "w")) == 0) {
                                printf("Cannot open tree.txt\n");
                            }
                            printf("\nPrinting tree\n\n");
                            astPrint($$, 0, treeFile);
                            decompile($$, yyout);
                        }
    ;

declarationList:
    variableDeclaration declarationList     {$$ = astCreate(AST_DEC_LIST, 0, $1, $2, 0, 0);}
    | functionDeclaration declarationList   {$$ = astCreate(AST_DEC_LIST, 0, $1, $2, 0, 0);}
    |                                       {$$ = 0;}
    ;

variableDeclaration:
    type identifier '(' literal ')' ';'                     {$$ = astCreate(AST_VAR_DEC, 0, $1, $2, $4, 0);}
    | type identifier '[' LIT_INTEGER ']' literalList ';'   {$$ = astCreate(AST_VEC_DEC, $4, $1, $2, $6, 0);} 
    ;

functionDeclaration:
    type identifier '(' parameterList ')' block {$$ = astCreate(AST_FUNC_DEC, 0, $1, $2, $4, $6);}
    ;

parameterList:
    parameter parameterList {$$ = astCreate(AST_PARAM_LIST, 0, $1, $2, 0, 0);}
    |                       {$$ = 0;}
    ;

parameter: 
    type identifier {$$ = astCreate(AST_PARAM, 0, $1, $2, 0, 0);}
    ;

block:
    '{' commandList '}' {$$ = astCreate(AST_BLOCK, 0, $2, 0, 0, 0);}
    ;

commandList:
    command ';' commandList {$$ = astCreate(AST_COMMAND_LIST, 0, $1, $3, 0, 0);}
    | command               {$$ = astCreate(AST_COMMAND_LIST, 0, $1, 0, 0, 0);}
    ;

command:
    identifier ASSIGNMENT expression                        {$$ = astCreate(AST_ASSIGNMENT, 0, $1, $3, 0, 0);}
    | identifier '[' expression ']' ASSIGNMENT expression   {$$ = astCreate(AST_VEC_ASSIGNMENT, 0, $1, $3, $6, 0);}
    | read
    | print
    | return
    | block
    | while
    | if
    | ifElse
    |                                                       {$$ = 0;}
    ;

expressionList:
    expression expressionList   {$$ = astCreate(AST_EXPR_LIST, 0, $1, $2, 0, 0);}
    |                           {$$ = 0;}
    ;

expression:
    identifier                              
    | identifier '[' expression ']'         {$$ = astCreate(AST_VEC_ACESS, 0, $1, $3, 0, 0);}
    | functionCall                          
    | literal                               
    | '(' expression ')'                    {$$ = astCreate(AST_PARENTHESIS, 0, $2, 0, 0, 0);}
    | expression '+' expression             {$$ = astCreate(AST_ADD, 0, $1, $3, 0, 0);}
    | expression '-' expression             {$$ = astCreate(AST_SUB, 0, $1, $3, 0, 0);}
    | expression '.' expression             {$$ = astCreate(AST_MUL, 0, $1, $3, 0, 0);}
    | expression '/' expression             {$$ = astCreate(AST_DIV, 0, $1, $3, 0, 0);}
    | expression '<' expression             {$$ = astCreate(AST_LES, 0, $1, $3, 0, 0);}
    | expression '>' expression             {$$ = astCreate(AST_GRE, 0, $1, $3, 0, 0);}
    | expression '|' expression             {$$ = astCreate(AST_OR, 0, $1, $3, 0, 0);}
    | expression '&' expression             {$$ = astCreate(AST_AND, 0, $1, $3, 0, 0);}
    | expression OPERATOR_LE expression     {$$ = astCreate(AST_LE, 0, $1, $3, 0, 0);}
    | expression OPERATOR_GE expression     {$$ = astCreate(AST_GE, 0, $1, $3, 0, 0);}
    | expression OPERATOR_EQ expression     {$$ = astCreate(AST_EQ, 0, $1, $3, 0, 0);}
    | expression OPERATOR_DIF expression    {$$ = astCreate(AST_DIF, 0, $1, $3, 0, 0);}
    | '~' expression                        {$$ = astCreate(AST_NOT, 0, $2, 0, 0, 0);}
    ;

read:
    KW_READ identifier                      {$$ = astCreate(AST_READ, 0, $2, 0, 0, 0);}
    | KW_READ identifier '[' expression ']' {$$ = astCreate(AST_VEC_READ, 0, $2, $4, 0, 0);}
    ;

printList:
    expression printList    {$$ = astCreate(AST_EXPR, 0, $1, $2, 0, 0);}
    | expression            {$$ = astCreate(AST_EXPR, 0, $1, 0, 0, 0);}
    | LIT_STRING printList  {$$ = astCreate(AST_STRING, $1, $2, 0, 0, 0);}
    | LIT_STRING            {$$ = astCreate(AST_STRING, $1, 0, 0, 0, 0);}
    ;

print:
    KW_PRINT printList  {$$ = astCreate(AST_PRINT, 0, $2, 0, 0, 0);}
    ;

return:
    KW_RETURN expression {$$ = astCreate(AST_RETURN, 0, $2, 0, 0, 0);}
    ;

while:
    KW_WHILE '(' expression ')' command {$$ = astCreate(AST_WHILE, 0, $3, $5, 0, 0);}
    ;

if:
    KW_IF '(' expression ')' command    {$$ = astCreate(AST_IF, 0, $3, $5, 0, 0);}
    ;

ifElse:
    KW_IF '(' expression ')' command KW_ELSE command    {$$ = astCreate(AST_IF_ELSE, 0, $3, $5, $7, 0);}
    ;

type:
    KW_CHAR     {$$ = astCreate(AST_CHAR, 0,0,0,0,0);}
    | KW_INT    {$$ = astCreate(AST_INT, 0,0,0,0,0);}
    | KW_FLOAT  {$$ = astCreate(AST_FLOAT, 0,0,0,0,0);}
    ;

identifier:
    TK_IDENTIFIER   {$$ = astCreate(AST_IDENTIFIER, $1,0,0,0,0);}
    ;

functionCall:
    identifier '(' expressionList ')'   {$$ = astCreate(AST_FUNC_CALL, 0, $1, $3, 0, 0);}
    ;

literalList:
    literal literalList {$$ = astCreate(AST_LITERAL_LIST, 0, $1, $2, 0, 0);}
    |                   {$$ = 0;}
    ;

literal:
    LIT_CHAR        {$$ = astCreate(AST_LITERAL, $1, 0,0,0,0);}
    | LIT_INTEGER   {$$ = astCreate(AST_LITERAL, $1, 0,0,0,0);}
    | LIT_FLOAT     {$$ = astCreate(AST_LITERAL, $1, 0,0,0,0);}
    ;

%%

int yyerror() {
    fprintf(stderr, "Syntax error at line %d \n", getLineNumber());
    exit(3);
}