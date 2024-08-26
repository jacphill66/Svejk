#include "Parsing.h"

int line(TokenArray* tokens){
	return tokens->tokens->line;
}

bool checkError(Token t){
	return t.type == ERROR_TOKEN;
}

Token advance(Parser* p, TokenArray* tokens){
	if(p->tokenIndex < tokens->tokenCount){
		Token token = *(tokens->tokens);
		tokens->tokens += 1;
		p->tokenIndex += 1;
		return token;
	}
	else return newToken(ERROR_TOKEN, NULL, -1, line(tokens));
}

bool checkToken(Token t){
	return t.type == ERROR_TOKEN;
}

bool checkASTNode(ASTNode* n){
	return n->type == ASTError_NODE_TYPE;
}

TokenType currentType(TokenArray* tokens){
	return tokens->tokens->type;
}

char* currentValue(TokenArray* tokens){
	return tokens->tokens->value;
}

bool matchWord(TokenArray* tokens, const char* word){
	return currentType(tokens) == WORD_TOKEN && (strcmp(currentValue(tokens), word)==0);
}

bool matchSymbol(TokenArray* tokens, const char* word){
	return currentType(tokens) == SYM_TOKEN && (strcmp(currentValue(tokens), word)==0);
}

void clearNewLines(Parser* p, TokenArray* tokens){
	//check this advance
	while(currentType(tokens) == END_LINE_TOKEN) advance(p, tokens);
}

void clearEnding(Parser* p, TokenArray* tokens){
	//check this advance
	if(currentType(tokens) == SEMI_COLON_TOKEN) advance(p, tokens);
	clearNewLines(p, tokens);
}

ASTNode* parseElse(Parser* p, TokenArray* tokens){
	if(checkToken(advance(p, tokens))) return newASTError("Ran Out of Tokens; expected an else", line(tokens));
	clearNewLines(p, tokens);
	return newASTElse(split(p, tokens, 0), line(tokens), NULL);
}

ASTNode* parseIf(Parser* p, TokenArray* tokens){
	clearNewLines(p, tokens);
	ASTNode* expr1 = split(p, tokens, 0);
	if(checkASTNode(expr1)) return expr1;
	clearNewLines(p, tokens);
	ASTNode* expr2 = split(p, tokens, 0);
	if(checkASTNode(expr2)) return expr2;
	if(matchWord(tokens, "else")) return newASTIf(expr1, expr2, parseElse(p, tokens), line(tokens), NULL);
	else return newASTIf(expr1, expr2, NULL, tokens->tokens->line, NULL);
}

ASTNode* parseParens(Parser* parser, TokenArray* tokens){
	clearNewLines(parser, tokens);
	ASTNode* result = split(parser, tokens, 0);
	if(checkASTNode(result)) return result;
	clearNewLines(parser, tokens);
	if(matchSymbol(tokens, ")") && checkToken(advance(parser, tokens))) return newASTError("Ran Out of Tokens; expected a \")\"", line(tokens));
	return result;
}

ASTNode* parseCurlys(Parser* parser, TokenArray* tokens){
	clearNewLines(parser, tokens);
	ASTNode* b = newASTBlock(tokens->tokens->line, NULL);
	//currentType(tokens) != SYM_TOKEN || !strComp(currentValue(tokens), "}")
	while(!matchSymbol(tokens, "}")) {
		ASTNode* l = parseStatement(parser, tokens);
		if(checkASTNode(l)) return l;
		emitNodeToBlock(l, b);
	}
	if(checkToken(advance(parser, tokens))) return newASTError("Ran Out of Tokens; expected a \"}\"", line(tokens));
	return b;
}

ASTNode* parseWhile(Parser* parser, TokenArray* tokens){
	clearNewLines(parser, tokens);
	ASTNode* expr1 = split(parser, tokens, 0);
	if(checkASTNode(expr1)) return expr1;
	clearNewLines(parser, tokens);
	ASTNode* expr2 = split(parser, tokens, 0);
	if(checkASTNode(expr2)) return expr2;
	return newASTWhile(expr1, expr2, line(tokens), NULL);
}

