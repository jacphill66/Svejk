#ifndef ENGINE_H_ 
#define ENGINE_H_
#include "Lexing.h"
#include "Parsing.h"

typedef struct{
	Lexer* lexer;
	Parser* parser;
}Engine;

Engine* newEngine();
void run(Engine* e, char* path, bool debug);
void freeEngine(Engine* engine);

#endif
