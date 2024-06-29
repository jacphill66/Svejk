#include "Lexing.h"

FILE* getFileToRead(const char* path){
	FILE* file;
	file = fopen(path, "r");
	if (file == NULL) {
		printf("Failed to open the file.\n");
		exit(1);
	}
	return file;
}

long getFileSize(FILE* file){
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	rewind(file);
	return fileSize;
}

char* newBuffer(FILE* file, long fileSize){
	char* buffer = (char*)malloc(fileSize + 1); // +1 for null-terminator
	if (buffer == NULL) {
		printf("Memory allocation failed.\n");
		fclose(file);
		exit(1);
	}
	return buffer;
}

char* fillBuffer(FILE* file, long fileSize){
	char * buffer = newBuffer(file, fileSize);
	size_t bytesRead = fread(buffer, 1, fileSize, file);

	if (bytesRead != fileSize) {
		printf("Error reading the entire file.\n");
		free(buffer);
		fclose(file);
		exit(1);
	}
	buffer[fileSize] = '\0';
	return buffer;
}

void printToken(Token* token){
	switch(token->type){
		case PLUS_OP_TOKEN:{
			printf("+");
			break;
		}
		case SUB_OP_TOKEN:{
			printf("-");
			break;
		}
		case MULT_OP_TOKEN:{
			printf("*");
			break;
		}
		case DIV_OP_TOKEN:{
			printf("/");
			break;
		}
		case REM_OP_TOKEN:{
			printf("%c", '%');
			break;
		}
		case EXP_OP_TOKEN:{
			printf("^");
			break;
		}
		case FACT_OP_TOKEN : {
			printf("!");
			break;
		}
		case EQUAL_OP_TOKEN:{
			printf("==");
			break;
		}
		case LESS_OP_TOKEN:{
			printf("<");
			break;
		}	
		case GREATER_OP_TOKEN:{
			printf(">");
			break;
		}	
		case LOE_OP_TOKEN:{
			printf("<=");
			break;
		}
		case GOE_OP_TOKEN:{
			printf(">=");
			break;
		}				
		case AND_OP_TOKEN:{
			printf("and");
			break;
		}		
		case OR_OP_TOKEN:{
			printf("or");
			break;
		}	
		case NOT_OP_TOKEN:{
			printf("not");
			break;
		}	
		case TRUE_VAL_TOKEN:{
			printf("true");
			break;		
		}
		case FALSE_VAL_TOKEN :{
			printf("false");
			break;
		}
		case STR_VAL_TOKEN :{
			printf("string[");
			printf("%s", token->value);
			printf("]");
			break;
		}
		case I32_VAL_TOKEN:{
			printf("i32[");
			printf("%s", token->value);
			printf("]");
			break;
		}
		case F32_VAL_TOKEN:{
			printf("f32[");
			printf("%s", token->value);
			printf("]");
			break;
		}
		case ID_TOKEN: {
			printf("ID[");
			printf("%s", token->value);
			printf("]");
			break;
		}
		case LPAREN_OP_TOKEN: {
			printf("(");
			break;
		}
		case RPAREN_OP_TOKEN: {
			printf(")");
			break;
		}
		case LC_BRACKET_TOKEN:{
			printf("{");
			break;
		}
		case RC_BRACKET_TOKEN:{
			printf("}");
			break;
		}
		case LS_BRACKET_TOKEN:{
			printf("[");
			break;
		}
		case RS_BRACKET_TOKEN:{
			printf("]");
			break;
		}
		case PRINT_TOKEN:{
			printf("print");
			break;
		}
		case ASS_TOKEN:{
			printf("=");
			break;
		}
		case LET_TOKEN:{
			printf("let");
			break;
		}
		case FOR_TOKEN:{
			printf("for");
			break;
		}
		case IN_TOKEN:{
			printf("in");
			break;
		}
		case COLON_TOKEN:{
			printf(":");
			break;
		}
		case I32_TOKEN:{
			printf("i32");
			break;
		}
		case F32_TOKEN:{
			printf("f32");
			break;
		}
		case STR_TOKEN:{
			printf("str");
			break;
		}
		case COMMA_TOKEN:{
			printf(",");
			break;
		}
		case END_LINE_TOKEN:{
			printf("End-Line");
			break;
		}
		case END_TOKEN:{
			printf("End-Token");
			break;
		}
		default : {
			printf("Unidentified Token: %d", token->type);
		}
	}
}

