#pragma once

/****************************************
	INF01147 - Compiladores
	Khin Baptista
	Lorenzo Dal'Aqua
****************************************/

#define SYMBOL_TK_IDENTIFIER	0
#define SYMBOL_LIT_INTEGER	1
#define SYMBOL_LIT_REAL		2
#define SYMBOL_LIT_CHAR		3
#define SYMBOL_LIT_STRING	4

// Globals
int lineNumber;
int running;

void initMe();
int isRunning();
int getLineNumber();
