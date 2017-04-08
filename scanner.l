%{
#include "tokens.h"
#define TRUE 1
#define FALSE 0

// Globals
int lineNumber = 1;
int running = TRUE;
void initMe() {}
int isRunning() { return running; }
int getLineNumber() { return lineNumber; }

%}

%%

"byte"                  { return KW_BYTE; }
"short"                 { return KW_SHORT; }
"long"                  { return KW_LONG; }
"float"                 { return KW_FLOAT; }
"double"                { return KW_DOUBLE; }
"when"                  { return KW_WHEN; }
"then"                  { return KW_THEN; }
"else"                  { return KW_ELSE; }
"while"                 { return KW_WHILE; }
"for"                   { return KW_FOR; }
"read"                  { return KW_READ; }
"return"                { return KW_RETURN; }
"print"                 { return KW_PRINT; }
"<="                    { return OPERATOR_LE; }
">="                    { return OPERATOR_GE; }
"=="                    { return OPERATOR_EQ; }
"!="                    { return OPERATOR_NE; }
"&&"                    { return OPERATOR_AND; }
"||"                    { return OPERATOR_OR; }
[_a-zA-Z][_a-zA-Z0-9]*  { return TK_IDENTIFIER; }
[0-9]+                  { return LIT_INTEGER; }
[0-9]+.[0-9]+           { return LIT_REAL; }
"'"."'"                 { return LIT_CHAR; }
\"(\\.|[^\"\n])*\"        { return LIT_STRING; }
[-,;:\(\)\[\]\{\}+*/<>=!&$#] { return yytext[0]; }
"\n"                    { lineNumber++; }
.                       { return TOKEN_ERROR; }
%%

int yywrap() { running = FALSE; };

int main() {
    for(;;) {
        if(running) {
            int token = yylex();
            printf("line %d: ", lineNumber);
            switch(token) {
                case KW_BYTE: printf("KW_BYTE\n"); break;
                case KW_SHORT: printf("KW_SHORT\n"); break;
                case KW_LONG: printf("KW_LONG\n"); break;
                case KW_FLOAT: printf("KW_FLOAT\n"); break;
                case KW_DOUBLE: printf("KW_DOUBLE\n"); break;
                case KW_WHEN: printf("KW_WHEN\n"); break;
                case KW_THEN: printf("KW_THEN\n"); break;
                case KW_ELSE: printf("KW_ELSE\n"); break;
                case KW_WHILE: printf("KW_WHILE\n"); break;
                case KW_FOR: printf("KW_FOR\n"); break;
                case KW_READ: printf("KW_READ\n"); break;
                case KW_RETURN: printf("KW_RETURN\n"); break;
                case KW_PRINT: printf("KW_PRINT\n"); break;
                case OPERATOR_LE: printf("OPERATOR_LE\n"); break;
                case OPERATOR_GE: printf("OPERATOR_GE\n"); break;
                case OPERATOR_EQ: printf("OPERATOR_EQ\n"); break;
                case OPERATOR_NE: printf("OPERATOR_NE\n"); break;
                case OPERATOR_AND: printf("OPERATOR_AND\n"); break;
                case OPERATOR_OR: printf("OPERATOR_OR\n"); break;
                case TK_IDENTIFIER: printf("TK_IDENTIFIER\n"); break;
                case LIT_INTEGER: printf("LIT_INTEGER\n"); break;
                case LIT_REAL: printf("LIT_REAL\n"); break;
                case LIT_CHAR: printf("LIT_CHAR\n"); break;
                case LIT_STRING: printf("LIT_STRING\n"); break;
                case TOKEN_ERROR: printf("TK_ERROR\n"); break;
                default: printf("%c\n", token); break;
            }
        }
    }
}