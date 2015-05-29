/** @file
  Prosta implementacja słownika.
  Słownik przechowuje listę słów.
 
  @ingroup dictionary
  @author Jakub Pawlewicz <pan@mimuw.edu.pl>
  @copyright Uniwerstet Warszawski
  @date 2015-05-11
  @todo Napisać efektywną implementację.
 */
 
#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include <assert.h>
#include <locale.h>
#include <string.h>
#include <wctype.h>
#include <wchar.h>
 
#define _GNU_SOURCE
 
/**
  Struktura przechowująca słownik.
  Na razie prosta implementacja z użyciem listy słów.
 */
 
/** @name Funkcje pomocnicze
  @{
 */
/**
  Czyszczenie pamięci słownika
  @param[in,out] dict słownik
 */
 
struct dictionary *dictionary_new()
{
    struct Vector *v = malloc(sizeof(Vector));
    vector_init(v);
    struct Node *n;
    n = malloc(sizeof(Node));
    n->vector = v;
    n->letter = '-';
    n->isInWord = 0;
    struct dictionary *dict;
    dict = malloc(sizeof(dictionary));
    dict->root = n;
    return dict;
}
 
static void tree_free(struct Node *node)
{
    for (int i = 0; i < node->vector->size; i++) {
        tree_free(node->vector->data[i]);
    }
    node_free(node);
}
 
static void dictionary_free(struct dictionary *dict)
{
    if (dict->root->vector->size > 0)
        tree_free(dict->root);
    else
        node_free(dict->root);
}
 
 
/* 
static void skip_equal(const wchar_t **a, const wchar_t **b)
{
    while (**a == **b && **a != L'\0')
    {
        (*a)++;
        (*b)++;
    }
}
*/
/**
  Zwraca czy słowo `a` można zamienić w `b` przez usunięcie znaku.
  @param[in] a Dłuższe słowo.
  @param[in] b Krótsze słowo.
  @return 1 jeśli się da zamienić `a` w `b` przez usunięcia znaku, 0 w p.p.
 *//*
static int can_transform_by_delete(const wchar_t *a, const wchar_t *b)
{
    //struct word_list *list;
 
    skip_equal(&a, &b);
    a++;
    skip_equal(&a, &b);
    return *a == L'\0' && *b == L'\0';
}
 */
/**
  Zwraca czy słowo `a` można zamienić w `b` przez zamianę znaku.
  @param[in] a Pierwsze słowo.
  @param[in] b Drugie słowo.
  @return 1 jeśli się da zamienić `a` w `b` przez zamianę znaku, 0 w p.p.
 *//*
static int can_transform_by_replace(const wchar_t *a, const wchar_t *b)
{
    skip_equal(&a, &b);
    a++; b++;
    skip_equal(&a, &b);
    return *a == L'\0' && *b == L'\0';
}*/
 
void dictionary_done(struct dictionary *dict)
{
    dictionary_free(dict);
    free(dict);
}
 
int where_in_vector(Vector *vector, wchar_t wch) {
    int i = 0;
    //printf("vector size: %d\n",vector->size );
    if (vector->size != 0) {
        //printf("Porównuję literę %lc z %lc \n", vector->data[i]->letter, wch);
        //printf("wynikiem porównania jest :  %d\n", (wcsncmp(&vector->data[i]->letter,&wch,1)));
        while (wcsncmp(&vector->data[i]->letter,&wch,1) < 0) {
            i++;
            if ((vector->size <= i) || (vector->data[i]->letter == wch))
              return i; ///add in the end
        }
        return i;
    }
    else
        return -1;
}
 
int dictionary_insert(struct dictionary *dict, const wchar_t *word) {
    int length = wcslen(word);
    int number;
    int i = 0;
    struct Node *node;
    struct Node *newNode;
    node = dict->root;
    for (i = 0; i < length; i++) {
        number = where_in_vector(node->vector, word[i]);
        /// put in the end
        if ((number == -1) || (number >= node->vector->size) || (node->vector->data[number]->letter != word[i])) {
            if (number == -1)
                number = 0;
            newNode = malloc(sizeof(Node));
            newNode->vector = malloc(sizeof(Vector));
            vector_init(newNode->vector);
            newNode->letter = word[i];
            if (length == i-1)
                newNode->isInWord = 1;
            else
                newNode->isInWord = 0;
            newNode->parent = node;
            insert_an_index(node->vector, newNode, number);
        }
        node = node->vector->data[number];
    }
    if (node->isInWord == 0){
        node->isInWord = 1;
        return 1;
    }
    else
        return 0;
}
 
