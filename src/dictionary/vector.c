/** @file
  Implementacja wektora.
  Wektor przechowuje wskaźniki do kolejnych węzłów słownika.
 
  @ingroup dictionary
  @author Jakub Pawlewicz <pan@mimuw.edu.pl>
  @copyright Uniwerstet Warszawski
  @date 2015-05-11
 */

#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include <assert.h>
#include <locale.h>
#include <string.h>
#include <wctype.h>
#include <wchar.h>

void vector_init(Vector *vector) {
  vector->size = 0;
  vector->capacity = VECTOR_INITIAL_CAPACITY;
  vector->data = malloc(sizeof(Node*)*vector->capacity);
}

void vector_append(Vector *vector, Node *value) {
  vector_double_capacity_if_full(vector);
  vector->data[vector->size++] = value;
}

void vector_remove(Vector *vector, Node *node) {
  for (int i = 0; i < vector->size; i++) {
    if (vector->data[i]->letter == node->letter) {
      for (int j = i; j < vector->size-1; j++){
        vector->data[j] = vector->data[j+1];
      }
      vector->size--;
    }
  }
}

void insert_an_index(Vector *vector, Node *node, int index) {
    vector_double_capacity_if_full(vector);
    for (int i = vector->size; i > index; i--)
        vector->data[i] = vector->data[i - 1];
    vector->data[index] = node;
    vector->size++;
}

void vector_double_capacity_if_full(Vector *vector) {
  if (vector->size >= vector->capacity) {
    // double vector->capacity and resize the allocated memory accordingly
    vector->capacity *= 2;
    vector->data = realloc(vector->data, sizeof(Node) * vector->capacity);
  }
}

void write_vector(Vector *vector) {
    for (int i = 0; i < vector->size; i++) {
        wprintf(L"%lc", vector->data[i]->letter);
        printf("%s\n","dupa" );
    }
    printf("\n");
}

void vector_free(Vector *vector) {
  free(vector->data);
  free(vector);
}
