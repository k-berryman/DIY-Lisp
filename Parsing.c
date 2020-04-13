int dir #include <stdio.h>
#include <stdlib.h>

// If this is compiled on Windows
#ifdef _WIN32
#include <string.h>

static char buffer[2048];

// Define behavior for Windows users
char* readline(char* prompt)
{
	fputs(prompt, stdout);
	fgets(buffer, 2048, stdin);
	char* cpy = malloc(strlen(buffer)+1);
	strcpy(cpy, buffer);
	cpy[strlen(cpy)-1] = '\0';
	return cpy;
}

void add_history(char* unused){}

// Define behavior for Mac and Linux users
#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

int main(int argc, char** argv) 
{
	puts("Lispy Version 0.0.0.0.1");
	puts("Press Ctrl+c to Exit\n");
	
	while(1)
	{
		//fputs("lispy> ", stdout);
		char* input = readline("lispy> ");
		add_history(input);
		
		printf("No you're a %s\n", input);
		free(input);
	}
	
	return 0;
}
