#include "Lexing.h"

/*Initialization Functions*/
TokenArray* newTokenArray(){
	TokenArray* arr = (TokenArray*)malloc(sizeof(TokenArray));
	arr->tokens = (Token*)malloc(sizeof(Token));
	arr->cappacity = 1;
	arr->tokenCount = 0;
	return arr;
}

Lexer* newLexer(){
	Lexer* l = (Lexer*)malloc(sizeof(Lexer));
	l->tokens = newTokenArray();
	l->trie = newTrie();
	l->invalidRead = false;
	return l;
}

Token newToken(TokenType type, char* value, long size, long lineNumber){
	Token t = {type, value, size, lineNumber};
	return t;
}

/*Reading From File*/
long getSizeOfFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    fclose(file);
    return file_size;
}

//https://stackoverflow.com/questions/2029103/correct-way-to-read-a-text-file-into-a-buffer-in-c
char* readFile(char* path){
	char *source = NULL;
	FILE *fp = fopen(path, "r");
	if (fp != NULL) {
		if (fseek(fp, 0L, SEEK_END) == 0) {
			long bufsize = ftell(fp);
			if (bufsize == -1) return NULL;
			source = malloc(sizeof(char) * (bufsize + 1));
			if (fseek(fp, 0L, SEEK_SET) != 0) return NULL;
			size_t newLen = fread(source, sizeof(char), bufsize, fp);
			if ( ferror( fp ) != 0 ) {
				fputs("Error reading file", stderr);
			} else {
				source[newLen++] = '\0';
			}
		}
		fclose(fp);
	}
	return source;
}

/*Resizing*/
Token* resizeTokens(Token* tokens, long size){
	Token* newTokens = (Token*)realloc(tokens, 2*size*sizeof(Token));
	if(newTokens == NULL) return NULL;
	return newTokens;
}

int emitToken(TokenType type, char* value, long size, TokenArray* tokenArr, long line){
	tokenArr->tokens[tokenArr->tokenCount] = newToken(type, value, size, line);
	tokenArr->tokenCount++;
	if(tokenArr->tokenCount == tokenArr->cappacity){
		tokenArr->tokens = resizeTokens(tokenArr->tokens, tokenArr->cappacity);
		if(tokenArr->tokens == NULL) return -1;
		tokenArr->cappacity *= 2;
	}
	return 1;
}

char* resizeString(char* str, long length){
	char* newStr = (char*)realloc(str, 2*length*sizeof(char));
	if(newStr == NULL) NULL;
	return newStr;
}

char* addChar(char* text, char* str, int* index, int* cappacity){
	str[*index] = *text;
	(*index) += 1;
	if(*index == *cappacity){
		str = resizeString(str, *cappacity);
		if(str == NULL) return NULL;
		(*cappacity) *= 2;
	}
	return str;
}

/*Printing*/
char* printToken(Token* token){
	switch(token->type){
		case SYM_TOKEN: return printf("SYMBOL["),printf("%s", token->value),printf("]"),NULL;
		case WORD_TOKEN: return printf("WORD["),printf("%s", token->value),printf("]"),NULL;
		case STR_A_TOKEN : return printf("STRING["),printf("%s", token->value),printf("]"),NULL;
		case INT_TOKEN: return printf("INT["),printf("%s", token->value),printf("]"),NULL;
		case FLOAT_TOKEN: return printf("FLOAT["), printf("%s", token->value),printf("]"),NULL;
		case SEMI_COLON_TOKEN: return printf(";"),NULL;
		case END_LINE_TOKEN: return printf("END-LINE"),NULL;
		case END_TOKEN: return printf("End-Token"),NULL;
		default : return newString("Attempting to Print Unidentified Token");
	}
}

char* printTokens(TokenArray* arr){
	printf("Tokens: ");
	for(int i = 0; i < arr->tokenCount; i++){
		char* res = printToken(&(arr->tokens[i]));
		if(res != NULL) return res;
		if(i != arr->tokenCount-1) printf(", ");
	}
	printf("\n\n");
	return NULL;
}

