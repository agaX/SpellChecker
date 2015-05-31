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
{
    list->word[0] = '\0';
    list->next = NULL;
}

void word_list_done(struct word_list *list)
{
    struct word_list *flunkey;
    if (list != NULL)
        list = list->next;
    while (list != NULL) {
        flunkey = list;
        list = list->next;
        free(flunkey);
    }
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
            l2 = l2->next;
            flunkey = flunkey->next;
        }
        else if (compare > 0) {
            new_word_list = malloc(sizeof(word_list));
            wcscpy(new_word_list->word, word);
            new_word_list->next = l2;
            flunkey->next = new_word_list;
            return 1;
        }
        else {
            return 0;
        }
    }
    new_word_list = malloc(sizeof(word_list));
    wcscpy(new_word_list->word, word);
    new_word_list->next = NULL;
    flunkey->next = new_word_list;
    return 1;
}


/**@}*/
