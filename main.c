#include <stdio.h>
#include "hash.h"
#include "etapa1.h"

int yylex();

int main() {
    initMe();
    for(;;) {
        if(isRunning()) {
            int token = yylex();
            printf("line %d: ", getLineNumber());
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
                default: printf("OPERATOR%c\n", token); break;
            }
        }
    }
}
