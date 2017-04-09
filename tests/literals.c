/*
    Lex scanner tests for the language literals
*/

void testLIT_INTEGER() {
    int tokens[] = {LIT_INTEGER};
    assert(compareTokens("12", tokens, 1), "LIT_INTEGER does not match");
}

void testLIT_REAL() {
    int tokens[] = {LIT_REAL};
    assert(compareTokens("12.12", tokens, 1), "LIT_REAL does not match");
}

void testLIT_CHAR() {
    int tokens[] = {LIT_CHAR};
    assert(compareTokens("'c'", tokens, 1), "LIT_CHAR does not match");
    int error[] = {TOKEN_ERROR, TK_IDENTIFIER, TOKEN_ERROR};
    assert(compareTokens("'ca'", error, 3), "LIT_CHAR false positive");
}

void testLIT_STRING() {
    int tokens[] = {LIT_STRING,LIT_STRING};
    assert(compareTokens("\"12,\\n\\\"12\"\"abcde\"", tokens, 2), "LIT_STRING does not match");
    int error[] = {LIT_STRING,TK_IDENTIFIER,TOKEN_ERROR};
    assert(compareTokens("\"abcde\"aaaa\"", error, 2), "LIT_STRING false positive");
}