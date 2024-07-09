#include "Parsing.h"

void printOP(OPCode op){
	switch(op){
		case PLUS_OP:{
			printf("+");
			break;
		}
		case SUB_OP:{
			printf("-");
			break;
		}
		case MULT_OP:{
			printf("*");
			break;
		}
		case DIV_OP:{
			printf("/");
			break;
		}
		case REM_OP:{
			printf("%c", '%');
			break;
		}
		case EXP_OP:{
			printf("^");
			break;
		}
		case FACT_OP:{
			printf("!");
			break;
		}
		case AND_OP:{
			printf("and");
			break;
		}
		case OR_OP:{
			printf("or");
			break;
		}
		case NOT_OP:{
			printf("not");
			break;
		}
		case UNARY_PLUS_OP:{
			printf("+");
			break;
		}
		case UNARY_MINUS_OP:{
			printf("-");
			break;
		}
		case EQUAL_OP:{
			printf("==");
			break;
		}
		case LESS_OP:{
			printf("<");
			break;
		}
		case GREATER_OP:{
			printf(">");
			break;
		}
		case LOE_OP:{
			printf("<=");
			break;
		}
		case GOE_OP:{
			printf("==");
			break;
		}

	}
}

void printASTNode(AST* ast, ASTNode* node){
	ASTNodeType type = node->type;
	switch(type){
		case ASTBinaryOP_NODE_TYPE :{
			printf("(");
			printASTNode(ast, node->binaryOP.lhs);
			printOP(node->binaryOP.op);
			printASTNode(ast, node->binaryOP.rhs);
			printf(")");
			break;
		}
		case ASTUnaryOP_NODE_TYPE : {
			printf("(");
			if(node->unaryOP.op == FACT_OP){
				printASTNode(ast, node->unaryOP.opperand);
				printOP(node->unaryOP.op);
			}
			else{
				printOP(node->unaryOP.op);
				printASTNode(ast, node->unaryOP.opperand);
			}
			printf(")");
			break;
		}
		case ASTString_NODE_TYPE : {
			printf("String[");
			printf("%s", node->str.str);
			printf("]");
			break;
		}
		case ASTCallOP_NODE_TYPE:{
			printASTNode(ast, node->callOP.opperand);
			printf("()");
			break;
		}
		case ASTGlobalID_NODE_TYPE:{
			printf("GlobalID[%s, %ld]", node->globalID.id, node->globalID.index);
			break;
		}
		case ASTLocalID_NODE_TYPE:{
			printf("LocalID[%s]", node->localID.id);
			break;
		}
		case ASTValue_NODE_TYPE:{
			switch(node->value.v.type){
				case I32_VAL :{
					printf("%d", node->value.v.i32);
					break;
				}
				case F32_VAL:{
					printf("%f", node->value.v.f32);
					break;
				}
				case BOOL_VAL:{
					if(node->value.v.boolean){
						printf("true");
					}
					else{ 
						printf("false");
					}
					break;
				}
				case STR_VAL:{
					//printf("%s", ast->strings[node->value.v.i32]);
					break;
				}
				default : {
					printf("%d\n", node->type);
					printf("Cannot print value of given type 1");
					exit(1);
				}
			}
			break;
		}
		case ASTExpression_NODE_TYPE:{
			printf("Expression[");
			printASTNode(ast, node->expr.expr);
			printf("]");
			break;
		}
		case ASTPrint_NODE_TYPE:{
			printf("Print[");
			printASTNode(ast, node->print.expr);
			printf("]");
			break;
		}
		case ASTGlobalVariable_NODE_TYPE:{
			printf("GlobalVariable[");
			printf("Id:%s, Type:%d, Expression:", node->globalVar.id, node->globalVar.type);
			printASTNode(ast, node->globalVar.expr);
			printf("]");
			break;
		}
		case ASTLocalVariable_NODE_TYPE:{
			printf("LocalVariable[");
			printf("Id:%s, Type:%d, Offset:%ld, Expression:", node->localVar.id, node->localVar.type, node->localVar.offset);
			printASTNode(ast, node->localVar.expr);
			printf("]");
			break;
		}
		case ASTLocalAssignment_NODE_TYPE:{
			printf("LocalAssignment[");
			printf("Id:%s, Offset:%ld, Expression:", node->localAss.id, node->localAss.offset);
			printASTNode(ast, node->localAss.expr);
			printf("]");
			break;
		}
		case ASTGlobalAssignment_NODE_TYPE:{
			printf("GlobalAssignment[");
			printf("Id:%s, Index:%ld, Expression:", node->globalAss.id, node->globalAss.index);
			printASTNode(ast, node->globalAss.expr);
			printf("]");
			break;
		}
		case ASTBlock_NODE_TYPE:{
			printf("Block[\n");
			for(int i = 0; i < node->block.numberOfNodes; i++) {
				printASTNode(ast, &node->block.nodes[i]);
				printf("\n");
			}
			printf("]");
			break;
		}
		case ASTForLoop_NODE_TYPE:{
			printf("for ");
			if(node->loop.n1 != NULL){
				printf("NODE1=[");
				printASTNode(ast, node->loop.n1);
				printf("]");
			}
			if(node->loop.n2 != NULL) {
				if(node->loop.n3 == NULL){
					printf(" in ");
					printf("COLLECTION=[");
					printASTNode(ast, node->loop.n2);
					printf("] ");
				}
				else{
					printf("NODE2=[");
					printASTNode(ast, node->loop.n2);
					printf("] ");
					printf("NODE3=[");
					printASTNode(ast, node->loop.n3);
					printf("]");
				}
			}
			printf("[");
			if(node->loop.min != NULL){
				printf("MIN=[");
				printASTNode(ast, node->loop.min);
				printf("]");
			}
			printf(", ");
			if(node->loop.max != NULL) {
				printf("MAX=[");
				printASTNode(ast, node->loop.max);
				printf("]");
			}
			printf("]");
			printf("{\n");
			printASTNode(ast, node->loop.b);
			printf("}");
			break;
		}
		case ASTLoop_NODE_TYPE:{
			printf("for ");
			printASTNode(ast, node->simpleLoop.expr);
			printf("{\n");
			printASTNode(ast, node->simpleLoop.block);
			printf("}");
			break;
		}
		default : {
			printf("Cannot print node of given type\n"); 
			printf("%d",type);
			exit(1);
		}
	}
}