/*Helper Functions*/
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

bool isSymChar(char c){
	return !(isAlnum(c)) 
	&& c != '\n'
	&& c != ' '
	&& c != '\r'
	&& c != '\t'
	&& c != ';'
	&& c != '\0'; 
}

TrieNode* findTrieNode(TrieNode* n, char c){
	while(n != NULL && n->letter != c) n = n->next;
	return n;
}

/*Tokenization Functions*/
int tokenizeString(Lexer* l, char* text, long textSize, TokenType type, int lineNumber){
	char* tempText = text;
	text++;
	char* str = (char*)malloc(sizeof(char));
	int* cappacity = (int*)malloc(sizeof(int));
	int* index = (int*)malloc(sizeof(int));
	*cappacity = 1;
	*index = 0;
	while(*text != '\"'){
		if(addChar(text, str, index, cappacity) == NULL) return -1;
		text++;
		if(*text == '\0') return -2;
	}
	str[*index] = '\0';
	if(emitToken(type, str, *index, l->tokens, lineNumber) == -1) return -1;
	free(cappacity);
	int i = *index;
	free(index);
	return i+2;
}

int tokenizeNumber(Lexer* l, char* text, long textSize, int lineNumber){
	char* newText = text;
	int* cappacity = (int*)malloc(sizeof(int));
	int* index = (int*)malloc(sizeof(int));
	*cappacity = 1;
	*index = 0;
	char* num = (char*)malloc(sizeof(char));
	short numOfDots = 0;
	while((isDigit(*text) || *text == '.') && numOfDots < 2){
		if(*text == '.') numOfDots++;
		if(addChar(text, num, index, cappacity) == NULL) return -1;
		text++;
	}
	num[*index] = '\0';
	if(numOfDots == 1) {if(emitToken(FLOAT_TOKEN, num, *index, l->tokens, lineNumber)==-1) return -1;}
	else if(numOfDots == 0){if(emitToken(INT_TOKEN, num, *index, l->tokens, lineNumber)==-1) return -1;}
	else return -2;
	free(cappacity);
	int i = *index;
	free(index);
	newText += i;
	return i;
}

int tokenizeWord(Lexer* l, char* text, long textSize, int lineNumber){
	char* word = (char*)malloc(sizeof(char));
	int* cappacity = (int*)malloc(sizeof(int));
	int* index = (int*)malloc(sizeof(int));
	*cappacity = 1;
	*index = 0;
	while(isAlnum(*text)) {
		word = addChar(text, word, index, cappacity);
		if(word == NULL) return -1;
		text++;
	}
	word[*index] = '\0';
	if(emitToken(WORD_TOKEN, word, *index, l->tokens, lineNumber)==-1) return -1;
	free(cappacity);
	int i = *index;
	free(index);
	return i;
}

int tokenizeSizedSymbolString(Lexer* l, char* text, long textSize, int lineNumber, int length){
	char* symString = (char*)malloc(sizeof(char));
	int* cappacity = (int*)malloc(sizeof(int));
	int* index = (int*)malloc(sizeof(int));
	*cappacity = 1;
	*index = 0;
	for(int i = 0; i < length; i++) {
		symString = addChar(text, symString, index, cappacity);
		if(symString == NULL) return -1;
		text++;
	}
	symString[*index] = '\0';
	if(emitToken(SYM_TOKEN, symString, *index, l->tokens, lineNumber)==-1) return -1;
	free(cappacity);
	int i = *index;
	free(index);
	return i;
}

