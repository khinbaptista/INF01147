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
        printf("\nFAILED: %s \n", message);
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
        printf("%s => %d -> %d\n",in, token, tokens[i]);
        if(tokens[i] != token) {
            result = 0;
            break;
        }
    }
    return result;
}

// Include tests, including *.c files for simplicity
#include "keywords.c"
#include "operators.c"
#include "literals.c"
#include "tokens.c"

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
    testOPERATORS();
    testLIT_INTEGER();
    testLIT_REAL();
    testLIT_CHAR();
    testLIT_STRING();
    testTK_IDENTIFIER();
    testTOKEN_ERROR();
    testCommentSingle();
    testCommentMulti();
}


int main() {
    printf("\n=============== RUNNING LEX TESTS ===============\n\n");
    run_all_tests();
    if (tests_failed == 0) {
        printf("\nALL TESTS PASSED\n");
    }
    printf("\n================== TEST RESULTS =================\n");
    printf("%d tests, %d failed, %d passed.\n\n\n", tests_run, tests_failed, tests_passed);
    return tests_failed;
}