void printAST(AST* ast){
	printf("AST:\n");
	for(long i = 0; i < ast->numberOfNodes; i++){
		printASTNode(ast, &ast->nodes[i]); printf("\n");
	}
	printf("\n");
}

ASTNode* resizeNodes(ASTNode* nodes, long cappacity){
	ASTNode* newNodes = (ASTNode*)realloc(nodes, 2*cappacity*sizeof(ASTNode));
	if(newNodes == NULL){
		printf("could not resize AST-Nodes!\n");
		free(nodes);
		exit(1);
	}
	return newNodes;
}

Token advance(TokenArray* tokens){
	Token token = *(tokens->tokens);
	tokens->tokens += 1;
	return token;
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
	};
	return precedence[t];
}

bool validExpressionToken(TokenArray* tokens){	
	TokenType validTypes[] = {
		I32_VAL_TOKEN,
		F32_VAL_TOKEN,
		STR_VAL_TOKEN,
		TRUE_VAL_TOKEN,
		FALSE_VAL_TOKEN,
		
		ID_TOKEN,

		PLUS_OP_TOKEN,
		SUB_OP_TOKEN,
		MULT_OP_TOKEN,
		DIV_OP_TOKEN,
		REM_OP_TOKEN,
		
		FACT_OP_TOKEN,
		
		EQUAL_OP_TOKEN,
		
		LESS_OP_TOKEN,
		GREATER_OP_TOKEN,
		LOE_OP_TOKEN,
		GOE_OP_TOKEN,
		
		AND_OP_TOKEN,
		OR_OP_TOKEN,
		NOT_OP_TOKEN,
		
		LPAREN_OP_TOKEN,
	};
	for(int i = 0; i < 21; i++){
		if(validTypes[i] == tokens->tokens->type){
			return true;
		}
	}
	return false;
}

