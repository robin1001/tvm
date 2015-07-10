/* TVM(Tiny Virtual Machine)
 * Author: robin1001
 * Date: 2015-07-09
 */
#include <stdio.h>

#include "parse.h"
#include "tvm.h"

int main() {
	Tvm tvm(true);	
	parse(&tvm);
	tvm.rebuild_instructions();
}
