// vector.h
#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <wchar.h>

#define VECTOR_INITIAL_CAPACITY 9


///Define a Node type
typedef struct Node {
	wchar_t letter; /// letter store in Node
	int toBuild; /// to create trie
	int isInWord; /// 1 - is a word; 0 - isn't a word
	struct Vector *vector; /// vector of pointers to nodes
	struct Node *parent; /// pointer to parent
	//int isInWord;
} Node;

/// Define a vector type
typedef struct Vector {
  int size;      /// slots used so far
  int capacity;  /// total available slots
  Node **data;     /// array of pointers to node's which we're storing
} Vector;

void vector_init(Vector *vector);

void vector_append(Vector *vector, Node *value);

void vector_remove(Vector *vector, Node *node);

//Node vector_get(Vector *vector, int index);

void insert_an_index(Vector *vector, Node *node, int index);

void vector_double_capacity_if_full(Vector *vector);

void vector_free(Vector *vector);

void write_vector(Vector *vector);

#endif /* __VECTOR_H__ */
