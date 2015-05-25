// vector.
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include <assert.h>
#include <locale.h>
#include <string.h>
#include <wctype.h>
#include <wchar.h>


void vector_init(Vector *vector) {
  // initialize size and capacity
  vector->size = 0;
  vector->capacity = VECTOR_INITIAL_CAPACITY;
  // allocate memory for vector->data
  vector->data = malloc(sizeof(Node*)*vector->capacity);
}

void vector_append(Vector *vector, Node *value) {
  // make sure there's room to expand into
  vector_double_capacity_if_full(vector);

  // append the value and increment vector->size
  vector->data[vector->size++] = value;
}


void vector_remove(Vector *vector, Node *node) {
  //Node *toDelete;
  //*toDelete = *node;
  for (int i = 0; i < vector->size; i++) {
  /*
    printf("Z vectora : ");
  for (int i = 0; i < vector->size; i++) {
    printf("%lc , ", vector->data[i].letter);
  }
  printf("\n");
*/
    if (vector->data[i]->letter == node->letter) {
      //printf("USUWAM!!!!\n");
      //node_free(node);
      for (int j = i; j < vector->size-1; j++){
        vector->data[j] = vector->data[j+1];
      }
      vector->size--;
    
    //printf("A na nodzie nadal jest %lc \n", toDelete->letter);
    //node_free(toDelete);
    }

  }
  /*printf("Z vectora : ");
  for (int i = 0; i < vector->size; i++) {
    printf("%lc , ", vector->data[i].letter);
  }
  printf("będę usuwać literę %lc \n", node->letter);

  for (int i = 0; i < vector->size; i++) {

    if (vector->data[i].letter == node->letter) {
      //printf("dupa1\n");
      //free(vector->data[i]);
      for (int j = i; j < vector->size-1; j++){
        //printf("dupa1,5\n");
        vector->data[j] = vector->data[j+1];
      }
      
      vector->size--;
      
      //printf("dupa2\n");
      
      printf("Vector po usunięciu : ");
      for (int i = 0; i < vector->size; i++) {
         printf("%lc , ", vector->data[i].letter);
      }
      printf("\n");
      node_free(node);
      return 1;
    }
  }
  return 0;*/
}

void insert_an_index(Vector *vector, Node *node, int index) {

    vector_double_capacity_if_full(vector);
    //printf("Wrzucamy %lc \n", node->letter);
    for (int i = vector->size; i > index; i--)
        vector->data[i] = vector->data[i - 1];
    vector->data[index] = node;
    vector->size++;
}
/*
Node vector_get(Vector *vector, int index) {
  if (index >= vector->size || index < 0) {
    printf("Index %d out of bounds for vector of size %d\n", index, vector->size);
    exit(1);
  }
  return vector->data[index];
}
*/

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
