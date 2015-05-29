/** @file
  Implementacja listy słów.

  @ingroup dictionary
  @author Jakub Pawlewicz <pan@mimuw.edu.pl>
  @copyright Uniwerstet Warszawski
  @date 2015-05-10
 */

#include "word_list.h"
#include <stdlib.h>

/** @name Elementy interfejsu 
   @{
 */

void word_list_init(struct word_list *list)
{/*
    word_list *new = malloc(sizeof(word_list));
    new->next = NULL;*/
}

void word_list_done(struct word_list *list)
{
}

int word_list_add(struct word_list *list, const wchar_t *word) {
    struct word_list *l2 = list;
    struct word_list *flunkey;
    struct word_list *new_word_list;
    
    int compare;

    flunkey = list;
    l2 = list->next;
    while (l2 != NULL) {
        compare = wcscoll(l2->word, word);
        if (compare < 0) {
            //printf("większe\n");
            l2 = l2->next;
            flunkey = flunkey->next;
        }
        else if (compare > 0) {
            //printf("mniejsze\n");
            new_word_list = malloc(sizeof(word_list));
            wcscpy(new_word_list->word, word);
            new_word_list->next = l2;
            flunkey->next = new_word_list;
            return 1;
        }
        else {
            //printf("równe\n");
            return 0;
        }

    }
    new_word_list = malloc(sizeof(word_list));
    wcscpy(new_word_list->word, word);
    new_word_list->next = NULL;
    flunkey->next = new_word_list;
    return 1;
}
   
/*    if (list->size >= WORD_LIST_MAX_WORDS)
        return 0;
    size_t len = wcslen(word) + 1;
    if (list->buffer_size + len > WORD_LIST_SUM)
        return 0;
    wchar_t *pos = list->buffer + list->buffer_size;
    list->array[list->size++] = pos;
    wcscpy(pos, word);
    list->buffer_size += len;
    return 1;
}*/

/**@}*/
