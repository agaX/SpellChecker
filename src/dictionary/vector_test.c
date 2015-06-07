/** @file
  Plik testujący poszczególne funkcje vectora.
  @ingroup Testy
  @author Agnieszka Kusnierz <ak332284@students.mimuw.edu.pl>
  @date 2015-05-31
*/


#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include "vector.h"
 
/**
  Test funkcji vector_init().
  Sprawdzenie, czy początkowo wektor jest pusty.
  */
static void vector_init_test(void** state) {    
    struct Vector *v = malloc(sizeof(Vector));
    vector_init(v);
    assert_int_equal(v->size, 0);
    vector_free(v);
}

/** 
  Test funkcji vector_append().
  Dodanie jednego elementu do pustego vectora.
  */
static void vector_append_simple_test(void** state) {
    struct Vector *v = malloc(sizeof(Vector));
    struct Node *n = malloc(sizeof(Node));
    vector_init(v);
    vector_append(v, n);
    assert_int_equal(v->size, 1);
    vector_free(v);
}

/**
  Stworzenie wektora do poszczególnych testów.
  */
static int vector_setup(void** state) {
	struct Vector *v = malloc(sizeof(Vector));
	struct Node *n1 = malloc(sizeof(Node));
	n1->letter = 'c';
	struct Node *n2 = malloc(sizeof(Node));
	n2->letter = 'm';
	struct Node *n3 = malloc(sizeof(Node));
	n3->letter = 'u';
	vector_init(v);
	vector_append(v, n1);
	vector_append(v, n2);
	vector_append(v, n3);
    *state = v;
    return 0;
}

/**
  Zwalnianie zajętej pamięci.
  */
static int vector_teardown(void** state) {
    struct Vector *v = *state;
    vector_free(v);
    return 0;
}

/**
  Test funkcji vector_remove().
  Sprawdzenie czy po dodaniu i usunięciu elementu liczba węzłów będzie się zgadzać.
  */
static void vector_remove_test(void** state) {
    struct Vector *v = malloc(sizeof(Vector));
    struct Node *n = malloc(sizeof(Node));
    n->letter = 'a';
    vector_init(v);
    vector_append(v, n);
    vector_remove(v, n);
    assert_int_equal(v->size, 0);
 	vector_free(v);
}

/**
  Test funkcji vector_insert_an_index().
  Sprawdzamy dodanie na początek, na koniec i wstawienie do środka.
  */
static void vector_insert_an_index_test(void** state) {
	struct Vector *v = *state;
	struct Node *nbegin = malloc(sizeof(Node));
	nbegin->letter = 'a';
	insert_an_index(v, nbegin, 0);
	assert_true(v->data[0]->letter == 'a');
	assert_true(v->data[1]->letter == 'c');
	struct Node *nend = malloc(sizeof(Node));
	nend->letter = 'z';
	insert_an_index(v, nend, 4);
	assert_true(v->data[3]->letter == 'u');
	assert_true(v->data[4]->letter == 'z');
	struct Node *nmiddle = malloc(sizeof(Node));
	nmiddle->letter = 'o';
	insert_an_index(v, nmiddle, 3);
	assert_true(v->data[2]->letter == 'm');
	assert_true(v->data[3]->letter == 'o');
	assert_true(v->data[4]->letter == 'u');
}

/**
  Test funkcji vector_double_capacity_if_full().
  Sprawdzenie, czy zwiększenie wektora odbywa się prawidłowo.
  */
static void vector_double_capacity_if_full_test(void** state) {
    struct Vector *v = malloc(sizeof(Vector));
    struct Node *n = malloc(sizeof(Node));
    n->letter = 'a';
    vector_init(v);
    for (int i = 0; i < 10; i++)
        vector_append(v, n);
    assert_int_equal(v->capacity, 18);
    vector_free(v);
}

/**
  Prosty test funkcji where_in_vector().
  Próba znalezienia nieistniejącego elementu.
  */
static void where_in_vector_simple_test(void** state) {
	struct Vector *v = malloc(sizeof(Vector));
	vector_init(v);
	assert_int_equal(where_in_vector(v, 'a'), -1);
}

/**
  Test funkcji where_in_vector().
  Sprawdzenie czy poszczególne litery znajdują się na prawidłowych miejscach.
  */
static void where_in_vector_test(void** state) {
	struct Vector *v = *state;
	assert_int_equal(where_in_vector(v, 'a'), 0);
	assert_int_equal(where_in_vector(v, 'c'), 0);
	assert_int_equal(where_in_vector(v, 'f'), 1);
	assert_int_equal(where_in_vector(v, 'm'), 1);
	assert_int_equal(where_in_vector(v, 'r'), 2);
	assert_int_equal(where_in_vector(v, 'u'), 2);
	assert_int_equal(where_in_vector(v, 'z'), 3);
}

/**
  Główna funkcja testu.
  */
int main(void) {
    const struct CMUnitTest tests[] = {
    	cmocka_unit_test(vector_init_test),
        cmocka_unit_test(vector_append_simple_test),
        cmocka_unit_test(vector_remove_test),
        cmocka_unit_test(vector_double_capacity_if_full_test),
        cmocka_unit_test_setup_teardown(vector_insert_an_index_test, vector_setup, vector_teardown),
        cmocka_unit_test(where_in_vector_simple_test),
        cmocka_unit_test_setup_teardown(where_in_vector_test, vector_setup, vector_teardown),
    };
 
    return cmocka_run_group_tests(tests, NULL, NULL);
}