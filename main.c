#define _BSD_SOURCE
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#define MAX_WORD_LENGTH 256

/* Kelly Keniston, CS 261, Spring 2015
   This program reads all of the words from a file and
	inserts each unique word into a hash table which keeps
	track of the count of each word. */

hashTable* createTable(int size);
void* search(hashTable* table, char *key);
void insert(hashTable* table, char* key, void* value);
void hashApply(hashTable* table, void (*func)(char*, void*));
void freeTable(hashTable* table);

/* Reads a file, returns the next word in the file */
char* getNextWord(FILE* fd) {
	assert(fd != NULL);
	char ch;	
	while ((ch = fgetc(fd)) != EOF) {
		if (isalnum(ch)) break;
	}
	if (ch == EOF) return NULL;
	char wordBuffer[MAX_WORD_LENGTH];
	int pos = 0;
	wordBuffer[pos++] = tolower(ch);
	while ((ch = fgetc(fd)) != EOF) {
		if (isspace(ch) || pos >= MAX_WORD_LENGTH - 1) break;
		if (isalnum(ch)) {
			wordBuffer[pos++] = tolower(ch);	
		}
	}
	wordBuffer[pos] = '\0';
	return strdup(wordBuffer);
}

/* Prints the hash table with the key and associated count */
void printList(char* key, void* value) {
	printf("%s %i\n", key, *((int*) value));
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr,"Error: %s expecting at least one filename.\n",
						argv[0]);
		exit(1);
	}

	hashTable* table = createTable(65536);
	for (int i = 1; i < argc; i++) {
		FILE* fd = fopen(argv[i], "r");
		if (fd) {
			char* word;
			while ((word = getNextWord(fd)) != NULL) {
				int* ptr = malloc(sizeof(int));
				int* wc = malloc(sizeof(int));
				wc = search(table, word);
				if (wc == 0) {
					*ptr = 1;
					insert(table, word, ptr);
				} else {
					free(word);
					(*wc)++;
				}
			}
			fclose(fd);
		} else {
			fprintf(stderr, "Error: unable to open file %s\n", argv[i]);
			exit(1);
		}		
	}

	hashApply(table, printList);
	freeTable(table);

	return 0;
}
