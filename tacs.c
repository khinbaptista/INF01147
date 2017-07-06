#include "tacs.h"

#include <stdio.h>
#include <stdlib.h>

void _tac_print(TAC*);
TAC* tac_create_numeric_op(int type, TAC* op1, TAC* op2);
TAC* tac_create_bool_op(int type, TAC* op1, TAC* op2);
TAC* tac_create_not(TAC* op);
TAC* tac_create_negative(TAC* op);
TAC* tac_create_var_attribution(TAC* var, TAC* value);
TAC* tac_create_array_decl(ASTree* node);
TAC* tac_create_array_access(TAC* var, TAC* index);
TAC* tac_create_array_attribution(TAC* var, TAC* index, TAC* value);
TAC* tac_create_function_call(ASTree* node);
TAC* tac_create_function_arg(ASTree* arg, HashNode* function, int arg_number);
TAC* tac_create_function_declaration(HashNode* function, TAC* body);
TAC* tac_create_function_return(TAC* value);
TAC* tac_create_when(TAC* condition, TAC* cmd);
TAC* tac_create_when_else(TAC* condition, TAC* when_cmd, TAC* else_cmd);
TAC* tac_create_while(TAC* condition, TAC* cmd);
TAC* tac_create_for(TAC* var, TAC* start, TAC* end, TAC* command);
TAC* tac_create_cmd_read(TAC* var);
TAC* tac_create_cmd_print(TAC* prev_arg, TAC* curr_arg);

TAC* tac_generate(ASTree* node) {
	if (!node)  {
		return NULL;
	}


	TAC *children[MAX_CHILDREN];
	if(node->type != AST_FUNC_CALL || node->type != AST_ARRAY_DECL) {
		// Recursion happens before current node except for cases above
		for (int i = 0; i < MAX_CHILDREN; i++) {
			children[i] = tac_generate(node->children[i]);
		}
	}

	switch (node->type) {
		case AST_LITERAL:
			return tac_create(TAC_SYMBOL, node->symbol, NULL, NULL);
		case AST_IDENTIFIER:
			return tac_create(TAC_SYMBOL, node->symbol, NULL, NULL);
		case AST_EXPR_SCALAR_ACCESS:
			return tac_create(TAC_SYMBOL, node->children[0]->symbol, NULL, NULL);
		case AST_EXPR_ARRAY_ACCESS:
			return tac_create_array_access(children[0], children[1]);

		/* Variables */
		case AST_CMD_VAR_ATTR:
			return tac_create_var_attribution(children[0], children[1]);
		case AST_CMD_ARRAY_ATTR:
			return tac_create_array_attribution(children[0], children[1], children[2]);
		case AST_VAR_DECL:
			return tac_create_var_attribution(children[0], children[2]);
		case AST_ARRAY_DECL:
			return tac_create_array_decl(node);
		/* Functions */
		case AST_FUNC_CALL:
			return tac_create_function_call(node);
		case AST_FUNC_DECL:
			return tac_create_function_declaration(node->children[1]->symbol, children[3]);
		case AST_CMD_RETURN:
			return tac_create_function_return(children[0]);

		/* Control flow */
		case AST_CMD_WHEN:
			return tac_create_when(children[0], children[1]);
		case AST_CMD_WHEN_ELSE:
			return tac_create_when_else(children[0], children[1], children[2]);
		case AST_CMD_WHILE:
			return tac_create_while(children[0], children[1]);
		case AST_CMD_FOR:
			return tac_create_for(children[0], children[1], children[2], children[3]);

		/* IO */
		case AST_CMD_READ:
			return tac_create_cmd_read(children[0]);
		case AST_PRINT_ARGS:
			return tac_create_cmd_print(children[0], children[1]);
		/* Binary operations */
		case AST_EXPR_SUM:
			return tac_create_numeric_op(TAC_ADD, children[0], children[1]);
		case AST_EXPR_SUB:
			return tac_create_numeric_op(TAC_SUB, children[0], children[1]);
		case AST_EXPR_MULT:
			return tac_create_numeric_op(TAC_MULT, children[0], children[1]);
		case AST_EXPR_DIV:
			return tac_create_numeric_op(TAC_DIV, children[0], children[1]);
		case AST_EXPR_LESSER:
			return tac_create_bool_op(TAC_LESSER,children[0], children[1]);
		case AST_EXPR_GREATER:
			return tac_create_bool_op(TAC_GREATER,children[0], children[1]);
		case AST_EXPR_LESSER_EQ:
			return tac_create_bool_op(TAC_LESSER_EQ,children[0], children[1]);
		case AST_EXPR_GREATER_EQ:
			return tac_create_bool_op(TAC_GREATER_EQ,children[0], children[1]);
		case AST_EXPR_EQUAL:
			return tac_create_bool_op(TAC_EQUAL,children[0], children[1]);
		case AST_EXPR_NOT_EQUAL:
			return tac_create_bool_op(TAC_NOT_EQUAL,children[0], children[1]);
		case AST_EXPR_OR:
			return tac_create_bool_op(TAC_OR,children[0], children[1]);
		case AST_EXPR_AND:
			return tac_create_bool_op(TAC_AND,children[0], children[1]);

		/* Unary operations */
		case AST_EXPR_NOT:
			return tac_create_not(children[0]);
		case AST_EXPR_NEGATIVE:
			return tac_create_negative(children[0]);

		default:
			return tac_join(tac_join(tac_join(
				children[0], children[1]), children[2]), children[3]);
	}
}

