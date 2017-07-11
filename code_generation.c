#include "code_generation.h"
#include <stdlib.h>

void generate_program(TAC *first, FILE* output) {
	if (first == NULL || output == NULL) { return; }

	fprintf(output, "\n# ==== ASM generated ==== #\n");

	/* Generate variables section */
	generate_variables_code(output);
	/* Generate strings section */
	fprintf(output, "\n.section\t.rodata\n");
	fprintf(output, ".percentD:\n");
	fprintf(output, "\t.string \"%%d \"\n");
	fprintf(output, ".percentDRead:\n");
	fprintf(output, "\t.string \"%%d\"\n");
	fprintf(output, ".percentS:\n");
	fprintf(output, "\t.string \"%%s \"\n");
	generate_string_declarations(output);
	fprintf(output, ".text\n");
	/* Generate rest of program */
	for (TAC* tac = first; tac; tac = tac->next) {
		generate_instruction(tac, output);
	}
	// Add newline at end
	fprintf(output, "\n");
}


void generate_instruction(TAC *tac, FILE* output) {
	switch (tac->type) {
		case TAC_LABEL:
		/*
			.label ## just print the label
		*/
			fprintf(output, ".%s:\n", tac->res->text);
			break;
		case TAC_MOV:
		/*
			movl	value(%rip), %eax
			movl	%eax, variable(%rip)
		*/
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->op1->text);
			fprintf(output, "movl\t%%eax, _%s(%%rip)\n", tac->res->text);
			break;
		case TAC_MOV_OFFSET:
		/*
			movl	offset(%rip), %eax
			movl	value(%rip), %edx
			cltq
			movl	%edx, array(,%rax,4)
		*/
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->op1->text);
			fprintf(output, "movl\t_%s(%%rip), %%edx\n", tac->op2->text);
			fprintf(output, "cltq\n");
			fprintf(output, "movl\t%%edx, _%s(,%%rax,4)\n", tac->res->text);
			break;
		case TAC_ACCESS_OFFSET:
		/*
			movl	index(%rip), %eax
			cltq
			movl	var(,%rax,4), %eax
			movl	%eax, destination(%rip)
		*/
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->op2->text);
			fprintf(output, "cltq\n");
			fprintf(output, "movl\t_%s(,%%rax,4), %%eax\n", tac->op1->text);
			fprintf(output, "movl\t%%eax, _%s(%%rip)\n", tac->res->text);
			break;
		case TAC_INC:
		/*
			movl	var(%rip), %eax
			addl	$1, %eax
			movl	%eax, var(%rip)
		*/
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->res->text);
			fprintf(output, "addl\t$1, %%eax\n");
			fprintf(output, "movl\t%%eax, _%s(%%rip)\n", tac->res->text);
			break;
		case TAC_ADD:
		/*
			movl	op1(%rip), %edx
			movl	op2(%rip), %eax
			addl	%edx, %eax
			movl	%eax, res(%rip)
		*/
			fprintf(output, "movl\t_%s(%%rip), %%edx\n", tac->op1->text);
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->op2->text);
			fprintf(output, "addl\t%%edx, %%eax\n");
			fprintf(output, "movl\t%%eax, _%s(%%rip)\n", tac->res->text);
			break;
		case TAC_SUB:
		/*
			movl	op1(%rip), %edx
			movl	op2(%rip), %eax
			subl	%eax, %edx
			movl	%edx, %eax
			movl	%eax, res(%rip)
		*/
			fprintf(output, "movl\t_%s(%%rip), %%edx\n", tac->op1->text);
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->op2->text);
			fprintf(output, "subl\t%%eax, %%edx\n");
			fprintf(output, "movl\t%%edx, _%s(%%rip)\n", tac->res->text);
			break;
		case TAC_MULT:
		/*
			movl	op1(%rip), %edx
			movl	op2(%rip), %eax
			imull	%edx, %eax
			movl	%eax, res(%rip)
		*/
			fprintf(output, "movl\t_%s(%%rip), %%edx\n", tac->op1->text);
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->op2->text);
			fprintf(output, "imull\t%%edx, %%eax\n");
			fprintf(output, "movl\t%%eax, _%s(%%rip)\n", tac->res->text);
			break;
		case TAC_DIV:
		/*
			movl	op1(%rip), %eax
			movl	op2(%rip), %ecx
			cltd
			idivl	%ecx
			movl	%eax, res(%rip)
		*/
			fprintf(output, "movl\t_%s(%%rip), %%edx\n", tac->op1->text);
			fprintf(output, "movl\t_%s(%%rip), %%ecx\n", tac->op2->text);
			fprintf(output, "cltd\n");
			fprintf(output, "idivl\t%%ecx\n");
			fprintf(output, "movl\t%%eax, _%s(%%rip)\n", tac->res->text);
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
			fprintf(output, "movl\t_%s(%%rip), %%edx\n", tac->op1->text);
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->op2->text);
			fprintf(output, "cmpl\t%%eax, %%edx\n");
			fprintf(output, "setl\t%%al\n");
			fprintf(output, "movzbl\t%%al, %%eax\n");
			fprintf(output, "movl\t%%eax, _%s(%%rip)\n", tac->res->text);
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
			fprintf(output, "movl\t_%s(%%rip), %%edx\n", tac->op1->text);
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->op2->text);
			fprintf(output, "cmpl\t%%eax, %%edx\n");
			fprintf(output, "setg\t%%al\n");
			fprintf(output, "movzbl\t%%al, %%eax\n");
			fprintf(output, "movl\t%%eax, _%s(%%rip)\n", tac->res->text);
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
			fprintf(output, "movl\t_%s(%%rip), %%edx\n", tac->op1->text);
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->op2->text);
			fprintf(output, "cmpl\t%%eax, %%edx\n");
			fprintf(output, "setle\t%%al\n");
			fprintf(output, "movzbl\t%%al, %%eax\n");
			fprintf(output, "movl\t%%eax, _%s(%%rip)\n", tac->res->text);
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
			fprintf(output, "movl\t_%s(%%rip), %%edx\n", tac->op1->text);
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->op2->text);
			fprintf(output, "cmpl\t%%eax, %%edx\n");
			fprintf(output, "setge\t%%al\n");
			fprintf(output, "movzbl\t%%al, %%eax\n");
			fprintf(output, "movl\t%%eax, _%s(%%rip)\n", tac->res->text);
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
			fprintf(output, "movl\t_%s(%%rip), %%edx\n", tac->op1->text);
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->op2->text);
			fprintf(output, "cmpl\t%%eax, %%edx\n");
			fprintf(output, "sete\t%%al\n");
			fprintf(output, "movzbl\t%%al, %%eax\n");
			fprintf(output, "movl\t%%eax, _%s(%%rip)\n", tac->res->text);
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
			fprintf(output, "movl\t_%s(%%rip), %%edx\n", tac->op1->text);
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->op2->text);
			fprintf(output, "cmpl\t%%eax, %%edx\n");
			fprintf(output, "setne\t%%al\n");
			fprintf(output, "movzbl\t%%al, %%eax\n");
			fprintf(output, "movl\t%%eax, _%s(%%rip)\n", tac->res->text);
			break;
		case TAC_OR:
		/*
			movl	op1(%rip), %edx
			movl	op2(%rip), %eax
			orl	%edx, %eax			## bitwise (??)
			movl	%eax, res(%rip)
		*/
			fprintf(output, "movl\t_%s(%%rip), %%edx\n", tac->op1->text);
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->op2->text);
			fprintf(output, "orl\t%%edx, %%eax\n");
			fprintf(output, "movl\t%%eax, _%s(%%rip)\n", tac->res->text);
			break;
		case TAC_AND:
		/*
			movl	op1(%rip), %edx
			movl	op2(%rip), %eax
			andl	%edx, %eax		## bitwise (??)
			movl	%eax, res(%rip)
		*/
			fprintf(output, "movl\t_%s(%%rip), %%edx\n", tac->op1->text);
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->op2->text);
			fprintf(output, "andl\t%%edx, %%eax\n");
			fprintf(output, "movl\t%%eax, _%s(%%rip)\n", tac->res->text);
			break;
		case TAC_NOT:
		/*
			movl	op1(%rip), %eax
			testl	%eax, %eax		## if %eax == 0, %al receives one, zero otherwise
			sete	%al
			movzbl	%al, %eax
			movl	%eax, res(%rip)
		*/
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->op1->text);
			fprintf(output, "testl\t%%eax, %%eax\n");
			fprintf(output, "sete\t%%al\n");
			fprintf(output, "movzbl\t%%al, %%eax\n");
			fprintf(output, "movl\t%%eax, _%s(%%rip)\n", tac->res->text);
			break;
		case TAC_NEGATIVE:
		/*
			movl	op1(%rip), %eax
			negl	%eax
			movl	%eax, res(%rip)
		*/
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->op1->text);
			fprintf(output, "negl\t%%eax\n");
			fprintf(output, "movl\t%%eax, _%s(%%rip)\n", tac->res->text);
			break;
		case TAC_IFZ:
		/*
			movl	var(%rip), %eax
			testl	%eax, %eax
			je	.label
		*/
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->op1->text);
			fprintf(output, "testl\t%%eax, %%eax\n");
			fprintf(output, "je\t.%s\n", tac->res->text);
			break;
		case TAC_JMP:
		/*
			jmp	.label
		*/
			fprintf(output, "jmp\t.%s\n", tac->res->text);
			break;
		case TAC_FUNC_ARG:
		/*
			movl	value(%rip), %eax
			movl	%eax, argument(%rip)
		*/
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->op1->text);
			fprintf(output, "movl\t%%eax, _%s(%%rip)\n", tac->res->text);
			break;
		case TAC_FUNC_CALL:
		/*
			call	function
			movl	%eax, res(%rip)	## result is in eax
		*/
			fprintf(output, "call\t%s\n", tac->op1->text);
			fprintf(output, "movl\t%%eax, _%s(%%rip)\n", tac->res->text);
			break;
		case TAC_FUNC_BEGIN:
		/*
				.globl funcname
			.funcname:
				.cfi_startproc
				pushq %rbp
				movq %rsp, %rbp
		*/
			fprintf(output, "\n.globl\t%s\n", tac->res->text);
			fprintf(output, "%s:\n", tac->res->text);
			fprintf(output, ".cfi_startproc\n");
			fprintf(output, "pushq\t%%rbp\n");
			fprintf(output, "movq\t%%rsp, %%rbp\n");
			break;
		case TAC_FUNC_END:
		/*
			popq %rbp
			ret
			.cfi_endproc
		*/
			fprintf(output, "popq\t%%rbp\n");
			fprintf(output, "ret\n");
			fprintf(output, ".cfi_endproc\n");
			break;
		case TAC_FUNC_RET:
		/*
			## move return value to %eax
			movl	returnValue(%rip), %eax
			## should probably move TAC_FUNC_END here and stop generating dead code
		*/
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->res->text);
			fprintf(output, "popq\t%%rbp\n");
			fprintf(output, "ret\n");
			break;
		case TAC_READ:
		/*
			movl	$variable, %esi
			movl	$.percentD, %edi
			call	scanf
		*/
			fprintf(output, "movl\t$_%s, %%esi\n", tac->res->text);
			fprintf(output, "movl\t$.percentDRead, %%edi\n");
			fprintf(output, "movl\t$0, %%eax\n");
			fprintf(output, "call\tscanf\n");
			break;
		case TAC_PRINT:
		/*
			movl	a(%rip), %eax
			movl	%eax, %esi
			movl	$.percentD, %edi	## .percentD is the %d string
			movl	$0, %eax	(optional?)
			call	printf
		*/
		if(tac->res->type == SYMBOL_LIT_STRING) {
			fprintf(output, "movl\t$.%s, %%edi\n", tac->res->string_name);
			fprintf(output, "movl\t$0, %%eax\n");
			fprintf(output, "call\tprintf\n");
		} else {
			fprintf(output, "movl\t_%s(%%rip), %%eax\n", tac->res->text);
			fprintf(output, "movl\t%%eax, %%esi\n");
			fprintf(output, "movl\t$.percentD, %%edi\n");
			fprintf(output, "movl\t$0, %%eax\n");
			fprintf(output, "call\tprintf\n");
		}
			break;
		default:
			return;
	}
}

