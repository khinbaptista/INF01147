/*
    Lex scanner tests for the language operators
*/

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

void testOPERATORS() {
    int operators[20] = { ',',';',':','(',')','[',']','{','}','+','-','*','/',
        '<','>','=','!','&','$','#' };
    for(int i = 0; i < 20; ++i) {
        int tokens[] = {operators[i]};
        char in[] = " ";
        char message[] = "ASCII OPERATOR x does not match";
        message[15] = operators[i];
        in[0] = operators[i];
        assert(compareTokens(in, tokens, 1), message);
    }
}