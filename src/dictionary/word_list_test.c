/** @file
  Plik testujący poszczególne funkcje słownika.
  @ingroup Testy
  @author Agnieszka Kusnierz <ak332284@students.mimuw.edu.pl>
  @date 2015-05-31
*/

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include "word_list.h"

/// Słowo stworzone na potrzeby testu.
const wchar_t* test   = L"Test string";
/// Słowo stworzone na potrzeby testu.
const wchar_t* first  = L"First string";
/// Słowo stworzone na potrzeby testu.
const wchar_t* second = L"Second string";
/// Słowo stworzone na potrzeby testu.
const wchar_t* third  = L"Third string";

/**
  Test funkcji word_list_init().
  Sprawdzenie, czy word_list powstaje prawidłowo.
  */
static void word_list_init_test(void** state) {
    struct word_list *l = malloc(sizeof(word_list));
    word_list_init(l);
    assert_int_equal(word_list_size(l), 0);
    word_list_done(l);
    free(l);
}

/**
  Test funckji word_list_add().
  Dodanie elementu do pustej listy.
  */
static void word_list_add_to_empty_test(void** state) {
    struct word_list *l = malloc(sizeof(word_list));
    word_list_init(l);
    word_list_add(l, test);
    assert_int_equal(word_list_size(l), 1);
    assert_true(wcscmp(test, word_list_get(l)[0]) == 0);
    word_list_done(l);
    free(l);
}

/**
  Ustawienie początkowych parametrów word_list.
  */
static int word_list_setup(void **state) {
    struct word_list *l = malloc(sizeof(struct word_list));
    if (!l) 
        return -1;
    word_list_init(l);
    word_list_add(l, third);
    word_list_add(l, first);
    word_list_add(l, second);
    *state = l;
    return 0;
}

/**
  Czyszczenei po wykonaniu testu.
  */
static int word_list_teardown(void **state) {
    struct word_list *l = *state;
    word_list_done(l);
    free(l);
    return 0;
}

/**
  Test funckji word_list_get().
  Sprawdzanie czy słowa są na liście w odpowiedniej kolejności (mimo innej kolejności wstawiania).
  */
static void word_list_get_test(void** state) {
    struct word_list *l = *state;
    assert_true(wcscmp(first, word_list_get(l)[0]) == 0);
    assert_true(wcscmp(second, word_list_get(l)[1]) == 0);
    assert_true(wcscmp(third, word_list_get(l)[2]) == 0);
}

/**
  Funkcja główna testu.
  */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(word_list_init_test),
        cmocka_unit_test(word_list_add_to_empty_test),
        cmocka_unit_test_setup_teardown(word_list_get_test, word_list_setup, word_list_teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}