TAC* tac_create(int type, HashNode *res, HashNode *op1, HashNode *op2) {
	TAC* tac	= calloc(1, sizeof(TAC));
	tac->type	= type;
	tac->res	= res;
	tac->op1	= op1;
	tac->op2	= op2;
	tac->prev	= NULL;
	tac->next	= NULL;
	return tac;
}

TAC* tac_create_numeric_op(int type, TAC* op1, TAC* op2) {
	int datatype = get_operation_datatype(op1->res->datatype, op2->res->datatype);
	return tac_join(tac_join(op1, op2),
		tac_create(type, hash_make_temp(datatype), op1->res, op2->res));
}

TAC* tac_create_bool_op(int type, TAC* op1, TAC* op2) {
	return tac_join(tac_join(op1, op2),
		tac_create(type, hash_make_temp(HASH_TYPE_BOOL), op1->res, op2->res));
}

TAC* tac_create_not(TAC* op) {
	return tac_join(op,
		tac_create(TAC_NOT, hash_make_temp(HASH_TYPE_BOOL), op->res, NULL));
}

TAC* tac_create_negative(TAC* op) {
	return tac_join(op,
		tac_create(TAC_NEGATIVE, hash_make_temp(op->res->datatype), op->res, NULL));
}

TAC* tac_create_var_attribution(TAC* var, TAC* value) {
	return tac_join(tac_join(var,value),
		tac_create(TAC_MOV, var->res, value->res, NULL));
}

TAC* tac_create_array_decl(ASTree* node) {
	if(node->children[3]) {
		int array_length = atoi(node->children[2]->symbol->text);
		HashNode* array = node->children[0]->symbol;
		ASTree* init_list = node->children[3];
		TAC* tac_list = NULL;
		char* position = calloc(17, sizeof(char));
		for(int i = array_length - 1; i >= 0; --i) {
			HashNode* value = init_list->children[1]->symbol;
			sprintf(position, "%d", i);
			HashNode* index = hash_insert(SYMBOL_LIT_INTEGER, position);
			tac_list = tac_join(
				tac_create(TAC_MOV_OFFSET, array, index, value),
				tac_list
			);
			init_list = init_list->children[0];
		}
		return tac_list;
	} else {
		return NULL;
	}
}

TAC* tac_create_array_attribution(TAC* array, TAC* index, TAC* value) {
	TAC* attribution = tac_create(TAC_MOV_OFFSET, array->res, index->res, value->res);
	return tac_join(tac_join(index, value), attribution);
}

