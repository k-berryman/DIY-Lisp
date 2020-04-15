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
 
 // Create enumeration of possible errors
 enum { LERR_DIV_ZERO, LERR_BAD_OP, LER_BAD_NUM };
 
 // Create enumeration of possible lval types
 enum { LVAL_NUM, LVAL_ERR };
 
 // Declare lval struct
 typedef struct
 {
	 int type;
	 long num;
	 int err;
 } lval;
	 
 // input - long x
 // output - lval
 // number type lval
 lval lval_num(long x)
 {
	lval v;
	v.type = LVAL_NUM;
	v.num  = x;
	return v;
 }
	 
 // error type lval
 void lval_err(int x)
 {
	 lval v;
	 v.type = LVAL_ERR;
	 v.err = x;
	 return v;
 }
 
 // Pring lval
 void lval_print(lval v)
 {
	switch(v.type)
	{
		case LVAL_NUM:
			printf("%li", v.num);
			break;
		
		// Errors
		case LVAL_ERR:
			if(v.err == LERR_DIV_ZERO)
			{
				printf("ERROR. Don't divide by zero, silly!");
			}
			
			if(v.err == LERR_BAD_OP)
			{
				printf("ERROR. That's not an operator :(");
			}
			
			if(v.err == LERR_BAD_NUM)
			{
				printf("ERROR. Imagine if your user could enter a valid number, gosh");
			}
			
			break;
	}
 }
 
 // Print lval and a new line
 void lval_println(lval v)
 {
	lval_print(v);
	putchar('\n');
 }
	 
 lval eval_op(lval x, char * op, lval y)
 {
	// If either x or y is an error just return it
	if(x.type == LVAL_ERR)
	{
		return x;
	}
	
	if(y.type == LVAL_ERR)
	{
		return y;
	}
	
	// Addition
	if(strcmp(op, "+") == 0)
	{
		return lval_num(x.num + y.num);
	}
	
	// Subtraction
	if(strcmp(op, "-") == 0)
	{
		return lval_num(x.num - y.num);
	}
	
	// Multiplication
	if(strcmp(op, "*") == 0)
	{
		return lval_num(x.num * y.num);
	}
	
	// Division
	if(strcmp(op, "/") == 0)
	{
		return y.num == 0
			? lval_err(LERR_DIV_ZERO)
			: lval_num(x.num / y.num);
			
		// This is new .. what is it?
		// The ternary operator
		// <condition> ? <then> : <else>
		// if condition is true, do what follows ?
		// if condition is false, do what follows :
	}
	
	// Other operators give an error
	return lval_err(LERR_BAD_OP);
 }
	 
  
 lval eval(mpc_ast_t * t)
 {
	 if(strstr(t -> tag, "number"))
	 {
		 // Check for errors
		 errno = 0;
		 long x = strtol(t -> contents, NULL, 10);
		 return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
	 }
	 
	 char * op = t -> children[1] -> contents;
	 lval x = eval(t -> children[2]);
	 
	 int i = 3;
	 while(strstr(t -> children[i] -> tag, "expr"))
	 {
		 x = eva;_op(x, op, eval(t -> children[i]));
		 i++;
	 }
	 return x;
 }
	 
 int main(int arg, char ** argv)
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
	
	puts("Lispy Version 0.0.0.0.4");
	puts("Press Ctrl+c to Exit\n");
	
	while(1)
	{
		char * input = readline("lispy> ");
		add_history(input);
		
		// Parse user input
		mpc_result_t r;
		
		if(mpc_parse("<stdin>", input, Lispy, &r))
		{
			lval result = eval(r.output);
			lval_println(result);
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
	 