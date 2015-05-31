/** @file
  Implementacja słownika.
  Słownika przechowuje słowa w drzewie PATRICIA.
 
  @ingroup dictionary
  @author Jakub Pawlewicz <pan@mimuw.edu.pl>
  @copyright Uniwerstet Warszawski
  @date 2015-05-11
 */
 
#include "dictionary.h"
#include "conf.h"
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include <assert.h>
#include <locale.h>
#include <string.h>
#include <wctype.h>
#include <wchar.h>
 
#define _GNU_SOURCE

void letter_list_done(struct letter_list *l) {
  struct letter_list *flunkey;
  while (l != NULL) {
    flunkey = l;
    l = l->next;
    free(flunkey);
  }
}

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
 
 
 
static void skip_equal(const wchar_t **a, const wchar_t **b)
{
    while (**a == **b && **a != L'\0')
    {
        (*a)++;
        (*b)++;
    }
}

/**
  Zwraca czy słowo `a` można zamienić w `b` przez usunięcie znaku.
  @param[in] a Dłuższe słowo.
  @param[in] b Krótsze słowo.
  @return 1 jeśli się da zamienić `a` w `b` przez usunięcia znaku, 0 w p.p.
 */
static int can_transform_by_delete(const wchar_t *a, const wchar_t *b)
{
    //struct word_list *list;
 
    skip_equal(&a, &b);
    a++;
    skip_equal(&a, &b);
    return *a == L'\0' && *b == L'\0';
}

/**
  Zwraca czy słowo `a` można zamienić w `b` przez zamianę znaku.
  @param[in] a Pierwsze słowo.
  @param[in] b Drugie słowo.
  @return 1 jeśli się da zamienić `a` w `b` przez zamianę znaku, 0 w p.p.
 */
static int can_transform_by_replace(const wchar_t *a, const wchar_t *b)
{
    skip_equal(&a, &b);
    a++; b++;
    skip_equal(&a, &b);
    return *a == L'\0' && *b == L'\0';
}

void dictionary_done(struct dictionary *dict)
{
    dictionary_free(dict);
    free(dict);
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

int dictionary_insert(struct dictionary *dict, const wchar_t *word)
{
    int length = wcslen(word);
    int number;
    int i = 0;
    struct Node *node;
    struct Node *newNode;
    node = dict->root;
    for (i = 0; i < length; i++) {
        number = where_in_vector(node->vector, word[i]);
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

int dictionary_delete(struct dictionary *dict, const wchar_t *word)
{
    struct Node *node;
    struct Node *parent;
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

bool dictionary_find(const struct dictionary *dict, const wchar_t *word)
{
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

int dictionary_save_node(const struct Node *node, FILE* stream)
{
    if (node->isInWord == 1) {
        if (fprintf(stream, "%lc", node->letter) < 0)
            return -1;
        else if (fprintf(stream, "%c", '.') < 0)
            return -1;
    }
    else if (fprintf(stream, "%lc", node->letter) < 0)
        return -1;
    else if (fprintf(stream, "%c", '-') < 0)
        return -1;
    if (fprintf(stream, "%d", node->vector->size) < 0)
        return -1;
    for (int i = 0; i < node->vector->size; i++)
        dictionary_save_node(node->vector->data[i], stream);
    return 0;
}

int dictionary_save(const struct dictionary *dict, FILE* stream) {
    dictionary_save_node(dict->root, stream);
    if (fprintf(stream, "#") < 0)
        return -1;
    return 0;
}

struct dictionary * dictionary_load(FILE* stream)
{
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
        if (wch != '-') {
            n = malloc(sizeof(Node));
            if (char1 == '.') {
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
    }
    if (ferror(stream)) {
        dictionary_done(dict);
        dict = NULL;
    }
    return dict;
}

void words_to_check(const wchar_t *word, struct word_list *li, const struct dictionary *dict, struct letter_list *l)
{
    size_t wlen = wcslen(word);
    wchar_t new_word[wlen + 1];
    struct letter_list *flunkey;
    int j;
    ///Słowa powstałe przez usunięcie jednej z liter.
    for (int i = 0; i < wlen; i++) {
        for (int j = 0; j < i; j++)
            new_word[j] = word[j];
        for (int j = i; j + 1 < wlen; j++)
            new_word[j] = word[j + 1];
        new_word[wlen-1] = '\0';
        if (dictionary_find(dict, new_word))
            word_list_add(li, new_word);  
    }
    ///Tworzenie alfabetu, każda litera występująca w słowniku bez powtórzeń.
    alphabet(dict->root, l);
    ///Słowa, które powstają przez dodanie dowolnej litery w dowolnym miejscu.
    flunkey = l;
    while (flunkey != NULL) {
        flunkey = flunkey->next;
    }
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
            if (dictionary_find(dict, new_word))
                word_list_add(li, new_word); 
        }
        l = flunkey;     
    }
    ///Słowa powstałe przez zamianę jednej litery na dowolną inną.
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
            if (dictionary_find(dict, new_word))
                word_list_add(li, new_word); 
        }
        l = flunkey; 
    }
}

bool is_char_in_list(struct letter_list *l, wchar_t c)
{
    struct letter_list *flunkey;
    flunkey = l;
    while (flunkey != NULL) {
        if (wcsncmp(&flunkey->letter, &c, 1) == 0)
            return true;
        flunkey = flunkey->next;
    }
    return false;
}

struct letter_list* add_to_list(struct letter_list *l, wchar_t c)
{
    struct letter_list *newList;
    struct letter_list *flunkey;
    if (!is_char_in_list(l, c)) {
        flunkey = l;
        while (flunkey->next != NULL)
            flunkey = flunkey->next;
        newList = malloc(sizeof(letter_list));
        newList->letter = c;
        newList->next = NULL;
        flunkey->next = newList;
    }
    return l;
}

void alphabet(const struct Node *n, struct letter_list *l)
{
    for (int i = 0; i < n->vector->size; i++)
        alphabet(n->vector->data[i], l);
    if (n->letter != '-')
        add_to_list(l, n->letter);
}

void dictionary_hints(const struct dictionary *dict, const wchar_t* word,
        struct word_list *list)
{
    word_list_init(list);
    struct letter_list *l;
    l = malloc(sizeof(letter_list));
    l->letter = '#';
    l->next = NULL;
    words_to_check(word, list, dict, l);
    letter_list_done(l);
}
