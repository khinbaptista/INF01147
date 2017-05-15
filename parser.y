%{

#include <stdio.h>
#include <stdlib.h>
#include "lang171.h"	// getLineNumber()
#include "astree.h"
#include "hash.h"

%}

%union {
	ASTree* tree;
}

%type <tree> type
%type <tree> literal
%type <tree> global_decl_set
%type <tree> global_decl
%type <tree> var_decl
%type <tree> array_init
%type <tree> func_decl
%type <tree> params_list
%type <tree> params
%type <tree> param
%type <tree> func_call
%type <tree> args_list
%type <tree> args
%type <tree> arg
%type <tree> command
%type <tree> command_block
%type <tree> command_seq
%type <tree> command_single
%type <tree> var_attr
%type <tree> basic_command
%type <tree> print_args
%type <tree> print_arg
%type <tree> flow_ctrl
%type <tree> expr
%type <tree> expr_arg

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

%token <tree>TK_IDENTIFIER
%token <tree>LIT_INTEGER
%token <tree>LIT_REAL
%token <tree>LIT_CHAR
%token <tree>LIT_STRING

%token TOKEN_ERROR

%left '+' '-'
%left '*' '/'

%start global_decl_set

%error-verbose

%%

type	:	KW_BYTE		{
	$$ = astree_create(AST_TYPE_BYTE, NULL, NULL, NULL, NULL, NULL);
}
		|	KW_SHORT	{
	$$ = astree_create(AST_TYPE_SHORT, NULL, NULL, NULL, NULL, NULL);
}
		|	KW_LONG		{
	$$ = astree_create(AST_TYPE_LONG, NULL, NULL, NULL, NULL, NULL);
}
		|	KW_FLOAT	{
	$$ = astree_create(AST_TYPE_FLOAT, NULL, NULL, NULL, NULL, NULL);
}
		|	KW_DOUBLE	{
	$$ = astree_create(AST_TYPE_DOUBLE, NULL, NULL, NULL, NULL, NULL);
}
;

literal	:	LIT_INTEGER
		|	LIT_REAL
		|	LIT_CHAR
;

global_decl_set	: 	global_decl global_decl_set {
	$$ = astree_create(AST_PROGRAM, $1, $2, NULL, NULL, NULL);
	astree_print($$,0);
}
				|								{$$ = NULL;}
;

global_decl	:	func_decl ';'					{$$ = $1;}
			|	var_decl ';'					{$$ = $1;}
;

var_decl	:	TK_IDENTIFIER ':' type literal	{
	$$ = astree_create(AST_VAR_DECL, $1, $3, $4, NULL, NULL);
}
			|	TK_IDENTIFIER ':' type'['LIT_INTEGER']' array_init 	{
	$$ = astree_create(AST_ARRAY_DECL, $1, $3, $5, $7, NULL);
}
;

array_init	:	literal array_init {
	$$ = astree_create(AST_ARRAY_INIT, $1, $2, NULL, NULL, NULL);
}
			|		{ $$ = NULL; }
;

func_decl	:	type TK_IDENTIFIER '(' params_list ')' command {
	$$ = astree_create(AST_FUNC_DECL, $1, $2, $4, $6, NULL);
}
;

params_list	:	params	{ $$ = $1; }
			|			{ $$ = NULL; }
;

params	:	params ',' param	{
	$$ = astree_create(AST_FUNC_PARAMS_LIST, $1, $3, NULL, NULL, NULL);
}
		|	param 			{
	$$ = astree_create(AST_FUNC_PARAMS_LIST, $1, NULL, NULL, NULL, NULL);
}
;

param	:	type TK_IDENTIFIER {
	$$ = astree_create(AST_FUNC_PARAM, $1, $2, NULL, NULL, NULL);
}
;

func_call	:	TK_IDENTIFIER '(' args_list ')' {
	$$ = astree_create(AST_FUNC_CALL, $1, $3, NULL, NULL, NULL);
}
;

args_list	:	args	{ $$ = $1; }
			|			{ $$ = NULL; }
;

args	:	args ',' arg	{
	$$ = astree_create(AST_FUNC_ARGS_LIST, $1, $3, NULL, NULL, NULL);
}
		|	arg		{
	$$ = astree_create(AST_FUNC_ARGS_LIST, $1, NULL, NULL, NULL, NULL);
}
;

arg	:	expr
;

command	:	command_block
		|	command_single
;

command_block	:	'{' command_seq '}' {
	$$ = astree_create(AST_CMD_BLOCK, $2, NULL, NULL, NULL, NULL);
}
;

command_seq	:	command_seq command ';'	{
	$$ = astree_create(AST_CMD_LIST, $1, $2, NULL, NULL, NULL);
}
			|		{ $$ = NULL; }
;

command_single	:	var_attr		{ $$ = $1; }
				|	flow_ctrl		{ $$ = $1; }
				|	basic_command	{ $$ = $1; }
				|					{ $$ = NULL; }
