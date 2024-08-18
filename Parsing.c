#include "Parsing.h"

Token advance(TokenArray* tokens){
	Token token = *(tokens->tokens);
	tokens->tokens += 1;
	return token;
}

Token currentToken(TokenArray* tokens){
	return *tokens->tokens;
}

Token peek(TokenArray* tokens){
	Token token = *(tokens->tokens+1);
	return token;
}

void clearEnding(TokenArray* tokens){
	if(peek(tokens).type == SEMI_COLON_TOKEN) advance(tokens);
	if(peek(tokens).type == END_LINE_TOKEN) advance(tokens);
}

ASTNode* parseElse(Parser* p, TokenArray* tokens){
	advance(tokens);
	return newASTElse(split(p, tokens, 0), tokens->tokens->line, NULL);
}

ASTNode* parseIf(Parser* p, TokenArray* tokens){
	ASTNode* expr1 = split(p, tokens, 0);
	ASTNode* expr2 = split(p, tokens, 0);
	clearEnding(tokens);
	if(tokens->tokens->type == WORD_TOKEN && strcmp(tokens->tokens->value,"else")==0) return newASTIf(expr1, expr2, parseElse(p, tokens), tokens->tokens->line, NULL);
	else return newASTIf(expr1, expr2, NULL, tokens->tokens->line, NULL);
}

ASTNode* parseParens(Parser* parser, TokenArray* tokens){
	ASTNode* result = split(parser, tokens, 0);
	advance(tokens);
	return result;
}

ASTNode* parseCurlys(Parser* parser, TokenArray* tokens){
	ASTNode* b = newASTBlock(tokens->tokens->line, NULL);
	while(tokens->tokens->type != SYM_TOKEN || strcmp(tokens->tokens->value, "}")!=0) parseLocal(parser, b, tokens);
	advance(tokens);
	return b;
}

ASTNode* parseWhile(Parser* parser, TokenArray* tokens){
	ASTNode* expr1 = split(parser, tokens, 0);
	ASTNode* expr2 = split(parser, tokens, 0);
	return newASTWhile(expr1, expr2, tokens->tokens->line, NULL);
}

ASTNode* literal(Parser* parser, Token t){
	switch(t.type){
		case INT_TOKEN : return newASTValue(newI32(atoi(t.value)), t.line, NULL);
		case FLOAT_TOKEN: return newASTValue(newF32(atof(t.value)), t.line, NULL);
		case STR_A_TOKEN: return newASTString(t.value, t.line, NULL);
		case STR_B_TOKEN: return newASTValue(newBool(true), t.line, NULL);
		case WORD_TOKEN:{
			if(strcmp(t.value, "true") == 0) return newASTValue(newBool(true), t.line, NULL);
			else if(strcmp(t.value, "false") == 0) return newASTValue(newBool(false), t.line, NULL);
			else return newASTID(t.value, t.line, NULL);
		}
		default:{
			printf("Invalid Literal Start\n");
			exit(1);
		}
	}
}

ASTNode* prefix(Parser* parser, TokenArray* tokens){
	Token t = advance(tokens);
	if(t.type == SYM_TOKEN){
		if(strcmp(t.value, "(")==0)	return parseParens(parser, tokens);
		else if(strcmp(t.value, "{")==0) return parseCurlys(parser, tokens);
	}
	else if(t.type == WORD_TOKEN){
		if(strcmp(t.value, "while")==0) return parseWhile(parser, tokens);
		else if(strcmp(t.value, "if")==0) return parseIf(parser, tokens);
		else if (strcmp(t.value, "true")==0) return literal(parser, t);
		else if (strcmp(t.value, "false")==0) return literal(parser, t);
		Template* temp = searchTemplate(parser->t, t.value);
		if(temp != NULL && temp->type == PREFIX_OP) return newASTUnaryOP(split(parser, tokens, temp->op.prec+1), t.value, t.line, NULL);
	}
	return literal(parser, t);
}

