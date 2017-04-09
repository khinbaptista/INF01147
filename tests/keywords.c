/*
    Lex scanner tests for the language keywords
*/

void testKW_BYTE() {
    int tokens[] = {KW_BYTE};
    assert(compareTokens("byte", tokens, 1), "KW_BYTE does not match");
    int error[] = {TK_IDENTIFIER};
    assert(compareTokens("bytes", error, 1), "KW_BYTE false positive");
}
void testKW_SHORT() {
    int tokens[] = {KW_SHORT};
    assert(compareTokens("short", tokens, 1), "KW_SHORT does not match");
    int error[] = {TK_IDENTIFIER};
    assert(compareTokens("sshort", error, 1), "KW_SHORT false positive");
}
void testKW_LONG() {
    int tokens[] = {KW_LONG};
    assert((compareTokens("long", tokens, 1)), "KW_LONG does not match");
    int error[] = {TK_IDENTIFIER};
    assert(compareTokens("longest", error, 1), "KW_LONG false positive");
}
void testKW_FLOAT() {
    int tokens[] = {KW_FLOAT};
    assert(compareTokens("float", tokens, 1), "KW_FLOAT does not match");
    int error[] = {TK_IDENTIFIER};
    assert(compareTokens("floating", error, 1), "KW_FLOAT false positive");
}
void testKW_DOUBLE() {
    int tokens[] = {KW_DOUBLE};
    assert(compareTokens("double", tokens, 1), "KW_DOUBLE does not match");
    int error[] = {TK_IDENTIFIER};
    assert(compareTokens("doubles", error, 1), "KW_DOUBLE false positive");

}
void testKW_WHEN() {
    int tokens[] = {KW_WHEN};
    assert(compareTokens("when", tokens, 1), "KW_WHEN does not match");
    int error[] = {TK_IDENTIFIER};
    assert(compareTokens("whenever", error, 1), "KW_WHEN false positive");
}
void testKW_THEN() {
    int tokens[] = {KW_THEN};
    assert(compareTokens("then", tokens, 1), "KW_THEN does not match");
    int error[] = {TK_IDENTIFIER};
    assert(compareTokens("athens", error, 1), "KW_THEN false positive");
}
void testKW_ELSE() {
    int tokens[] = {KW_ELSE};
    assert(compareTokens("else", tokens, 1), "KW_ELSE does not match");
    int error[] = {TK_IDENTIFIER};
    assert(compareTokens("elsewhere", error, 1), "KW_ELSE false positive");
}
void testKW_WHILE() {
    int tokens[] = {KW_WHILE};
    assert(compareTokens("while", tokens, 1), "KW_WHILE does not match");
    int error[] = {TK_IDENTIFIER};
    assert(compareTokens("meanwhile", error, 1), "KW_WHILE false positive");
}
void testKW_FOR() {
    int tokens[] = {KW_FOR};
    assert(compareTokens("for", tokens, 1), "KW_FOR does not match");
    int error[] = {TK_IDENTIFIER};
    assert(compareTokens("fora", error, 1), "KW_FOR false positive");
}
void testKW_READ() {
    int tokens[] = {KW_READ};
    assert(compareTokens("read", tokens, 1), "KW_READ does not match");
    int error[] = {TK_IDENTIFIER};
    assert(compareTokens("bread", error, 1), "KW_READ false positive");
}
void testKW_RETURN() {
    int tokens[] = {KW_RETURN};
    assert(compareTokens("return", tokens, 1), "KW_RETURN does not match");
    int error[] = {TK_IDENTIFIER};
    assert(compareTokens("returning", error, 1), "KW_RETURN false positive");
}
void testKW_PRINT() {
    int tokens[] = {KW_PRINT};
    assert(compareTokens("print", tokens, 1), "KW_PRINT does not match");
    int error[] = {TK_IDENTIFIER};
    assert(compareTokens("printer", error, 1), "KW_PRINT false positive");
}