TAC* tac_create_array_access(TAC* array, TAC* index) {
	return tac_join(tac_join(
		array,
		index),
		tac_create(
			TAC_ACCESS_OFFSET,
			hash_make_temp(array->res->datatype),
			array->res,
			index->res));
}

TAC* tac_create_function_call(ASTree* node){
	int num_args = ast_param_list_count(node->children[1]);
	HashNode* function = node->children[0]->symbol;
	ASTree* list = node->children[1];
	TAC* arg_list = NULL;
	for(int i = num_args - 1; i >= 0; --i) {
		// Join new argument to list
		// (inverted since arg list is inverted in AST)
		arg_list = tac_join(
			tac_create_function_arg(list->children[1], function, i),
			arg_list);
		// Point list to next element
		list = list->children[0];
	}
	TAC* call = tac_create(TAC_FUNC_CALL, hash_make_temp(function->datatype), function, NULL);
	return tac_join(arg_list,call);
}
TAC* tac_create_function_arg(ASTree* arg, HashNode* function, int arg_number) {
	TAC* value_tac = tac_generate(arg);
	char* num_string = calloc(17, sizeof(char));
	sprintf(num_string, "%d", arg_number);
	HashNode* number = hash_insert(SYMBOL_LIT_INTEGER, num_string);
	TAC* arg_tac = tac_create(TAC_FUNC_ARG,value_tac->res,function, number);
	return tac_join(value_tac,arg_tac);
}

TAC* tac_create_function_declaration(HashNode* function, TAC* body) {
	TAC* begin = tac_create(TAC_FUNC_BEGIN, function, NULL, NULL);
	TAC* end = tac_create(TAC_FUNC_END, function, NULL, NULL);
	return tac_join(tac_join(begin, body), end);
}
TAC* tac_create_function_return(TAC* value) {
	return tac_join(value, tac_create(TAC_FUNC_RET, value->res, NULL, NULL));
}
TAC* tac_create_when(TAC* condition, TAC* cmd) {
	HashNode* hash_label = hash_make_label();
	TAC* jump = tac_create(TAC_IFZ, hash_label, condition->res, NULL);
	TAC* label = tac_create(TAC_LABEL, hash_label, NULL, NULL);
	return tac_join(tac_join(tac_join(
		condition,
		jump),
		cmd),
		label);
}

TAC* tac_create_when_else(TAC* condition, TAC* when_cmd, TAC* else_cmd) {
	HashNode* hash_label_else = hash_make_label();
	HashNode* hash_label_end = hash_make_label();
	TAC* jump_to_else = tac_create(TAC_IFZ, hash_label_else, condition->res, NULL);
	TAC* label_else = tac_create(TAC_LABEL, hash_label_else, NULL, NULL);
	TAC* jump_to_end = tac_create(TAC_JMP, hash_label_end, NULL, NULL);
	TAC* label_end = tac_create(TAC_LABEL, hash_label_end, NULL, NULL);
	return tac_join(tac_join(tac_join(tac_join(tac_join(tac_join(
		condition,
		jump_to_else),
		when_cmd),
		jump_to_end),
		label_else),
		else_cmd),
		label_end);
}

TAC* tac_create_while(TAC* condition, TAC* cmd) {
	HashNode* hash_label_start = hash_make_label();
	HashNode* hash_label_skip = hash_make_label();
	TAC* jump_start = tac_create(TAC_JMP, hash_label_start, NULL, NULL);
	TAC* label_start = tac_create(TAC_LABEL, hash_label_start, NULL, NULL);
	TAC* jump_skip = tac_create(TAC_IFZ, hash_label_skip, condition->res, NULL);
	TAC* label_skip = tac_create(TAC_LABEL, hash_label_skip, NULL, NULL);
	return tac_join(tac_join(tac_join(tac_join(tac_join(
		label_start,
		condition),
		jump_skip),
		cmd),
		jump_start),
		label_skip);
}

