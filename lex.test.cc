#include <stdio.h>

#include "lex.h"

int main() 
{
	while(get_token()) {
		printf("%s\n", g_token);	
	}
    return 0;
}

