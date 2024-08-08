#include "Lexing.h"


TrieNode* newTrieNode(char c, bool validEnd){
	TrieNode* n = (TrieNode*)malloc(sizeof(TrieNode));
	n->letter = c;
	n->validEnd = NULL;
	n->next = NULL;
	n->child = NULL;
	return n;
}

Trie* newTrie(){
	Trie* t = (Trie*)malloc(sizeof(Trie));
	t->root = NULL;
	return t;
}

TokenArray* initTokenArray(){
	TokenArray* arr = (TokenArray*)malloc(sizeof(TokenArray));
	arr->tokens = (Token*)malloc(sizeof(Token));
	arr->trie = newTrie();
	arr->cappacity = 1;
	arr->tokenCount = 0;
	return arr;
}

TrieNode* addRest(char* str, int index){
	//if and when NULL is hit, this adds the rest of a string
	TrieNode* n = newTrieNode(str[index], false);
	int count = index+1;
	TrieNode* t = n;
	while(count < strlen(str)){
		t->child = newTrieNode(str[count], false);
		t = t->child;
		count += 1;
	}
	t->validEnd = true;
	return n;
}

void printDown(TrieNode* n){
	while(n != NULL){
		printf("%c, Next:", n->letter);
		if(n->next == NULL) printf("NULL");
		else printf("%c", n->next->letter);
		printf(", Valid End: ");
		if(n->validEnd) printf("True\n");
		else printf("False\n");
		//if(n->next != NULL) printf("Error!");
		n = n->child;
	}
}

bool searchForStringNode(TrieNode* n, char* str, int index){
	if(n == NULL) return false;
	else if((n->letter == str[index]) && (index == strlen(str)-1) && n->validEnd) return true;
	else if(n->letter == str[index]) return searchForStringNode(n->child, str, index+1);
	else if(n->next != NULL) return searchForStringNode(n->next, str, index);
	else return false;
}

bool searchForString(Trie* t, char* str){
	searchForStringNode(t->root->child, str, 0);
}

void printSearch(Trie* t, char* str){
	if(searchForString(t, str)) printf("%s is contained in the trie!\n", str);
	else printf("%s isn't contained in trie! Perhaps you should trie again.\n", str);
}

TrieNode* addStringNode(TrieNode* n, char* str, int index){	
	if(index >= strlen(str)){
		return n;
	}
	else if (n == NULL){
		return addRest(str, index);
	}
	else if(n->letter > str[index]){
		TrieNode* newNode = newTrieNode(str[index], false);
		if(index+1 < strlen(str)) newNode->child = addRest(str, index+1);
		else newNode->validEnd = true;
		newNode->next = n;
		return newNode;
	}
	else{
		TrieNode* t = n;
		while(t->next != NULL && t->next->letter <= str[index]) t = t->next;	
		if(t->letter == str[index]){
			t->child = addStringNode(t->child, str, index+1);
			if(index+1 >= strlen(str)) t->validEnd = true;
			return n;
		}
		else{
			TrieNode* t2 = t->next;
			t->next = newTrieNode(str[index], false);
			if(index+1 < strlen(str)) t->next->child = addRest(str, index+1);
			else t->next->validEnd = true;
			t->next->next = t2;
			return n;
		}
	}
}

void addString(Trie* t, char* str){
	if(t->root == NULL){
		t->root = (TrieNode*)malloc(sizeof(TrieNode));
		t->root->letter = '\0';
		t->root->validEnd = false;
		t->root->next = NULL;
		t->root->child = addRest(str, 0);
	}
	else t->root->child = addStringNode(t->root->child, str, 0);
}

void addToken(char** tokens, int tokenCount, const char* token){
	tokens[tokenCount] = (char*)malloc(sizeof(char)*strlen(token));
	for(int i = 0; i < strlen(token); i++) tokens[tokenCount][i] = token[i];
}