int tokenizeSymbolString(Lexer* l, char* text, long textSize, int lineNumber){
	char* symString = (char*)malloc(sizeof(char));
	int* cappacity = (int*)malloc(sizeof(int));
	int* index = (int*)malloc(sizeof(int));
	int longestIndex = 0;
	int currentIndex = 0;
	*cappacity = 1;
	*index = 0;
	char* text2 = text;
	TrieNode* n = findTrieNode(l->trie->root->child, *text2);
	while(isSymChar(*text2) && n != NULL) {
		symString = addChar(text2, symString, index, cappacity);
		if(symString == NULL) return -1;
		text2++;
		currentIndex++;
		if(n->validEnd) longestIndex = currentIndex;
		n = findTrieNode(n->child, *text2);
	}
	symString[*index] = '\0';
	if(longestIndex > 0) {
		int i = tokenizeSizedSymbolString(l, text, textSize, lineNumber, longestIndex);
		if(i < 0) return i;
	}
	else return -2;
	free(symString);
	free(cappacity);
	int i = *index;
	free(index);
	return i;
}

char* tokenize(Lexer* l, char* text, long textSize){
	long lineNumber = 1;
	while(*text != '\0'){
		switch(*text){
			case ' ':
			case '\r':
			case '\t':{
				text++;
				break;
			}
			case ';':{
				if(emitToken(SEMI_COLON_TOKEN, NULL, 0, l->tokens, lineNumber) == -1) return newString("Internal Error");
				text++;
				break;
			}
			case '\n':{
				if(emitToken(END_LINE_TOKEN, NULL, 0, l->tokens, lineNumber)==-1) return newString("Internal Error");
				lineNumber++;
				text++;
				break;
			}
			case '\"' :{
				int i = tokenizeString(l, text, textSize, STR_A_TOKEN, lineNumber);
				if(i == -1) return newString("Internal Error");
				else if(i == -2) return newString("Nonterminated String; Missing a \"");
				text += i;
				break;
			}	
			case '\'' :{
				int i = tokenizeString(l, text, textSize, STR_B_TOKEN, lineNumber);
				if(i == -1) return newString("Internal Error");
				else if(i == -2) return newString("Nonterminated String; Missing a \'");
				text += i;
				break;
			}
			default:{
				if(isDigit(*text)){
					int i = tokenizeNumber(l, text, textSize, lineNumber);
					if(i == -1) return newString("Internal Error");
					else if(i==-2) return newString("Too many .\'s");
					text += i;
				}
				else if(isAlnum(*text)){
					int i = tokenizeWord(l, text, textSize, lineNumber);
					if(i == -1) return newString("Internal Error");
					text += i;
				}
				else{
					int i = tokenizeSymbolString(l, text, textSize, lineNumber);
					if(i == -1) return newString("Internal Error");
					else if(i==-2) return newString("Invalid Symbol String");
					text += i;
				}
			}
		}
	}
	if(emitToken(END_TOKEN, NULL, 0, l->tokens, lineNumber)==-1) return newString("Internal Error");
	return NULL;
}

/*Core Lex Method*/
char* lex(Lexer* l, char * path){
	long fileSize = getSizeOfFile(path);
	if(fileSize < 0) return newString("Internal Error");
	char* buffer = readFile(path);
	if(buffer == NULL) return newString("Internal Error");
	char* res = tokenize(l, buffer, fileSize);
	free(buffer);
	return res;
}

/*Freeing*/
char* freeToken(Token* token){
	if(token->value != NULL) free(token->value);
	return newString("Invalid Freeing of Token");
}

char* freeTokens(TokenArray* tokens){
	for(int i = 0; i < tokens->tokenCount; i++) freeToken(&tokens->tokens[i]);
	free(tokens->tokens);
	return NULL;
}

char* freeLexer(Lexer* l){
	freeTrie(l->trie);
	freeTokens(l->tokens);
	free(l->tokens);
	return NULL;
}




/*
FILE* getFileToRead(const char* path){
	FILE* file;
	file = fopen(path, "r");
	if (file == NULL) panic("Failed to open the file.\n");
	return file;
}

long getFileSize(FILE* file){
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	rewind(file);
	return fileSize;
}*/