ASTNode* literal(Parser* parser, Token t){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	switch(t.type){
		case I32_VAL_TOKEN : {
			node->type = ASTValue_NODE_TYPE;
			Value v;
			v.type = I32_VAL;
			v.i32 = atoi(t.value);
			ASTValue val = {v, t.line};
			node->value = val;
			return node;
		}
		case F32_VAL_TOKEN:{
			node->type = ASTValue_NODE_TYPE;
			Value v;
			v.type = F32_VAL;
			v.f32 = atof(t.value);
			ASTValue val = {v, t.line};
			node->value = val;
			return node;
		}
		case STR_VAL_TOKEN:{
			node->type = ASTString_NODE_TYPE;
			ASTString str = {t.value, search(parser->strings, t.value).i32, t.line};
			if(str.index == -1) {
				Value v;
				v.i32 = parser->stringCount;
				str.index = parser->stringCount;
				insert(parser->strings, t.value, t.size, v);
				parser->stringCount++;
				parser->ast->stringCount++;
			}
			node->str = str;
			return node;
		}
		case TRUE_VAL_TOKEN:{
			node->type = ASTValue_NODE_TYPE;
			Value v;
			v.type = BOOL_VAL;
			v.boolean = true;
			ASTValue val = {v, t.line};
			node->value = val;
			return node;	
		}
		case FALSE_VAL_TOKEN:{
			node->type = ASTValue_NODE_TYPE;
			Value v;
			v.type = BOOL_VAL;
			v.boolean = false;
			ASTValue val = {v, t.line};
			node->value = val;
			return node;	
		}
		case ID_TOKEN:{
			if(searchScopes(parser->scopes, t.value) == -1){
				ASTGlobalID id = {t.value, get(parser->globalVariables, t.value).i32, t.line};
				node->globalID = id;
				node->type = ASTGlobalID_NODE_TYPE;
			}
			else{
				ASTLocalID id = {t.value, searchScopes(parser->scopes, t.value), t.line};
				node->localID = id;
				node->type = ASTLocalID_NODE_TYPE;
			}
			return node;
		}
	}
}

ASTNode* prefix(Parser* parser, TokenArray* tokens){
	Token t = advance(tokens);
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	if(t.type == LPAREN_OP_TOKEN){
		ASTNode* result = split(parser, tokens, 0);
		advance(tokens);
		return result;
	}
	switch(t.type){
		case PLUS_OP_TOKEN:{
			node->type = ASTUnaryOP_NODE_TYPE;
			ASTUnaryOP uOP = {UNARY_PLUS_OP, split(parser, tokens, getPrefixPrec(t.type)+1), t.line};
			node->unaryOP = uOP;
			return node;
		}
		case SUB_OP_TOKEN:{
			node->type = ASTUnaryOP_NODE_TYPE;		
			ASTUnaryOP uOP = {UNARY_MINUS_OP, split(parser, tokens, getPrefixPrec(t.type)+1), t.line};
			node->unaryOP = uOP;
			return node;
		}
		case NOT_OP_TOKEN:{
			node->type = ASTUnaryOP_NODE_TYPE;		
			ASTUnaryOP uOP = {NOT_OP, split(parser, tokens, getPrefixPrec(t.type)+1), t.line};
			node->unaryOP = uOP;
			return node;
		}
		default:{
			return literal(parser, t);
		}
	}
}

ASTNode* binaryOP(Parser* parser, Token op, ASTNode* lhs, TokenArray* tokens){
	ASTNode* n = (ASTNode*) malloc(sizeof(ASTNode));
	n->type = ASTBinaryOP_NODE_TYPE;
	ASTBinaryOP binOP;
	binOP.line = op.line;
	switch(op.type){
		case EQUAL_OP_TOKEN :{
			binOP.lhs = lhs;
			binOP.op = EQUAL_OP;
			break;
		}
		case LESS_OP_TOKEN : {
			binOP.lhs = lhs;
			binOP.op = LESS_OP;
			break;
		}
		case GREATER_OP_TOKEN : {
			binOP.lhs = lhs;
			binOP.op = GREATER_OP;
			break;
		}
		case LOE_OP_TOKEN : {
			binOP.lhs = lhs;
			binOP.op = LOE_OP;
			break;
		}
		case GOE_OP_TOKEN : {
			binOP.lhs = lhs;
			binOP.op = GOE_OP;
			break;
		}				
		case AND_OP_TOKEN : {
			binOP.lhs = lhs;
			binOP.op = AND_OP;
			break;
		}
		case OR_OP_TOKEN : {
			binOP.lhs = lhs;
			binOP.op = OR_OP;
			break;
		}
		case PLUS_OP_TOKEN : {
			binOP.lhs = lhs;
			binOP.op = PLUS_OP;
			break;
		}
		case SUB_OP_TOKEN : {
			binOP.lhs = lhs;
			binOP.op = SUB_OP;
			break;
		}
		case MULT_OP_TOKEN : {
			binOP.lhs = lhs;
			binOP.op = MULT_OP;
			break;
		}
		case DIV_OP_TOKEN : {
			binOP.lhs = lhs;
			binOP.op = DIV_OP;
			break;
		}
		case REM_OP_TOKEN : {
			binOP.lhs = lhs;
			binOP.op = REM_OP;
			break;
		}		
		case EXP_OP_TOKEN: {
			binOP.lhs = lhs;
			binOP.op = EXP_OP;
			break;
		}
		default:{
			printf("Unidentified Binary OP");
			exit(1);
		}
	}
	//if op is right associative
	//if op is left associative
	if(op.type == EXP_OP_TOKEN){
		binOP.rhs = split(parser, tokens, getOtherfixPrecedence(op.type));
	}
	else{
		binOP.rhs = split(parser, tokens, getOtherfixPrecedence(op.type)+1);
	}
	n->binaryOP = binOP;
	return n;
}

