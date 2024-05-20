#include "Svejk.h"


int main(){
	//printf("hello world\n");
	TokenArray tokens = lex("arithmeticTest.txt");	
	printTokens(&tokens);	
	AST ast = parse(&tokens);
	freeTokens(&tokens);
	printAST(&ast);
	Program p = compile(&ast);
	freeAST(&ast);
	printProgram(&p);
	VM vm;
	
	vm.p = p;
	initVM(&vm);
	execute(&vm);
	//freeProgram(&p);
	
	/*
	typedef enum {
	EQUAL_PREC,
	COMP_PREC,
	BOOL_PREC,
	SUM_PREC,
	FACTOR_PREC,
	UNARY_PREC,
	PRIMARY_PREC,
} Precedence;
	
	
		int precedence[] = { 
			[PLUS_OP_TOKEN] = SUM_PREC,
			[SUB_OP_TOKEN] = SUM_PREC,
			[MULT_OP_TOKEN] = FACTOR_PREC,
			[DIV_OP_TOKEN] = FACTOR_PREC,
			[MOD_OP_TOKEN] = FACTOR_PREC,
			[EQUAL_OP_TOKEN] = EQUAL_PREC,
			[LESS_OP_TOKEN] = COMP_PREC,
			[GREATER_OP_TOKEN] = COMP_PREC,
			[LOE_OP_TOKEN] = COMP_PREC,
			[GOE_OP_TOKEN] = COMP_PREC,
			[AND_OP_TOKEN] = BOOL_PREC,
			[OR_OP_TOKEN] = BOOL_PREC,
			[NOT_OP_TOKEN] = BOOL_PREC,	
			[FACT_OP_TOKEN] = UNARY_PREC,
			[I32_VAL_TOKEN] = PRIMARY_PREC,
			[F32_VAL_TOKEN] = PRIMARY_PREC,
			[STR_VAL_TOKEN] = PRIMARY_PREC,
			[TRUE_VAL_TOKEN] = PRIMARY_PREC,
			[FALSE_VAL_TOKEN] = PRIMARY_PREC,
			[ID_TOKEN] = PRIMARY_PREC,		
			[LPAREN_OP_TOKEN] = PRIMARY_PREC,
	};
	
	
	for(int i = 0; i < 21; i++){
		printf("%d\n", precedence[i]);
	}
	*/
	return 0;
}