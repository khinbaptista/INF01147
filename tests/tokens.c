/*
    Lex scanner tests for the language's other tokens
*/

void testTK_IDENTIFIER() {
    int tokens[]= {TK_IDENTIFIER, TK_IDENTIFIER, TK_IDENTIFIER};
    assert(compareTokens("vAr_12 _var1 A12_", tokens, 3), "TK_IDENTIFIER not matching");
    int error[] = {LIT_INTEGER, TK_IDENTIFIER};
    assert(compareTokens("12var", error, 2), "TK_IDENTIFIER false positive");
}

void testTOKEN_ERROR() {
    int tokens[]= {TOKEN_ERROR, LIT_INTEGER, TOKEN_ERROR};
    assert(compareTokens("|1'", tokens, 3), "TK_ERROR not matching");
}

void testCommentSingle() {
    int tokens[]= {LIT_INTEGER, TK_IDENTIFIER, LIT_REAL};
    assert(compareTokens("10//var 12.2 && 30\n var 1.1", tokens, 3), "Single line comment not matching");
    int error[]= {TK_IDENTIFIER, '/', '/', LIT_REAL};
    assert(compareTokens("test/ /1.1", error, 4), "Single line comment false positive");
}
void testCommentMulti() {
    int tokens[]= {LIT_INTEGER, TK_IDENTIFIER, LIT_REAL};
    assert(compareTokens("10/*var 12.2 && 30\n10*/ var 1.1", tokens, 3), "Multi line comment not matching");
    int error[]= {TK_IDENTIFIER, '*', '/', LIT_REAL};
    assert(compareTokens("test*/1.1", error, 4), "Multi line comment false positive");
}
