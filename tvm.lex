/* TVM(Tiny Virtual Machine)
 * Author: robin1001
 * Date: 2015-07-08
 */
%{
#include <stdio.h>
#include <string.h>

#include "lex.h"

char g_token[MAX_TOKEN];
TokenType g_token_type;
OpType g_op_type;

%}

%option noyywrap

digit [0-9]
number {digit}+
letter [a-zA-Z]
label {letter}({digit}|{letter})*

%%

[\t \n]+  /*ignore \t and ' ' */
"hlt"       {/*printf("%s\n", yytext);*/ g_op_type = OP_HLT;  return TOKEN_HLT;}
"mov"       {/*printf("%s\n", yytext);*/ g_op_type = OP_MOV;  return TOKEN_MOV;}
"ld"        {/*printf("%s\n", yytext);*/ g_op_type = OP_LD;   return TOKEN_LDST;}
"st"        {/*printf("%s\n", yytext);*/ g_op_type = OP_ST;   return TOKEN_LDST;}
"add"       {/*printf("%s\n", yytext);*/ g_op_type = OP_ADD;  return TOKEN_CALC;}
"sub"       {/*printf("%s\n", yytext);*/ g_op_type = OP_SUB;  return TOKEN_CALC;}
"mul"       {/*printf("%s\n", yytext);*/ g_op_type = OP_MUL;  return TOKEN_CALC;}
"dvi"       {/*printf("%s\n", yytext);*/ g_op_type = OP_DVI;  return TOKEN_CALC;}
"push"      {/*printf("%s\n", yytext);*/ g_op_type = OP_PUSH; return TOKEN_STACK;}
"pop"       {/*printf("%s\n", yytext);*/ g_op_type = OP_POP;  return TOKEN_STACK;}
"cmp"       {/*printf("%s\n", yytext);*/ g_op_type = OP_CMP;  return TOKEN_CMP;}

"jmp"       {/*printf("%s\n", yytext);*/ g_op_type = OP_JMP;  return TOKEN_JMP;}
"je"        {/*printf("%s\n", yytext);*/ g_op_type = OP_JE;   return TOKEN_JMP;}
"jne"       {/*printf("%s\n", yytext);*/ g_op_type = OP_JNE;  return TOKEN_JMP;}
"jg"        {/*printf("%s\n", yytext);*/ g_op_type = OP_JG;   return TOKEN_JMP;}
"jl"        {/*printf("%s\n", yytext);*/ g_op_type = OP_JL;   return TOKEN_JMP;}
"jge"       {/*printf("%s\n", yytext);*/ g_op_type = OP_JGE;  return TOKEN_JMP;}
"jle"       {/*printf("%s\n", yytext);*/ g_op_type = OP_JLE;  return TOKEN_JMP;}
"inc"       {/*printf("%s\n", yytext);*/ g_op_type = OP_INC;  return TOKEN_ONE;}
"dec"       {/*printf("%s\n", yytext);*/ g_op_type = OP_DEC;  return TOKEN_ONE;}
"in"        {/*printf("%s\n", yytext);*/ g_op_type = OP_IN;   return TOKEN_IO;}
"out"       {/*printf("%s\n", yytext);*/ g_op_type = OP_OUT;  return TOKEN_IO;}
"r0" | 
"r1" |
"r2" |
"r3" |
"r4" |
"r5" | 
"r6" |
"r7"        {/*printf("%s\n", yytext);*/ return TOKEN_REG;}
{label}     {/*printf("%s\n", yytext);*/ return TOKEN_LABEL;}
{number}    {/*printf("%s\n", yytext);*/ return TOKEN_NUMBER;}

","         {/*printf("%s\n", yytext);*/ return TOKEN_COMMA;}
":"         {/*printf("%s\n", yytext);*/ return TOKEN_COLON;}
"["         {/*printf("%s\n", yytext);*/ return TOKEN_LEFT_BRACKT;}
"]"         {/*printf("%s\n", yytext);*/ return TOKEN_RIGHT_BRACKT;}
"#"         { char ch;
              do {
                ch = input();
                if (ch == EOF) return TOKEN_EOF;
              } while ( ch != '\n'); 
            }
. {return TOKEN_EOF;}
 
%%

TokenType get_token() {
    TokenType t = (TokenType)yylex();
    strcpy(g_token, yytext);
    return t;
}


//int main() 
//{
//  //while(yylex()) {
//  //}
//  while(get_token()) {
//      printf("%s\n", g_token);    
//  }
//    return 0;
//}
