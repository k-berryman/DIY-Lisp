 // An Abstract Syntax Tree represents the 
 // structure of a program based in user input
 // Tree leaves is the data needing processing (numbers, operators)
 // Tree branches are rules (info on how to tranverse and evaluate)
 
 // Take a look at mpc_ast_t in mpc.h
 // This is the data structure from the parse
 // It's a struct with five fields
 // Two of the fields are "children_num" and "children"
 // These will help traverse the tree
 
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
 
 // Use strcmp to check which operator to use
 // Use strstr to check if a tag contains a substring
 
 long eval_op(long x, char * op, long y)
 {
	if(strcmp(op, "+") == 0)
	{
		return x + y;
	}
	
	if(strcmp(op, "-") == 0)
	{
		return x - y;
	}
	
	if(strcmp(op, "*") == 0)
	{
		return x * y;
	}
	
	if(strcmp(op, "/") == 0)
	{
		return x / y;
	}
	
	return 0;
 }
 
 // Recursive evaluation function
 long eval(mpc_ast_t * t)
 {
	// If it's tagged as a number, return it
	if(strstr(t -> tag, "number"))
	{
		return atoi(t -> contents);
	}
	 
	// Operator is always the second child
	char * op = t -> children[1] -> contents;
	 
	// Store the third child in 'x'
	long x = eval(t -> children[2]);
	
	// Iterate remainging children and combining
	int i = 3;
	while(strstr(t -> children[i] -> tag, "expr"))
	{
		x = eval_op(x, op, eval(t -> children[i]));
		i++;
	}
	
	return x;
 }
 
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
		"                                                       \
			number   : /-?[0-9]+/ ;                             \
			operator : '+' | '-' | '*' | '/' ;                  \
			expr     : <number> | '(' <operator> <expr>+ ')' ;  \
			lispy    : /^/ <operator> <expr>+ /$/ ;             \
		",
		Number, Operator, Expr, Lispy);
	
	puts("Lispy Version 0.0.0.0.3");
	puts("Press Ctrl+c to Exit\n");
	
	while(1)
	{
		char * input = readline("lispy> ");
		add_history(input);
		
		// Parse user input
		mpc_result_t r;
		
		if(mpc_parse("<stdin>", input, Lispy, &r))
		{
			// Calling eval function
			long result = eval(r.output);
			printf("%li\n", result);
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
 
 
 
 /* The below code is for notes and record keeping, please ignore
 // Load AST from output
 // Field access of a pointer uses an arrow -> not a dot .
 mpc_ast_t * a = r.output;
 printf("Tag: %s\n",     		    a -> tag);
 printf("Contents: %s\n", 			a -> contents);
 printf("Number of Children: %i\n", a -> children_num);
 
 // Get first child
 mpc_ast_t * c0 = a -> children[0];
 printf("First Child Tag: %s\n",			    c0 -> tag);
 printf("First Child Contents: %s\n",		    c0 -> contents);
 printf("First Child Number of Children: %i\n", c0 -> children_num);
 
 // Note -- every child is a tree itself
 // Recursion -- a function that calls itself
 // Want to find the number of nodes in all children? Use recursion!
 
 int number_of_nodes(mpc_ast_t * t)
 {
	// Base case
	if(t -> children_num == 0)
	{
		return 1;
	}
	
	// Recursive case
	if(t -> children_num >= 1)
	{
		int total = 1;
		for(int i=0; i < t -> children_num; i++)
		{
			total += number_of_nodes(t -> children[i]);
		}
		return total;
	}
	
	return 0;
 } */
	 