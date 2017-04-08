#include "../lex.yy.c"

int tests_run = 0;
int tests_passed = 0;
int tests_failed = 0;

void assert(int result, char* message) {
    if (result) {
        tests_passed++;
    }
    else {
        tests_failed++;
        printf("FAILED: %s \n", message);
    }
    tests_run++;
}

void stringToYYIN(char* string) {
    yyin = fmemopen(string, strlen (string), "r");
    YY_NEW_FILE;
}

int compareTokens(char* in, int* tokens, int numTokens) {
    stringToYYIN(in);
    int result = 1;
    for(int i = 0; i < numTokens; ++i) {
        int token = yylex();
        if(tokens[i] != token) {
            result = 0;
            break;
        }
    }
    return result;
}

/* Keywords and operators*/
void testKW_BYTE() {
    int tokens[] = {KW_BYTE};
    assert(compareTokens("byte", tokens, 1), "KW_BYTE does not match");
}
void testKW_SHORT() {
    int tokens[] = {KW_SHORT};
    assert(compareTokens("short", tokens, 1), "KW_SHORT does not match");
}
void testKW_LONG() {
    int tokens[] = {KW_LONG};
    assert((compareTokens("long", tokens, 1)), "KW_LONG does not match");
}
void testKW_FLOAT() {
    int tokens[] = {KW_FLOAT};
    assert(compareTokens("float", tokens, 1), "KW_FLOAT does not match");
}
void testKW_DOUBLE() {
    int tokens[] = {KW_DOUBLE};
    assert(compareTokens("double", tokens, 1), "KW_DOUBLE does not match");
}
void testKW_WHEN() {
    int tokens[] = {KW_WHEN};
    assert(compareTokens("when", tokens, 1), "KW_WHEN does not match");
}
void testKW_THEN() {
    int tokens[] = {KW_THEN};
    assert(compareTokens("then", tokens, 1), "KW_THEN does not match");
}
void testKW_ELSE() {
    int tokens[] = {KW_ELSE};
    assert(compareTokens("else", tokens, 1), "KW_ELSE does not match");
}
void testKW_WHILE() {
    int tokens[] = {KW_WHILE};
    assert(compareTokens("while", tokens, 1), "KW_WHILE does not match");
}
void testKW_FOR() {
    int tokens[] = {KW_FOR};
    assert(compareTokens("for", tokens, 1), "KW_FOR does not match");
}
void testKW_READ() {
    int tokens[] = {KW_READ};
    assert(compareTokens("read", tokens, 1), "KW_READ does not match");
}
void testKW_RETURN() {
    int tokens[] = {KW_RETURN};
    assert(compareTokens("return", tokens, 1), "KW_RETURN does not match");
}
void testKW_PRINT() {
    int tokens[] = {KW_PRINT};
    assert(compareTokens("print", tokens, 1), "KW_PRINT does not match");
}
void testOPERATOR_LE() {
    int tokens[] = {OPERATOR_LE};
    assert(compareTokens("<=", tokens, 1), "OPERATOR_LE does not match");
}
void testOPERATOR_GE() {
    int tokens[] = {OPERATOR_GE};
    assert(compareTokens(">=", tokens, 1), "OPERATOR_GE does not match");
}
void testOPERATOR_EQ() {
    int tokens[] = {OPERATOR_EQ};
    assert(compareTokens("==", tokens, 1), "OPERATOR_EQ does not match");
}
void testOPERATOR_NE() {
    int tokens[] = {OPERATOR_NE};
    assert(compareTokens("!=", tokens, 1), "OPERATOR_NE does not match");
}
void testOPERATOR_AND() {
    int tokens[] = {OPERATOR_AND};
    assert(compareTokens("&&", tokens, 1), "OPERATOR_AND does not match");
}
void testOPERATOR_OR() {
    int tokens[] = {OPERATOR_OR};
    assert(compareTokens("||", tokens, 1), "OPERATOR_OR does not match");
}

void testTK_IDENTIFIER() {
    int tokens[]= {TK_IDENTIFIER, LIT_INTEGER, TK_IDENTIFIER};
    assert(compareTokens("vAr_ 12V_Ar", tokens, 3), "TK_IDENTIFIER not matching");
}
// [_a-zA-Z][_a-zA-Z0-9]*  { return TK_IDENTIFIER; }
// [0-9]+                  { return LIT_INTEGER; }
// [0-9]+.[0-9]+           { return LIT_REAL; }
// "'"."'"                 { return LIT_CHAR; }
// \"(\\.|[^\"\n])*\"        { return LIT_STRING; }
// [-,;:\(\)\[\]\{\}+*/<>=!&$#] { return yytext[0]; }
// "\n"                    { lineNumber++; }
// [ \t]
// .                       { return TOKEN_ERROR; }
void run_all_tests() {
    testKW_BYTE();
    testKW_SHORT();
    testKW_LONG();
    testKW_FLOAT();
    testKW_DOUBLE();
    testKW_WHEN();
    testKW_THEN();
    testKW_ELSE();
    testKW_WHILE();
    testKW_FOR();
    testKW_READ();
    testKW_RETURN();
    testKW_PRINT();
    testOPERATOR_LE();
    testOPERATOR_GE();
    testOPERATOR_EQ();
    testOPERATOR_NE();
    testOPERATOR_AND();
    testOPERATOR_OR();
    testTK_IDENTIFIER();
}


int main() {
    printf("\n=============== Running lex tests ===============\n\n");
    run_all_tests();
    if (tests_failed == 0) {
        printf("ALL TESTS PASSED\n");
    }
    printf("\n=============== Test Results ===============\n");
    printf("%d tests, %d failed, %d passed.\n\n\n", tests_run, tests_failed, tests_passed);
    return tests_failed;
}