ASTNode* literal(Parser* parser, Token t){
	switch(t.type){
		case INT_TOKEN : return newASTNumber(newString(t.value), true, t.line, NULL);
		case FLOAT_TOKEN: return newASTNumber(newString(t.value), false, t.line, NULL);
		case STR_A_TOKEN: return newASTString(newString(t.value), t.line, NULL);
		case STR_B_TOKEN: return newASTValue(newBool(true), t.line, NULL);
		case WORD_TOKEN:{
			if(strComp(t.value, "true")) return newASTValue(newBool(true), t.line, NULL);
			else if(strComp(t.value, "false")) return newASTValue(newBool(false), t.line, NULL);
			else return newASTID(newString(t.value), t.line, NULL);
		}
		default: return newASTError("Invalid Literal", t.line);
	}
}

ASTNode* prefix(Parser* parser, TokenArray* tokens){
	Token t = advance(parser, tokens);
	if(checkToken(t)) return newASTError("Ran Out of Tokens; expected a prefix op or a literal", t.line);
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
		if(temp != NULL && temp->type == PREFIX_OP){
			ASTNode* res = split(parser, tokens, temp->op.prec+1);
			if(checkASTNode(res)) return res;
			return newASTUnaryOP(res, t.value, t.line, NULL);
		}
		else return newASTError("Invalid Unary Operator", t.line);
	}
	return literal(parser, t);
}

ASTNode* binaryOP(Parser* parser, Template* t, ASTNode* lhs, TokenArray* tokens){
	if(t==NULL) return newASTError("Invalid Binary Operator", line(tokens));
	if(t->op.ass) {
		ASTNode* res = split(parser, tokens, t->op.prec+1);
		if(checkASTNode(res)) return res;
		return newASTBinaryOP(lhs, newString(t->op.op), res, line(tokens), NULL);
	}
	else {
		ASTNode* res = split(parser, tokens, t->op.prec+1);
		if(checkASTNode(res)) return res;
		return newASTBinaryOP(lhs, newString(t->op.op), res, line(tokens), NULL);
	}
}

ASTNode* otherfix(Parser* parser, Template* t, ASTNode* node, TokenArray* tokens){
	if(checkToken(advance(parser, tokens))) return newASTError("Ran out of Tokens; missing postfix or binary operator", line(tokens));
	if(t != NULL && t->op.type == POSTFIX_OP) return newASTUnaryOP(node, newString(t->op.op), line(tokens), NULL);
	return binaryOP(parser, t, node, tokens);
}

ASTNode* split(Parser* parser, TokenArray* tokens, int prec){
	ASTNode* lhs = prefix(parser, tokens);
	if(checkASTNode(lhs)) return lhs;
	Template* t = currentValue(tokens) != NULL ? searchTemplate(parser->t, currentValue(tokens)) : NULL;
	int precedence = t == NULL ? -1 : t->op.prec;
	bool ass = t == NULL ? false : t->op.ass;
	while(prec <= precedence){
		lhs = otherfix(parser, t, lhs, tokens);
		if(checkASTNode(lhs)) return lhs;
		t = tokens->tokens->value == NULL ? NULL : searchTemplate(parser->t, currentValue(tokens));
		precedence = t == NULL ? -1 : t->op.prec;
		ass = t == NULL ? false : t->op.ass;
	}
	return lhs;
}

ASTNode* parseOnTemplate(){
	return NULL;
}

ASTNode* parseExpression(Parser* parser, TokenArray* tokens, bool statement){
	ASTNode* res = split(parser, tokens, 0);
	if(checkASTNode(res)) return res;
	return newASTExpression(res, statement, line(tokens), NULL);
}

ASTNode* parseVarVar(Parser* p, TokenArray* tokens){
	return NULL;
}

