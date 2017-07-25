/****************************************
	INF01147 - Compiladores
	Khin Baptista
	Lorenzo Dal'Aqua
****************************************/

#include "lang171.h"
#include "hash.h"

void initMe() {
	lineNumber = 1;
	running = 1;
	syntaxError = 0;
	specificSyntaxError = 0;
	hash_init();
}

int isRunning()		{ return running;	}
int getLineNumber()	{ return lineNumber;	}

void setSyntaxError(int value) {
	syntaxError = value;
}
int hasSyntaxError() {
	return syntaxError;
}
void toggleSpecificSyntaxError() {
	specificSyntaxError = !specificSyntaxError;
}
int checkSpecificSyntaxError() {
	return specificSyntaxError;
}
