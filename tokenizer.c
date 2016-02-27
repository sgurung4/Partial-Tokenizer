/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_ {
	char* token; /* token being used */
	char* originalstring; /* string that was input */
	size_t length; /* string length */
	int pos; /* keeps track of position in string */
};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 * 
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate( char * ts ) {
	TokenizerT* t = (TokenizerT*)malloc(sizeof(TokenizerT));
	t->length = strlen(ts);
	if(t->length == 0){
		return NULL;
	}

	t->pos = 0;
	t->originalstring = (char*)malloc(sizeof(char)*(1+t->length));
	strcpy(t->originalstring, ts);
	t->token = (char*)malloc(sizeof(char)*(1+t->length));

	return t;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy( TokenizerT * tk ) {
	free (tk->originalstring);
	free (tk->token);
	free (tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken( TokenizerT * tk ) {
	char* finishedtoken = (char*)malloc(sizeof(char));
	char* stream = (char*)malloc(sizeof(char) * (1+tk->length));
	strcpy(stream, tk->originalstring);

	int i;
	i = tk->pos;
	int j = 0;
	int k; int l;

	/* checks for space */
	while(isspace(stream[i])) {
		i++;
	}

	if(stream[i]=='0' && (stream[i+1]!='x' && stream[i+1]!='X')) {
		if((!isdigit(stream[i+1]) || isalpha(stream[i+1]) || ispunct(stream[i+1]))) {
			while(stream[i]=='0'){
				finishedtoken[j] = stream[i];
				char x = finishedtoken[j];
				printf("[%#x] ", x);
				i++; j++;
				tk->pos = i;
				return finishedtoken;
			}
		} else if(isdigit(stream[i]) && stream[i]=='0' && isdigit(stream[i+1]) && stream[i+1]<='7' && stream[i+1]>='0') {    /* octal */
			while(stream[i]<='7' && stream[i]>='0') {
				finishedtoken[j] = stream[i];
				i++; j++;
			} tk->pos = i; 
			printf("octal ");
			return finishedtoken;
		}
	} 

	if(isalpha(stream[i])) {               /* if token is an escape character; starts with a letter */
		while(isalpha(stream[i])) {
			finishedtoken[j] = stream[i];
			char x = finishedtoken[j];
			printf("[%#x] ", x);
			i++; j++;
			tk->pos = i;
			return finishedtoken;
		}
	}

	if(ispunct(stream[i]) && (stream[i]!='-' || stream[i]!='+')) {       /* starts with a punctuation */
		while(ispunct(stream[i]) && (stream[i]!='-' || stream[i]!='+')) {
			finishedtoken[j] = stream[i];
			char x = finishedtoken[j];
			printf("[%#x] ", x);
			i++; j++;
			tk->pos = i;
			return finishedtoken;
		}
	}


	if(isdigit(stream[i]) && stream[i]=='0' && (stream[i+1]=='x' || stream[i+1]=='X')) {     /* hex */
		if(isxdigit(stream[i+2])) {
			while(j<2){
				finishedtoken[j] = stream[i];
				j++; i++;
			} while(isxdigit(stream[i])) {
				finishedtoken[j] = stream[i];
				i++; j++;
			} tk->pos = i;
			printf("hex ");
			return finishedtoken;
		} else {
			while(j<3){
				finishedtoken[j] = stream[i];
				i++; j++;
			} tk->pos = i;
			printf("error ");
			return finishedtoken;
		}
	}

	if(isdigit(stream[i]) && stream[i]=='0' && (stream[i+1]=='.')) { 
		if(isdigit(stream[i+2])) {
			while(j<2){
				finishedtoken[j] = stream[i];
				j++; i++;
			} while(isdigit(stream[i])){
				finishedtoken[j] = stream[i];
				i++; j++; 
			} if((stream[i]=='E' || stream[i]=='e') && (stream[i+1]=='+' || stream[i+1]=='-')) { /* floats beginning with 0 */
				if(isdigit(stream[i+2])) {
					for(k=0; k<2; k++) {
						finishedtoken[j] = stream[i];
						j++; i++;
					} while(isdigit(stream[i])) {
						finishedtoken[j] = stream[i];
						i++; j++;
					} tk->pos = i;
					printf("float ");
					return finishedtoken;
				} else {                  
					for(k=0; k<3; k++) {
						finishedtoken[j] = stream[i];
						j++; i++;
					} tk->pos = i;
					printf("error ");
					return finishedtoken;
				}
			} if(stream[i]=='E' || stream[i]=='e') {
				if(isdigit(stream[i+1])) {
					for(k=0; k<2; k++) {
						finishedtoken[j] = stream[i];
						j++; i++;
					} while(isdigit(stream[i])) {
						finishedtoken[j] = stream[i];
						i++; j++;
					} tk->pos = i;
					printf("float ");
					return finishedtoken;
				} else {
					for(k=0; k<2; k++) {
						finishedtoken[j] = stream[i];
						j++; i++;
					} tk->pos = i;
					printf("error ");
					return finishedtoken;
				}
			}
		} tk->pos = i+1;      
		printf("error ");
		return finishedtoken;
	}

	if(stream[i]!='0'){ 
		if(isdigit(stream[i])){                                  /* Not starting with 0 */
			while(isdigit(stream[i])) {
				finishedtoken[j] = stream[i];
				i++; j++;
			} 
			if(stream[i]=='.' || stream[i]=='E' || stream[i] == 'e'){
				if((stream[i]=='E' || stream[i]=='e') && (stream[i+1]=='+' || stream[i+1]=='-')) {
					if(isdigit(stream[i+2])) {
						for(l=0; l<2; l++) {
							finishedtoken[j] = stream[i];
							j++; i++;
						} while(isdigit(stream[i])) {
							finishedtoken[j] = stream[i];
							i++; j++;
						} tk->pos = i;
						printf("float ");
						return finishedtoken;
					} else {
						for(l=0; l<3; l++) {
							finishedtoken[j] = stream[i];
							j++; i++;
						} tk->pos = i;
						printf("error ");
						return finishedtoken;
					}
				} else if((stream[i]=='E' || stream[i]=='e') && (isdigit(stream[i+1]))) {
					for(l=0; l<2; l++){	
						finishedtoken[j] = stream[i];
						j++; i++;
					} while(isdigit(stream[i])){
						finishedtoken[j] = stream[i];
						i++; j++;
					} tk->pos = i;
					printf("float ");
					return finishedtoken;
				} else if((stream[i]=='E' || stream[i]=='e') && (!isdigit(stream[i+1]))) {
					for(l=0; l<2; l++){
						finishedtoken[j] = stream[i];
						j++; i++;
					} tk->pos = i;
					printf("error ");
					return finishedtoken;
				}
				if(stream[i]=='.' && isdigit(stream[i+1])) {
					for(k=0; k<2; k++) {
						finishedtoken[j] = stream[i];
						j++; i++;
					} while(isdigit(stream[i])) {
						finishedtoken[j] = stream[i];
						i++; j++;
					} if((stream[i]=='E' || stream[i]=='e') && (stream[i+1]=='+' || stream[i+1]=='-')) {
						if(isdigit(stream[i+2])) {
							for(l=0; l<2; l++) {
								finishedtoken[j] = stream[i];
								j++; i++;
							} while(isdigit(stream[i])) {
								finishedtoken[j] = stream[i];
								i++; j++;
							} tk->pos = i;
							printf("float ");
							return finishedtoken;
						} else {
							for(l=0; l<3; l++) {
								finishedtoken[j] = stream[i];
								j++; i++;
							} tk->pos = i;
							printf("error ");
							return finishedtoken;	
						}
					} else if((stream[i]=='E' || stream[i]=='e') && isdigit(stream[i+1])) {
						for(l=0; l<2; l++){
							finishedtoken[j] = stream[i];
							j++; i++;
						} while(isdigit(stream[i])) {
							finishedtoken[j] = stream[i];
							i++; j++;
						} tk->pos = i;
						printf("float ");
						return finishedtoken;
					} else if((stream[i]=='E' || stream[i]=='e') && !isdigit(stream[i+1])) {
						for(l=0; l<2; l++) {
							finishedtoken[j] = stream[i];
							j++; i++;
						} tk->pos = i;
						printf("error ");
						return finishedtoken;
					} tk->pos = i;
					printf("float ");
					return finishedtoken;
				}
			} tk->pos = i;
			printf("decimal ");
			return finishedtoken;
		}
	}
}

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
	if(argc!=2){
		printf("Error input \n");
		return 0;
	}

	char* string = argv[1];
	TokenizerT* t = TKCreate(string);

	while( (t->pos) < (t->length) ) {
		t->token = TKGetNextToken(t);
		printf("\"%s\"\n", t->token);
	}

	TKDestroy(t);

  return 0;
}

