ASTNode* parseLetVar(Parser* p, TokenArray* tokens){
	return NULL;
}

ASTNode* parseStatement(Parser* parser, TokenArray* tokens){
	ASTNode* node = NULL;
	switch(tokens->tokens->type){
		case SYM_TOKEN:
		case INT_TOKEN:
		case FLOAT_TOKEN:
		case STR_A_TOKEN: 
		case STR_B_TOKEN: {
			node = parseExpression(parser, tokens, true);
			if(checkASTNode(node)) return node;
			break;
		}
		case WORD_TOKEN:{
			if (strComp(currentValue(tokens), "var")) node = parseVarVar(parser, tokens);
			else if (strComp(currentValue(tokens), "let")) node = parseLetVar(parser, tokens);
			else node = parseExpression(parser, tokens, true);
			if(checkASTNode(node)) return node;
			break;
		}
		default: return newASTError("Invalid Statement Start", line(tokens));
	}
	if(currentType(tokens) != END_LINE_TOKEN && currentType(tokens) != SEMI_COLON_TOKEN && currentType(tokens) != END_TOKEN) return newASTError("Invalid Statement Ending", line(tokens)); 
	clearEnding(parser, tokens);
	return node;
}

ASTNode* attemptParsingPrimitiveForm(){
	return NULL;
}

ASTNode* attemptParsingFormTemplate(Parser* parser, TokenArray* tokens, Template* t){
	return NULL;
}

ASTNode* attemptParsingForm(Parser* parser, TokenArray* tokens){
	TokenArray* tempTokens = tokens;
	int i = parser->tokenIndex;
	char* word = currentValue(tokens);
	ASTNode* n = attemptParsingPrimitiveForm(parser, tokens);
	if(!checkASTNode(node)) return n;
	TMNode* n = getTemplateTable(parser->table, word);
	if(n == NULL) return newASTError("Failed Parsing Form");
	while(n != NULL){
		ASTNode* node = attemptParsingFormTemplate(parser, tempTokens, n->t);
		if(checkASTNode(node)) {
			freeASTNode(node, true);
			tempTokens = tokens;
			parser->tokenIndex = i;
		}
		else {
			while(tempTokens != tokens) tokens++;//optimize this
			return node;
		}
	}
	return newASTError("Failed Parsing Form", line(tokens));
} 

ASTNode* attemptParsingStatement(Parser* parser, TokenArray* tokens){
	ASTNode* node = NULL;
	switch(currentType(tokens)){
		case SYM_TOKEN:
		case INT_TOKEN:
		case FLOAT_TOKEN:
		case STR_A_TOKEN: 
		case STR_B_TOKEN: {
			node = parseExpression(parser, tokens, true);
			if(checkASTNode(node)) return node;
			break;
		}
		case WORD_TOKEN:{
			node = attemptParsingForm(parser, tokens);
			break;
		}
	}
	if(checkASTNode(node)) return newASTError("Failed Parsing Statement", line(tokens));
	return NULL;
}

ASTNode* parse(Parser* parser, TokenArray* tokens){
	Token* saveTokens = tokens->tokens;
	clearNewLines(parser, tokens);
	while(currentType(tokens) != END_TOKEN){
		ASTNode* n = parseStatement(parser, tokens);
		if(checkASTNode(n)){
			tokens->tokens = saveTokens;
			return n;
		}
		char* res = emitNode(n, parser->ast);
		if(res != NULL){
			tokens->tokens = saveTokens;
			return newASTError("Internal Error", line(tokens));
		}
	}
	tokens->tokens = saveTokens;
	return NULL;
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

Parser* newParser(int formCount){
	Parser* parser = (Parser*) malloc(sizeof(Parser));
	parser->ast = newAST();	
	parser->t = newTemplateTree();
	parser->table = newTemplateTable(formCount);
	parser->tokenIndex = 0;
	return parser;
}
