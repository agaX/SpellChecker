/** @file
  Plik testujący poszczególne funkcje drzewa.
  @ingroup Testy
  @author Agnieszka Kusnierz <ak332284@students.mimuw.edu.pl>
  @date 2015-05-31
*/

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include <wctype.h>
#include <wchar.h>
#include "trie.h"
#include "vector.h"
#include "dictionary.h"
#include "conf.h"

/// litera do wstawienia w alfabet.
const wchar_t char1 = L'a';
/// litera do wstawienia w alfabet.
const wchar_t char2 = L'b';
/// litera do wstawienia w alfabet.
const wchar_t char3 = L'c';
/// litera do wstawienia w alfabet.
const wchar_t char4 = L'ć';
/// litera do wstawienia w alfabet.
const wchar_t char5 = L'z';

/**
  Stworzenie letter_list do poszczególnych testów.
  */
static int letter_list_setup(void** state) {
    struct letter_list *l = malloc(sizeof(letter_list));
    if (!l) 
        return -1;
    l->letter = '#';
    l->next = NULL;
    add_to_list(l, char1);
    add_to_list(l, char2);
    add_to_list(l, char3);
    add_to_list(l, char4);
    add_to_list(l, char5);
    *state = l;
    return 0;
}

/**
  Czyszczenei po wykonaniu testu.
  */
static int letter_list_teardown(void** state) {
    struct letter_list *l = *state;
    letter_list_done(l);
    return 0;
}

/**
  Test funkcji letter_list_add().
  Sprawdzenie czy poszczególne litery znajdują się na odpowiednich miejscach.
  */
static void letter_list_add_test(void **state) {
    struct letter_list *l = *state;
    assert_true(l->next->letter == char1);
    assert_true(l->next->next->letter == char2);
    assert_true(l->next->next->next->letter == char3);
    assert_true(l->next->next->next->next->letter == char4);
    assert_true(l->next->next->next->next->next->letter == char5);
}

/**
  Test funkcji is_char_in_list().
  Sprawdzenie, czy poszczególne litery znajdują się w liście.
  */
static void letter_list_is_char_in_list_test(void **state) {
    struct letter_list *l = *state;
    assert_true(is_char_in_list(l, char1));
    assert_true(is_char_in_list(l, char2));
    assert_true(is_char_in_list(l, char3));
    assert_true(is_char_in_list(l, char4));
    assert_true(is_char_in_list(l, char5));
    wchar_t falsechar = L'f';
    assert_false(is_char_in_list(l, falsechar));
}

/**
  Główna funkcja testu.
  */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(letter_list_add_test, letter_list_setup, letter_list_teardown),
        cmocka_unit_test_setup_teardown(letter_list_is_char_in_list_test, letter_list_setup, letter_list_teardown),
    };
 
    return cmocka_run_group_tests(tests, NULL, NULL);
}