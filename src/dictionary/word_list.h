/** @file
    Interfejs listy słów.

    @ingroup dictionary
    @author Jakub Pawlewicz <pan@mimuw.edu.pl>
    @copyright Uniwerstet Warszawski
    @date 2015-05-10
 */

#ifndef __WORD_LIST_H__
#define __WORD_LIST_H__

#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>
#include <wchar.h>

/**
  Maksymalna liczba słów przechowywana w liście słów.
  */
#define WORD_LIST_MAX_WORDS 1024


/**
  Łączna liczba znaków słów przechowywanych w liście słów
  włącznie ze znakami \0 kończącymi słowo.
  */
#define WORD_LIST_SUM 1024

/// Maksymalny rozmiar słowa.
#define MAX_WORD_SIZE 100

/**
  Struktura przechowująca listę słów.
  */
typedef struct word_list {
    ///Wskaźnik na kolejny element.
    struct word_list *next;
    ///Słowo przechowywane w tablicy zawierającej wchar_t.
    wchar_t word[MAX_WORD_SIZE];
} word_list;

/**
  Inicjuje listę słów.
  @param[in,out] list Lista słów.
  */
void word_list_init(struct word_list *list);

/**
  Destrukcja listy słów.
  @param[in,out] list Lista słów.
  */
void word_list_done(struct word_list *list);

/**
  Dodaje słowo do listy.
  @param[in,out] list Lista słów.
  @param[in] word Dodawane słowo.
  @return 1 jeśli się udało, 0 w p.p.
  */
int word_list_add(struct word_list *list, const wchar_t *word);

/**
  Zwraca liczbę słów w liście.
  @param[in] list Lista słów.
  @return Liczba słów w liście.
  */
static inline
size_t word_list_size(const struct word_list *list)
{
    int i = 0;
    while (list != NULL) {
      list = list->next;
      i++;
    }
    return i-1;
}

/**
  Zwraca tablicę słów w liście.
  @param[in] list Lista słów.
  @return Tablica słów.
  */
static inline
const wchar_t * const * word_list_get(const struct word_list *list)
{
    wchar_t *a[WORD_LIST_MAX_WORDS];
    struct word_list *flunkey;
    flunkey = list->next;
    int size = word_list_size(list);
    for (int i = 0; i < size; i++) {
        a[i] = flunkey->word;
        flunkey = flunkey->next;
    }
    const wchar_t * const * b = (const wchar_t * const * ) a;
    wchar_t *c = L"";
    printf("%ls", c);
    return b;
}

#endif /* __WORD_LIST_H__ */
