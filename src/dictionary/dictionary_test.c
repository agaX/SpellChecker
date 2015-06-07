/**@defgroup Testy Moduł Testy
  Biblioteka testująca.
  Umożliwia testowanie poszczególnych funkcji.
  */

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
#include "dictionary.h"
#include "vector.c"
#include "word_list.c"
#include "trie.c"

/// Słowo stworzone na potrzeby testu.
const wchar_t* first = L"testword";
/// Słowo stworzone na potrzeby testu.
const wchar_t* second  = L"test";
/// Słowo stworzone na potrzeby testu.
const wchar_t* third = L"testmore";
/// Słowo stworzone na potrzeby testu.
const wchar_t* fourth = L"anotherword";
/// Słowo stworzone na potrzeby testu.
const wchar_t* fifth  = L"polishłord";

/**
  Test funkcji dictionary_new().
  Stworzenie słownika.
  */
static void dictionary_new_test(void** state) {
	struct dictionary *dict;
	dict = dictionary_new();
	assert_true(dict->root->letter == '-');
	assert_true(dict->alphabet->letter == '#');
	assert_int_equal(dict->root->isInWord, 0);
	assert_null(dict->alphabet->next);
	dictionary_done(dict);
}

/**
  Test funkcji dictionary_insert().
  Wstawienie pięciu słów do słownika.
  */
static int dictionary_insert_test(void** state) {
	struct dictionary *dict;
	dict = dictionary_new();
	if (!dict)
		return -1;
	dictionary_insert(dict, first);
	assert_true(dictionary_find(dict, first));
	assert_false(dictionary_find(dict,second));
	dictionary_insert(dict, second);
	dictionary_insert(dict, third);
	dictionary_insert(dict, fourth);
	dictionary_insert(dict, fifth);
	*state = dict;
	return 0;
}

/**
  Zwolneienie pamięci po wykonaniu testu.
  */
static int dictionary_teardown(void** state) {
	struct dictionary *dict = *state;
	dictionary_done(dict);
	return 0;
}

/** 
  Test funkcji dictionary_find().
  Wyszukanie pięciu słów, wstawionych wczęsniej.
  */
static void dictionary_find_test(void** state) {
	struct dictionary *dict = *state;
	assert_true(dictionary_find(dict, first));
	assert_true(dictionary_find(dict, second));
	assert_true(dictionary_find(dict, third));
	assert_true(dictionary_find(dict, first));
	assert_true(dictionary_find(dict, fifth));
}

/** 
  Test funkcji dictionary_delete().
  Usuwanie wstawionych słów i próba usunięcia nieistniejących.
  */
static void dictionary_delete_test(void** state) {
	struct dictionary *dict = *state;
	assert_true(dictionary_find(dict, first));
	dictionary_delete(dict, first);
	assert_false(dictionary_find(dict, first));
	dictionary_insert(dict,first);
	assert_true(dictionary_find(dict, first));
	dictionary_delete(dict, second);
	assert_false(dictionary_find(dict, second));
	assert_true(dictionary_find(dict, third));
}

/** 
  Test funckji dictionary_hints().
  Wstawienie do słownika 7 słów.
  Sprawdzenie, czy funkcja zwraca prawidłowe podpowiedzi.
  */
static void dictionary_hints_test(void** state) {
	struct dictionary *dict;
	dict = dictionary_new();
	struct word_list *l = malloc(sizeof(word_list));
    word_list_init(l);
    wchar_t *word1 = L"at";
    wchar_t *word2 = L"car";
    wchar_t *word3 = L"cat";
    wchar_t *word4 = L"cats";
    wchar_t *word5 = L"cut";
    wchar_t *word6 = L"mat";
    wchar_t *word7 = L"rat";
	dictionary_insert(dict, word1);
	dictionary_insert(dict, word2);
	dictionary_insert(dict, word3);
	dictionary_insert(dict, word4);
	dictionary_insert(dict, word5);
	dictionary_insert(dict, word6);
	dictionary_insert(dict, word7);
	dictionary_hints(dict, word3, l);
	assert_string_equal(l->next->word, word1);
	assert_string_equal(l->next->next->word, word2);
	assert_string_equal(l->next->next->next->word, word3);
	assert_string_equal(l->next->next->next->next->word, word4);
	assert_string_equal(l->next->next->next->next->next->word, word5);
	assert_string_equal(l->next->next->next->next->next->next->word, word6);
	assert_string_equal(l->next->next->next->next->next->next->next->word, word7);
	word_list_done(l);
	dictionary_done(dict);
	free(l);
}

/** 
  test funkcji alphabet_init().
  Sprawdzenie czy w alfabecie znajdują się jedynie litery ze slownika.
  */
static void alphabet_init_test(void** state) {
	struct dictionary *dict = *state;
	struct letter_list *l = malloc(sizeof(letter_list));
	l->letter = '#';
	l->next = NULL;
	alphabet_init(dict->root, l);
	for (int i = 0; i < wcslen(first); i++)
		assert_true(is_char_in_list(l, first[i]));
	for (int i = 0; i < wcslen(second); i++)
		assert_true(is_char_in_list(l, second[i]));
	for (int i = 0; i < wcslen(third); i++)
		assert_true(is_char_in_list(l, third[i]));
	for (int i = 0; i < wcslen(fourth); i++)
		assert_true(is_char_in_list(l, fourth[i]));
	for (int i = 0; i < wcslen(fifth); i++)
		assert_true(is_char_in_list(l, fifth[i]));
	assert_false(is_char_in_list(l, 'k'));
}

/**
  Funkcja główna programu.
  */
int main(void) {
    const struct CMUnitTest tests[] = {
    	cmocka_unit_test(dictionary_new_test),
    	cmocka_unit_test_setup_teardown(dictionary_find_test, dictionary_insert_test, dictionary_teardown),
    	cmocka_unit_test_setup_teardown(dictionary_delete_test, dictionary_insert_test, dictionary_teardown),
    	cmocka_unit_test_setup_teardown(alphabet_init_test, dictionary_insert_test, dictionary_teardown),
    	cmocka_unit_test(dictionary_hints_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}