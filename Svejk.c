#include "Svejk.h"
#include "TypeMap.h"

char randomCharacter(char charArr[]){
	srand(time(NULL));
    int index = rand() % 62;
	return charArr[index];
}
/*
TypeMap* t = newTypeMap(1);
    char ascii_chars[] = {
        ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
        '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        '[', '\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        '{', '|', '}', '~', '\0' // Null-terminate the array
    };
	int stringCount = 100000;
	int stringLength = 10;
	char ** strings = (char**)malloc(sizeof(char*)*stringCount);
	for(int i = 0; i < stringCount; i++){
		strings[i] = (char*)malloc(sizeof(char)*stringLength);
		for(int j = 0; j < 10; j++){
			strings[i][j] = randomCharacter(ascii_chars);
		}
	}
	for(int i = 0; i < stringCount; i++){
		setTypeMap(t, strings[i], newTrivialType(I32_TYPE));
	}
	freeTypeMap(t);
	
	
	printf("completed");
	exit(1);
*/

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
	//freeAST(parser->ast);
	Compiler* c = newCompiler(rewrittenAST);
	compile(c, rewrittenAST);
	freeParser(parser);
	freeRewriter(r);
	printProgram(c->prog);

	VM* vm = initVM(c->prog);
	execute(vm);
	freeCompiler(c);
	printf("completed\n");
	exit(1);

	return 0;
}

//Current:
//parser, analyzer and rewriter have only one of: ref, assignment, dec
//analyzer keeps track of global count
//get rid of the type table
//figure out when I can free types
//make sure I free temp types, check all freeing and copying

/*
TODO:
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
		
	Cleaning and Behind the Scenes:
		Priorities:
			For should just take a statement
			Create new ASTObject functions
			Change Type to a struct
			Add heap
			Work on newlines and placement of: ;
			Add line counter to rewriter 
			Ensure all nodes can be analyzed
			Work and the Type System and Language as a whole
			Reduce responsibilities throughout parser, analyzer, compiler and rewriter
			In rewriter and analyzer, don't pass ast node, pass the specific type
		Add annotations and add them to ast objects
		Add optimizer for byte code 
		Add rewriting to print
		ASTPrint should only print offsets with a flag parameter set - not that important
		Optimize opcodes after compiling?, get rid of vestigal stuff with oparray, etc ...

*/