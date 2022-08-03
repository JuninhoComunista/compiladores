%{
    //Nome: Nicolas Paris - Cartao: 302650
    #include "hash.h"
    #include "symbolTypes.h"
    #include "ast.h"
    #include "y.tab.h"

    #define TABLESIZE 997

    int lineCount = 1;
    int running = 1;
    HashTable* identifiersTable;
    int isRunning();
    int getLineNumber();
    void initMe();
    int yyparse();

%}

%x COMMENT

%%

"##".*              {}
"\\#"               {BEGIN COMMENT;}
<COMMENT>\n         {lineCount++;}
<COMMENT>"#\\"      {BEGIN INITIAL;}
<COMMENT>.          {}

"char"              {return KW_CHAR;}
"int"               {return KW_INT;}
"float"             {return KW_FLOAT;}

"if"                {return KW_IF;}
"else"              {return KW_ELSE;}
"while"             {return KW_WHILE;}
"read"              {return KW_READ;}
"print"             {return KW_PRINT;}
"return"            {return KW_RETURN;}

[-;()[\]{}+./<>&|~] {return yytext[0];}

(<-)                {return ASSIGNMENT;}
(<=)                {return OPERATOR_LE;}    
(>=)                {return OPERATOR_GE;}
(==)                {return OPERATOR_EQ;}
(!=)                {return OPERATOR_DIF;}

[a-z_A-Z]*          {
                        yylval.symbol = hashInsert(identifiersTable, yytext, yytext, SYMBOL_IDENTIFIER); 
                        return TK_IDENTIFIER;
                    }

[0-9]+              {
                        yylval.symbol = hashInsert(identifiersTable, yytext, yytext, SYMBOL_INT); 
                        return LIT_INTEGER;
                    }

[0-9]+,[0-9]+       {
                        yylval.symbol = hashInsert(identifiersTable, yytext, yytext, SYMBOL_FLOAT); 
                        return LIT_FLOAT;
                    }

\'.\'               {
                        yylval.symbol = hashInsert(identifiersTable, yytext, yytext, SYMBOL_CHAR); 
                        return LIT_CHAR;
                    }
                    
\"(\\.|[^"\\])*\"   {
                        yylval.symbol = hashInsert(identifiersTable, yytext, yytext, SYMBOL_STRING); 
                        return LIT_STRING;
                    }

[ \t]*              {}
\n                  {lineCount++;}

.                   {return TOKEN_ERROR;}

%%

#include "main.h"

int isRunning() {
    return running;
}

int getLineNumber() {
    return lineCount;
}

void initMe() { 
    identifiersTable = createTable(TABLESIZE);
}

int yywrap() {
    running = 0;
    return 1;
}