void printTokens(TokenArray* arr){
	printf("Tokens: ");
	for(int i = 0; i < arr->tokenCount; i++){
		printToken(&(arr->tokens[i]));
		if(i != arr->tokenCount-1) printf(", ");
	}
	printf("\n\n");
}

Token newToken(TokenType type, char* value, long size, long lineNumber){
	Token t = {type, value, size, lineNumber};
	return t;
}

bool isDigit(char c){
	return '0' <= c && '9' >= c;
}

bool isAl(char c){
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool isAlnum(char c){
	return isAl(c) || isDigit(c) || c == '_';
}

bool match(char* text, const char* str, int strSize){
	return 0==memcmp(text, str, strSize);
}

bool matchAndDelimited(char* text, const char* str, int strSize){
	return match(text, str, strSize) && !isAl(*(text+strSize));
}

Token* resizeTokens(Token* tokens, long size){
	Token* newTokens = (Token*)realloc(tokens, 2*size*sizeof(Token));
	if(newTokens == NULL){
		printf("could not resize tokens!, %ld", 2*size);
		free(tokens);
		free(newTokens);
		exit(1);
	}
	return newTokens;
}

void emitToken(TokenType type, char* value, long size, TokenArray* tokenArr, long line){
	tokenArr->tokens[tokenArr->tokenCount] = newToken(type, value, size, line);
	tokenArr->tokenCount++;
	if(tokenArr->tokenCount == tokenArr->cappacity){
		tokenArr->tokens = resizeTokens(tokenArr->tokens, tokenArr->cappacity);
		tokenArr->cappacity *= 2;
	}
}

char* resizeString(char* str, long length){
	char* newStr = (char*)realloc(str, 2*length*sizeof(char));
	if(newStr == NULL){
		printf("could not resize!\n");
		free(str);
		free(newStr);
		exit(1);
	}
	return newStr;
}

long lexID(TokenArray* tokenArr, char* text, long lineNumber){
	char* id = (char*)malloc(sizeof(char));
	long cappacity = 1;
	long chars = 0;
	while(isAlnum(*text)){
		id[chars] = *text;
		chars++;
		text++;
		if(chars == cappacity){
			id = resizeString(id, cappacity);
			cappacity *= 2;
		}
	}
	id[chars] = '\0';
	chars++;
	emitToken(ID_TOKEN, id, chars, tokenArr, lineNumber);
	return chars-1;
}

TokenArray* initTokenArray(){
	TokenArray* arr = (TokenArray*)malloc(sizeof(TokenArray));
	arr->tokens = (Token*)malloc(sizeof(Token));
	arr->cappacity = 1;
	arr->tokenCount = 0;
	return arr;
}

void tokenize(TokenArray* tokenArr, char* text, long textSize){
	long lineNumber = 1;
	//printf("here");
	//exit(1);
	/*Token* tokens = (Token*)malloc(sizeof(Token));
	long cappacity = 1;
	long numberOfTokens = 0;
	TokenArray tokenArr = {tokens, cappacity, numberOfTokens};*/
	//Token* tokens = tokenArr->tokens;
	while(*text != '\0'){
		switch(*text){
			case ' ':{
				text++;
				break;
			}
			case '\t':{
				text++;
				break;
			}
			case '\n':{
				//emitToken(END_LINE_TOKEN, NULL, 0, tokenArr, lineNumber);
				lineNumber++;
				text++;
				break;
			}
			case '\r':{
				text++;
				break;
			}
			case '+':{
				text++;
				emitToken(PLUS_OP_TOKEN, NULL, 0, tokenArr, lineNumber);
				break;
			}
			case '-':{
				text++;
				emitToken(SUB_OP_TOKEN, NULL, 0, tokenArr, lineNumber);
				break;
			}
			case '*':{
				text++;
				emitToken(MULT_OP_TOKEN, NULL, 0, tokenArr, lineNumber);
				break;
			}
			case '/':{
				text++;
				if(*text == '/'){
					while((*text != '\n') && (*text != '\0')){
						text++;
					}
				}
				else{
					emitToken(DIV_OP_TOKEN, NULL, 0, tokenArr, lineNumber);
				}
				break;
			}
			case '%':{
				text++;
				emitToken(REM_OP_TOKEN, NULL, 0, tokenArr, lineNumber);
				break;
			}
			case '^':{
				text++;
				emitToken(EXP_OP_TOKEN, NULL, 0, tokenArr, lineNumber);
				break;
			}
			case '!':{
				text++;
				emitToken(FACT_OP_TOKEN, NULL, 0, tokenArr, lineNumber);
				break;
			}
			case ',':{
				text++;
				emitToken(COMMA_TOKEN, NULL, 0, tokenArr, lineNumber);
				break;
			}
			case '(':{
				text++;
				emitToken(LPAREN_OP_TOKEN, NULL, 0, tokenArr, lineNumber);
				break;
			}
			case ')':{
				text++;
				emitToken(RPAREN_OP_TOKEN, NULL, 0, tokenArr, lineNumber);
				break;
			}
			case '{':{
				text++;
				emitToken(LC_BRACKET_TOKEN, NULL, 0, tokenArr, lineNumber);
				break;
			}
			case '}':{
				text++;
				emitToken(RC_BRACKET_TOKEN, NULL, 0, tokenArr, lineNumber);
				break;
			}
			case '[':{
				text++;
				emitToken(LS_BRACKET_TOKEN, NULL, 0, tokenArr, lineNumber);
				break;
			}
			case ']':{
				text++;
				emitToken(RS_BRACKET_TOKEN, NULL, 0, tokenArr, lineNumber);
				break;
			}
			case ';':{
				text++;
				emitToken(END_LINE_TOKEN, NULL, 0, tokenArr, lineNumber);
				break;
			}
			case ':':{
				text++;
				emitToken(COLON_TOKEN, NULL, 0, tokenArr, lineNumber);
				break;
			}
			case '=':{
				if(matchAndDelimited(text, "==", 2)){
					text+=2;
					emitToken(EQUAL_OP_TOKEN, NULL, 0, tokenArr, lineNumber);
					break;
				}
				else{
					text++;
					emitToken(ASS_TOKEN, NULL, 0, tokenArr, lineNumber);
					break;
				}
			}
			case '<':{
				if(matchAndDelimited(text, "<=", 2)){
					text+=2;
					emitToken(LOE_OP_TOKEN, NULL, 0, tokenArr, lineNumber);
					break;
				}
				else{
					text++;
					emitToken(LESS_OP_TOKEN, NULL, 0, tokenArr, lineNumber);
					break;
				}
			}
			case '>':{
				if(matchAndDelimited(text, ">=", 2)){
					text+=2;
					emitToken(GOE_OP_TOKEN, NULL, 0, tokenArr, lineNumber);
					break;
				}
				else{
					text++;
					emitToken(GREATER_OP_TOKEN, NULL, 0, tokenArr, lineNumber);
					break;
				}
			}	
			case 'a' :{
				if(matchAndDelimited(text, "and", 3)){
					text+=3;
					emitToken(AND_OP_TOKEN, NULL, 0, tokenArr, lineNumber);
					break;
				}
				else{
					text += lexID(tokenArr, text, lineNumber);
					break;
				}
			}
			case 'b':{
				if(matchAndDelimited(text, "bool", 4)){
					text+=4;
					emitToken(BOOL_TOKEN, NULL, 0, tokenArr, lineNumber);
					break;
				}
				else{
					text += lexID(tokenArr, text, lineNumber);
					break;
				}				
			}
			case 'f' :{
				if(matchAndDelimited(text, "false", 5)){
					text+=5;
					emitToken(FALSE_VAL_TOKEN, NULL, 0, tokenArr, lineNumber);
					break;
				}
				else if(matchAndDelimited(text, "f32", 3)){
					text+=3;
					emitToken(F32_TOKEN, NULL, 0, tokenArr, lineNumber);
					break;
				}
				else if(matchAndDelimited(text, "for", 3)){
					text+=3;
					emitToken(FOR_TOKEN, NULL, 0, tokenArr, lineNumber);
					break;
				}
				else{
					text += lexID(tokenArr, text, lineNumber);
					break;
				}
			}
			case 'n' :{
				if(matchAndDelimited(text, "not", 3)){
					text+=3;
					emitToken(NOT_OP_TOKEN, NULL, 0, tokenArr, lineNumber);
					break;
				}
				else{
					text += lexID(tokenArr, text, lineNumber);
					break;
				}
				
			}
			case 'o' :{
				if(matchAndDelimited(text, "or", 2)){
					text+=2;
					emitToken(OR_OP_TOKEN, NULL, 0, tokenArr, lineNumber);
					break;
				}
				else{
					text += lexID(tokenArr, text, lineNumber);
					break;
				}
			}
			case 'p' :{
				if(matchAndDelimited(text, "print", 5)){
					text+=5;
					emitToken(PRINT_TOKEN, NULL, 0, tokenArr, lineNumber);
					break;
				}
				else{
					text += lexID(tokenArr, text, lineNumber);
					break;
				}
			}
			case 'l' :{
				if(matchAndDelimited(text, "let", 3)){
					text+=3;
					emitToken(LET_TOKEN, NULL, 0, tokenArr, lineNumber);
					break;
				}
				else{
					text += lexID(tokenArr, text, lineNumber);
					break;
				}
			}		
			case 't' :{
				if(matchAndDelimited(text, "true", 4)){
					text+=4;
					emitToken(TRUE_VAL_TOKEN, NULL, 0, tokenArr, lineNumber);
					break;
				}
				else{
					text += lexID(tokenArr, text, lineNumber);
					break;
				}
			}			
			case 'i' :{
				if(matchAndDelimited(text, "i32", 3)){
					text+=3;
					emitToken(I32_TOKEN, NULL, 0, tokenArr, lineNumber);
					break;
				}
				else if(matchAndDelimited(text, "in", 2)){
					text+=2;
					emitToken(IN_TOKEN, NULL, 0, tokenArr, lineNumber);
					break;
				}
				else{
					text += lexID(tokenArr, text, lineNumber);
					break;
				}
			}	
			case 's' :{
				if(matchAndDelimited(text, "str", 3)){
					text+=3;
					emitToken(STR_TOKEN, NULL, 0, tokenArr, lineNumber);
					break;
				}
				else{
					text += lexID(tokenArr, text, lineNumber);
					break;
				}
			}
			case '\"' :{
				text++;//"
				char* str = (char*)malloc(sizeof(char));
				long cappacity = 1;
				long chars = 0;
				while(*text != '\"'){
					str[chars] = *text;
					chars++;
					text++;
					if(chars == cappacity){
						str = resizeString(str, cappacity);
						cappacity *= 2;
					}
					if(*text == '\0'){
						printf("Non-terminating string; missing a: \" ");
						exit(1);
					}
				}
				str[chars] = '\0';
				chars++;
				text++;//"
				emitToken(STR_VAL_TOKEN, str, chars, tokenArr, lineNumber);
				break;
			}	
			case '\'' :{
				text++;//"
				char* str = (char*)malloc(sizeof(char));
				long cappacity = 1;
				long chars = 0;
				while(*text != '\''){
					str[chars] = *text;
					chars++;
					text++;
					if(chars == cappacity){
						str = resizeString(str, cappacity);
						cappacity *= 2;
					}
					if(*text == '\0'){
						printf("Non-terminating string; missing a: \" ");
						exit(1);
					}
				}
				str[chars] = '\0';
				chars++;
				text++;//"
				emitToken(IMUT_STR_VAL_TOKEN, str, chars, tokenArr, lineNumber);
				break;
			}
			default : {
				if(isDigit(*text)){
					long cappacity = 1;
					long digits = 0;
					char* num = (char*)malloc(sizeof(char));
					short numOfDots = 0;
					while((isDigit(*text) || *text == '.') && numOfDots < 2){
						if(*text == '.') numOfDots++;
						num[digits] = *text;
						digits++;
						text++;
						if(cappacity == digits){
							num = resizeString(num, cappacity);
							cappacity *= 2;
						}
					}
					num[digits] = '\0';
					digits++;
					if(numOfDots > 0){
						emitToken(F32_VAL_TOKEN, num, digits, tokenArr, lineNumber);
					}
					else{
						emitToken(I32_VAL_TOKEN, num, digits, tokenArr, lineNumber);
					}
				}
				else if(isAl(*text)){
					char* id = (char*)malloc(sizeof(char));
					long cappacity = 1;
					long chars = 0;
					while(isAlnum(*text)){
						id[chars] = *text;
						chars++;
						text++;
						if(chars == cappacity){
							id = resizeString(id, cappacity);
							cappacity *= 2;
						}
					}
					id[chars] = '\0';
					chars++;
					emitToken(ID_TOKEN, id, chars, tokenArr, lineNumber);
				}
				else{
					printf("unlexable token\n");
					printf("x");
					exit(1);
				}
			}
		}
	}
	//printToken(&(tokenArr->tokens[0]));

	//tokenArr->tokens = tokens;
	//printToken(&(tokenArr->tokens[0]));
	emitToken(END_TOKEN, NULL, 0, tokenArr, lineNumber);
}

//https://stackoverflow.com/questions/2029103/correct-way-to-read-a-text-file-into-a-buffer-in-c
char* readFile(char* path){
	char *source = NULL;
	FILE *fp = fopen(path, "r");
	if (fp != NULL) {
		/* Go to the end of the file. */
		if (fseek(fp, 0L, SEEK_END) == 0) {
			/* Get the size of the file. */
			long bufsize = ftell(fp);
			if (bufsize == -1) { /* Error */ }

			/* Allocate our buffer to that size. */
			source = malloc(sizeof(char) * (bufsize + 1));

			/* Go back to the start of the file. */
			if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

			/* Read the entire file into memory. */
			size_t newLen = fread(source, sizeof(char), bufsize, fp);
			if ( ferror( fp ) != 0 ) {
				fputs("Error reading file", stderr);
			} else {
				source[newLen++] = '\0'; /* Just to be safe. */
			}
		}
		fclose(fp);
	}
	return source;
}

//Chat-GPT baby!
long getSizeOfFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return -1; // Return -1 to indicate an error
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);

    rewind(file); // Reset the file position to the beginning

    fclose(file);

    return file_size;
}

void lex(TokenArray* tokens, char * path){
	/*FILE* file = getFileToRead(path);
	long fileSize = getFileSize(file);
	printf("%d", fileSize);
	char* buffer = fillBuffer(file, fileSize);*/
	long fileSize = getSizeOfFile(path);
	char* buffer = readFile(path);
	tokenize(tokens, buffer, fileSize);
	//fclose(file);
	free(buffer);
}

void freeTokens(TokenArray* tokens){
	//Values are used by the parser - maybe copy them?
	//for(int i = 0; i < tokens->tokenCount; i++) if(tokens->tokens[i].value != NULL) free(tokens->tokens[i].value);
	free(tokens->tokens);
	free(tokens);
}