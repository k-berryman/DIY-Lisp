#include <stdio.h>

// global variables
static char input[2048];

int main(int argc, char** argv) 
{
	// Print version and Exit Info
	puts("Lispy Version 0.0.0.0.1");
	puts("Press Ctrl+c to Exit\n");
	
	while(1)
	{
		// Output prompt
		fputs("lispy> ", stdout);
		
		// Read a line max size 2048
		fgets(input, 2048, stdin);
		
		// Tell user their input
		printf("No you're a %s", input);
	}
	
	return 0;
}
