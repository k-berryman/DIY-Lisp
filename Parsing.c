#include <stdio.h>
#include <stdlib.h>
#include "mpc.h"

// To link to mpc, put mpc.c into the compile command
// Windows      --  gcc -std=c99 -Wall Parsing.c mpc.c -o Parsing   then  Parsing.exe
// Linux or Max --  cc -std=c99 -Wall Parsing.c mpc.c -ledit -lm -o Parsing

// If this is compiled on Windows
#ifdef _WIN32
#include <string.h>

static char buffer[2048];

// Define behavior for Windows users
char * readline(char * prompt)
{
	fputs(prompt, stdout);
	fgets(buffer, 2048, stdin);
	char * cpy = malloc(strlen(buffer)+1);
	strcpy(cpy, buffer);
	cpy[strlen(cpy)-1] = '\0';
	return cpy;
}

void add_history(char * unused){}

// Define behavior for Mac and Linux users
#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

int main(int argc, char ** argv) 
{
	// Polish notation grammar
	// Create parsers
	mpc_parser_t * Number   = mpc_new("number");
	mpc_parser_t * Operator = mpc_new("operator");
	mpc_parser_t * Expr     = mpc_new("expr");
	mpc_parser_t * Lispy    = mpc_new("lispy");

	// Define parsers with language
	mpca_lang(MPCA_LANG_DEFAULT,
		"                                                     \
			number   : /-?[0-9]+/ ;                             \
			operator : '+' | '-' | '*' | '/' ;                  \
			expr     : <number> | '(' <operator> <expr>+ ')' ;  \
			lispy    : /^/ <operator> <expr>+ /$/ ;             \
		",
		Number, Operator, Expr, Lispy);
	
	puts("Lispy Version 0.0.0.0.2");
	puts("Press Ctrl+c to Exit\n");
	
	while(1)
	{
		//fputs("lispy> ", stdout);
		char * input = readline("lispy> ");
		add_history(input);
		
		// Parse user input
		mpc_result_t r;
		
		if(mpc_parse("<stdin>", input, Lispy, &r))
		{
			// Print AST
			mpc_ast_print(r.output);
			mpc_ast_delete(r.output);
		}
		
		else
		{
			// Print error
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}
		
		free(input);
	}
	
	// Undefine and delete parsers
	mpc_cleanup(4, Number, Operator, Expr, Lispy);
	return 0;
}
