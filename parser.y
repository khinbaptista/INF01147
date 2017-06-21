%{

#include <stdio.h>
#include <stdlib.h>
#include "lang171.h"	// getLineNumber()
#include "hash.h"
#include "astree.h"

%}

%union {
	ASTree* tree;
	HashNode* symbol;
}

%type <tree> type
%type <tree> identifier
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

%token <symbol>TK_IDENTIFIER
%token <symbol>LIT_INTEGER
%token <symbol>LIT_REAL
%token <symbol>LIT_CHAR
%token <symbol>LIT_STRING

%token TOKEN_ERROR

%left '!'
%left OPERATOR_AND
%left OPERATOR_OR
%left '<' '>' OPERATOR_EQ OPERATOR_NE OPERATOR_GE OPERATOR_LE
%left '*' '/'
%left '+' '-'
%left '='

%start global_decl_set

%error-verbose

%%

type	:	KW_BYTE		{ $$ = ast_type_byte();		}
| KW_SHORT				{ $$ = ast_type_short();	}
| KW_LONG				{ $$ = ast_type_long();		}
| KW_FLOAT				{ $$ = ast_type_float();	}
| KW_DOUBLE				{ $$ = ast_type_double();	}
;

identifier : TK_IDENTIFIER { $$ = ast_identifier($1);}

literal	: LIT_INTEGER { $$ = ast_literal($1); }
| LIT_REAL			  { $$ = ast_literal($1); }
| LIT_CHAR			  { $$ = ast_literal($1); }
;

global_decl_set	: global_decl global_decl_set { $$ = ast_program($1, $2); astree_root = $$; }
| {$$ = NULL;}
;

global_decl	: func_decl ';' { $$ = $1; }
| var_decl ';'				{ $$ = $1; }
;

var_decl :	TK_IDENTIFIER ':' type literal	{ $$ = ast_var_decl($1, $3, $4); }
| TK_IDENTIFIER ':' type'['LIT_INTEGER']' array_init 	{ $$ = ast_array_decl($1, $3, $5, $7); }
;

array_init : array_init literal { $$ = ast_array_init($1, $2); }
|					   			{ $$ = NULL; }
;

func_decl :	type TK_IDENTIFIER '(' params_list ')' command { $$ =   ast_func_decl($1, $2, $4, $6); }
;

params_list	: params	{ $$ = $1; }
|						{ $$ = NULL; }
;

params : params ',' param	{ $$ = ast_func_params_list($1, $3); }
| param 					{ $$ = ast_func_params_list(NULL, $1); }
;

param :	type TK_IDENTIFIER { $$ = ast_func_param($1, $2); }
;

func_call :	identifier '(' args_list ')' { $$ = ast_func_call($1, $3);}
;

args_list :	args	{ $$ = $1; }
| 					{ $$ = NULL; }
;

args : args ',' arg	{ $$ = ast_func_args_list($1, $3); }
| arg 				{ $$ = ast_func_args_list(NULL, $1); }
;

arg	: expr { $$ = ast_func_arg($1); }
;

command	: command_block
| command_single
;

command_block :	'{' command_seq '}' { $$ = ast_cmd_block($2); }
;

command_seq	: command_seq command ';' { $$ = ast_cmd_list($1, $2); }
| 									  { $$ = NULL; }
;

command_single : var_attr		{ $$ = $1; }
| flow_ctrl		{ $$ = $1; }
| basic_command	{ $$ = $1; }
| 				{ $$ = NULL; }
;

var_attr :	identifier '=' expr	{ $$ = ast_cmd_var_attr($1, $3); }
|	identifier '#' expr '=' expr {$$ = ast_cmd_array_attr($1, $3, $5);}
;

basic_command :	KW_READ identifier	{ $$ = ast_cmd_read($2); }
| KW_PRINT print_args { $$ = ast_cmd_print($2); }
| KW_RETURN expr		{ $$ = ast_cmd_return($2); }
;

print_args : print_args print_arg { $$ = ast_print_args($1, $2); }
| print_arg	{ $$ = ast_print_args(NULL, $1); }
;

print_arg :	LIT_STRING	{ $$ = ast_literal($1);}
| expr { $$ = $1;}
;

flow_ctrl :	KW_WHEN '(' expr ')' KW_THEN command { $$ = ast_cmd_when($3, $6); }
| KW_WHEN '(' expr ')' KW_THEN command KW_ELSE command { $$ = ast_cmd_when_else($3, $6, $8); }
| KW_WHILE '(' expr ')' command	{ $$ = ast_cmd_while($3, $5); }
| KW_FOR '(' identifier '=' expr KW_TO expr ')' command { $$ = ast_cmd_for($3, $5, $7, $9); }
;

expr : '(' expr ')'	{ $$ = ast_expr_parens($2); }
| expr '+' expr		{ $$ = ast_op(AST_EXPR_SUM, $1, $3); }
| expr '-' expr		{ $$ = ast_op(AST_EXPR_SUB, $1, $3); }
| expr '*' expr		{ $$ = ast_op(AST_EXPR_MULT, $1, $3); }
| expr '/' expr		{ $$ = ast_op(AST_EXPR_DIV, $1, $3); }
| expr '<' expr		{ $$ = ast_op(AST_EXPR_LESSER, $1, $3); }
| expr '>' expr		{ $$ = ast_op(AST_EXPR_GREATER, $1, $3); }
| expr OPERATOR_LE expr	{ $$ = ast_op(AST_EXPR_LESSER_EQ, $1, $3); }
| expr OPERATOR_GE expr	{ $$ = ast_op(AST_EXPR_GREATER_EQ, $1, $3); }
| expr OPERATOR_EQ expr	{ $$ = ast_op(AST_EXPR_EQUAL, $1, $3); }
| expr OPERATOR_NE expr	{ $$ = ast_op(AST_EXPR_NOT_EQUAL, $1, $3); }
| expr OPERATOR_OR expr	{ $$ = ast_op(AST_EXPR_OR, $1, $3); }
| expr OPERATOR_AND expr	{ $$ = ast_op(AST_EXPR_AND, $1, $3); }
| '!' expr { $$ = ast_unary_op(AST_EXPR_NOT, $2); }
| '-' expr { $$ = ast_unary_op(AST_EXPR_NEGATIVE, $2); } %prec  '*'
| expr_arg { $$ = $1; }
;

expr_arg : identifier	{ $$ = ast_expr_scalar_access($1); }
|	identifier'['expr']'	{ $$ = ast_expr_array_access($1, $3); }
|	func_call		{ $$ = $1; }
|	literal			{ $$ = $1; }
;

%%

int yyerror(char* what) {
	fprintf(stderr, "\nParser error at line %d: %s\n", getLineNumber(), what);
	exit(3);
}
