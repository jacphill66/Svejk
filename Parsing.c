#include "Parsing.h"

void freeType(Type* t){
	if(t != NULL){
		switch(t->kind){
			case Trivial_KIND:{
				free(t);
				break;
			}
			default:{
				printf("Invalid Kind\n");
				exit(1);
				break;
			}
		}
	}
}

TrivialType getTrivialType(Type* t){
	if(t->kind == Trivial_KIND) return t->trivial;
	else return 0;
}

Type* newTrivialType(TrivialType type){
	Type* t = (Type*)malloc(sizeof(Type));
	t->kind = Trivial_KIND;
	t->trivial = type;
	return t;
}

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
		case ASTID_NODE_TYPE:{
			printf("ID[%s]", node->id.id);
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
		case ASTVariable_NODE_TYPE:{
			printf("Variable[");
			printf("Id:%s, Expression:", node->var.id);
			printASTNode(ast, node->var.expr);
			printf("]");
			break;
		}
		case ASTAssignment_NODE_TYPE:{
			printf("Assignment[");
			printf("Id:%s, Expression:", node->ass.id);
			printASTNode(ast, node->ass.expr);
			printf("]");
			break;
		}
		case ASTBlock_NODE_TYPE:{
			printf("Block[\n");
			for(int i = 0; i < node->block.numberOfNodes; i++) {
				printASTNode(ast, &node->block.nodes[i]);
				printf("\n");
			}
			printf("]\n");
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
					/*// work this out...
					printf(" in ");
					printf("COLLECTION=[");
					printASTNode(ast, node->loop.n2);
					printf("] ");*/
				}
				else{
					printf("NODE2=[");
					printASTNode(ast, node->loop.n2);
					printf("] ");
					if(node->loop.n3 != NULL){
						printf("NODE3=[");
						printASTNode(ast, node->loop.n3);
						printf("]");
					}
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
		case ASTIf_NODE_TYPE:{
			printf("if ");
			printASTNode(ast, node->ifS.expr);
			printASTNode(ast, node->ifS.s);
			printf("\n");
			if(node->ifS.elseS != NULL) {
				printf("( "); printASTNode(ast, node->ifS.elseS); printf(" )");
			}
			break;
		}
		case ASTElse_NODE_TYPE:{
			printf("else ");
			printASTNode(ast, node->elseS.s);
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
			[IF_TOKEN] = -1,
			[ELSE_TOKEN] = -1,
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
			ASTString str = {t.value, t.line};
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
			ASTID id = {t.value, t.line};
			node->id = id;
			node->type = ASTID_NODE_TYPE;
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

ASTNode* parseLet(TokenArray* tokens, Parser* parser){
	advance(tokens);
	Token t = advance(tokens);
	ASTNode* n = malloc(sizeof(ASTNode));
	ASTVariable var;
	var.line = t.line;
	var.id = t.value;
	int idSize = t.size;
	t = advance(tokens);
	bool typed = false;
	if(t.type == COLON_TOKEN){
		t = advance(tokens);
		switch (t.type){
			case I32_TOKEN:{
				var.type = newTrivialType(I32_TYPE);
				break;
			}
			case F32_TOKEN:{
				var.type = newTrivialType(F32_TYPE);
				break;
			}
			case BOOL_TOKEN:{
				var.type = newTrivialType(BOOL_TYPE);
				break;
			}
			case STR_TOKEN:{
				var.type = newTrivialType(STR_TYPE);
				break;
			}
			default:{
				var.type = NULL;
				printf("Invalid Type Anotation\n");
				exit(1);
			}
		}
		typed = true;
		advance(tokens);
	}
	ASTNode* expr = {split(parser, tokens, 0)};
	var.expr = expr;
	if(!typed) var.type = newTrivialType(INFERRED_TYPE);
	n->type = ASTVariable_NODE_TYPE;
	n->var = var;
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
	advance(tokens);
	advance(tokens);
	ASTNode* expr = split(parser, tokens, 0);
	ASTAssignment ass = {id, expr, t.line};
	node->ass = ass;
	node->type = ASTAssignment_NODE_TYPE;
	return node;
}

ASTNode* parseAssignmentOrReference(TokenArray* tokens, Parser* parser){
	if(isAssignmentToken(peek(tokens).type)) return parseAssignment(tokens, parser);
	return parseExpression(tokens, parser);
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
	while(tokens->tokens->type != RC_BRACKET_TOKEN){
		parseLocal(parser, b, tokens);
	}
	advance(tokens);
	return b;
}

ASTNode* parseBlockOrTable(TokenArray* tokens, Parser* p){
	//parse a statement. if it is an expression, extract it, parse a dictionary
	return parseBlockExpression(tokens, p);
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

ASTNode* parseElse(TokenArray* tokens, Parser* p){
	ASTNode* elseS = (ASTNode*)malloc(sizeof(ASTNode));
	elseS->type = ASTElse_NODE_TYPE;
	Token t = advance(tokens);
	elseS->elseS.line = t.line;
	elseS->elseS.s = parseStatement(p, tokens, NULL);
	return elseS;
}

ASTNode* parseIf(TokenArray* tokens, Parser* p){
	ASTNode* ifS = (ASTNode*)malloc(sizeof(ASTNode));
	ifS->type = ASTIf_NODE_TYPE;
	Token t = advance(tokens);
	ifS->ifS.line = t.line;
	ifS->ifS.expr = split(p, tokens, 0);
	ifS->ifS.s = parseStatement(p, tokens, NULL);
	if(tokens->tokens->type == END_LINE_TOKEN) advance(tokens);
	if(tokens->tokens->type == ELSE_TOKEN){
		ifS->ifS.elseS = parseElse(tokens, p);
	}
	else {
		ifS->ifS.elseS = NULL;
	}
	return ifS;
}

ASTNode* parseFor(TokenArray* tokens, Parser* parser){
	Token t = advance(tokens);
	ASTNode* b = newBlock();
	ASTNode* loop = newLoop();
	loop->loop.line = t.line;
	if((tokens->tokens->type != LC_BRACKET_TOKEN)&&(tokens->tokens->type != LS_BRACKET_TOKEN)){
		//split(parser, tokens, 0);
		loop->loop.n1 = parseStatement(parser, tokens, b);
		if(tokens->tokens->type == IN_TOKEN){
			advance(tokens);
			ASTNode* expr = parseExpression(tokens, parser);
			loop->loop.n2 = expr;
		}
		else if((tokens->tokens->type != LS_BRACKET_TOKEN)&&(tokens->tokens->type != LC_BRACKET_TOKEN)){
			advance(tokens);
			loop->loop.n2 = parseStatement(parser, tokens, b);
			advance(tokens);
			loop->loop.n3 = parseStatement(parser, tokens, b);
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

ASTNode* parseStatement(Parser* parser, TokenArray* tokens, ASTNode* b){
	switch(tokens->tokens->type){
		case I32_VAL_TOKEN : 
		case F32_VAL_TOKEN :
		case STR_VAL_TOKEN :
		case TRUE_VAL_TOKEN :
		case FALSE_VAL_TOKEN :
		case PLUS_OP_TOKEN :
		case SUB_OP_TOKEN :
		case LPAREN_OP_TOKEN :
		case NOT_OP_TOKEN : {
			return parseExpression(tokens, parser);
		}
		case ID_TOKEN:{
			return parseAssignmentOrReference(tokens, parser);
		}
		case LC_BRACKET_TOKEN:{
			return parseBlockOrTable(tokens, parser);
		}
		case PRINT_TOKEN:{
			return parsePrint(tokens, parser);
		}
		case FOR_TOKEN:{
			return parseFor(tokens, parser);
		}
		case LET_TOKEN:{
			return parseLet(tokens, parser);
		}
		case IF_TOKEN:{
			return parseIf(tokens, parser);
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

void freeASTNode(ASTNode* node, bool typeFreeing){
	ASTNodeType type = node->type;
	switch(node->type){
		case ASTBinaryOP_NODE_TYPE :{
			freeASTNode(node->binaryOP.lhs, typeFreeing);
			freeASTNode(node->binaryOP.rhs, typeFreeing);
			free(node->binaryOP.lhs);
			free(node->binaryOP.rhs);
			if(typeFreeing) freeType(node->binaryOP.t);
			break;
		}
		case ASTString_NODE_TYPE:{
			if(typeFreeing) freeType(node->str.t);
			break;
		}
		case ASTUnaryOP_NODE_TYPE : {
			freeASTNode(node->unaryOP.opperand, typeFreeing);
			free(node->unaryOP.opperand);
			if(typeFreeing) freeType(node->unaryOP.t);
			break;
		}
		case ASTCallOP_NODE_TYPE:{
			freeASTNode(node->callOP.opperand, typeFreeing);
			free(node->callOP.opperand);
			if(typeFreeing) freeType(node->callOP.t);
			break;
		}
		case ASTID_NODE_TYPE:{
			if(typeFreeing) freeType(node->id.t);
			break;
		}
		case ASTValue_NODE_TYPE:{
			if(typeFreeing) freeType(node->value.t);
			break;
		}
		case ASTExpression_NODE_TYPE:{
			freeASTNode(node->expr.expr, typeFreeing);
			free(node->expr.expr);
			if(typeFreeing) freeType(node->expr.t);
			break;
		}
		case ASTPrint_NODE_TYPE:{
			freeASTNode(node->print.expr, typeFreeing);
			free(node->print.expr);
			if(typeFreeing) freeType(node->print.t);
			break;
		}
		case ASTBlock_NODE_TYPE:{
			for(long i = 0; i < node->block.numberOfNodes; i++)	freeASTNode(&node->block.nodes[i], typeFreeing);
			if(typeFreeing) freeType(node->block.t);
			break;
		}
		case ASTVariable_NODE_TYPE:{
			freeASTNode(node->var.expr, typeFreeing);
			free(node->var.expr);
			if(typeFreeing) freeType(node->var.t);
			break;
		}
		case ASTAssignment_NODE_TYPE:{
			freeASTNode(node->ass.expr, typeFreeing);
			free(node->ass.expr);
			if(typeFreeing) freeType(node->ass.t);
			break;			
		}
		case ASTForLoop_NODE_TYPE:{
			if(node->loop.n1 != NULL) {
				freeASTNode(node->loop.n1, typeFreeing);
				free(node->loop.n1);
			}
			if(node->loop.n2 != NULL){
				freeASTNode(node->loop.n2, typeFreeing);
				free(node->loop.n2);
			}
			if(node->loop.n3 != NULL) {
				freeASTNode(node->loop.n3, typeFreeing);
				free(node->loop.n3);
			}
			if(node->loop.max != NULL) {
				freeASTNode(node->loop.min, typeFreeing);
				free(node->loop.min);
			}
			if(node->loop.max != NULL) {
				freeASTNode(node->loop.max, typeFreeing);
			}
			freeASTNode(node->loop.b, typeFreeing);
			if(typeFreeing) freeType(node->loop.t);
			break;
		}
		case ASTIf_NODE_TYPE:{
			freeASTNode(node->ifS.expr, typeFreeing);
			free(node->ifS.expr);
			freeASTNode(node->ifS.s, typeFreeing);
			free(node->ifS.s);
			if(node->ifS.elseS != NULL){
				freeASTNode(node->ifS.elseS, typeFreeing);
				free(node->ifS.elseS);
			}
			if(typeFreeing) freeType(node->ifS.t);
			break;
		}
		case ASTElse_NODE_TYPE:{
			freeASTNode(node->elseS.s, typeFreeing);
			free(node->elseS.s);
			if(typeFreeing) freeType(node->elseS.t);
			break;
		}
		case ASTLoop_NODE_TYPE:{
			freeASTNode(node->simpleLoop.expr, typeFreeing);
			free(node->simpleLoop.expr);
			freeASTNode(node->simpleLoop.block, typeFreeing);
			free(node->simpleLoop.block);
			if(typeFreeing) freeType(node->simpleLoop.t);
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

void freeAST(AST* ast, bool freeType){
	for(long i = 0; i < ast->numberOfNodes; i++) freeASTNode(&ast->nodes[i], freeType);
	free(ast);
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