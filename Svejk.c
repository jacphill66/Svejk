#include "Svejk.h"

/*
Refactor:
	TemplateMap
	TemplateTable
	Lexer
	Trie
	Common
*/

/*
Finish:
	ASTObject
	Parser
*/


void addToken(char** tokens, int tokenCount, const char* token){
	tokens[tokenCount] = (char*)malloc(sizeof(char)*strlen(token));
	for(int i = 0; i < strlen(token); i++) tokens[tokenCount][i] = token[i];
}

char** initStrings(Engine* e){
	int numberOfTokens = 17;
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
	addToken(strings, 12, "^");
	addToken(strings, 13, "{");
	addToken(strings, 14, "}");
	addToken(strings, 15, "(");
	addToken(strings, 16, ")");
	for(int i = 0; i < numberOfTokens; i++) addString(e->lexer->trie, strings[i]);
	return strings;
}

void initOpTemplates(Engine* e, char** strings){
	int opCount = 13;
	Template** opTemplates = (Template**)malloc(sizeof(Template*)*opCount);
	for(int i = 0; i < opCount; i++) opTemplates[i] = (Template*)malloc(sizeof(Template));
	opTemplates[0] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[0], newTrivialType(BOOL_TYPE), 3, false, INFIX_OP);
	opTemplates[1] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[1], newTrivialType(BOOL_TYPE), 3, false, INFIX_OP);
	opTemplates[2] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[2], newTrivialType(BOOL_TYPE), 3, false, INFIX_OP);
	opTemplates[3] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[3], newTrivialType(BOOL_TYPE), 3, false, INFIX_OP);	
	opTemplates[4] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[4], NULL, 4, false, INFIX_OP);
	opTemplates[5] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[5], NULL, 4, false, INFIX_OP);
	opTemplates[6] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[6], NULL, 5, false, INFIX_OP);
	opTemplates[7] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[7], NULL, 5, false, INFIX_OP);
	opTemplates[8] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[8], NULL, 5, false, INFIX_OP);	
	opTemplates[9] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[9], newTrivialType(BOOL_TYPE), 2, false, INFIX_OP);
	opTemplates[10] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[10], newTrivialType(BOOL_TYPE), 2, false, INFIX_OP);
	opTemplates[11] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[11], NULL, 3, false, POSTFIX_OP);
	opTemplates[12] = newOpTemplate(newExpressionPart(NULL, NULL), newExpressionPart(NULL, NULL), strings[12], NULL, 3, true, PREFIX_OP);
	for(int i = 0; i < opCount; i++) insertTemplate(e->parser->t, strings[i], -1, opTemplates[i]);
}

void initFormTemplate(Engine* e){
	int formCount = 2;
	Template** formTemplates = (Template**)malloc(sizeof(Template*)*formCount);
	//for(int i = 0; i < formCount; i++) formTemplates[i] = (Template*)malloc(sizeof(Template));
	
	int parts1Size = 3;
	TemplatePart** parts1 = newFormArr(parts1Size);
	parts1[0] = newWordPart(newString("for"));
	parts1[1] = newExpressionPart(newString("e1"), newTrivialType(BOOL_TYPE));
	parts1[2] = newBlockPart(newString("b"), NULL);
	formTemplates[0] = newFormTemplate(parts1, parts1Size, NULL);

	int parts2Size = 3;
	TemplatePart** parts2 = newFormArr(parts2Size);
	parts2[0] = newWordPart(newString("for"));
	parts2[1] = newExpressionPart(newString("e1"), newTrivialType(I32_TYPE));
	parts2[2] = newBlockPart(newString("b"), NULL);
	formTemplates[1] = newFormTemplate(parts2, parts2Size, NULL);

	for(int i = 0; i < formCount; i++) 	printTemplate(formTemplates[i]);
	
	exit(1);
	TemplateTable* forms = newTemplateTable(formCount);
	for(int i = 0; i < formCount; i++) setTemplateTable(forms, formTemplates[i]->form.parts[0]->id, formTemplates[i]);
	
	for(int i = 0; i < formCount; i++) {
		TMNode* n = getTemplateTable(forms, formTemplates[i]->form.parts[0]->id);
		printTemplate(n->t);
	}
}
//Fix template tree and template table



int main(){	
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
	
	Engine* e = newEngine();
	char** strings = initStrings(e);
	initOpTemplates(e, strings);
	initFormTemplate(e);
	printf("Completed\n");
	//free strings?
	//run(e, simpleTest, true);
	return 0;
}

/*
Tonight Microsoft app, finish parsing up to variables
Maybe loops and templates

Make Type its own file?
add parsing for partial application/first class operators
pass templates
fix freeing in front end, if there is a crash, everything should be freed
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
