#include "Parsing.h"

/*
Fix postfix
Add parens
Add more operators


Create compiler
Create VM

*/


void printOP(OPCode op){
	//prints binary, prefix and postfix ops
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
			printf("%");
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

void printASTNode(ASTNode* node){
	//add a boolean to optionally print parens
	ASTNodeType type = node->type;
	switch(type){
		case ASTBinaryOP_NODE_TYPE :{
			printf("(");
			printASTNode(node->binaryOP.lhs);
			printOP(node->binaryOP.op);
			printASTNode(node->binaryOP.rhs);
			printf(")");
			break;
		}
		case ASTUnaryOP_NODE_TYPE : {
			printf("(");
			if(node->unaryOP.op == FACT_OP){
				printASTNode(node->unaryOP.opperand);
				printOP(node->unaryOP.op);
			}
			else{
				printOP(node->unaryOP.op);
				printASTNode(node->unaryOP.opperand);
			}
			printf(")");
			break;
		}
		case ASTCallOP_NODE_TYPE:{
			printASTNode(node->callOP.opperand);
			printf("()");
			break;
		}
		case ASTID_NODE_TYPE:{
			printf(node->id.id);
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
			printASTNode(node->expr.expr);
			printf("]");
			break;
		}
		case ASTPrint_NODE_TYPE:{
			printf("Print[");
			printASTNode(node->print.expr);
			printf("]");
			break;
		}
		default : {
			printf("Cannot print node of given type 2"); 
			printf("%d",type);
			exit(1);
		}
	}
}

void printAST(AST* ast){
	//printASTNode(&ast->nodes[0]);
	printf("AST: ");
	for(long i = 0; i < ast->numberOfNodes; i++){
		printASTNode(&ast->nodes[i]);
		printf("\n");
	}
	printf("\n");
}

ASTNode* resizeNodes(ASTNode* nodes, long cappacity){
	ASTNode* newNodes = (ASTNode*)realloc(nodes, 2*cappacity*sizeof(ASTNode));
	if(newNodes == NULL){
		printf("could not resize AST-Nodes!\n");
		free(newNodes);
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

ASTNode* literal(Token t){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	switch(t.type){
		case I32_VAL_TOKEN : {
			node->type = ASTValue_NODE_TYPE;
			Value v;
			v.type = I32_VAL;
			v.i32 = atoi(t.value);
			ASTValue val = {v};
			node->value = val;
			return node;
		}
		case F32_VAL_TOKEN:{
			node->type = ASTValue_NODE_TYPE;
			Value v;
			v.type = F32_VAL;
			v.f32 = atof(t.value);
			ASTValue val = {v};
			node->value = val;
			return node;
		}
		case STR_VAL_TOKEN:{
			//change this
			node->type = ASTValue_NODE_TYPE;
			Value v;
			v.type = F32_VAL;
			v.f32 = atof(t.value);
			ASTValue val = {v};
			node->value = val;
			return node;
		}
		case TRUE_VAL_TOKEN:{
			node->type = ASTValue_NODE_TYPE;
			Value v;
			v.type = BOOL_VAL;
			v.boolean = true;
			ASTValue val = {v};
			node->value = val;
			return node;	
		}
		case FALSE_VAL_TOKEN:{
			node->type = ASTValue_NODE_TYPE;
			Value v;
			v.type = BOOL_VAL;
			v.boolean = false;
			ASTValue val = {v};
			node->value = val;
			return node;	
		}
		case ID_TOKEN:{
			node->type = ASTID_NODE_TYPE;
			ASTID id = {t.value};
			node->id = id;
			return node;
		}
	}
}

ASTNode* prefix(TokenArray* tokens){
	Token t = advance(tokens);
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	if(t.type == LPAREN_OP_TOKEN){
		ASTNode* result = split(tokens, 0);
		advance(tokens);//should be ")"
		return result;
	}
	switch(t.type){
		case PLUS_OP_TOKEN:{
			node->type = ASTUnaryOP_NODE_TYPE;
			ASTUnaryOP uOP = {UNARY_PLUS_OP, split(tokens, getPrefixPrec(t.type)+1)};
			node->unaryOP = uOP;
			return node;
		}
		case SUB_OP_TOKEN:{
			node->type = ASTUnaryOP_NODE_TYPE;		
			ASTUnaryOP uOP = {UNARY_MINUS_OP, split(tokens, getPrefixPrec(t.type)+1)};
			node->unaryOP = uOP;
			return node;
		}
		default:{
			return literal(t);
			/*printf("unidentified unary op");
			exit(1);*/
		}
	}
}

ASTNode* binaryOP(TokenType op, ASTNode* lhs, TokenArray* tokens){
	ASTNode* n = (ASTNode*) malloc(sizeof(ASTNode));
	n->type = ASTBinaryOP_NODE_TYPE;
	ASTBinaryOP binOP;
	switch(op){
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
		case GOE_OP_TOKEN : 	{
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
	if(op == EXP_OP_TOKEN){
		binOP.rhs = split(tokens, getOtherfixPrecedence(op));
	}
	else{
		binOP.rhs = split(tokens, getOtherfixPrecedence(op)+1);
	}
	n->binaryOP = binOP;
	//printf("n type: %d", n->type);
}

ASTNode* otherfix(ASTNode* node, TokenArray* tokens){
	Token token = advance(tokens);
	switch(token.type){		
		case LPAREN_OP_TOKEN : {
			advance(tokens);//for the ")" change this later
			ASTNode* n = (ASTNode*) malloc(sizeof(ASTNode));
			ASTCallOP cOP = {node}; 
			n->callOP = cOP;
			n->type = ASTCallOP_NODE_TYPE;
			return n;
		}
		case FACT_OP_TOKEN: {
			ASTNode* n = (ASTNode*) malloc(sizeof(ASTNode));
			ASTUnaryOP uOP = {FACT_OP, node}; 
			n->unaryOP = uOP;
			n->type = ASTUnaryOP_NODE_TYPE;
			return n;
		}
		default :{
			return binaryOP(token.type, node, tokens);
		}
	}
}

ASTNode* split(TokenArray* tokens, int prec){
	//change to prefix
	//ASTNode* lhs = literal(tokens);
	ASTNode* lhs = prefix(tokens);
	while(prec <= getOtherfixPrecedence(tokens->tokens->type)){
		lhs = otherfix(lhs, tokens);
		//change to other fix
		//lhs = binaryOP(advance(tokens).type, lhs, tokens);
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

ASTNode* parseExpression(TokenArray* tokens, AST* ast){
	ASTExpression expr = {split(tokens, 0)};
	ASTNode* n = malloc(sizeof(ASTNode));
	n->type = ASTExpression_NODE_TYPE;
	n->expr = expr;
	emitNode(n, ast);
}

ASTNode* parsePrint(TokenArray* tokens, AST* ast){
	advance(tokens);
	ASTNode* n = malloc(sizeof(ASTNode));
	ASTExpression expr = {split(tokens, 0)};
	n->type = ASTPrint_NODE_TYPE;
	n->expr = expr;
	emitNode(n, ast);
}

AST parse(TokenArray* tokens){
	//rework this
	ASTNode* nodes = (ASTNode*) malloc(sizeof(ASTNode));
	AST ast = {nodes, 1, 0};
	Token* tokens2 = tokens->tokens;
	while(tokens->tokens->type != END_TOKEN){
		switch(tokens->tokens->type){
			case I32_VAL_TOKEN : {
				//nodes = split(tokens, UNARY_PREC);
				//nodes = split(tokens, 0);
				//emitNode(split(tokens, 0), &ast);
				parseExpression(tokens, &ast);
				break;
			}
			case F32_VAL_TOKEN : {
				//emitNode(split(tokens, 0), &ast);
				parseExpression(tokens, &ast);
				break;
			}	
			case STR_VAL_TOKEN : {
				//emitNode(split(tokens, 0), &ast);
				parseExpression(tokens, &ast);
				break;
			}			
			case TRUE_VAL_TOKEN : {
				//emitNode(split(tokens, 0), &ast);
				parseExpression(tokens, &ast);
				break;
			}		
			case FALSE_VAL_TOKEN : {
				//emitNode(split(tokens, 0), &ast);
				parseExpression(tokens, &ast);
				break;
			}		
			case ID_TOKEN:{
				//emitNode(split(tokens, 0), &ast);
				parseExpression(tokens, &ast);
				break;		
			}
			case PLUS_OP_TOKEN : {
				//emitNode(split(tokens, 0), &ast);
				parseExpression(tokens, &ast);
				break;
			}		
			case SUB_OP_TOKEN : {
				//emitNode(split(tokens, 0), &ast);
				parseExpression(tokens, &ast);
				break;
			}	
			case LPAREN_OP_TOKEN : {
				//emitNode(split(tokens, 0), &ast);
				parseExpression(tokens, &ast);
				break;
			}
			case PRINT_TOKEN:{
				parsePrint(tokens, &ast);
				break;
			}
			case END_LINE_TOKEN:{
				advance(tokens);
				break;
			}
			default:{
				printf("Unparsable token");
				exit(1);
			}
		}
		//+
	}
	//printf("%d", tokens->tokens - tokens2);
	//free(tokens->tokens);
	tokens->tokens = tokens2;
	//printf("ast->nodes[0].type);
	//tokens->tokens-=tokens->tokenCount;
	//printf(tokens->tokens->value);
	return ast;
}


void freeASTNode(ASTNode* node){
	ASTNodeType type = node->type;
	switch(type){
		case ASTBinaryOP_NODE_TYPE :{
			freeASTNode(node->binaryOP.lhs);
			freeASTNode(node->binaryOP.rhs);
			break;
		}
		case ASTUnaryOP_NODE_TYPE : {
			freeASTNode(node->unaryOP.opperand);
			break;
		}
		case ASTCallOP_NODE_TYPE:{
			freeASTNode(node->callOP.opperand);
			break;
		}
		case ASTID_NODE_TYPE:{
			free(node->id.id);
			break;
		}
		case ASTValue_NODE_TYPE:{
			//freed at the bottom, values have no inner pointer
			break;
		}
		case ASTExpression_NODE_TYPE:{
			freeASTNode(node->expr.expr);
			break;
		}
		case ASTPrint_NODE_TYPE:{
			freeASTNode(node->print.expr);
			break;
		}
		
		
		default : {
			printf("Cannot free node of given type"); 
			printf("%d", type);
			exit(1);
		}
	}
	free(node);
}


void freeAST(AST* ast){
	for(long i = 0; i < ast->numberOfNodes; i++){
		freeASTNode(&ast->nodes[i]);
	}
	free(ast);
}

//}

