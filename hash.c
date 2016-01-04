#define _BSD_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Kelly Keniston, CS 261, Spring 2015
   This program creates a hash table out of char* keys and
   with any type of value given. It uses separate chaining
	to deal with hash collisions. */

/* structure for the key/value pair inside of the hash table */
typedef struct valueNodeStr {
	char* key;
	void* value;
	struct valueNodeStr* next;
} valueNode;

/* structure for the hash table */
typedef struct hashTableStr {
	int size;
	struct valueNodeStr** list;
} hashTable;
 
/* hash table initialization, given a size to create the table */
hashTable* createTable(int size) {
	hashTable* table = malloc(sizeof(hashTable)); 
	table->list = malloc(sizeof(valueNode*) * size);
	table->size = size;
	for(int i = 0; i < size; i++) {
		table->list[i] = NULL;
	}
	return table;
}
 
/* creates the hash  value for a key given */
int hashFunction(hashTable* table, char* key) {
	int hash = 0;
	int n;
	for (int i = 0; key[i] != '\0'; i++) {
		if(isalpha(key[i])) {
			n = key [i] - 'a' + 1;
		} else {
			n = 27;
		}
		hash = ((hash << 3) + n) % table->size;
	}
	return hash;
}
 
/* make key value pair object */
valueNode* hashPair(char* key, void* value) {
	valueNode* pair = malloc(sizeof(valueNode));
	pair->key = strdup(key);
	pair->value = value;
	pair->next = NULL;
	return pair;
}

/* inserts the key value pair into the hash table */
void insert(hashTable* table, char* key, void* value) {
	valueNode* pair = NULL;
	valueNode* nextNode = NULL;
	valueNode* endNode = NULL;
	int hashValue = hashFunction(table, key);
	nextNode = table->list[hashValue];
 
	while(nextNode != NULL && nextNode->key != NULL) {
		if (strcmp(key, nextNode->key) != 0) break;
		endNode = nextNode;	
		nextNode = nextNode->next;
	}

	/* if already a pair, replace value */
	if(nextNode != NULL && strcmp(key, nextNode->key) == 0) {
		free(nextNode->value);
		nextNode->value = value;
	/* if not, create a new pair and put in list */
	} else {
		pair = hashPair(key, value);
		if(nextNode == table->list[hashValue]) {
			pair->next = nextNode;
			table->list[hashValue] = pair;
		} else if (nextNode == NULL) {
			endNode->next = pair;
		} else {
			pair->next = nextNode;
			endNode->next = pair;
		}
	}
}
 
/* searches and returns the value associated with the key */
void* search(hashTable* table, char *key) {
	int hashValue = hashFunction(table, key);	
	valueNode* pair = table->list[hashValue];
	while(pair != NULL && strcmp(key, pair->key) != 0) {
		pair = pair->next;
	}
	/* if found return value, if not return null */
	if(pair == NULL || pair->key == NULL || strcmp(key, pair->key) != 0) {
		return NULL;
	} else {
		return pair->value;
	}
}

/* frees allocated memory for table */
void freeTable(hashTable* table) {
	for (int i = 0; i < table->size; i++) {
		valueNode *temp = table->list[i];
		while (temp != NULL) {
			valueNode *fwd = temp->next;
			free(temp->value);
			free(temp->key);
			free(temp);
			temp = fwd;
		}
	}
	free(table);
}

/* applies any passed in function to the values in the table */
void hashApply(hashTable* table, void (*func)(char*, void*)) {
	for (int i = 0; i < table->size; i++) {
		valueNode* start = table->list[i];
		if (start) {
			func(start->key, start->value);
		}
	}
}
