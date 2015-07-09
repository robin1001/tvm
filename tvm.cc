#include "tvm.h"

Tvm::Tvm():pc_(0), sp_(0), flag_(0), num_ins_(0), num_labels_(0),  
		   regs_(NUM_REGISTERS, 0), data_mem_(DATA_MEM_SIZE, 0), 
		   code_mem_(CODE_MEM_SIZE, Instruction()) {
	
}


bool Tvm::check_symbol_table() {
	std::unordered_map<std::string, int>::iterator it;
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
	return true;
}




