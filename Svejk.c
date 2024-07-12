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
	
	//exit(1);
	
	Analyzer* analyzer = newAnalyzer(parser->globalCount);
	analyze(analyzer, parser->ast);
	printErrors(analyzer->errors);
	if(analyzer->errors->errorCount > 0) exit(1);
	freeAnalyzer(analyzer);
		
	Rewriter* r = newRewriter(parser->ast, parser->globalCount);
	AST* rewrittenAST = rewrite(r, parser->ast);
	printAST(rewrittenAST);
	//printf("Completed\n");
	//exit(1);
	//exit(1);
	Compiler* c = newCompiler(rewrittenAST);
	compile(c, rewrittenAST);
	freeParser(parser);
	printProgram(c->prog);

	VM* vm = initVM(c->prog);
	execute(vm);
	freeCompiler(c);
	printf("completed\n");
	
	return 0;
}



/*
TODO:
	Features:
		Add this
		Add Macros
		Add break, continue, etc.
		Add alpha type
		Match, if-else, ...
		Add functions
		Add pure functions
		Add Big ints and arbitrary precision arithmetic
		Add data structures
		Add multiple returns, named and default args, var args
		Add assignment ops and desugar them
		Add ADTs and Classes
		Concurrency, Parallelism
		Parametric Polymorphism

	Cleaning and Behind the Scenes:
		Create new ASTObject functions
		Change Type to a struct
		Add annotations and add them to ast objects
		Add optimizer for byte code 
		Add rewriting to print
		Add heap
		In rewriter and analyzer, don't pass ast node, pass the specific type
		ASTPrint should only print offsets with a flag parameter set - not that important
		Optimize opcodes after compiling?, get rid of vestigal stuff with oparray, etc ...
		Work and the Type System and Language as a whole
*/