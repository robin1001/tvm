#include <sstream>

#include "tvm.h"


Tvm::Tvm():pc_(0), sp_(0), flag_(0), num_ins_(0), num_labels_(0),  
		   regs_(NUM_REGISTERS, 0), data_mem_(DATA_MEM_SIZE, 0), 
		   code_mem_(CODE_MEM_SIZE, Instruction()) {
	
}


bool Tvm::check_symbol_table() {
	std::unordered_map<std::string, int>::iterator it;
	printf("printing symbol table:\n");
	printf("label\tid\tpc\n");
	for (it = label_map_.begin(); it != label_map_.end(); it++) {
		if (label_addr_.find(it->second) != label_addr_.end()) {
			printf("%s\t%d\t%d\n", it->first.c_str(), it->second, label_addr_[it->second]);	
		}
		else {
			printf("%s\t%d\t?\n", it->first.c_str(), it->second);	
			printf("label %s are not defined, but referenced\n", it->first.c_str());	
			return false;
		}
	}
	printf("\n");
	return true;
}

void Tvm::run() {
	printf("start running tvm code...\n");
	//printf("num instructions %d\n", num_ins_);
	printf("pc\t\top_type\t\targ0\t\targ1\t\targ2\n");
	while (code_mem_[pc_].op_type != OP_HLT) {
		ArgType type0 = code_mem_[pc_].arg0.arg_type, 
			    type1 = code_mem_[pc_].arg1.arg_type, 
			    type2 = code_mem_[pc_].arg2.arg_type;
		int value0 = code_mem_[pc_].arg0.val, 
			value1 = code_mem_[pc_].arg1.val, 
			value2 = code_mem_[pc_].arg2.val;
		
		printf("%d\t\t%s", pc_, get_op_type_str(code_mem_[pc_].op_type).c_str());
		printf("\t\t%s", get_args_str(code_mem_[pc_].arg0).c_str());
		printf("\t\t%s", get_args_str(code_mem_[pc_].arg1).c_str());
		printf("\t\t%s\n", get_args_str(code_mem_[pc_].arg2).c_str());

		switch (code_mem_[pc_].op_type) {
			case OP_MOV:
				assert(ARG_REG == type0);
				if (ARG_REG == type1)
					regs_[value0] = regs_[value1];
				else if (ARG_CONST == type1) 
					regs_[value0] = value1;	
				break;
			case OP_ST:
				assert(ARG_REG == type0);
				assert(ARG_REG == type1);
				if (ARG_REG == type2) {
					data_mem_[regs_[value1] + regs_[value2]] = regs_[value0];
				} else {
					data_mem_[regs_[value1]] = regs_[value0];
				}
				break;
			case OP_LD:
				assert(ARG_REG == type0);
				assert(ARG_REG == type1);
				if (ARG_REG == type2) {
					regs_[value0] = data_mem_[regs_[value1] + regs_[value2]];
				} else {
					regs_[value0] = data_mem_[regs_[value1]];
				}
				break;
			case OP_ADD:
				regs_[value0] = regs_[value1] + regs_[value2];
				break;
			case OP_SUB:
				regs_[value0] = regs_[value1] - regs_[value2];
				break;
			case OP_MUL:
				regs_[value0] = regs_[value1] * regs_[value2];
				break;
			case OP_DVI:
				regs_[value0] = regs_[value1] / regs_[value2];
				break;
			case OP_INC:
				regs_[value0]++;
				break;
			case OP_DEC:
				regs_[value0]--;
				break;
			case OP_IN:
				printf("tvm input:");
				scanf("%d", &regs_[value0]);
				break;
			case OP_OUT:
				printf("tvm output:%d\n", regs_[value0]);
				break;
			case OP_POP:
				break;
			case OP_PUSH:
				break;
			case OP_CMP:
				assert(ARG_REG == type0);
				assert(ARG_REG == type1);
				flag_ = regs_[value0] - regs_[value1];
				break;
			case OP_JMP:
				assert(ARG_JUMP_ADDR == type0);
				pc_	= label_addr_[value0];		
				continue;
			case OP_JE:
				if (0 == flag_) {
					pc_	= label_addr_[value0];		
					continue;
				} else {
					break;
				}
			case OP_JNE:
				if (0 != flag_) {
					pc_	= label_addr_[value0];		
					continue;
				} else {
					break;
				}
			case OP_JG:
				if (flag_ > 0) {
					pc_	= label_addr_[value0];		
					continue;
				} else {
					break;
				}
			case OP_JL:
				if (flag_ < 0) {
					pc_	= label_addr_[value0];		
					continue;
				} else {
					break;
				}
			case OP_JGE:
				if (flag_ >= 0) {
					pc_	= label_addr_[value0];		
					continue;
				} else {
					break;
				}
			case OP_JLE:
				if (flag_ <= 0) {
					pc_	= label_addr_[value0];		
					continue;
				} else {
					break;
				}
			default:
				printf("runtime error: unknown operation type\n");
				exit(-1);
				//error
				break;
		}
		pc_++;
	}
	printf("tvm finished!!!\n");
}

void Tvm::rebuild_instructions() {
	printf("start rebuild instructions from parser result:\n");
	printf("i\t\top_type\t\targ0\t\targ1\t\targ2\n");
	for (int i = 0; i < num_ins_; i++) {
		printf("%d\t\t%s", i, get_op_type_str(code_mem_[i].op_type).c_str());
		printf("\t\t%s", get_args_str(code_mem_[i].arg0).c_str());
		printf("\t\t%s", get_args_str(code_mem_[i].arg1).c_str());
		printf("\t\t%s\n", get_args_str(code_mem_[i].arg2).c_str());
	}
	printf("\n");
}

std::string Tvm::get_op_type_str(OpType t) {
	switch (t) {
		case OP_HLT:  return "hlt";
		case OP_MOV:  return "mov";
		case OP_ST:   return "st";
		case OP_LD:   return "ld"; 
		case OP_ADD:  return "add";
		case OP_SUB:  return "sub";
		case OP_MUL:  return "mul";
		case OP_DVI:  return "dvi";
		case OP_INC:  return "inc"; 
		case OP_DEC:  return "dec";
		case OP_IN:   return "in";
		case OP_OUT:  return "out";
		case OP_POP:  return "pop"; 
		case OP_PUSH: return "push"; 
		case OP_CMP:  return "cmp";
		case OP_JMP:  return "jmp";
		case OP_JE:   return "je";
		case OP_JNE:  return "jne";
		case OP_JG:   return "jg";
		case OP_JL:   return "jl";
		case OP_JGE:  return "jge";
		case OP_JLE:  return "jle";
		default: return "unknown";
	}
}

std::string Tvm::get_args_str(const Args &arg) {
	std::stringstream ss;
	switch(arg.arg_type) {
		case ARG_NONE:
			ss << "none";
			break;
		case ARG_REG:
			ss << regs_[arg.val] << "(r"  << arg.val << ")"; 
			break;
		case ARG_CONST:
			ss << "#" << arg.val << "(c)";
			break;
		case ARG_JUMP_ADDR:
			ss << label_addr_[arg.val] << "(addr)";
			break;
		case ARG_MEM:
			ss << "(mem)";
			break;
	}
	return ss.str();
}
