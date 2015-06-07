/** @file
  Implementacja wektora.
  Wektor przechowuje wskaźniki do kolejnych węzłów słownika.
  @ingroup dictionary
  @author Agnieszka Kusnierz <ak332284@students.mimuw.edu.pl>
  @date 2015-05-29
 */

#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
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
    vector->capacity *= 2;
    vector->data = realloc(vector->data, sizeof(Node) * vector->capacity);
  }
}

int where_in_vector(Vector *vector, wchar_t wch)
{
    if (vector->size == 0)
        return -1;
    int i = 0;
    int j = vector->size-1;
    int middle;
    int cmp;
    while (i < j) {
        middle = (j+i) / 2;
        cmp = wcsncmp(&vector->data[middle]->letter, &wch, 1);
        if (cmp > 0)
            j = middle;
        else if (cmp < 0)
            i = middle+1;
        else{
            return middle;
        }
    }
    if ((i == vector->size-1) && (wcsncmp(&vector->data[i]->letter, &wch, 1) < 0))
        i++;
    return i;
}

void vector_free(Vector *vector) {
    free(vector->data);
    free(vector);
}

void node_free(struct Node *node)
{
    vector_free(node->vector);
    free(node);
    node = NULL;
}