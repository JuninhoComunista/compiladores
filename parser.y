%{
    //Nome: Nicolas Paris - Cartao: 302650
    #include <stdio.h>
    #include <stdlib.h>
    #include "semantic.h"
    #include "decompiler.h"
    #include "tac.h"
    extern FILE *yyout;
    extern HashTable* identifiersTable;
    int yyerror();
    int yylex();
    int yyparse();
    int getLineNumber();
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
                            FILE *treeFile;
                            if ((treeFile = fopen("tree.txt", "w")) == 0) {
                                printf("Cannot open tree.txt\n");
                            }
                            printf("\nPrinting tree\n\n");
    
                            int errors = runOnce($1, identifiersTable);
                            astPrint($1, 0, treeFile);
                            decompile($1, yyout);
                            if (errors) {
                                fprintf(stderr, "Semantic errors: %d\n", errors);
                                exit(4);
                            }
                            Tac *tacList = generateCode($1, identifiersTable, 0);
                            reverseTacList(&tacList);
                            tacPrintForwards(tacList, stderr);
                        }
    ;

declarationList:
    variableDeclaration declarationList     {$$ = astCreate(getLineNumber(), AST_DEC_LIST, 0, $1, $2, 0, 0);}
    | functionDeclaration declarationList   {$$ = astCreate(getLineNumber(), AST_DEC_LIST, 0, $1, $2, 0, 0);}
    |                                       {$$ = 0;}
    ;

variableDeclaration:
    type identifier '(' literal ')' ';'                     {
                                                                $$ = astCreate(getLineNumber(), AST_VAR_DEC, 0, $1, $2, $4, 0);
                                                                $2->symbol->lineNumber = getLineNumber();
                                                            }
    | type identifier '[' LIT_INTEGER ']' literalList ';'   {
                                                                $$ = astCreate(getLineNumber(), AST_VEC_DEC, $4, $1, $2, $6, 0);
                                                                $4->dataType = DT_INT;
                                                                $2->symbol->lineNumber = getLineNumber();
                                                            } 
    ;

functionDeclaration:
    type identifier '(' parameterList ')' block {
                                                    $$ = astCreate(getLineNumber(), AST_FUNC_DEC, 0, $1, $2, $4, $6);
                                                    $2->symbol->lineNumber = getLineNumber();
                                                }
    ;

parameterList:
    parameter parameterList {$$ = astCreate(getLineNumber(), AST_PARAM_LIST, 0, $1, $2, 0, 0);}
    |                       {$$ = 0;}
    ;

parameter: 
    type identifier {
                        $$ = astCreate(getLineNumber(), AST_PARAM, 0, $1, $2, 0, 0);
                        $2->symbol->lineNumber = getLineNumber();
                    }
    ;

block:
    '{' commandList '}' {$$ = astCreate(getLineNumber(), AST_BLOCK, 0, $2, 0, 0, 0);}
    ;

commandList:
    command ';' commandList {$$ = astCreate(getLineNumber(), AST_COMMAND_LIST, 0, $1, $3, 0, 0);}
    | command               {$$ = astCreate(getLineNumber(), AST_COMMAND_LIST, 0, $1, 0, 0, 0);}
    ;

command:
    identifier ASSIGNMENT expression                        {
                                                                $$ = astCreate(getLineNumber(), AST_ASSIGNMENT, 0, $1, $3, 0, 0); 
                                                                $1->symbol->lineNumber = getLineNumber();
                                                            }
    | identifier '[' expression ']' ASSIGNMENT expression   {
                                                                $$ = astCreate(getLineNumber(), AST_VEC_ASSIGNMENT, 0, $1, $3, $6, 0); 
                                                                $1->symbol->lineNumber = getLineNumber();
                                                            }
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
    expression expressionList   {$$ = astCreate(getLineNumber(), AST_EXPR_LIST, 0, $1, $2, 0, 0);}
    |                           {$$ = 0;}
    ;

