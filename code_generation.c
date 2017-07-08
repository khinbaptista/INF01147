#include "code_generation.h"

void generate_program(TAC *first, FILE* output) {
	generate_variables_code(output);
	/*
    ## Strings declaration
		.section	.rodata
	.percentd:
		.string	"%d"	## there will be only one %d

    ## TODO: make a second pass through hash_table getting only strings
    ## (or find alternative)
	.{name??}:
		.string	"stringvalue"

	## Main:
			.text
			.globl	main
		main:
			.cfi_startproc
			pushq	%rbp
	## Program here
	*/
	for (TAC* tac = first; tac; tac = tac->next) {
		generate_instruction(tac, output);
	}
	/* ## End of main:
		popq	%rbp
				ret
				.cfi_endproc
	*/
}


void generate_instruction(TAC *tac, FILE* output) {
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
}


void generate_variables_code(FILE* output) {
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
	}
    /*
        TODO: generate initialized variables for all LIT_REAL LIT_INTEGER LIT_CHAR
    */
}

void generate_array_init_code(ASTree* list, FILE* output) {
	if(list) {
		generate_array_init_code(list->children[0], output);
		fprintf(output, "\t.long	%s\n", list->children[1]->symbol->text);
	}
}

