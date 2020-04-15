 // An Abstract Syntax Tree represents the 
 // structure of a program based in user input
 // Tree leaves is the data needing processing (numbers, operators)
 // Tree branches are rules (info on how to tranverse and evaluate)
 
 // Take a look at mpc_ast_t in mpc.h
 // This is the data structure from the parse
 // It's a struct with five fields
 // Two of the fields are "children_num" and "children"
 // These will help traverse the tree
 
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
 }
 
 // Use strcmp to check which operator to use
 // Use strstr to check if a tag contains a substring
 
 long eval_op(long x, char * op, long y)
 {
	if(strccmp(op, "+") == 0)
	{
		return x + y;
	}
	
	if(strccmp(op, "-") == 0)
	{
		return x - y;
	}
	
	if(strccmp(op, "*") == 0)
	{
		return x * y;
	}
	
	if(strccmp(op, "/") == 0)
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
		i++
	}
	
	return x;
 }
	 