ASTNode* otherfix(Parser* parser, ASTNode* node, TokenArray* tokens){
	Token token = advance(tokens);
	switch(token.type){		
		case LPAREN_OP_TOKEN : {
			advance(tokens);
			ASTNode* n = (ASTNode*) malloc(sizeof(ASTNode));
			ASTCallOP cOP = {node, token.line}; 
			n->callOP = cOP;
			n->type = ASTCallOP_NODE_TYPE;
			return n;
		}
		case FACT_OP_TOKEN: {
			ASTNode* n = (ASTNode*) malloc(sizeof(ASTNode));
			ASTUnaryOP uOP = {FACT_OP, node, token.line}; 
			n->unaryOP = uOP;
			n->type = ASTUnaryOP_NODE_TYPE;
			return n;
		}
		default :{
			return binaryOP(parser, token, node, tokens);
		}
	}
}

ASTNode* split(Parser* parser, TokenArray* tokens, int prec){
	ASTNode* lhs = prefix(parser, tokens);
	while(prec <= getOtherfixPrecedence(tokens->tokens->type)){
		lhs = otherfix(parser, lhs, tokens);
	}
	return lhs;
}

void emitNode(ASTNode* node, AST* ast){
	ast->nodes[ast->numberOfNodes] = *node;
	ast->numberOfNodes++;
	if(ast->numberOfNodes == ast->cappacity){
		ast->nodes = resizeNodes(ast->nodes, ast->cappacity);
		ast->cappacity *= 2;
	}

}

void emitNodeToBlock(ASTNode* node, ASTNode* b){
	b->block.nodes[b->block.numberOfNodes] = *node;
	b->block.numberOfNodes++;
	if(b->block.numberOfNodes == b->block.cappacity){
		b->block.nodes = resizeNodes(b->block.nodes, b->block.cappacity);
		b->block.cappacity *= 2;
	}
}

ASTNode* parseExpression(TokenArray* tokens, Parser* parser){
	ASTExpression expr = {split(parser, tokens, 0), tokens->tokens->line};
	ASTNode* n = malloc(sizeof(ASTNode));
	n->type = ASTExpression_NODE_TYPE;
	n->expr = expr;
	return n;
}

ASTNode* parsePrint(TokenArray* tokens, Parser* parser){
	advance(tokens);
	ASTNode* n = malloc(sizeof(ASTNode));
	ASTPrint p = {split(parser, tokens, 0), tokens->tokens->line};
	n->type = ASTPrint_NODE_TYPE;
	n->print = p;
	return n;
}