void node_free(struct Node *node)
{
    vector_free(node->vector);
    free(node);
    node = NULL;
}
 
int dictionary_delete(struct dictionary *dict, const wchar_t *word) {
    struct Node *node;
    struct Node *parent;
    //struct Node *toDelete;
    node = dict->root;
    int length = wcslen(word);
    int number;
    int i = 0;
    for (i = 0; i < length; i++) {
        number = where_in_vector(node->vector, word[i]);
        if ((number == -1) || (node->vector->size <= number) || (node->vector->data[number]->letter != word[i])) {
            return 0;
        }
        node = node->vector->data[number];
    }
    if (node->isInWord != 1)
        return 0;
    node->isInWord = 0;
 
    while ((node->vector->size == 0) && (node->isInWord != 1) && (node->letter != '-')){
        parent = node->parent;
        vector_remove(parent->vector, node);
        node_free(node);
        node = parent;
    }
    return 1;
}
 
 
 
bool dictionary_find(const struct dictionary *dict, const wchar_t *word) {
    struct Node *node;
    node = dict->root;
    int length = wcslen(word);
    int number;
    int i = 0;
    for (i = 0; i < length; i++) {
        number = where_in_vector(node->vector, word[i]);
        if ((number == -1) || (node->vector->size <= number) || (node->vector->data[number]->letter != word[i])) {
            return false;
        }
        node = node->vector->data[number];
    }
    return node->isInWord;
}
 
 
int dictionary_save_node(const struct Node *node, FILE* stream) {
    if (node->isInWord == 1) {
        if (fprintf(stream, "%lc", node->letter) < 0)
            return -1;
        else if (fprintf(stream, "%c", '@') < 0)
            return -1;
    }
    else if (fprintf(stream, "%lc", node->letter) < 0)
        return -1;
   // else if (fprintf(stream, "%c", '-') < 0)
     //   return -1;
    if (fprintf(stream, "%d", node->vector->size) < 0)
        return -1;
    //printf("%lc", node->letter);
    //printf("%i", node->vector->size);
    for (int i = 0; i < node->vector->size; i++)
        dictionary_save_node(node->vector->data[i], stream);
    return 0;
}
 
int dictionary_save(const struct dictionary *dict, FILE* stream) {
    dictionary_save_node(dict->root, stream);
    if (fprintf(stream, "#") < 0)
        return -1;
    printf("\n");
    return 0;
}
 
struct dictionary * dictionary_load(FILE* stream) {
    char char1 = 'a';
    int number2 = 0;
    wchar_t wch;
    Node *n;
    Node *parent;
    struct dictionary *dict = dictionary_new();
    n = dict->root;
    parent = dict->root;
    while ((fscanf(stream, "%lc", &wch) != EOF) && (wch != '#')) {
        if (fscanf(stream, "%c", &char1) < 0)
           return NULL;
        if (fscanf(stream, "%d", &number2) < 0)
           return NULL;
        //printf("Wczytany obiekt to %lc%d \n", wch, number);
        if (wch != '-') {
            n = malloc(sizeof(Node));
            if (char1 == '.') {
                //wch = towlower(wch);
                n->isInWord = 1;
            }
            else
                n->isInWord = 0;
            n->letter = wch;
            n->parent = parent;
            struct Vector *v = malloc(sizeof(Vector));
            vector_init(v);
            n->vector = v;
            vector_append(parent->vector, n);
            parent = n;
        }
        n->toBuild = number2;
        while ((n != dict->root) && (n->toBuild == 0)) {
            n = n->parent;
            parent = n;
            n->toBuild--;
        }
        //printf("%lc%d", wch, number2);
    }
    if (ferror(stream)) {
        dictionary_done(dict);
        dict = NULL;
    }
    return dict;
}
 
 
 
