#ifndef HASH_H
#define HASH_H

typedef struct valueNodeStr {
	char* key;
	void* value;
	struct valueNodeStr* next;
} valueNode;

typedef struct hashTableStr {
	int size;
	struct valueNodeStr** list;
} hashTable;
 
hashTable* createTable(int size);
void insert(hashTable* table, char* key, void* value);
void* search(hashTable* table, char *key);
void hashApply(hashTable* table, void (*func)(char*, void*));
void freeTable(hashTable* table);

#endif
