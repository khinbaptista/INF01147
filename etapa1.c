#include "etapa1.h"

int yylex();
void initMe() {
    lineNumber = 1;
    running = 1;
}
int isRunning() { return running; }
int getLineNumber() { return lineNumber; }