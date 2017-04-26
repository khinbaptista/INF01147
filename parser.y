%token KW_BYTE
%token KW_SHORT
%token KW_LONG
%token KW_FLOAT
%token KW_DOUBLE
%token KW_WHEN
%token KW_THEN
%token KW_ELSE
%token KW_WHILE
%token KW_FOR
%token KW_TO
%token KW_READ
%token KW_RETURN
%token KW_PRINT

%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_NE
%token OPERATOR_AND
%token OPERATOR_OR

%token TK_IDENTIFIER
%token LIT_INTEGER
%token LIT_REAL
%token LIT_CHAR
%token LIT_STRING

%token TOKEN_ERROR

%left '+' '-'
%left '*' '/'

%{

#include <stdio.h>
#include <stdlib.h>
#include "lang171.h"	// getLineNumber()

%}

%start global_decl_set

%error-verbose

%%

type	:	KW_BYTE
		|	KW_SHORT
		|	KW_LONG
		|	KW_FLOAT
		|	KW_DOUBLE
;

literal	:	LIT_INTEGER
		|	LIT_REAL
		|	LIT_CHAR
;

global_decl_set	: 	global_decl global_decl_set
				|
;

global_decl	:	func_decl ';'
			|	var_decl ';'
;

var_decl	:	TK_IDENTIFIER ':' type literal
			|	TK_IDENTIFIER ':' type'['LIT_INTEGER']' array_init
;

array_init	:	literal array_init
			|
;

func_decl	:	type TK_IDENTIFIER '(' args_list ')' command
;

args_list	:	args
			|
;

args	:	args ',' arg
		|	arg
;

arg	:	type TK_IDENTIFIER
;

func_call	:	TK_IDENTIFIER '(' params_list ')'
;

params_list	:	params
			|
;

params	:	params ',' param
		|	param
;

param	:	expr_arg
;

command	:	command_block
		|	command_simple
;

command_block	:	'{' command_seq '}'
;

command_seq	:	command_seq command ';'
			|
;

command_simple	:	var_attr
				|	flow_ctrl
				|	basic_command
				|
;

var_attr	:	TK_IDENTIFIER '=' expr
			|	TK_IDENTIFIER '#' expr '=' expr
;

basic_command	:	KW_READ TK_IDENTIFIER
				| 	KW_PRINT print_args
				| 	KW_RETURN expr
;

print_args	:	print_args print_arg
			|	print_arg
;

print_arg	:	LIT_STRING
			|	expr
;

flow_ctrl	:	KW_WHEN '(' expr ')' KW_THEN command
			|	KW_WHEN '(' expr ')' KW_THEN command KW_ELSE command
			| 	KW_WHILE '(' expr ')' command
			|	KW_FOR '(' TK_IDENTIFIER '=' expr KW_TO expr ')' command
;

expr	:	'(' expr ')'
		|	expr expr_op expr_arg
		|	'!' expr
		|	'-' expr %prec  '-'
		| 	expr_arg
;

expr_arg	:	TK_IDENTIFIER
			|	TK_IDENTIFIER'['expr']'
			|	func_call
			|	literal
;

expr_op	:	'+'		|	'-' 	|	'*'
		|	'/' 	|	'<'		|	'>'
		|	OPERATOR_LE | OPERATOR_GE | OPERATOR_EQ
		|	OPERATOR_NE | OPERATOR_OR | OPERATOR_AND
;

%%

int yyerror(char* what) {
	fprintf(stderr, "Parser error at line %d: %s\n", getLineNumber(), what);
	exit(3);
}