ASTNode* binaryOP(Parser* parser, char* op, ASTNode* lhs, TokenArray* tokens){
	Template* temp = searchTemplate(parser->t, op);
	if(temp==NULL) printf("Invalid OP!\n"),exit(1);
	if(temp->op.ass) return newASTBinaryOP(lhs, op, split(parser, tokens, temp->op.prec), tokens->tokens->line, NULL);
	else return newASTBinaryOP(lhs, op, split(parser, tokens, temp->op.prec+1), tokens->tokens->line, NULL);
}

ASTNode* otherfix(Parser* parser, ASTNode* node, TokenArray* tokens){
	Token t = advance(tokens);
	Template* temp = searchTemplate(parser->t, t.value);
	if(temp != NULL && temp->op.type == POSTFIX_OP) return newASTUnaryOP(split(parser, tokens, temp->op.prec+1), t.value, t.line, NULL);
	return binaryOP(parser, t.value, node, tokens);
}

ASTNode* split(Parser* parser, TokenArray* tokens, int prec){
	ASTNode* lhs = prefix(parser, tokens);
	Template* t = NULL;
	if(tokens->tokens->value != NULL) t = searchTemplate(parser->t, tokens->tokens->value);
	int precedence = t == NULL ? -1 : t->op.prec;
	bool ass = t == NULL ? false : t->op.ass;
	while(prec <= precedence){
		lhs = otherfix(parser, lhs, tokens);
		t = tokens->tokens->value == NULL ? NULL : searchTemplate(parser->t, tokens->tokens->value);
		precedence = t == NULL ? -1 : t->op.prec;
		ass = t == NULL ? false : t->op.ass;
	}
	return lhs;
}

ASTNode* parseOnTemplate(){
	return NULL;
}

ASTNode* parseExpression(Parser* parser, TokenArray* tokens){
	return split(parser, tokens, 0);
}

ASTNode* parseStatement(Parser* parser, TokenArray* tokens){
	switch(tokens->tokens->type){
		case SYM_TOKEN:
		case WORD_TOKEN:
		case INT_TOKEN:
		case FLOAT_TOKEN:
		case STR_A_TOKEN: 
		case STR_B_TOKEN: {
			parseExpression(parser, tokens);
			exit(1);
			return NULL;
		}
		case END_LINE_TOKEN:
		case SEMI_COLON_TOKEN:{
			advance(tokens);
			return parseStatement(parser, tokens);
		}
		default:{
			printf("Unparsable token\n");
			printf("%d\n", tokens->tokens->type);
			exit(1);
		}
	}
	return NULL;
}

void parseLocal(Parser* parser, ASTNode* b, TokenArray* tokens){
	if(tokens->tokens->type == END_LINE_TOKEN || tokens->tokens->type == SEMI_COLON_TOKEN) {
		advance(tokens);
		return;
	}
	emitNodeToBlock(parseStatement(parser, tokens), b);
}

void parse(Parser* parser, TokenArray* tokens){
	Token* saveTokens = tokens->tokens;
	while(tokens->tokens->type != END_TOKEN){
		if(tokens->tokens->type == END_LINE_TOKEN || tokens->tokens->type == SEMI_COLON_TOKEN){
			advance(tokens); 
			continue;
		}
		if(tokens->tokens->type == END_TOKEN) return;
		emitNode(parseStatement(parser, tokens), parser->ast);
	}
	tokens->tokens = saveTokens;
}


/*
Initialization and Freeing
*/

void freeParser(Parser* parser){
	freeAST(parser->ast, false);
	freeTemplateTreeNoKeys(parser->t);
	free(parser);
}

AST* newAST(){
	AST* ast = (AST*)malloc(sizeof(AST));
	ast->nodes = (ASTNode*) malloc(sizeof(ASTNode));
	ast->cappacity = 1;
	ast->numberOfNodes = 0;
	return ast;
}

Parser* newParser(){
	Parser* parser = (Parser*) malloc(sizeof(Parser));
	parser->ast = newAST();	
	parser->t = newTemplateTree();
	return parser;
}