;

var_attr	:	TK_IDENTIFIER '=' expr	{
	$$ = astree_create(AST_CMD_VAR_ATTR, $1, $3, NULL, NULL, NULL);
}
			|	TK_IDENTIFIER '#' expr '=' expr	{
	$$ = astree_create(AST_CMD_ARRAY_ATTR, $1, $3, $5, NULL, NULL);
}
;

basic_command	:	KW_READ TK_IDENTIFIER	{
	$$ = astree_create(AST_CMD_READ, $2, NULL, NULL, NULL, NULL);
}
				| 	KW_PRINT print_args		{
	$$ = astree_create(AST_CMD_PRINT, $2, NULL, NULL, NULL, NULL);
}
				| 	KW_RETURN expr			{
	$$ = astree_create(AST_CMD_RETURN, $2, NULL, NULL, NULL, NULL);
}
;

print_args	:	print_args print_arg {
	$$ = astree_create(AST_PRINT_ARGS, $1, $2, NULL, NULL, NULL);
}
			|	print_arg	{
	$$ = astree_create(AST_PRINT_ARGS, $1, NULL, NULL, NULL, NULL);
}
;

print_arg	:	LIT_STRING
			|	expr
;

flow_ctrl	:	KW_WHEN '(' expr ')' KW_THEN command	{
	$$ = astree_create(AST_CMD_WHEN, $3, $6, NULL, NULL, NULL);
}
			|	KW_WHEN '(' expr ')' KW_THEN command KW_ELSE command	{
	$$ = astree_create(AST_CMD_WHEN, $3, $6, $8, NULL, NULL);
}
			| 	KW_WHILE '(' expr ')' command	{
	$$ = astree_create(AST_CMD_WHILE, $3, $5, NULL, NULL, NULL);
}
			|	KW_FOR '(' TK_IDENTIFIER '=' expr KW_TO expr ')' command	{
	$$ = astree_create(AST_CMD_FOR, $3, $5, $7, $9, NULL);
}
;

expr	:	'(' expr ')'	{
	$$ = astree_create(AST_EXPR_PARENS, $2, NULL, NULL, NULL, NULL);
}
		|	expr '+' expr	{
	$$ = astree_create(AST_EXPR_SUM, $1, $3, NULL, NULL, NULL);
}
		|	expr '-' expr	{
	$$ = astree_create(AST_EXPR_SUB, $1, $3, NULL, NULL, NULL);
}
		|	expr '*' expr	{
	$$ = astree_create(AST_EXPR_MULT, $1, $3, NULL, NULL, NULL);
}
		|	expr '/' expr	{
	$$ = astree_create(AST_EXPR_DIV, $1, $3, NULL, NULL, NULL);
}
		|	expr '<' expr	{
	$$ = astree_create(AST_EXPR_LESSER, $1, $3, NULL, NULL, NULL);
}
		|	expr '>' expr	{
	$$ = astree_create(AST_EXPR_GREATER, $1, $3, NULL, NULL, NULL);
}
		|	expr OPERATOR_LE expr	{
	$$ = astree_create(AST_EXPR_LESSER_EQ, $1, $3, NULL, NULL, NULL);
}
		|	expr OPERATOR_GE expr	{
	$$ = astree_create(AST_EXPR_GREATER_EQ, $1, $3, NULL, NULL, NULL);
}
		|	expr OPERATOR_EQ expr	{
	$$ = astree_create(AST_EXPR_EQUAL, $1, $3, NULL, NULL, NULL);
}
		|	expr OPERATOR_NE expr	{
	$$ = astree_create(AST_EXPR_NOT_EQUAL, $1, $3, NULL, NULL, NULL);
}
		|	expr OPERATOR_OR expr	{
	$$ = astree_create(AST_EXPR_OR, $1, $3, NULL, NULL, NULL);
}
		|	expr OPERATOR_AND expr	{
	$$ = astree_create(AST_EXPR_AND, $1, $3, NULL, NULL, NULL);
}
		|	'!' expr	{
	$$ = astree_create(AST_EXPR_NOT, $2, NULL, NULL, NULL, NULL);
}
		|	'-' expr	{
	$$ = astree_create(AST_EXPR_NEGATIVE, $2, NULL, NULL, NULL, NULL);
}	%prec  '-'
		| 	expr_arg	{ $$ = $1; }
;

expr_arg	:	TK_IDENTIFIER	{ $$ = $1; }
			|	TK_IDENTIFIER'['expr']'	{
	$$ = astree_create(AST_EXPR_ARRAY_ACCESS, $1, $3, NULL, NULL, NULL);
}
			|	func_call		{ $$ = $1; }
			|	literal			{ $$ = $1; }
;

%%

int yyerror(char* what) {
	fprintf(stderr, "Parser error at line %d: %s\n", getLineNumber(), what);
	exit(3);
}
