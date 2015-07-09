#include <stdio.h>

#include "parse.h"
#include "tvm.h"


int main() {
	Tvm tvm;	
	parse(&tvm);
	if (!tvm.check_symbol_table()) {
		printf("semantic symbol error, aborting...\n");
	}
	tvm.rebuild_instructions();
	tvm.run();
}
