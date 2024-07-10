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
	printAST(parser->ast);
	freeParser(parser);
	printProgram(c->prog);
	exit(1);

	VM* vm = initVM(c->prog);
	execute(vm);
	freeCompiler(c);
	printf("completed\n");
	
	return 0;
}



/*
TODO:
Create new ASTObject functions
Change Type to a struct
Add Loop backend
Add annotations and add them to ast objects
Add optimizer for byte code 
Add assignment ops and desugar them
Add alpha type
Develop and Add universal conditional statement
Add break, continue, etc.
Add this
Add rewriting to print
Add heap
Add data structures
Add functions
Add multiple returns, named and default args, var args
Add pure functions
Add Big ints and arbitrary precision arithmetic
change analyzer to only have errors at the top level, so move the errors up to the statement level
if an expr is nested in a statement, catch it there
need different kinds of errors to return, not just error type
In rewriter and analyzer, don't pass ast node, pass the specific type
Work and the Type System and Language as a whole
ASTPrint should only print offsets with a flag parameter set - not that important
*/