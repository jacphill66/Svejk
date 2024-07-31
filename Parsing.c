#include "Parsing.h"

Token advance(TokenArray* tokens){
	Token token = *(tokens->tokens);
	tokens->tokens += 1;
	return token;
}

TokenType currentType(TokenArray* tokens){
	return tokens->tokens->type;
}

Token currentToken(TokenArray* tokens){
	return *tokens->tokens;
}

Token peek(TokenArray* tokens){
	Token token = *(tokens->tokens+1);
	return token;
}

typedef enum {
	EQUAL_PREC,
	COMP_PREC,
	BOOL_PREC,
	SUM_PREC,
	FACTOR_PREC,
	EXP_PREC,
	UNARY_PREC,
	PRIMARY_PREC,
} Precedence;

int getPrefixPrec(TokenType t){
	int  precedence[] = {
		[PLUS_OP_TOKEN] = UNARY_PREC,
		[SUB_OP_TOKEN] = UNARY_PREC,
		[NOT_OP_TOKEN] = UNARY_PREC,
	};
	return precedence[t];
}

int getOtherfixPrecedence(TokenType t){
	//make invalid tokens have a -1 precedence
	int precedence[] = { 
			[PLUS_OP_TOKEN] = SUM_PREC,
			[SUB_OP_TOKEN] = SUM_PREC,
			[MULT_OP_TOKEN] = FACTOR_PREC,
			[DIV_OP_TOKEN] = FACTOR_PREC,
			[REM_OP_TOKEN] = FACTOR_PREC,
			[EXP_OP_TOKEN] = EXP_PREC,
			[EQUAL_OP_TOKEN] = EQUAL_PREC,
			[LESS_OP_TOKEN] = COMP_PREC,
			[GREATER_OP_TOKEN] = COMP_PREC,
			[LOE_OP_TOKEN] = COMP_PREC,
			[GOE_OP_TOKEN] = COMP_PREC,
			[AND_OP_TOKEN] = BOOL_PREC,
			[OR_OP_TOKEN] = BOOL_PREC,
			[NOT_OP_TOKEN] = BOOL_PREC,
			[FACT_OP_TOKEN] = UNARY_PREC,
			[I32_VAL_TOKEN] = PRIMARY_PREC,
			[F32_VAL_TOKEN] = PRIMARY_PREC,
			[STR_VAL_TOKEN] = PRIMARY_PREC,
			[TRUE_VAL_TOKEN] = PRIMARY_PREC,
			[FALSE_VAL_TOKEN] = PRIMARY_PREC,
			[ID_TOKEN] = PRIMARY_PREC,	
				// LPAREN will be special, function application
			[LPAREN_OP_TOKEN] = PRIMARY_PREC,
			
			//invalid tokens
			[END_LINE_TOKEN] = -1,
			[LC_BRACKET_TOKEN] = -1,
			[LS_BRACKET_TOKEN] = -1,
			[RC_BRACKET_TOKEN] = -1,
			[LS_BRACKET_TOKEN] = -1,
			[RS_BRACKET_TOKEN] = -1,
			[PRINT_TOKEN] = -1,
			[FOR_TOKEN] = -1,
			[IN_TOKEN] = -1,
			[LET_TOKEN] = -1,
			[COLON_TOKEN] = -1,
			[ASS_TOKEN] = -1,
			[COMMA_TOKEN] = -1,
			[COMMENT_TOKEN] = -1,
			[END_LINE_TOKEN] = -1,
			[END_TOKEN] = -1,
			[RPAREN_OP_TOKEN] = -1,
			[IF_TOKEN] = -1,
			[ELSE_TOKEN] = -1,
	};
	return precedence[t];
}

ASTNode* literal(Parser* parser, Token t){
	switch(t.type){
		case I32_VAL_TOKEN : return newASTValue(newI32(atoi(t.value)), t.line, NULL);
		case F32_VAL_TOKEN: return newASTValue(newF32(atof(t.value)), t.line, NULL);
		case STR_VAL_TOKEN: return newASTString(t.value, t.line, NULL);
		case TRUE_VAL_TOKEN: return newASTValue(newBool(true), t.line, NULL);
		case FALSE_VAL_TOKEN: return newASTValue(newBool(false), t.line, NULL);
		case ID_TOKEN: return newASTID(t.value, t.line, NULL);
		default:{
			printf("Invalid Literal Start\n");
			exit(1);
		}
	}
}

