#pragma once

/****************************************
	INF01147 - Compiladores
	Khin Baptista
	Lorenzo Dal'Aqua
****************************************/

// Globals
int lineNumber;
int running;
int syntaxError;
int specificSyntaxError;

void initMe();
int isRunning();
int getLineNumber();
void setSyntaxError(int value);
int hasSyntaxError();
void toggleSpecificSyntaxError();
int checkSpecificSyntaxError();