void generate_variables_code(FILE* output) {
	fprintf(output, ".data\n");
	int i;
	for (i = 0; i < HASH_SIZE; i++) {
		if (_table[i]) {
			generate_var_code(_table[i], output);
			HashNode *it = _table[i]->next;
			while(it) {
				generate_var_code(it, output);
				it = it->next;
			}
		}
	}
}

void generate_var_code(HashNode* item, FILE* output) {
	if(item->id_type == ID_SCALAR) {
		if(item->scalar_init){
		/* ##Var Declarations:
				.globl	{var}
				.align 4
				.size	{var}, 4
			{var}:
				.long	{value}
		*/
			fprintf(output, "\t.globl	_%s\n", item->text);
			fprintf(output, "\t.align 4\n");
			fprintf(output, "\t.size	_%s, 4\n", item->text);
			fprintf(output, "_%s:\n", item->text);
			fprintf(output, "\t.long	%s\n", item->scalar_init->text);
		} else {
		/*	## Var uninitialized (temp, function params)
			.comm	{var}, 4
		*/
			fprintf(output, "\t.comm	_%s, 4\n", item->text);
		}
	} else if(item->id_type == ID_ARRAY) {
		if(item->array_init) {
		/* ## Array - initialized
				.globl	{array}
				.data
				.align 32
				.size	{array}, {size*4}
			{array}:
				.long	{value1}
				.long	{value2}
				[... other values]
		*/
			fprintf(output, "\t.globl	_%s\n", item->text);
			fprintf(output, "\t.data\n");
			fprintf(output, "\t.align 32\n");
			fprintf(output, "\t.size	_%s, %d\n", item->text, item->array_size*4);
			fprintf(output, "_%s:\n", item->text);
			generate_array_init_code(item->array_init,output);
		} else {
		/*
			## Array - uninitialized
			.comm	array, {size*4}
		*/
			fprintf(output, "\t.comm	_%s, %d\n", item->text, item->array_size*4);
		}
	} else if(item->type == SYMBOL_LIT_INTEGER) {
		// Use number for variable name
		fprintf(output, "\t.globl	_%s\n", item->text);
		fprintf(output, "\t.align 4\n");
		fprintf(output, "\t.size	_%s, 4\n", item->text);
		fprintf(output, "_%s:\n", item->text);
		fprintf(output, "\t.long	%s\n", item->text);

	}  else if(item->type == SYMBOL_LIT_REAL) {
		// Use number for variable name and convert value to int
		fprintf(output, "\t.globl	_%s\n", item->text);
		fprintf(output, "\t.align 4\n");
		fprintf(output, "\t.size	_%s, 4\n", item->text);
		fprintf(output, "_%s:\n", item->text);
		fprintf(output, "\t.long	%d\n", atoi(item->text));

	} else if(item->type == SYMBOL_LIT_CHAR) {
		char* new_name = calloc(7,sizeof(char));	//char_X\0
		char value = item->text[1];
		sprintf(new_name,"char_%c", value);
		fprintf(output, "\t.globl	_%s\n", new_name);
		fprintf(output, "\t.align 4\n");
		fprintf(output, "\t.size	_%s, 4\n", new_name);
		fprintf(output, "_%s:\n", new_name);
		fprintf(output, "\t.long	%d\n", value);
		item->text = new_name;
	}
}

void generate_array_init_code(ASTree* list, FILE* output) {
	if(list) {
		generate_array_init_code(list->children[0], output);
		fprintf(output, "\t.long	%s\n", list->children[1]->symbol->text);
	}
}

void generate_string_declarations(FILE *output) {
	for (int i = 0; i < HASH_SIZE; i++) {
		if (_table[i] != NULL && _table[i]->datatype == HASH_TYPE_STRING) {
			fprintf(output, ".%s:\n", _table[i]->string_name);
			fprintf(output, "\t.string %s\n", _table[i]->text);
		}
	}
}
