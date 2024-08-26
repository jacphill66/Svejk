#include "Engine.h"

Engine* newEngine(){
	Engine* e = (Engine*)malloc(sizeof(Engine));
	e->lexer = newLexer();
	e->parser = newParser(2);
	return e;
}

void run(Engine* e, char* path, bool debug){
	//Lexing
	char* lex_res = lex(e->lexer, path);
	if(lex_res != NULL){
		printf("Lexing Error: %s\n", lex_res);
		freeLexer(e->lexer);
		return;
	}
	if(debug) printTokens(e->lexer->tokens);
	
	//Parsing
	ASTNode* parse_res = parse(e->parser, e->lexer->tokens);
	if(parse_res != NULL){
		printf("Parsing Error: %s\n", parse_res->error.msg);
		freeParser(e->parser);
		freeLexer(e->lexer);
		free(parse_res);
		return;
	}
	if(debug) printAST(e->parser->ast);
	freeLexer(e->lexer);
	freeParser(e->parser);
	if(debug) printf("Completed!\n");
}

void freeEngine(Engine* e){
	freeLexer(e->lexer);
}
