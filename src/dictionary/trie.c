/** @file
  Implementacja drzewa trie.
 
 */

#include "trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>
#include <wchar.h>

void letter_list_done(struct letter_list *l) {
  struct letter_list *flunkey;
  while (l != NULL) {
    flunkey = l;
    l = l->next;
    free(flunkey);
  }
}

void tree_free(struct Node *node)
{
    for (int i = 0; i < node->vector->size; i++) {
        tree_free(node->vector->data[i]);
    }
    node_free(node);
}

void alphabet_init(const struct Node *n, struct letter_list *l)
{
    for (int i = 0; i < n->vector->size; i++)
        alphabet_init(n->vector->data[i], l);
    if (n->letter != '-')
        add_to_list(l, n->letter);
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
