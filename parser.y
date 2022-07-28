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

%left '<' '>' OPERATOR_DIF OPERATOR_EQ OPERATOR_GE OPERATOR_LE
%right '~'
%left '&'
%left '|'
%left '+' '-'
%left '.' '/'

%%

program:
    declarationList
    ;

declarationList:
    variableDeclaration declarationList
    | functionDeclaration declarationList
    |
    ;

variableDeclaration:
    type identifier '(' literal ')' ';'
    | type identifier '[' LIT_INTEGER ']' literalList ';'
    ;

functionDeclaration:
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
    '{' commandList '}'
    ;

commandList:
    command ';' commandList
    | command
    ;

command:
    identifier ASSIGNMENT expression
    | identifier '[' expression ']' ASSIGNMENT expression
    | read
    | print
    | return
    | block
    | while
    | if
    | ifElse
    |
    ;

expressionList:
    expression expressionList
    |
    ;

expression:
    identifier
    | identifier '[' expression ']'
    | functionCall
    | literal
    | '(' expression ')'
    | expression '+' expression
    | expression '-' expression
    | expression '.' expression
    | expression '/' expression
    | expression '<' expression
    | expression '>' expression
    | expression '|' expression
    | expression '&' expression
    | expression OPERATOR_LE expression
    | expression OPERATOR_GE expression
    | expression OPERATOR_EQ expression
    | expression OPERATOR_DIF expression
    | '~' expression
    ;

read:
    KW_READ identifier
    | KW_READ identifier '[' expression ']'
    ;

print:
    KW_PRINT printList
    ;

printList:
    expression printList
    | expression
    | LIT_STRING printList 
    | LIT_STRING
    ;

return:
    KW_RETURN expression
    ;

while:
    KW_WHILE '(' expression ')' command
    ;

if:
    KW_IF '(' expression ')' command
    ;

ifElse:
    KW_IF '(' expression ')' command KW_ELSE command
    ;

type:
    KW_CHAR
    | KW_INT
    | KW_FLOAT
    ;

identifier:
    TK_IDENTIFIER
    ;

functionCall:
    identifier '(' expressionList ')'
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