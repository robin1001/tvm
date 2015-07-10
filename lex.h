/* TVM(Tiny Virtual Machine)
 * Author: robin1001
 * Date: 2015-07-08
 */
#ifndef _LEX_H_
#define _LEX_H_

#define MAX_TOKEN 128

typedef enum {
	TOKEN_EOF = 0, //lex return 0 if EOF
	TOKEN_HLT,//hlt
	TOKEN_LDST, //LD, ST		
	TOKEN_MOV, //mov 
	TOKEN_REG, //reg 		
	TOKEN_ONE, // inc dec		
	TOKEN_IO, // in out		
	TOKEN_STACK, //POP PUSH
	TOKEN_CALC, //ADD SUB DVI MUL
	TOKEN_CMP, //CMP
	TOKEN_JMP, //JMP, JE,	JNE ...
	TOKEN_NUMBER, //123
	TOKEN_LABEL, //l1
	TOKEN_COMMA, //,
	TOKEN_COLON, //:
	TOKEN_LEFT_BRACKT,
	TOKEN_RIGHT_BRACKT,
}TokenType;

typedef enum { //tvm operator
	OP_HLT = 0,// hlt
	OP_MOV,//mov 
	OP_ST, OP_LD,//load store op
	OP_ADD, OP_SUB, OP_MUL, OP_DVI, //arithmetic op
	OP_INC, OP_DEC,//inc dec
	OP_IN, OP_OUT, //in out
	OP_POP,OP_PUSH, //stack op
	OP_CMP, //compare op
	OP_JMP, OP_JE, OP_JNE, OP_JG, OP_JL, OP_JGE, OP_JLE //jump op
} OpType;


#ifdef __cplusplus
extern "C" {
#endif

TokenType get_token(); 
extern char g_token[MAX_TOKEN];
extern TokenType g_token_type;
extern OpType g_op_type;

#ifdef __cplusplus
}
#endif

#endif