ASTNode* parseGlobalLet(TokenArray* tokens, Parser* parser){
	advance(tokens);
	Token t = advance(tokens);
	ASTNode* n = malloc(sizeof(ASTNode));
	ASTGlobalVariable var;
	var.line = t.line;
	var.id = t.value;
	bool redeclared = false;
	if(get(parser->globalVariables, t.value).i32 != -1) redeclared = true;
	int idSize = t.size;
	t = advance(tokens);
	bool typed = false;
	if(t.type == COLON_TOKEN){
		t = advance(tokens);
		switch (t.type){
			case I32_TOKEN:{
				var.type = I32_TYPE;
				break;
			}
			case F32_TOKEN:{
				var.type = F32_TYPE;
				break;
			}
			case BOOL_TOKEN:{
				var.type = BOOL_TYPE;
				break;
			}
			case STR_TOKEN:{
				var.type = STR_TYPE;
				break;
			}
			default:{
				var.type = -1;
				printf("Invalid Type Anotation\n");
				exit(1);
			}
		}
		typed = true;
		advance(tokens);
	}
	ASTNode* expr = {split(parser, tokens, 0)};
	var.expr = expr;
	if(redeclared) var.index = -2;
	else var.index = parser->globalCount;
	Value index;
	index.i32 = parser->globalCount;
	set(parser->globalVariables, var.id, idSize, index);
	parser->globalCount += 1;
	if(!typed) var.type = INFERRED_TYPE;
	n->type = ASTGlobalVariable_NODE_TYPE;
	n->globalVar = var;
	return n;
}

bool isAssignmentToken(TokenType t){
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
	int localIndex = searchScopes(parser->scopes, t.value);
	int globalIndex = get(parser->globalVariables, id).i32;
	advance(tokens);
	advance(tokens);
	ASTNode* expr = split(parser, tokens, 0);
	if(localIndex == -1){
		ASTGlobalAssignment ass = {id, expr, globalIndex, t.line};
		node->globalAss = ass;
		node->type = ASTGlobalAssignment_NODE_TYPE;
	}
	else {
		ASTLocalAssignment ass = {id, expr, localIndex, t.line};
		node->localAss = ass;
		node->type = ASTLocalAssignment_NODE_TYPE;
	}
	return node;
}

ASTNode* parseAssignmentOrReference(TokenArray* tokens, Parser* parser){
	if(isAssignmentToken(peek(tokens).type)) return parseAssignment(tokens, parser);
	return parseExpression(tokens, parser);
}

ASTNode* parseLocalLet(TokenArray* tokens, Parser* parser){
	advance(tokens);
	Token t = advance(tokens);
	ASTNode* n = malloc(sizeof(ASTNode));
	ASTLocalVariable var;
	var.line = tokens->tokens->line;
	var.id = t.value;
	bool redeclared = false;
	if(searchScope(parser->scopes->tail, t.value) != -1) redeclared = true;
	int idSize = t.size;
	//change advance... for infered types
	t = advance(tokens);
	bool typed = false;
	if(t.type == COLON_TOKEN){
		t = advance(tokens);
		switch (t.type){
			case I32_TOKEN:{
				var.type = I32_TYPE;
				break;
			}
			case F32_TOKEN:{
				var.type = F32_TYPE;
				break;
			}
			case BOOL_TOKEN:{
				var.type = BOOL_TYPE;
				break;
			}
			case STR_TOKEN:{
				var.type = STR_TYPE;
				break;
			}
			default:{
				printf("Invalid Type Token\n");
			}
		}
		typed = true;
		advance(tokens);
	}
	ASTNode* expr = {split(parser, tokens, 0)};
	var.expr = expr;
	if(redeclared) var.offset = -2;
	else var.offset = parser->scopes->tail->offset;
	addToCurrentScope(parser->scopes, var.id, idSize, -1);
	if(!typed) var.type = INFERRED_TYPE;
	n->type = ASTLocalVariable_NODE_TYPE;
	n->localVar = var;
	return n;
}

ASTNode* newBlock(){
	ASTNode* n = malloc(sizeof(ASTNode));
	ASTBlock b;
	b.nodes = (ASTNode*)malloc(sizeof(ASTNode));
	b.cappacity = 1;
	b.numberOfNodes = 0;
	b.variableCount = 0;
	n->block = b;
	n->type = ASTBlock_NODE_TYPE;
	return n;
}

ASTNode* parseBlockExpression(TokenArray* tokens, Parser* parser){
	ASTNode* b = newBlock();
	advance(tokens);
	parser->scopeDepth += 1;
	newScope(parser->scopes);
	while(tokens->tokens->type != RC_BRACKET_TOKEN){
		parseLocal(parser, b, tokens);
	}
	parser->scopeDepth -= 1;
	closeScope(parser->scopes);
	advance(tokens);
	return b;
}