ASTNode* prefix(Parser* parser, TokenArray* tokens){
	Token t = advance(tokens);
	if(t.type == LPAREN_OP_TOKEN){
		ASTNode* result = split(parser, tokens, 0);
		advance(tokens);
		return result;
	}
	switch(t.type){
		case PLUS_OP_TOKEN: return newASTUnaryOP(split(parser, tokens, getPrefixPrec(t.type)+1), UNARY_PLUS_OP, t.line, NULL);
		case SUB_OP_TOKEN: return newASTUnaryOP(split(parser, tokens, getPrefixPrec(t.type)+1), UNARY_MINUS_OP, t.line, NULL);
		case NOT_OP_TOKEN: return newASTUnaryOP(split(parser, tokens, getPrefixPrec(t.type)+1), NOT_OP, t.line, NULL);
		case LC_BRACKET_TOKEN: return parseBlockOrTable(tokens, parser);
		default: return literal(parser, t);
	}
}

ASTNode* binaryOP(Parser* parser, Token op, ASTNode* lhs, TokenArray* tokens){
	ASTNode* rhs;
	if(op.type == EXP_OP_TOKEN)	rhs = split(parser, tokens, getOtherfixPrecedence(op.type));
	else rhs = split(parser, tokens, getOtherfixPrecedence(op.type)+1);
	switch(op.type){
		case EQUAL_OP_TOKEN : return newASTBinaryOP(lhs, EQUAL_OP, rhs, op.line, NULL);
		case LESS_OP_TOKEN : return newASTBinaryOP(lhs, LESS_OP, rhs, op.line, NULL);
		case GREATER_OP_TOKEN :	return newASTBinaryOP(lhs, GREATER_OP, rhs, op.line, NULL);
		case LOE_OP_TOKEN : return newASTBinaryOP(lhs, LOE_OP, rhs, op.line, NULL);
		case GOE_OP_TOKEN : return newASTBinaryOP(lhs, GOE_OP, rhs, op.line, NULL);	
		case AND_OP_TOKEN : return newASTBinaryOP(lhs, AND_OP, rhs, op.line, NULL);
		case OR_OP_TOKEN : return newASTBinaryOP(lhs, OR_OP, rhs, op.line, NULL);
		case PLUS_OP_TOKEN : return newASTBinaryOP(lhs, PLUS_OP, rhs, op.line, NULL);
		case SUB_OP_TOKEN :  return newASTBinaryOP(lhs, SUB_OP, rhs, op.line, NULL);
		case MULT_OP_TOKEN :  return newASTBinaryOP(lhs, MULT_OP, rhs, op.line, NULL);
		case DIV_OP_TOKEN :  return newASTBinaryOP(lhs, DIV_OP, rhs, op.line, NULL);
		case REM_OP_TOKEN : return newASTBinaryOP(lhs, REM_OP, rhs, op.line, NULL);
		case EXP_OP_TOKEN: return newASTBinaryOP(lhs, EXP_OP, rhs, op.line, NULL);
		default:{
			printf("Unidentified Binary OP");
			exit(1);
		}
	}
	//if op is right associative
	//if op is left associative
}

ASTNode* otherfix(Parser* parser, ASTNode* node, TokenArray* tokens){
	Token token = advance(tokens);
	switch(token.type){		
		case LPAREN_OP_TOKEN:return NULL;//callop
		case FACT_OP_TOKEN:return newASTUnaryOP(node, FACT_OP, token.line, NULL);
		default: return binaryOP(parser, token, node, tokens);
	}
}

ASTNode* split(Parser* parser, TokenArray* tokens, int prec){
	ASTNode* lhs = prefix(parser, tokens);
	while(prec <= getOtherfixPrecedence(tokens->tokens->type)){
		lhs = otherfix(parser, lhs, tokens);
	}
	return lhs;
}

