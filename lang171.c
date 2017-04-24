/****************************************
	INF01147 - Compiladores
	Khin Baptista
	Lorenzo Dal'Aqua
****************************************/

#include "etapa1.h"
#include "hash.h"

void initMe() {
	lineNumber = 1;
	running = 1;

	hash_init();
}

int isRunning()		{ return running;	}
int getLineNumber()	{ return lineNumber;	}
