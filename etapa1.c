#include "etapa1.h"
#include "hash.h"

int yylex();

void initMe() {
	lineNumber = 1;
	running = 1;

	hash_init();
}

int isRunning()		{ return running;	}
int getLineNumber()	{ return lineNumber;	}
