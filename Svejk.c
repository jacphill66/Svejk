#include "Svejk.h"

/*
Right now:
	1.) modify analyzer and rewriter to work with changes, before I premptivley add parser features
	2.) have more consistent newline checking
	2.) for and if-else should just take one statement each
	2.) add operators: <<, !, expr[i], 
	3.) add index assignment: expr[i] =
	4.) add type parsing method and add
	5.) add other types of tables
	5.) add assignment ops
	5.) add for in statement
	6.) fix analyzer to ensure the last statement of a block is a value 
*/


int main(){
	TokenArray* tokens = initTokenArray(tokens);
	
	//operators
	int numberOfTokens = 26;
	char** strings = (char**)malloc(sizeof(char*)*numberOfTokens);
	
	addToken(strings, 0, "<");
	addToken(strings, 1, ">");
	addToken(strings, 2, "<=");
	addToken(strings, 3, ">=");
	addToken(strings, 4, "+");
	addToken(strings, 5, "-");
	addToken(strings, 6, "*");
	addToken(strings, 7, "/");
	addToken(strings, 8, "%");
	addToken(strings, 9, "==");
	addToken(strings, 10, "!=");
	addToken(strings, 11, "!");
	addToken(strings, 12, "not");
	addToken(strings, 13, "and");
	addToken(strings, 14, "or");
	addToken(strings, 15, "^");

	int opCount = 16;
	for(int i = 0; i < 16; i++) addString(tokens->trie, strings[i]);
	Template** opTemplates = (Template**)malloc(sizeof(Template*)*opCount);
	for(int i = 0; i < opCount; i++) opTemplates[i] = (Template*)malloc(sizeof(Template));

	opTemplates[0] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[0], newTrivialType(BOOL_TYPE), 3, false, INFIX_OP);
	opTemplates[1] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[1], newTrivialType(BOOL_TYPE), 3, false, INFIX_OP);
	opTemplates[2] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[2], newTrivialType(BOOL_TYPE), 3, false, INFIX_OP);
	opTemplates[3] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[3], NULL, 3, false, INFIX_OP);	
	opTemplates[4] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[4], NULL, 4, false, INFIX_OP);
	opTemplates[5] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[5], NULL, 4, false, INFIX_OP);
	opTemplates[6] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[6], NULL, 5, false, INFIX_OP);
	opTemplates[7] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[7], NULL, 5, false, INFIX_OP);
	opTemplates[8] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[8], NULL, 5, false, INFIX_OP);
	opTemplates[9] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[9], newTrivialType(BOOL_TYPE), 2, false, INFIX_OP);
	opTemplates[10] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[10], newTrivialType(BOOL_TYPE), 2, false, INFIX_OP);
	opTemplates[11] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[3], NULL, 3, false, POSTFIX_OP);
	opTemplates[12] = newOpTemplate(newExpressionPart(NULL, newTrivialType(BOOL_TYPE)), newExpressionPart(NULL, newTrivialType(BOOL_TYPE)), strings[3], newTrivialType(BOOL_TYPE), 3, false, PREFIX_OP);
	opTemplates[13] = newOpTemplate(newExpressionPart(NULL, newTrivialType(BOOL_TYPE)), newExpressionPart(NULL, newTrivialType(BOOL_TYPE)), strings[13], newTrivialType(BOOL_TYPE), 6, false, INFIX_OP);
	opTemplates[14] = newOpTemplate(newExpressionPart(NULL, newTrivialType(BOOL_TYPE)), newExpressionPart(NULL, newTrivialType(BOOL_TYPE)), strings[14], newTrivialType(BOOL_TYPE), 6, false, INFIX_OP);
	opTemplates[15] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[15], NULL, 6, true, INFIX_OP);

	char* arithmeticTest = "tests/Arithmetic Test.txt";
	char* stringTest = "tests/string Test.txt";
	char* globalVariableTest = "tests/Global Variables Test.txt";
	char* localVariableTest = "tests/Local Variables and Blocks test.txt";
	char* variablesTest = "tests/Variables Test.txt";
	char* errorTest = "tests/Errors Test.txt";
	char* inferenceTest = "tests/Inference Test.txt";
	char* forTest = "tests/for Test.txt";
	char* ifTest = "tests/If Test.txt";
	char* tableTest = "tests/Just Parsing/Table Test.txt";
	char* lexerTest = "tests/apples.txt";
	char* simpleTest = "tests/simple.txt";
	lex(tokens, simpleTest);
	printTokens(tokens);
	Parser* parser = newParser();
	for(int i = 0; i < opCount; i++) insertTemplate(parser->t, strings[i], -1, opTemplates[i]);
	
	//different prefix, postfix, infix trees?
	//union types
	//special types like number
	//something else?
	
	/*printTemplate(searchTemplate(parser->t, strings[0]));
	printTemplate(searchTemplate(parser->t, strings[1]));
	printTemplate(searchTemplate(parser->t, strings[2]));
	printTemplate(searchTemplate(parser->t, strings[3]));
	printTemplate(searchTemplate(parser->t, strings[4]));*/

	parse(parser, tokens);
	freeTokens(tokens);
	printAST(parser->ast);
	exit(1);
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
//maybe sometime be able to do parens through primitive decorators


/*
1.) extract type from type map into its own files
2.) be able to parse infix, prefix and postfix expressions by tonight
3.) extract template from tree

expr
print
let
reference
assignment
if-else
for
*/

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
		Add var

	Cleaning and Behind the Scenes:
		If statements should be in a local scopes, as should loops
		Make an Error ASTNode
		Clean up errors

		Ensure all nodes can be analyzed
		Work on the Type System and Language as a whole
		In rewriter and analyzer, don't pass ast node, pass the specific type
		Clean up freeing
		For should just take a statement
		Add rewriting to print (and other operators)
*/

/*
	should blocks be expressions? should procedures
	Modify emit node to block to count variables
	Finish parsing imperative tables
	Add parsing error to parser
	for loop should take one statement
	if statement and loop should create a new scope
	Clean up rewriter, analyzer 
	Work on newlines and placement of: ;
	Add line counter to rewriter 
*/

/*
same op different type
easy to parse, hard to analyze
*/
//+i32i32
//template to id function?
//tomorrow: finish parser, be able to parse everything most of the tests should pass
//templates for procedures, blocks...
//maybe keep everything as is, just add syntax extension? idk