void words_to_check(const wchar_t *word, struct word_list *li, const struct Node *n, struct list *l) {
    size_t wlen = wcslen(word);
    wchar_t new_word[wlen+1];//[wlen - 1];
    //wchar_t *flunkey;
    //struct list *l;
    //l = NULL;
    struct list *flunkey;
    int j;
 
    for (int i = 0; i < wlen; i++) {
        //new_word = malloc(sizeof(wchar_t)*(wlen-1));
        for (int j = 0; j < i; j++)
            new_word[j] = word[j];
        for (int j = i; j + 1 < wlen; j++)
            new_word[j] = word[j + 1];
        new_word[wlen-1] = '\0';
        //for (int i = 0; i < wlen - 1; i++)
        //    printf("%lc", new_word[i]);
        //printf("\n");
        //printf("DUPA\n");
        printf("stworzone słowo = %ls\n", new_word);
        word_list_add(li, new_word);  
        //struct word_list *l2 = l;
        //int j = 0;
    }
    /// tworzę alfabet, przelatuję szybko drzewo i zapamiętuję każdą literkę bez powtórzeń
    alphabet(n, l); //init vector
    /// dodajemy literkę gdzieś w środku
    flunkey = l;
    while (flunkey != NULL) {
        printf("%lc->", flunkey->letter);
        flunkey = flunkey->next;
    }
    printf("\n");
    printf("drugi for!\n");
    for (int i = 0; i <= wlen; i++) {
        flunkey = l;
        while (l != NULL) {
            for (int k = 0; k < i; k++)
                new_word[k] = word[k];
            new_word[i] = l->letter;
            j = i;
            for (; j < wlen; j++)
                new_word[j + 1] = word[j];
            l = l->next;
            new_word[wlen+1] = '\0';
            printf("Dodajemy słowo %ls\n", new_word);
            word_list_add(li, new_word);
        }
        l = flunkey;     
    }
    /// zamieniamy jedną literkę
    for (int i = 0; i < wlen; i++) {
        flunkey = l;
        while (l != NULL) {
            for (int k = 0; k < i; k++)
                new_word[k] = word[k];
            new_word[i] = l->letter;
            j = i + 1;
            for (; j <= wlen; j++)
                new_word[j] = word[j];
            l = l->next;
            new_word[wlen] = '\0';
            word_list_add(li, new_word);
            printf("słówko: %ls\n", new_word);
        }
        l = flunkey; 
    }

}
 
bool is_char_in_list(struct list *l, wchar_t c) {
    struct list *flunkey;
    flunkey = l;
    while (flunkey != NULL) {
        if (wcsncmp(&flunkey->letter, &c, 1) == 0)
            return true;
        flunkey = flunkey->next;
    }
    return false;
}
 
struct list* add_to_list(struct list *l, wchar_t c) {
    struct list *newList;
    struct list *flunkey;
    if (!is_char_in_list(l, c)) {
        printf("Dodajemy literkę %c!\n", c);
        flunkey = l;
        while (flunkey->next != NULL)
            flunkey = flunkey->next;
        newList = malloc(sizeof(list));
        newList->letter = c;
        newList->next = NULL;
        flunkey->next = newList;
    }
    return l;
}
 
void alphabet(const struct Node *n, struct list *l) {
    for (int i = 0; i < n->vector->size; i++)
        alphabet(n->vector->data[i], l);
    if (n->letter != '-')
        add_to_list(l, n->letter);
}
 
 
void dictionary_hints(const struct dictionary *dict, const wchar_t* word,
        struct word_list *list)
{
    word_list_init(list);
    //size_t wlen = wcslen(word);
    //const wchar_t * const * a = word_list_get(&dict->list);
    /*for (size_t i = 0; i < word_list_size(&dict->list); i++)
 
    {
        size_t len = wcslen(a[i]);
        if (len == wlen - 1)
        {
            if (can_transform_by_delete(word, a[i]))
                word_list_add(list, a[i]);
        }
        else if (len == wlen)
        {
            if (can_transform_by_replace(word, a[i]))
                word_list_add(list, a[i]);
        }
        else if (len == wlen + 1)
        {
            if (can_transform_by_delete(a[i], word))
                word_list_add(list, a[i]);
        }
    }*/
 
 
   
}