ASTNode* newLoop(){
	ASTNode* loop = (ASTNode*)malloc(sizeof(ASTNode));
	loop->loop.n1 = NULL;
	loop->loop.n2 = NULL;
	loop->loop.n3 = NULL;
	loop->loop.min = NULL;
	loop->loop.max = NULL;
	loop->loop.b = NULL;
	loop->type = ASTForLoop_NODE_TYPE;
	return loop;
}

ASTNode* parseFor(Parser* parser, TokenArray* tokens){
	Token t = advance(tokens);
	ASTNode* b = newBlock();
	parser->scopeDepth += 1;
	newScope(parser->scopes);
	ASTNode* loop = newLoop();
	loop->loop.line = t.line;
	if((tokens->tokens->type != LC_BRACKET_TOKEN)&&(tokens->tokens->type != LS_BRACKET_TOKEN)){
		loop->loop.n1 = parseStatement(parser, tokens, b);
		if(loop->loop.n1->type == ASTLocalVariable_NODE_TYPE) addToCurrentScope(parser->scopes, loop->loop.n1->localVar.id, -1, -1);
		if(tokens->tokens->type == IN_TOKEN){
			//modify this later:
				// need to add local to scope, among other things
				// need to ensure the first parse statment will get a local id and need to deal with it
			advance(tokens);
			ASTNode* expr = parseExpression(tokens, parser);
			loop->loop.n2 = expr;
		}
		else if((tokens->tokens->type != LS_BRACKET_TOKEN)&&(tokens->tokens->type != LC_BRACKET_TOKEN)){
			advance(tokens);
			loop->loop.n2 = parseStatement(parser, tokens, b);
			if(loop->loop.n2->type == ASTLocalVariable_NODE_TYPE) addToCurrentScope(parser->scopes, loop->loop.n2->localVar.id, -1, -1);
			advance(tokens);
			loop->loop.n3 = parseStatement(parser, tokens, b);
			if(loop->loop.n3->type == ASTLocalVariable_NODE_TYPE) addToCurrentScope(parser->scopes, loop->loop.n3->localVar.id, -1, -1);
		}
	}
	if(tokens->tokens->type == LS_BRACKET_TOKEN){
		advance(tokens);
		if(tokens->tokens->type != COMMA_TOKEN) loop->loop.min = parseExpression(tokens, parser);
		advance(tokens);
		if(tokens->tokens->type != RS_BRACKET_TOKEN) loop->loop.max = parseExpression(tokens, parser);
		advance(tokens);
	}
	/*parseLocal(parser, b, tokens);
	closeScope(parser->scopes);
	loop->loop.b = b;*/
	
	//Scope resolution - fix this, Need to have them in a scope, also need to ensure this is done before the statements are done

	advance(tokens);
	parser->scopeDepth += 1;
	newScope(parser->scopes);
	while(tokens->tokens->type != RC_BRACKET_TOKEN){
		parseLocal(parser, b, tokens);
	}
	parser->scopeDepth -= 1;
	closeScope(parser->scopes);
	parser->scopeDepth -= 1;
	closeScope(parser->scopes);
	loop->loop.b = b;
	advance(tokens);
	return loop;
}