/*
void trieNodePrintString(TrieNode* n){
	//work on this...
	printf("%c", n->letter);
	TrieNode* t = n->child;
	while(t != NULL && t->validEnd != true){
		trieNodePrintString(t);
		t = t->next;
	}
	t->validEnd = false;
}

void triePrintStrings(Trie* t){
	trieNodePrintString(t->root->child);
}

void printTrie(Trie* trie){
	
}
*/

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
		case SYM_TOKEN:{
			printf("SYMBOL[");
			printf("%s", token->value);
			printf("]");
			break;
		}	
		case WORD_TOKEN: {
			printf("WORD[");
			printf("%s", token->value);
			printf("]");
			break;
		}
		case STR_A_TOKEN :{
			printf("STRING[");
			printf("%s", token->value);
			printf("]");
			break;
		}
		case INT_TOKEN:{
			printf("I32[");
			printf("%s", token->value);
			printf("]");
			break;
		}
		case FLOAT_TOKEN:{
			printf("F32[");
			printf("%s", token->value);
			printf("]");
			break;
		}
		case SEMI_COLON_TOKEN:{
			printf(";");
			break;
		}
		case END_LINE_TOKEN:{
			printf("END-LINE");
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

char* addChar(char* text, char* str, int* index, int* cappacity){
	str[*index] = *text;
	(*index) += 1;
	if(*index == *cappacity){
		str = resizeString(str, *cappacity);
		(*cappacity) *= 2;
	}
	return str;
}

int tokenizeString(TokenArray* tokenArr, char* text, long textSize, TokenType type, int lineNumber){
	char* tempText = text;
	text++;
	char* str = (char*)malloc(sizeof(char));
	int* cappacity = (int*)malloc(sizeof(int));
	int* index = (int*)malloc(sizeof(int));
	*cappacity = 1;
	*index = 0;
	while(*text != '\"'){
		addChar(text, str, index, cappacity);
		text++;
		if(*text == '\0'){
			printf("Non-terminating string; missing a: \" ");
			exit(1);
		}
	}
	str[*index] = '\0';
	emitToken(type, str, *index, tokenArr, lineNumber);
	free(cappacity);
	int i = *index;
	free(index);
	return i+2;
}

int tokenizeNumber(TokenArray* tokenArr, char* text, long textSize, int lineNumber){
	char* newText = text;
	int* cappacity = (int*)malloc(sizeof(int));
	int* index = (int*)malloc(sizeof(int));
	*cappacity = 1;
	*index = 0;
	char* num = (char*)malloc(sizeof(char));
	short numOfDots = 0;
	while((isDigit(*text) || *text == '.') && numOfDots < 2){
		if(*text == '.') numOfDots++;
		addChar(text, num, index, cappacity);
		text++;
	}
	num[*index] = '\0';
	if(numOfDots == 1){
		emitToken(FLOAT_TOKEN, num, *index, tokenArr, lineNumber);
	}
	else if(numOfDots == 0){
		emitToken(INT_TOKEN, num, *index, tokenArr, lineNumber);
	}
	else{
		printf("Error lexing number; too many '.'s");
		exit(1);
	}
	free(cappacity);
	int i = *index;
	free(index);
	newText += i;
	return i;
}

int tokenizeWord(TokenArray* tokenArr, char* text, long textSize, int lineNumber){
	char* word = (char*)malloc(sizeof(char));
	int* cappacity = (int*)malloc(sizeof(int));
	int* index = (int*)malloc(sizeof(int));
	*cappacity = 1;
	*index = 0;
	while(isAlnum(*text)) {
		word = addChar(text, word, index, cappacity);
		text++;
	}
	word[*index] = '\0';
	emitToken(WORD_TOKEN, word, *index, tokenArr, lineNumber);
	free(cappacity);
	int i = *index;
	free(index);
	return i;
}

bool isSymChar(char c){
	//probably need to change what a symbol can't be
	return !(isAlnum(c)) 
	&& c != '\n'
	&& c != ' '
	&& c != '\r'
	&& c != '\t'
	&& c != ';'
	&& c != '\0'; 
}

int tokenizeSizedSymbolString(TokenArray* tokenArr, char* text, long textSize, int lineNumber, int length){
	char* symString = (char*)malloc(sizeof(char));
	int* cappacity = (int*)malloc(sizeof(int));
	int* index = (int*)malloc(sizeof(int));
	*cappacity = 1;
	*index = 0;
	for(int i = 0; i < length; i++) {
		symString = addChar(text, symString, index, cappacity);
		text++;
	}
	symString[*index] = '\0';
	emitToken(SYM_TOKEN, symString, *index, tokenArr, lineNumber);
	free(cappacity);
	int i = *index;
	free(index);
	return i;
}

TrieNode* findTrieNode(TrieNode* n, char c){
	while(n != NULL && n->letter != c) n = n->next;
	return n;
}

int tokenizeSymbolString(TokenArray* tokenArr, char* text, long textSize, int lineNumber){
	char* symString = (char*)malloc(sizeof(char));
	int* cappacity = (int*)malloc(sizeof(int));
	int* index = (int*)malloc(sizeof(int));
	int longestIndex = 0;
	int currentIndex = 0;
	*cappacity = 1;
	*index = 0;
	char* text2 = text;
	TrieNode* n = findTrieNode(tokenArr->trie->root->child, *text2);
	while(isSymChar(*text2) && n != NULL) {
		symString = addChar(text2, symString, index, cappacity);
		text2++;
		currentIndex++;
		if(n->validEnd) longestIndex = currentIndex;
		n = findTrieNode(n->child, *text2);
	}
	symString[*index] = '\0';
	if(longestIndex > 0) tokenizeSizedSymbolString(tokenArr, text, textSize, lineNumber, longestIndex);
	else {
		printf("Error parsing symbol");
		exit(1);
	}
	free(symString);
	free(cappacity);
	int i = *index;
	free(index);
	return i;
}

//while findnode != NULL and char isn't ascii
//	add to curr tok
//	if it is in the trie, ascii
// 	if not it's a word/identifier


//words: apples, harry, Cj, 
//symbols: !?!, <<@, ....

void tokenize(TokenArray* tokenArr, char* text, long textSize){
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
				emitToken(SEMI_COLON_TOKEN, NULL, 0, tokenArr, lineNumber);
				text++;
				break;
			}
			case '\n':{
				emitToken(END_LINE_TOKEN, NULL, 0, tokenArr, lineNumber);
				lineNumber++;
				text++;
				break;
			}
			case '\"' :{
				text += tokenizeString(tokenArr, text, textSize, STR_A_TOKEN, lineNumber);
				break;
			}	
			case '\'' :{
				text += tokenizeString(tokenArr, text, textSize, STR_B_TOKEN, lineNumber);
				break;
			}
			default:{
				if(isDigit(*text)){
					text += tokenizeNumber(tokenArr, text, textSize, lineNumber);
				}
				else if(isAlnum(*text)){
					text += tokenizeWord(tokenArr, text, textSize, lineNumber);
				}
				else{
					text += tokenizeSymbolString(tokenArr, text, textSize, lineNumber);
				}
			}
		}
	}
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
	long fileSize = getSizeOfFile(path);
	char* buffer = readFile(path);
	tokenize(tokens, buffer, fileSize);
	free(buffer);
}

