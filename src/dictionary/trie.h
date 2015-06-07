/** @file
  Interfejs drzewa trie.
  @ingroup dictionary
  @author Agnieszka Kusnierz <ak332284@students.mimuw.edu.pl>
  @date 2015-05-29
  */

#ifndef __TRIE_H__
#define __TRIE_H__

#include "vector.h"
#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>

/**
  Struktura przechowująca alfabet.
  */
typedef struct letter_list {
  ///Wskaźnik na kolejny element slfabetu.
  struct letter_list *next;
  ///Litera znajdująca się konkretnym węźle.
  wchar_t letter;
} letter_list;

/** 
  Pomocnicza funkcja do czyszczenia listy liter.
  @param[in,out] l lista przeznaczona do skasowania.
  */
void letter_list_done(struct letter_list *l);

/** 
  Pomocnicza funkcja do czyszczenia słownika.
  @param[in,out] node na którym wykonywane są operacje.
  */
void tree_free(struct Node *node);

/**
  Tworzenie alfabetu.
  @param[in] n węzeł, z którego aktualnie pobieramy literki.s
  @param[in] l lista zawierająca alfabet.
  @return
  */ 
void alphabet_init(const struct Node *n, struct letter_list *l);

/**Dodanie litery do alfabetu.
  @param[in] l alfabet, każdy węzeł zawiera 1 literkę.
  @param[in] c litera, która jest dodawana.
  @return list l - zaaktualizowana lista.
  */
struct letter_list* add_to_list(struct letter_list *l, wchar_t c);

/**
  Sprawdzanie, czy dana litera znajduje się w aktualnym alfabecie.
  @param[in] l alfabet, każdy węzeł zawiera 1 literkę.
  @param[in] c litera, która jest sprawdzana.
  @return true, jeśli litera znajduje się w alfabecie, false w p.p.
  */
bool is_char_in_list(struct letter_list *l, wchar_t c);


#endif /* __TRIE_H__ */
