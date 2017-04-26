%{

#include "y.tab.h"
#include "lang171.h"
#include "hash.h"

%}

%x COMMENT

%%

"byte"                          { return KW_BYTE; }
"short"                         { return KW_SHORT; }
"long"                          { return KW_LONG; }
"float"                         { return KW_FLOAT; }
"double"                        { return KW_DOUBLE; }
"when"                          { return KW_WHEN; }
"then"                          { return KW_THEN; }
"else"                          { return KW_ELSE; }
"while"                         { return KW_WHILE; }
"for"                           { return KW_FOR; }
"to"                           	{ return KW_TO; }
"read"                          { return KW_READ; }
"return"                        { return KW_RETURN; }
"print"                         { return KW_PRINT; }
"<="                            { return OPERATOR_LE; }
">="                            { return OPERATOR_GE; }
"=="                            { return OPERATOR_EQ; }
"!="                            { return OPERATOR_NE; }
"&&"                            { return OPERATOR_AND; }
\|\|                            { return OPERATOR_OR; }
[_a-zA-Z][_a-zA-Z0-9]*          { hash_insert(SYMBOL_TK_IDENTIFIER, yytext); return TK_IDENTIFIER; }
[0-9]+                          { hash_insert(SYMBOL_LIT_INTEGER, yytext); return LIT_INTEGER; }
[0-9]+\.[0-9]+                  { hash_insert(SYMBOL_LIT_REAL, yytext); return LIT_REAL; }
"'"."'"                         { hash_insert(SYMBOL_LIT_CHAR, yytext); return LIT_CHAR; }
\"(\\.|[^\"\n])*\"              { hash_insert(SYMBOL_LIT_STRING, yytext); return LIT_STRING; }
[-,;:\(\)\[\]\{\}+*/<>=!&$#]    { return yytext[0]; }
\/\/.*
\/\*                            { BEGIN(COMMENT); }
"\n"                            { lineNumber++; }
[ \t]
.                               { return TOKEN_ERROR; }

<COMMENT>\*\/                   { BEGIN(INITIAL); }
<COMMENT>.
<COMMENT>"\n"                   { lineNumber++; }

%%

int yywrap() {
	running = 0;
	return 1;
}