ASTNode* parseExpression(TokenArray* tokens, Parser* parser){
	bool statement = false;
	if(currentToken(tokens).type == END_LINE_TOKEN) statement = true;
	return newASTExpression(split(parser, tokens, 0), statement, tokens->tokens->line, NULL);
}

ASTNode* parsePrint(TokenArray* tokens, Parser* parser){
	advance(tokens);
	return newASTPrint(split(parser, tokens, 0), tokens->tokens->line, NULL);

}

ASTNode* parseLet(TokenArray* tokens, Parser* parser){
	advance(tokens);
	Token t = advance(tokens);
	char* id = t.value;
	Type* type = NULL;
	t = advance(tokens);
	bool typed = false;
	if(t.type == COLON_TOKEN){
		t = advance(tokens);
		switch (t.type){
			case I32_TOKEN:{
				type = newTrivialType(I32_TYPE);
				break;
			}
			case F32_TOKEN:{
				type = newTrivialType(F32_TYPE);
				break;
			}
			case BOOL_TOKEN:{
				type = newTrivialType(BOOL_TYPE);
				break;
			}
			case STR_TOKEN:{
				type = newTrivialType(STR_TYPE);
				break;
			}
			default:{
				type = NULL;
				printf("Invalid Type Anotation\n");
				exit(1);
			}
		}
		typed = true;
		advance(tokens);
	}
	ASTNode* var = newASTVariable(id, split(parser, tokens, 0), type, t.line, NULL);
	if(!typed) var->var.type = newTrivialType(INFERRED_TYPE);
	return var;
}

bool isAssignmentToken(TokenType t){
	//later add: +=, -=, ...
	switch(t){
		case ASS_TOKEN:{
			return true;
		}
		default:{
			return false;
		}
	}
	return false;
}

ASTNode* parseAssignment(TokenArray* tokens, Parser* parser){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	Token t = *tokens->tokens;
	char* id = t.value;
	advance(tokens);
	advance(tokens);
	return newASTAssignment(id, split(parser, tokens, 0), t.line, NULL);
}

ASTNode* parseAssignmentOrReference(TokenArray* tokens, Parser* parser){
	if(isAssignmentToken(peek(tokens).type)) return parseAssignment(tokens, parser);
	return parseExpression(tokens, parser);
}

ASTNode* parseElse(TokenArray* tokens, Parser* p){
	Token t = advance(tokens);
	return newASTElse(parseStatement(p, tokens), t.line, NULL);
}

ASTNode* parseIf(TokenArray* tokens, Parser* p){
	Token t = advance(tokens);
	ASTNode* expr = split(p, tokens, 0);
	ASTNode* s = parseStatement(p, tokens);
	if(tokens->tokens->type == END_LINE_TOKEN) advance(tokens);
	if(tokens->tokens->type == ELSE_TOKEN) return newASTIf(expr, s, parseElse(tokens, p), t.line, NULL);
	else return newASTIf(expr, s, NULL, t.line, NULL);
}

ASTNode* parseFor(TokenArray* tokens, Parser* parser){
	Token t = advance(tokens);
	ASTNode* b = newASTBlock(t.line, NULL);
	ASTNode* loop = newASTForLoop(t.line, NULL);
	loop->loop.line = t.line;
	if((tokens->tokens->type != LC_BRACKET_TOKEN)&&(tokens->tokens->type != LS_BRACKET_TOKEN)){
		loop->loop.n1 = parseStatement(parser, tokens);
		if(tokens->tokens->type == IN_TOKEN){}
		else if((tokens->tokens->type != LS_BRACKET_TOKEN)&&(tokens->tokens->type != LC_BRACKET_TOKEN)){
			advance(tokens);
			loop->loop.n2 = parseStatement(parser, tokens);
			advance(tokens);
			loop->loop.n3 = parseStatement(parser, tokens);
		}
	}
	if(tokens->tokens->type == LS_BRACKET_TOKEN){
		advance(tokens);
		if(tokens->tokens->type != COMMA_TOKEN) loop->loop.min = parseExpression(tokens, parser);
		advance(tokens);
		if(tokens->tokens->type != RS_BRACKET_TOKEN) loop->loop.max = parseExpression(tokens, parser);
		advance(tokens);
	}
	advance(tokens);
	while(tokens->tokens->type != RC_BRACKET_TOKEN){
		parseLocal(parser, b, tokens);
	}
	loop->loop.b = b;
	advance(tokens);
	return loop;
}

