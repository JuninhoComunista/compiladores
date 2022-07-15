%{
    //Nome: Nicolas Paris - Cartao: 302650
    int yyerror();
%}

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

program: ;


%%

int yyerror() {
    fprintf(stderr, "Syntax error at line %d \n", getLineNumber());
    exit(3);
}