ASTNode* parseStatement(Parser* parser, TokenArray* tokens, ASTNode* b){
	switch(tokens->tokens->type){
		case I32_VAL_TOKEN : {
			return parseExpression(tokens, parser);
		}
		case F32_VAL_TOKEN : {
			return parseExpression(tokens, parser);
		}	
		case STR_VAL_TOKEN : {
			return parseExpression(tokens, parser);
		}			
		case TRUE_VAL_TOKEN : {
			return parseExpression(tokens, parser);
		}		
		case FALSE_VAL_TOKEN : {
			return parseExpression(tokens, parser);
		}		
		case ID_TOKEN:{
			return parseAssignmentOrReference(tokens, parser);
		}
		case PLUS_OP_TOKEN : {
			return parseExpression(tokens, parser);
		}		
		case SUB_OP_TOKEN : {
			return parseExpression(tokens, parser);
		}	
		case LPAREN_OP_TOKEN : {
			return parseExpression(tokens, parser);
		}
		case LC_BRACKET_TOKEN:{
			return parseBlockExpression(tokens, parser);
		}
		case PRINT_TOKEN:{
			return parsePrint(tokens, parser);
		}
		case FOR_TOKEN:{
			return parseFor(parser, tokens);
		}
		case LET_TOKEN:{
			if(parser->scopeDepth > 0){
				b->block.variableCount += 1;
				return parseLocalLet(tokens, parser);			
			}
			else return parseGlobalLet(tokens, parser);
		}
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
	emitNodeToBlock(parseStatement(parser, tokens, b), b);
}

void parse(Parser* parser, TokenArray* tokens){
	Token* tokens2 = tokens->tokens;
	while(tokens->tokens->type != END_TOKEN){
		if(tokens->tokens->type == END_LINE_TOKEN){
			advance(tokens); 
			continue;
		}
		emitNode(parseStatement(parser, tokens, NULL), parser->ast);
	}
	tokens->tokens = tokens2;
}

void freeASTNode(ASTNode* node){
	ASTNodeType type = node->type;
	switch(type){
		case ASTBinaryOP_NODE_TYPE :{
			freeASTNode(node->binaryOP.lhs);
			freeASTNode(node->binaryOP.rhs);
			free(node->binaryOP.lhs);
			free(node->binaryOP.rhs);
			break;
		}
		case ASTString_NODE_TYPE:{
			//free(node->str.str);
			break;
		}
		case ASTUnaryOP_NODE_TYPE : {
			freeASTNode(node->unaryOP.opperand);
			free(node->unaryOP.opperand);
			break;
		}
		case ASTCallOP_NODE_TYPE:{
			freeASTNode(node->callOP.opperand);
			free(node->callOP.opperand);
			break;
		}
		case ASTLocalID_NODE_TYPE:{
			free(node->localID.id);
			break;
		}
		case ASTGlobalID_NODE_TYPE:{
			free(node->globalID.id);
			break;
		}
		case ASTValue_NODE_TYPE:{
			break;
		}
		case ASTExpression_NODE_TYPE:{
			freeASTNode(node->expr.expr);
			free(node->expr.expr);
			break;
		}
		case ASTPrint_NODE_TYPE:{
			freeASTNode(node->print.expr);
			free(node->print.expr);
			break;
		}
		case ASTBlock_NODE_TYPE:{
			for(long i = 0; i < node->block.numberOfNodes; i++) freeASTNode(&node->block.nodes[i]);
			break;
		}
		case ASTLocalVariable_NODE_TYPE:{
			freeASTNode(node->localVar.expr);
			free(node->localVar.expr);
			free(node->localVar.id);
			break;
		}
		case ASTGlobalVariable_NODE_TYPE:{
			freeASTNode(node->globalVar.expr);
			free(node->globalVar.expr);
			free(node->globalVar.id);
			break;
		}
		case ASTLocalAssignment_NODE_TYPE:{
			freeASTNode(node->localAss.expr);
			free(node->localAss.expr);
			//free(node->globalAss.id);
			break;			
		}
		case ASTGlobalAssignment_NODE_TYPE:{
			freeASTNode(node->globalAss.expr);
			free(node->globalAss.expr);
			//free(node->globalAss.id);
			break;
		}
		default : {
			printf("Cannot free node of given type"); 
			printf("%d", type);
			exit(1);
		}
	}
	//free(node);
}

void freeAST(AST* ast){
	for(long i = 0; i < ast->numberOfNodes; i++) freeASTNode(&ast->nodes[i]);
	free(ast);
}

void freeParser(Parser* parser){
	freeTable(parser->globalVariables);
	freeTreeNoKeys(parser->strings);
	freeAST(parser->ast);
	free(parser->scopes);
	free(parser);
}

AST* newAST(){
	AST* ast = (AST*)malloc(sizeof(AST));
	ast->nodes = (ASTNode*) malloc(sizeof(ASTNode));
	ast->cappacity = 1;
	ast->numberOfNodes = 0;
	ast->stringCount = 0;
	return ast;
}

Parser* newParser(){
	Parser* parser = (Parser*) malloc(sizeof(Parser));
	parser->ast = newAST();
		
	parser->globalVariables = newTable(10);
	parser->globalCount = 0;

	parser->scopes = newScopeChain();
	parser->scopeDepth = 0;

	parser->strings = newTree();
	parser->stringCount = 0;
	
}