TAC* tac_create_for(TAC* var, TAC* start, TAC* end, TAC* command) {
	TAC* init_var = tac_create(TAC_MOV, var->res, start->res, NULL);
	HashNode* hash_label_start = hash_make_label();
	TAC* label_start = tac_create(TAC_LABEL, hash_label_start, NULL, NULL);
	HashNode* hash_label_end = hash_make_label();
	TAC* label_end = tac_create(TAC_LABEL, hash_label_end, NULL, NULL);
	TAC* test_var = tac_create_bool_op(TAC_LESSER_EQ, var, end);
	TAC* jump_to_end = tac_create(TAC_IFZ, hash_label_end, test_var->res, NULL);
	TAC* increment_var = tac_create(TAC_INC, var->res, NULL, NULL);
	TAC* jump_to_test = tac_create(TAC_JMP, hash_label_start, NULL, NULL);
	return tac_join(tac_join(tac_join(tac_join(tac_join(tac_join(tac_join(
		init_var,
		label_start),
		test_var),
		jump_to_end),
		command),
		increment_var),
		jump_to_test),
		label_end);
}

TAC* tac_create_cmd_read(TAC* var) {
	return tac_create(TAC_READ, var->res, NULL, NULL);
}
TAC* tac_create_cmd_print(TAC* prev_arg, TAC* curr_arg) {
	return tac_join(tac_join(
		prev_arg,
		curr_arg),
		tac_create(TAC_PRINT, curr_arg->res, NULL, NULL));
}


TAC* tac_join(TAC *tac1, TAC *tac2) {
	TAC *tac;

	if (!tac1) return tac2;
	if (!tac2) return tac1;

	tac = tac2;
	while (tac->prev) { tac = tac->prev; }
	tac->prev = tac1;

	return tac2;
}

TAC* tac_reverse(TAC* tac) {
	TAC *t = NULL;

	for (t = tac; t->prev; t = t->prev)
		t->prev->next = t;

	return t;
}

void _tac_print(TAC *tac) {
	//if(tac->type == TAC_SYMBOL) return;
	//fprintf(stderr, "TAC (");

	switch (tac->type) {
		case TAC_SYMBOL:		return;fprintf(stderr,"SYMBOL");		break;
		case TAC_LABEL:			fprintf(stderr,"LABEL");		break;
		case TAC_MOV:			fprintf(stderr,"MOV");			break;
		case TAC_MOV_OFFSET:	fprintf(stderr,"MOV_OFFSET");	break;
		case TAC_ACCESS_OFFSET:	fprintf(stderr,"ACCESS_OFFSET");	break;
		case TAC_INC:			fprintf(stderr,"INC");			break;
		case TAC_ADD:			fprintf(stderr,"ADD");			break;
		case TAC_SUB:			fprintf(stderr,"SUB");			break;
		case TAC_MULT:			fprintf(stderr,"MULT");			break;
		case TAC_DIV:			fprintf(stderr,"DIV");			break;
		case TAC_LESSER:		fprintf(stderr,"LESSER"); 		break;
		case TAC_GREATER:		fprintf(stderr,"GREATER"); 		break;
		case TAC_LESSER_EQ:		fprintf(stderr,"LESSER_EQ"); 	break;
		case TAC_GREATER_EQ:	fprintf(stderr,"GREATER_EQ"); 	break;
		case TAC_EQUAL:			fprintf(stderr,"EQUAL"); 		break;
		case TAC_NOT_EQUAL:		fprintf(stderr,"NOT_EQUAL"); 	break;
		case TAC_OR:			fprintf(stderr,"OR"); 			break;
		case TAC_AND:			fprintf(stderr,"AND"); 			break;
		case TAC_NOT:			fprintf(stderr,"NOT"); 			break;
		case TAC_NEGATIVE:		fprintf(stderr,"NEGATIVE"); 	break;
		case TAC_IFZ:			fprintf(stderr,"IFZ");			break;
		case TAC_JMP:			fprintf(stderr,"JMP");			break;
		case TAC_FUNC_ARG:		fprintf(stderr,"FUNC_ARG");		break;
		case TAC_FUNC_CALL:		fprintf(stderr,"FUNC_CALL");	break;
		case TAC_FUNC_BEGIN:	fprintf(stderr,"FUNC_BEGIN");	break;
		case TAC_FUNC_END:		fprintf(stderr,"FUNC_END");		break;
		case TAC_FUNC_RET:		fprintf(stderr,"RETURN");		break;
		case TAC_READ:			fprintf(stderr,"READ");			break;
		case TAC_PRINT:			fprintf(stderr,"PRINT");		break;
		default:				fprintf(stderr,"UNKNOWN");		break;
	}

	if (tac->res)	fprintf(stderr, " %s", tac->res->text);
	//else			fprintf(stderr, ", NULL");
	if (tac->op1)	fprintf(stderr, " %s", tac->op1->text);
	//else			fprintf(stderr, ", NULL");
	if (tac->op2)	fprintf(stderr, " %s", tac->op2->text);
	//else			fprintf(stderr, ", NULL");

	fprintf(stderr, "\n");
}

