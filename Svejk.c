#include "Svejk.h"
#include "TypeMap.h"

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
	char* ifTest = "tests/If Test.txt";
	lex(tokens, ifTest);
	printTokens(tokens);
	Parser* parser = newParser();
	parse(parser, tokens);
	freeTokens(tokens);
	printAST(parser->ast);
	Analyzer* analyzer = newAnalyzer();
	analyze(analyzer, parser->ast);
	printErrors(analyzer->a->errors);
	if(analyzer->a->errors->errorCount > 0) exit(1);
	Rewriter* r = newRewriter(parser->ast);
	AST* rewrittenAST = rewrite(r, parser->ast);
	printAST(rewrittenAST);
	Compiler* c = newCompiler(rewrittenAST, analyzer->a);
	//split analysis and analyzer, free doesn't free analyzer
	compile(c, rewrittenAST);
	freeAnalyzer(analyzer);
	freeParser(parser);
	freeRewriter(r);
	printProgram(c->prog);
	VM* vm = initVM(c->prog);
	execute(vm);
	freeCompiler(c);
	printf("completed\n");
	return 0;
}

/*
TODO:
	Work on language and Type System
	Features:
		Meta Programming and Extensibility:
			Add Macros
			Add, work on this
			Add Operator definitions
			Rewrite rules
			Procedure definitions
			
		Procedures:	
			Add break, continue, etc.
			Add Big ints and arbitrary precision arithmetic
			Add data structures
			
		Misc:
			Add alpha type
			Add assignment ops and desugar them
			Add scripts
			Add equality
			Add more features to print

		Functions:
			Add functions
			Add pure functions
			Add multiple returns, named and default args, var args
			Match
			Add ADTs
			
		OOP:
			Classes
			pattern decorators
			
		Other:
			Figure out and Add: Concurrency, Parallelism...
			Parametric Polymorphism
			Regions and ownership!

	Other Changes and Additions mostly behind the scene stuff:
		Add optimizer: spits out optimized byte code
		Add solver: small step operationally solves a program
		Add deducer: deduces facts about programs, termination, etc
		Add reassembler: takes byte code and reassemblies it - needs to reassemble data too?
		Add transpiler: transpiles it to various languages		
		Add heap
		Add annotations and add them to ast objects

	Cleaning and Behind the Scenes:
		Make an Error ASTNode
		Clean up errors
		New ast object methods/initialization methods
		Clean up rewriter, parser, analyzer 
		Work on newlines and placement of: ;
		Add line counter to rewriter 
		Ensure all nodes can be analyzed
		Work on the Type System and Language as a whole
		In rewriter and analyzer, don't pass ast node, pass the specific type
		Clean up freeing
		For should just take a statement
		Add rewriting to print (and other operators)
*/