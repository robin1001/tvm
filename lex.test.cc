/* TVM(Tiny Virtual Machine)
 * Author: robin1001
 * Date: 2015-07-08
 */

#include <stdio.h>

#include "lex.h"

int main() 
{
	while(get_token()) {
		printf("%s\n", g_token);	
	}
    return 0;
}