expression:
    identifier                              {
                                                $$ = $1; 
                                                $1->symbol->lineNumber = getLineNumber();
                                            }
    | identifier '[' expression ']'         {
                                                $$ = astCreate(getLineNumber(), AST_VEC_ACESS, 0, $1, $3, 0, 0); 
                                                $1->symbol->lineNumber = getLineNumber();
                                            }
    | functionCall                          
    | literal                               
    | '(' expression ')'                    {$$ = astCreate(getLineNumber(), AST_PARENTHESIS, 0, $2, 0, 0, 0);}
    | expression '+' expression             {$$ = astCreate(getLineNumber(), AST_ADD, 0, $1, $3, 0, 0);}
    | expression '-' expression             {$$ = astCreate(getLineNumber(), AST_SUB, 0, $1, $3, 0, 0);}
    | expression '.' expression             {$$ = astCreate(getLineNumber(), AST_MUL, 0, $1, $3, 0, 0);}
    | expression '/' expression             {$$ = astCreate(getLineNumber(), AST_DIV, 0, $1, $3, 0, 0);}
    | expression '<' expression             {$$ = astCreate(getLineNumber(), AST_LES, 0, $1, $3, 0, 0);}
    | expression '>' expression             {$$ = astCreate(getLineNumber(), AST_GRE, 0, $1, $3, 0, 0);}
    | expression '|' expression             {$$ = astCreate(getLineNumber(), AST_OR, 0, $1, $3, 0, 0);}
    | expression '&' expression             {$$ = astCreate(getLineNumber(), AST_AND, 0, $1, $3, 0, 0);}
    | expression OPERATOR_LE expression     {$$ = astCreate(getLineNumber(), AST_LE, 0, $1, $3, 0, 0);}
    | expression OPERATOR_GE expression     {$$ = astCreate(getLineNumber(), AST_GE, 0, $1, $3, 0, 0);}
    | expression OPERATOR_EQ expression     {$$ = astCreate(getLineNumber(), AST_EQ, 0, $1, $3, 0, 0);}
    | expression OPERATOR_DIF expression    {$$ = astCreate(getLineNumber(), AST_DIF, 0, $1, $3, 0, 0);}
    | '~' expression                        {$$ = astCreate(getLineNumber(), AST_NOT, 0, $2, 0, 0, 0);}
    ;

read:
    KW_READ identifier                      {
                                                $$ = astCreate(getLineNumber(), AST_READ, 0, $2, 0, 0, 0); 
                                                $2->symbol->lineNumber = getLineNumber();
                                            }
    | KW_READ identifier '[' expression ']' {
                                                $$ = astCreate(getLineNumber(), AST_VEC_READ, 0, $2, $4, 0, 0); 
                                                $2->symbol->lineNumber = getLineNumber();
                                            }
    ;

printList:
    expression printList    {$$ = astCreate(getLineNumber(), AST_EXPR, 0, $1, $2, 0, 0);}
    | expression            {$$ = astCreate(getLineNumber(), AST_EXPR, 0, $1, 0, 0, 0);}
    | LIT_STRING printList  {
                                $$ = astCreate(getLineNumber(), AST_STRING, $1, $2, 0, 0, 0);
                                $1->dataType = DT_STRING;
                            }
    | LIT_STRING            {
                                $$ = astCreate(getLineNumber(), AST_STRING, $1, 0, 0, 0, 0);
                                $1->dataType = DT_STRING;
                            }
    ;

print:
    KW_PRINT printList  {$$ = astCreate(getLineNumber(), AST_PRINT, 0, $2, 0, 0, 0);}
    ;

return:
    KW_RETURN expression {$$ = astCreate(getLineNumber(), AST_RETURN, 0, $2, 0, 0, 0);}
    ;

while:
    KW_WHILE '(' expression ')' command {$$ = astCreate(getLineNumber(), AST_WHILE, 0, $3, $5, 0, 0);}
    ;

if:
    KW_IF '(' expression ')' command    {$$ = astCreate(getLineNumber(), AST_IF, 0, $3, $5, 0, 0);}
    ;

ifElse:
    KW_IF '(' expression ')' command KW_ELSE command    {$$ = astCreate(getLineNumber(), AST_IF_ELSE, 0, $3, $5, $7, 0);}
    ;

type:
    KW_CHAR     {$$ = astCreate(getLineNumber(), AST_CHAR, 0,0,0,0,0);}
    | KW_INT    {$$ = astCreate(getLineNumber(), AST_INT, 0,0,0,0,0);}
    | KW_FLOAT  {$$ = astCreate(getLineNumber(), AST_FLOAT, 0,0,0,0,0);}
    ;

identifier:
    TK_IDENTIFIER   {$$ = astCreate(getLineNumber(), AST_IDENTIFIER, $1,0,0,0,0);}
    ;

functionCall:
    identifier '(' expressionList ')'   {
                                            $$ = astCreate(getLineNumber(), AST_FUNC_CALL, 0, $1, $3, 0, 0); 
                                            $1->symbol->lineNumber = getLineNumber();
                                        }
    ;

literalList:
    literal literalList {$$ = astCreate(getLineNumber(), AST_LITERAL_LIST, 0, $1, $2, 0, 0);}
    |                   {$$ = 0;}
    ;

literal:
    LIT_CHAR        {
                        $$ = astCreate(getLineNumber(), AST_LITERAL, $1, 0,0,0,0);
                        $1->dataType = DT_CHAR;
                    }
    | LIT_INTEGER   {
                        $$ = astCreate(getLineNumber(), AST_LITERAL, $1, 0,0,0,0);
                        $1->dataType = DT_INT;
                    }
    | LIT_FLOAT     {
                        $$ = astCreate(getLineNumber(), AST_LITERAL, $1, 0,0,0,0);
                        $1->dataType = DT_FLOAT;
                    }
    ;

%%

int yyerror() {
    fprintf(stderr, "Syntax error at line %d \n", getLineNumber());
    exit(3);
}
