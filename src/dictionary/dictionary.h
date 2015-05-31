/** @defgroup dictionary Moduł dictionary
    Biblioteka obsługująca słownik.
  */
/** @file 
    Interfejs biblioteki obsługującej słownik.
    @ingroup dictionary
    @author Jakub Pawlewicz <pan@mimuw.edu.pl>
    @copyright Uniwersytet Warszawski
    @date 2015-05-27
 */
#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include "vector.h"
#include "word_list.h"
#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>

/**
  Struktura przechowująca słownik.
  */
typedef struct dictionary {
    ///korzeń słownika
    struct Node *root; 
} dictionary;

/**
  Struktura przechowująca alfabet
  */
typedef struct letter_list {
  ///Wskaźnik na kolejny element slfabetu
  struct letter_list *next;
  ///Litera znajdująca się konkretnym węźle
  wchar_t letter;
} letter_list;

/** 
  Pomocnicza funkcja do czyszczenia listy liter
  @param[in,out] letter_list, lista przeznaczona do skasowania.
  */
void letter_list_done(struct letter_list *l);

/** 
  Tworzenie słownika
  @return dict - stworzony słownik z pierwszym węzłem (root).
  */
struct dictionary *dictionary_new(void);

/** 
  Czyszczenie słownika
  @param[in,out] dict Słownik, na którym wykonywane są operacje.
  */
static void dictionary_free(struct dictionary *dict);

/** 
  Pomocnicza funkcja do czyszczenia słownika
  @param[in,out] node, na którym wykonywane są operacje.
  */
static void tree_free(struct Node *node);

/**
  Destrukcja słownika.
  @param[in,out] dict Słownik.
  */
void dictionary_done(struct dictionary *dict);

/**
  Tworzenie alfabetu.
  @param[in]n węzeł, z którego aktualnie pobieramy literki.s
  @param[in]l lista zawierająca alfabet.
  @return
  */ 
void alphabet(const struct Node *n, struct letter_list *l);

/**Usuwanie node'a
  @param[in] node, który zostaje usunięty, razem z vectorem, który zawiera
  */
void node_free(struct Node *node);

/**Dodanie litery do alfabetu.
  @param[in]l alfabet, każdy węzeł zawiera 1 literkę.
  @param[in]c litera, która jest dodawana.
  @return list l - zaaktualizowana lista.
  */
struct letter_list* add_to_list(struct letter_list *l, wchar_t c);

/** Zwraca miejsce w wektorze, na które zostanie wstawiony znak
  @param[in] vector do którego zostanie wstawiony znak wch
  @param[in] wch to znak, który zostanie wstawiony
  */
int where_in_vector(Vector *vector, wchar_t wch);

/**
  Wstawia podane słowo do słownika.
  @param[in,out] dict Słownik.
  @param[in] word Słowo, które należy wstawić do słownika.
  @return 0 jeśli słowo było już w słowniku, 1 jeśli udało się wstawić.
  */
int dictionary_insert(struct dictionary *dict, const wchar_t* word);

/**
  Usuwa podane słowo ze słownika, jeśli istnieje.
  @param[in,out] dict Słownik.
  @param[in] word Słowo, które należy usunąć ze słownika.
  @return 1 jeśli udało się usunąć, zero jeśli nie.
  */
int dictionary_delete(struct dictionary *dict, const wchar_t* word);

/**
  Sprawdza, czy dane słowo znajduje się w słowniku.
  @param[in] dict Słownik.
  @param[in] word Szukane słowo.
  @return Wartość logiczna czy `word` jest w słowniku.
  */
bool dictionary_find(const struct dictionary *dict, const wchar_t* word); 

/**
  Zapisanie słownika, każdy węzeł osobno
  @param[in]node węzęł, który zostaje zapisany w danym wywołaniu
  @param[in]stream plik, w którym zostaje zapisany węzeł
  @return 0, jeśli zapisanie się powiodło, -1 w p.p.
  */ 
int dictionary_save_node(const struct Node *node, FILE* stream);

/**
  Zapisuje słownik.
  @param[in] dict Słownik.
  @param[in,out] stream Strumień, gdzie ma być zapisany słownik.
  @return <0 jeśli operacja się nie powiedzie, 0 w p.p.
  */
int dictionary_save(const struct dictionary *dict, FILE* stream);

/**
  Inicjuje i wczytuje słownik.
  Słownik ten należy zniszczyć za pomocą dictionary_done().
  @param[in,out] stream Strumień, skąd ma być wczytany słownik.
  @return Wczytany słownik lub NULL, jeśli operacja się nie powiedzie.
  */
struct dictionary * dictionary_load(FILE* stream);

/**
  Tworzenie wszystkich możliwych słów do hintsów.
  @param[in]word słowo, dla którego powstaną hintsy.
  @param[in]li lista słówm które mogą być hintsami (jeśli są w słowniku).
  @param[in]n pierwszy węzeł słownika.
  @param[in]l aktualny alfabet.
  */ 
void words_to_check(const wchar_t *word, struct word_list *li, const struct dictionary *dict, struct letter_list *l);

/**
  Sprawdza, czy dane słowo znajduje się w słowniku.
  @param[in] dict Słownik.
  @param[in] word Szukane słowo.
  @param[in] list Lista, w której zostaną umieszczone podpowiedzi.
  */
void dictionary_hints(const struct dictionary *dict, const wchar_t* word,
        struct word_list *list);

/**
  Sprawdzanie, czy dana litera znajduje się w aktualnym alfabecie.
  @param[in]l alfabet, każdy węzeł zawiera 1 literkę.
  @param[in]c litera, która jest sprawdzana.
  @return true, jeśli litera znajduje się w alfabecie, false w p.p.
  */
bool is_char_in_list(struct letter_list *l, wchar_t c);



/**
  Zwraca nazwy języków, dla których dostępne są słowniki.
  Powinny to być nazwy lokali bez kodowania. np.
  Przykładowe nazwy pl_PL, albo en_US.
  Reprezentacja listy języków jest podobna do
  list łańcuchów znakowych [argz w glibc'u](http://www.gnu.org/software/libc/manual/html_mono/libc.html#Argz-and-Envz-Vectors).
  `*list` jest wskaźnikiem na początek bufora,
  który ma długość `*list_len`.
  W buforze znajdują się łańcuchy znakowe
  jeden po drugim pooddzielane znakiem '\0'.
  Jeśli lista jest niepusta cały bufor też się kończy znakiem '\0'.
  Użytkownik jest odpowiedzialny za zwolnienie tej listy,
  w tym celu wystarczy wywołać `free(*list)`.
  @param[out] list Lista dostępnych języków.
  @param[out] list_len Długość bufora z listą dostępnych języków.
  @return <0 jeśli operacja się nie powiodła, 0 w p.p.
  */
int dictionary_lang_list(char **list, size_t *list_len);


/**
  Inicjuje i wczytuje słownik dla zadanego języka.
  Słownik ten należy zniszczyć za pomocą dictionary_done().
  @param[in] lang Nazwa języka, patrz dictionary_lang_list().
  @return Słownik dla danego języka lub NULL, jeśli operacja się nie powiedzie.
  */
struct dictionary * dictionary_load_lang(const char *lang);


/**
  Zapisuje słownik jak słownik dla ustalonego języka.
  @param[in] dict Słownik.
  @param[in] lang Nazwa języka, patrz dictionary_lang_list().
  @return <0 jeśli operacja się nie powiedzie, 0 w p.p.
  */
int dictionary_save_lang(const struct dictionary *dict, const char *lang);
    
#endif /* __DICTIONARY_H__ */