void tac_print_back(TAC *last) {
	TAC *tac = NULL;
	for (tac = last; tac; tac = tac->prev) {
		_tac_print(tac);
	}
}

void tac_print_forward(TAC *first) {
	TAC *tac = NULL;
	for (tac = first; tac; tac = tac->next) {
		_tac_print(tac);
	}
}

void _tac_print_instruction(TAC *tac, FILE* output) {
	switch (tac->type) {
		case TAC_LABEL:
		/*
			.label ## just print the label
		*/
			break;
		case TAC_MOV:
		/*
			movl	value(%rip), %eax
			movl	%eax, variable(%rip)
		*/
			break;
		case TAC_MOV_OFFSET:
		/*
			movl	value(%rip), %eax
			movl	%eax, variable+{4*offset}(%rip)
		*/
			break;
		case TAC_ACCESS_OFFSET:
		/*
			movl	source+{4*offset}(%rip), %eax
			movl	%eax, destination(%rip)
		*/
			break;
		case TAC_INC:
		/*
			movl	var(%rip), %eax
			addl	$1, %eax
			movl	%eax, var(%rip)
		*/
			break;
		case TAC_ADD:
		/*
			movl	op1(%rip), %edx
			movl	op2(%rip), %eax
			addl	%edx, %eax
			movl	%eax, res(%rip)
		*/
			break;
		case TAC_SUB:
		/*
			movl	op1(%rip), %edx
			movl	op2(%rip), %eax
			subl	%eax, %edx
			movl	%edx, %eax
			movl	%eax, res(%rip)
		*/
			break;
		case TAC_MULT:
		/*
			movl	op1(%rip), %edx
			movl	op2(%rip), %eax
			imull	%edx, %eax
			movl	%eax, res(%rip)
		*/
			break;
		case TAC_DIV:
		/*
			movl	op1(%rip), %eax
			movl	op2(%rip), %ecx
			cltd
			idivl	%ecx
			movl	%eax, res(%rip)
		*/
			break;
		case TAC_LESSER:
		/*
			movl	op1(%rip), %edx
			movl	op2(%rip), %eax
			cmpl	%eax, %edx
			setl	%al
			movzbl	%al, %eax
			movl	%eax, res(%rip)
		*/
			break;
		case TAC_GREATER:
		/*
			movl	op1(%rip), %edx
			movl	op2(%rip), %eax
			cmpl	%eax, %edx
			setg	%al
			movzbl	%al, %eax
			movl	%eax, res(%rip)
		*/
			break;
		case TAC_LESSER_EQ:
		/*
			movl	op1(%rip), %edx
			movl	op2(%rip), %eax
			cmpl	%eax, %edx
			setle	%al
			movzbl	%al, %eax
			movl	%eax, res(%rip)
		*/
			break;
		case TAC_GREATER_EQ:
		/*
			movl	op1(%rip), %edx
			movl	op2(%rip), %eax
			cmpl	%eax, %edx
			setge	%al
			movzbl	%al, %eax
			movl	%eax, res(%rip)
		*/
			break;
		case TAC_EQUAL:
		/*
			movl	op1(%rip), %edx
			movl	op2(%rip), %eax
			cmpl	%eax, %edx
			sete	%al
			movzbl	%al, %eax
			movl	%eax, res(%rip)
		*/
			break;
		case TAC_NOT_EQUAL:
		/*
			movl	op1(%rip), %edx
			movl	op2(%rip), %eax
			cmpl	%eax, %edx
			setne	%al
			movzbl	%al, %eax
			movl	%eax, res(%rip)
		*/
			break;
		case TAC_OR:
		/*
			movl	op1(%rip), %edx
			movl	op2(%rip), %eax
			orl	%edx, %eax			## bitwise
			movl	%eax, res(%rip)
		*/
			break;
		case TAC_AND:
		/*
			movl	op1(%rip), %edx
			movl	op2(%rip), %eax
			andl	%edx, %eax		## bitwise
			movl	%eax, res(%rip)
		*/
			break;
		case TAC_NOT:
		/*
			movl	op1(%rip), %eax
			testl	%eax, %eax		## if %eax == 0, %al receives one, zero otherwise
			sete	%al
			movzbl	%al, %eax
			movl	%eax, res(%rip)
		*/
			break;
		case TAC_NEGATIVE:
		/*
			movl	op1(%rip), %eax
			negl	%eax
			movl	%eax, res(%rip)
		*/
			break;
		case TAC_IFZ:
		/*
			movl	var(%rip), %eax
			testl	%eax, %eax
			je	.label
		*/
			break;
		case TAC_JMP:
		/*
			jmp	.label
		*/
			break;
		case TAC_FUNC_ARG:
		/*
			movl	value(%rip), %eax
			movl	%eax, argument(%rip)
		*/
			break;
		case TAC_FUNC_CALL:
		/*
			call	function
			movl	%eax, res(%rip)	## result is in eax
		*/
			break;
		case TAC_FUNC_BEGIN:
		/*
				.globl funcname
			.funcname:
				.cfi_startproc
				pushq %rbp
				movq %rsp, %rbp
		*/
			break;
		case TAC_FUNC_END:
		/*
			popq %rbp
			ret
			.cfi_endproc
		*/
			break;
		case TAC_FUNC_RET:
		/*
			## move return value to %eax
			movl	returnValue(%rip), %eax
			## should probably move TAC_FUNC_END here and stop generating dead code
		*/
			break;
		case TAC_READ:
		/*
			movl	$variable, %esi
			movl	$.LC0, %edi
			call	__isoc99_scanf
		*/
			break;
		case TAC_PRINT:
		/*
			movl	a(%rip), %eax
			movl	%eax, %esi
			movl	$.percentD, %edi	## .percentD is the %d string
			movl	$0, %eax	(optional?)
			call	printf
		*/
			break;
		default:
			return;
	}

	fprintf(output, "\n");
}

void tac_print_assembly(TAC *first, FILE* output) {
	/*
	##Var Declarations:
			.globl	a
			.align 4
			.size	a, 4
		a:
			.long	6616
	## Array - uninitialized
		.comm	array, size
	## Array - initialized
			.globl	a
			.data
			.align 32
			.size	a, size
		a:
			.long	value1
			.long	value2
			[... other values]
	## Strings declaration
		.section	.rodata
	.percentd:
		.string	"%d"	## there will be only one %d
	.LC0:
		.string	"string1"
	.LC1:
		.string	"%d"
	## Main:
			.text
			.globl	main
		main:
			.cfi_startproc
			pushq	%rbp
	## Program here
	*/
	TAC *tac = NULL;
	for (tac = first; tac; tac = tac->next) {
		_tac_print_instruction(tac, output);
	}
	/* ## End of main:
		popq	%rbp
				ret
				.cfi_endproc
	*/
}