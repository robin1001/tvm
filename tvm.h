#ifndef _TVM_H_
#define _TVM_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <vector>
#include <unordered_map>

#include "lex.h"

#define DATA_MEM_SIZE 1024
#define CODE_MEM_SIZE 1024
#define NUM_REGISTERS 8


typedef enum {
	ARG_NONE = 0,
	ARG_REG, //add r1, r2, r3
	ARG_CONST, //LDC r1, 300
	ARG_JUMP_ADDR, //jmp l1
	ARG_MEM //LD r1, [r2]; LD r1, [r2, #4]; LD r1, [r2, r3]
}ArgType;


// 
struct Args {
	Args(): arg_type(ARG_NONE), val(0) {};
	Args(ArgType t, int v): arg_type(t), val(v) {}
	ArgType arg_type;
	int val;
};

//instruction eg, add r0, r1, r2; etc
struct Instruction {
	Instruction() {};
	Instruction(OpType t, const Args &a0, const Args &a1, const Args &a2): 
		op_type(t), arg0(a0), arg1(a1), arg2(a2) {}
	OpType op_type;
	Args arg0, arg1, arg2;
};


//tiny virtual machine
class Tvm{ 
public:
	Tvm(); 
	void add_instruction(const Instruction &ins) {
		code_mem_[num_ins_++] = ins;	
		assert(num_ins_ < CODE_MEM_SIZE);
	}

	void add_label(std::string label) {
		if (label_map_.find(label) == label_map_.end()) {
			label_map_.insert(std::make_pair(label, num_labels_));	
			num_labels_++;
		}
	}
	
	void add_label_and_set_addr(std::string label) {
		add_label(label);	
		assert(label_map_.find(label) != label_map_.end());
		label_addr_.insert(std::make_pair(label_map_[label], num_ins_));
	}

	int get_label_id(std::string label) {
		assert(label_map_.find(label) != label_map_.end());
		return label_map_[label];
	}

	void run();
	bool check_symbol_table();
	void rebuild_instructions(); 

	std::string get_op_type_str(OpType t); 
	std::string get_args_str(const Args &arg); 
protected:
	int pc_; //program counter
	int sp_; //stack pointer
	int flag_; //compare flags
	int num_ins_; 
	int num_labels_;
	std::vector<int> regs_;
	std::vector<int> data_mem_;
	std::vector<Instruction> code_mem_;
	//TODO symbol table
	std::unordered_map<std::string, int> label_map_; //map label to int id 
	std::unordered_map<int, int> label_addr_; //map label id to label pc addr
};

#endif
