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
	lex(tokens, localVariableTest);
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
	//exit(1);
	//exit(1);
	//exit(1);
	Compiler* c = newCompiler(parser->ast);
	compile(c, parser->ast);
	freeParser(parser);
	printProgram(c->prog);
	VM* vm = initVM(c->prog);
	execute(vm);
	freeCompiler(c);
	printf("completed\n");
	
	return 0;
}

//change analyzer to only have errors at the top level, so move the errors up to the statement level
//if an expr is nested in a statement, catch it there
//need different kinds of errors to return, not just error type


//In rewriter and analyzer, don't pass ast node, pass the specific type
//Finish rewriter, add loop analysis, do the loop backend
//Add void and alpha
//Add assignment ops
//Add Big ints
//Add data structures
//Add If
//Add backend


//Might need to gradually move to a type structure similar to ast node to accomodate bigger types... probably sooner the better...