#include <stdio.h>

#include "parse.h"
#include "tvm.h"


int main() {
	bool trace = false;
	Tvm tvm(trace);	
	parse(&tvm);
	if (!tvm.check_symbol_table()) {
		printf("semantic symbol error, aborting...\n");
	}
	if (trace) tvm.rebuild_instructions();
	tvm.run();
}
