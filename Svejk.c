#include "Svejk.h"


char randomCharacter(char charArr[]){
	srand(time(NULL));
    int index = rand() % 62;
	return charArr[index];
}


int main(){
	TokenArray* tokens = initTokenArray(tokens);
	char* arithmeticTest = "tests/Arithmetic Test.txt";
	char* stringTest = "tests/string Test.txt";
	char* globalVariableTest = "tests/Global Variables Test.txt";
	char* localVariableTest = "tests/Local Variables Test.txt";
	char* variablesTest = "tests/Variables Test.txt";
	char* errorTest = "tests/Errors Test.txt";
	char* inferenceTest = "tests/Inference Test.txt";
	char* forTest = "tests/for Test.txt";
	lex(tokens, forTest);
	printTokens(tokens);		
	Parser* parser = newParser();
	parse(parser, tokens);
	freeTokens(tokens);
	printAST(parser->ast);
	exit(1);
	Analyzer* analyzer = newAnalyzer(parser->globalCount);
	analyze(analyzer, parser->ast);
	printErrors(analyzer->errors);
	freeAnalyzer(analyzer);
	exit(1);
	Program* p = newProgram(parser->stringCount);
	compile(p, parser->ast);
	freeParser(parser);
	printProgram(p);

	//freeProgram(p);
	//exit(1);
	VM* vm = initVM(p);
	execute(vm);

	//free(&p->ops);
	//free(&p->values);
	freeProgram(p);
	printf("completed\n");
	exit(1);
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
//Free Identifiers?
//check and advance in parser