ASTNode* parseBlockExpression(TokenArray* tokens, Parser* parser, ASTNode* s){
	ASTNode* b = newASTBlock(tokens->tokens->line, NULL);
	if(s!=NULL) emitNodeToBlock(s, b);
	while(tokens->tokens->type != RC_BRACKET_TOKEN){
		parseLocal(parser, b, tokens);
	}
	advance(tokens);
	return b;
}

ASTNode* parseBlockOrTable(TokenArray* tokens, Parser* p){
	ASTNode* s = NULL;
	if(tokens->tokens->type != RC_BRACKET_TOKEN) s = parseStatement(p, tokens);
	if(tokens->tokens->type == END_LINE_TOKEN || (s!=NULL && (s->type != ASTExpression_NODE_TYPE || (s->type == ASTExpression_NODE_TYPE && s->expr.statement)))){
		if(currentToken(tokens).type == END_LINE_TOKEN) advance(tokens);
		ASTNode* b = newASTBlock(tokens->tokens->line, NULL);
		if(s != NULL) emitNodeToBlock(s, b);
		while(tokens->tokens->type != RC_BRACKET_TOKEN){
			parseLocal(p, b, tokens);
		}
		advance(tokens);
		return b;
	}
	else{
		ASTNode* table = newASTTable(TABLE_1A, tokens->tokens->line, NULL);
		if(s != NULL) emitNodeToTable(NULL, s->expr.expr, table);
		free(s);//might need to free stuff in rewritter.
		advance(tokens);//maybe advance
		while(tokens->tokens->type != RC_BRACKET_TOKEN){
			emitNodeToTable(NULL, split(p, tokens, 0), table);
			advance(tokens);//maybe advance
		}
		advance(tokens);
		return table;
	}
}

ASTNode* parseExpressionOrTuple(TokenArray* tokens, Parser* p){
	return parseExpression(tokens, p);
}

ASTNode* parseArray(TokenArray* tokens, Parser* p){
	return NULL;
}

ASTNode* parseStatement(Parser* parser, TokenArray* tokens){
	switch(tokens->tokens->type){
		case I32_VAL_TOKEN : 
		case F32_VAL_TOKEN :
		case STR_VAL_TOKEN :
		case TRUE_VAL_TOKEN :
		case FALSE_VAL_TOKEN :
		case PLUS_OP_TOKEN :
		case SUB_OP_TOKEN :
		case NOT_OP_TOKEN : 
		case LPAREN_OP_TOKEN :
		case LC_BRACKET_TOKEN: 
		case LS_BRACKET_TOKEN : return parseExpression(tokens, parser);
		case ID_TOKEN: return parseAssignmentOrReference(tokens, parser);
		case PRINT_TOKEN: return parsePrint(tokens, parser);
		case FOR_TOKEN:	return parseFor(tokens, parser);
		case LET_TOKEN:	return parseLet(tokens, parser);
		case IF_TOKEN: return parseIf(tokens, parser);
		default:{
			printf("Unparsable token\n");
			printf("%d", tokens->tokens->type);
			exit(1);
		}
	}
	return NULL;
}

void parseLocal(Parser* parser, ASTNode* b, TokenArray* tokens){
	if(tokens->tokens->type == END_LINE_TOKEN) {
		advance(tokens);
		return;
	}
	emitNodeToBlock(parseStatement(parser, tokens), b);
}

void parse(Parser* parser, TokenArray* tokens){
	Token* tokens2 = tokens->tokens;
	while(tokens->tokens->type != END_TOKEN){
		if(tokens->tokens->type == END_LINE_TOKEN){
			advance(tokens); 
			continue;
		}
		emitNode(parseStatement(parser, tokens), parser->ast);
	}
	tokens->tokens = tokens2;
}

void freeParser(Parser* parser){
	freeAST(parser->ast, true);
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
	return parser;
}