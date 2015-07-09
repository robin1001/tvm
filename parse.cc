#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lex.h"
#include "parse.h"
#include "tvm.h"

static char g_prev_token[MAX_TOKEN];

static void syntax_unexpected(char *s) {
	printf("unexpted token %s\n", s);
	exit(-1);
}


static void match(TokenType expected) {
	strcpy(g_prev_token, g_token);
	if (g_token_type == expected) g_token_type = get_token();
	else {
		printf("syntax error, expected %d but get %d %s\n", expected, g_token_type, g_token);
		exit(-1);
	}
}

static int get_reg_id(char *token) {
	return atoi(token+1);	
}

static void expr(Tvm *tvm) {
	Args arg0, arg1, arg2;
	if (TOKEN_LABEL == g_token_type) {
		//TODO add label in symbol table	
		tvm->add_label_and_set_addr(g_token);
		match(TOKEN_LABEL);
		match(TOKEN_COLON);
		//printf("%s\n", g_token);
	}
	OpType t = g_op_type;
	//printf("%s\n", tvm->get_op_type_str(t).c_str());	
	switch (g_token_type) {
		case TOKEN_LDST: //ld st
			match(TOKEN_LDST);
			arg0.arg_type = ARG_REG;
			match(TOKEN_REG);
			arg0.arg_type = ARG_REG;
			arg0.val = get_reg_id(g_prev_token);
			match(TOKEN_COMMA);
			//TODO mem
			match(TOKEN_LEFT_BRACKT);
			match(TOKEN_REG);
			arg1.arg_type = ARG_REG;
			arg1.val = get_reg_id(g_prev_token);
			if (TOKEN_COMMA == g_token_type) {
				match(TOKEN_COMMA);
				match(TOKEN_REG);
				arg2.arg_type = ARG_REG;
				arg2.val = get_reg_id(g_prev_token);
			}
			match(TOKEN_RIGHT_BRACKT);
			break;
		case TOKEN_MOV:
			match(TOKEN_MOV);
			match(TOKEN_REG);
			arg0.arg_type = ARG_REG;
			arg0.val = get_reg_id(g_prev_token);
			match(TOKEN_COMMA);
			if (TOKEN_NUMBER == g_token_type) {
				match(TOKEN_NUMBER);
				arg1.arg_type = ARG_CONST;
				arg1.val = atoi(g_prev_token);
			}
			else {
				match(TOKEN_REG);
				arg1.arg_type = ARG_REG;
				arg1.val = get_reg_id(g_prev_token);
			}
			break;
		case TOKEN_STACK:
		case TOKEN_ONE:
		case TOKEN_IO:
			match(g_token_type);
			match(TOKEN_REG);
			arg0.arg_type = ARG_REG;
			arg0.val = get_reg_id(g_prev_token);
			break;
		case TOKEN_CMP:
			match(TOKEN_CMP);
			match(TOKEN_REG);
			arg0.arg_type = ARG_REG;
			arg0.val = get_reg_id(g_prev_token);
			match(TOKEN_COMMA);
			match(TOKEN_REG);
			arg1.arg_type = ARG_REG;
			arg1.val = get_reg_id(g_prev_token);
			break;
		case TOKEN_CALC:
			match(TOKEN_CALC);
			match(TOKEN_REG);
			arg0.arg_type = ARG_REG;
			arg0.val = get_reg_id(g_prev_token);
			match(TOKEN_COMMA);
			match(TOKEN_REG);
			arg1.arg_type = ARG_REG;
			arg1.val = get_reg_id(g_prev_token);
			match(TOKEN_COMMA);
			match(TOKEN_REG);
			arg2.arg_type = ARG_REG;
			arg2.val = get_reg_id(g_prev_token);
			break;
		case TOKEN_JMP:
			//t =
			match(TOKEN_JMP);
			match(TOKEN_LABEL);
			tvm->add_label(g_prev_token);	
			arg0.arg_type = ARG_JUMP_ADDR;
			arg0.val = tvm->get_label_id(g_prev_token);
			break;
		case TOKEN_HLT:
			match(TOKEN_HLT);
			break;
		default:
			syntax_unexpected(g_token);	
	}
	tvm->add_instruction(Instruction(t, arg0, arg1, arg2));
}

void parse(Tvm *tvm) {
	g_token_type = get_token();
	while (g_token_type) {
		expr(tvm);
	}
}