void freeTrieNode(TrieNode* t){
	//don't free key -- it will be used later
	if(t->child!=NULL) freeTrieNode(t->child);
	if(t->next!=NULL) freeTrieNode(t->next);
	free(t);
}

void freeTrie(Trie* t){
	freeTrieNode(t->root);
}

void freeTokens(TokenArray* tokens){
	freeTrie(tokens->trie);
	free(tokens->tokens);
	free(tokens);
}

/*
//add check for when it already exists
int main(){
	TokenArray* tokens = initTokenArray(tokens);
	int numberOfTokens = 100;
	char** strings = (char**)malloc(sizeof(char*)*numberOfTokens);
	addToken(strings, 0, "<");
	addToken(strings, 1, ">");
	addToken(strings, 2, "<=");
	addToken(strings, 3, ">=");
	addToken(strings, 4, "+");
	addToken(strings, 5, "+=");
	addToken(strings, 6, "-");
	addToken(strings, 7, "-=");
	addToken(strings, 8, "*");
	addToken(strings, 9, "*=");
	addToken(strings, 10, "/");
	addToken(strings, 11, "/=");
	addToken(strings, 12, "%");
	addToken(strings, 13, "%=");
	addToken(strings, 14, "==");
	addToken(strings, 15, "[");
	addToken(strings, 16, "]");
	addToken(strings, 17, "{");
	addToken(strings, 18, "}");
	addToken(strings, 19, "(");
	addToken(strings, 20, ")");
	addToken(strings, 21, "^");
	addToken(strings, 22, ",");
	addToken(strings, 23, "!");
	addToken(strings, 24, ":");
	addToken(strings, 25, "=");

	addString(tokens->trie, strings[0]);
	addString(tokens->trie, strings[1]);
	addString(tokens->trie, strings[2]);
	addString(tokens->trie, strings[3]);
	addString(tokens->trie, strings[4]);
	addString(tokens->trie, strings[5]);
	addString(tokens->trie, strings[6]);
	addString(tokens->trie, strings[7]);
	addString(tokens->trie, strings[8]);
	addString(tokens->trie, strings[9]);
	addString(tokens->trie, strings[10]);
	addString(tokens->trie, strings[11]);
	addString(tokens->trie, strings[12]);
	addString(tokens->trie, strings[13]);
	addString(tokens->trie, strings[14]);
	addString(tokens->trie, strings[15]);
	addString(tokens->trie, strings[16]);
	addString(tokens->trie, strings[17]);
	addString(tokens->trie, strings[18]);
	addString(tokens->trie, strings[19]);
	addString(tokens->trie, strings[20]);
	addString(tokens->trie, strings[21]);
	addString(tokens->trie, strings[22]);
	addString(tokens->trie, strings[23]);
	addString(tokens->trie, strings[24]);
	addString(tokens->trie, strings[25]);

	
	if(tokens->trie->root->child != NULL){
		printf("called");
		exit(1);
	}
	exit(1);
	//Arithmetic Test
	lex(tokens, "apples.txt");
	printTokens(tokens);
	
	int numberOfTokens = 100;
	char** tokens = (char**)malloc(sizeof(char*)*numberOfTokens);
	addToken(tokens, 0, "<");
	addToken(tokens, 1, "<<");
	addToken(tokens, 2, "<<?");
	addToken(tokens, 3, "<<!");
	addToken(tokens, 4, "<<??");
}*/



/*
op e1:bool ? e2:t : e3:t		
op e1:i32 + e2:i32 = sum(e1, e2);	

e1 must be bool, e1,e3 can be any expression type but they have to be equal


*/

/*
need to parse ops - for now could do it manually?

*/


/*

After Ops:

For tokenizing words and sym strings
while next_char() is valid


need a valid so far string


ammend this with numbers, strings and words

words
numbers
symbol strings
*/