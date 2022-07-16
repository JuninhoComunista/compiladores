%{
    //Nome: Nicolas Paris - Cartao: 302650
    #include <stdio.h>
    #include <stdlib.h>
    int yyerror();
    extern int getLineNumber();
%}

%union {char *symbol;}
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

%token TK_IDENTIFIER

%token LIT_INTEGER  
%token LIT_FLOAT    
%token LIT_CHAR     
%token LIT_STRING   

%token TOKEN_ERROR

%left '<' '>' '&' OPERATOR_DIF OPERATOR_EQ OPERATOR_GE OPERATOR_LE
%left '|'
%left '^'
%right '~'
%left '+' '-'
%left '.' '/'

%%

program: 
    declarationList
    | functionList
    ;

declarationList:
    declaration declarationList
    |
    ;

declaration:
    type identifier '(' literal ')' ';'
    | type identifier '[' LIT_INTEGER ']' literalList ';'
    ;

functionList:
    function functionList
    |
    ;

function:
    type identifier '(' parameterList ')' block
    ;

parameterList:
    parameter parameterList
    |
    ;

parameter: 
    type identifier
    ;

block:
    '{' '}'
    ;

type:
    KW_CHAR
    | KW_INT
    | KW_FLOAT
    ;

identifier:
    TK_IDENTIFIER
    ;

literalList:
    literal literalList
    |
    ;

literal:
    LIT_CHAR
    | LIT_INTEGER
    | LIT_FLOAT
    ;

%%

int yyerror() {
    fprintf(stderr, "Syntax error at line %d \n", getLineNumber());
    exit(3);
}