/** @defgroup dict-check Moduł dict-check
  Program sprawdzający pisownię w danym tekście.
  Program zwraca podpowiedzi do słów, dla zapytań, których nie ma w słowniku.
  */
/** 
  @file
  Główny plik modułu dict-check.
  @ingroup dict-check
  @author Jakub Pawlewicz <pan@mimuw.edu.pl>
  @copyright Uniwerstet Warszawski
  @date 2015-05-29
 */


#include "vector.h"
#include "word_list.h"
#include "dictionary.h"
#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wctype.h>
#include <wchar.h>

/** 
  Maksymalna długość słowa.
  Maksymalna długość słowa bez kończącego znaku '\0'
  */
#define MAX_WORD_LENGTH 100


/** 
  Zamienia słowo na złożone z małych liter.
  @param[in,out] word Modyfikowane słowo.
  @return 0, jeśli słowo nie jest złożone z samych liter, 1 w p.p.
  */
int make_lowercase(wchar_t *word)
{
    for (wchar_t *w = word; *w; ++w)
        if (!iswalpha(*w))
            return 0;
        else
            *w = towlower(*w);
    return 1;
}

/**
  Przepisanie tekstu, wypisanie podpowiedzi.
  @param[in]dict, słownik, na którym są wykonywane operacje.
  */
static void dict_check_command(struct dictionary *dict, bool vMode) {
    wchar_t word[MAX_WORD_LENGTH + 1];
    wchar_t wordCopy[MAX_WORD_LENGTH + 1];
    int i = 0;
    wchar_t c;
    int letterPosition = 1;
    int letterLine = 1;
    int isItWord = 0;
    int first = 1;
    struct word_list *l;
    l = malloc(sizeof(word_list));
    word_list_init(l);
    struct word_list *l2;
    while (scanf("%lc", &c) != EOF) {
            letterPosition++;
        if (iswalpha(c)) {
            word[i] = c;
            i++;
            isItWord = 1;
        }
        else {
            if (isItWord == 1) {
                word[i] = '\0';
                i++;
                wordCopy[i] = '\0';
                isItWord = 0;
                wcscpy(wordCopy,word);
                if (make_lowercase(word)) {
                    if (!dictionary_find(dict, word)) {
                        printf("#%ls", wordCopy);
                        if (vMode){
                            if (first == 1) {
                                first = 0;
                                fprintf(stderr, "%d,%d ",letterLine, letterPosition - i);
                            }
                            else 
                                fprintf(stderr, "\n%d,%d ",letterLine, letterPosition - i);
                        }
                        if (vMode)
                            fprintf(stderr, "%ls: ", wordCopy);
                        if (make_lowercase(word)) {
                            dictionary_hints(dict, word, l);
                            l2 = l->next;
                            if (vMode) {
                                while (l2 != NULL) {
                                    if (l2->next != NULL) {
                                        if (dictionary_find(dict, l2->word))
                                            fprintf(stderr, "%ls ", l2->word);
                                    }
                                    else {
                                        if (dictionary_find(dict, l2->word))
                                            fprintf(stderr, "%ls", l2->word);
                                    }
                                    l2 = l2->next;
                                }
                            }
                            word_list_done(l);
                            l->next = NULL;
                        }
                        printf("%lc", c);
                    }
                    else
                        printf("%ls%lc", wordCopy, c);
                }
            }
            else {
                printf("%lc", c);
            }
            i = 0;
        }
        if (c == '\n') {
            letterLine++;
            letterPosition = 1; 
        }
    }
    word_list_done(l);
    free(l);
}

/**
  Funkcja main.
  Główna funkcja programu do testowania słownika. 
  */
int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "pl_PL.UTF-8");
    char *dictionary_name;
    bool vMode = false;
    //printf("%s \n", argv[1]);
    if (argc == 2)
        dictionary_name = argv[1];
    else if (argc == 3) {
        if ((argv[1][0] == '-') && (argv[1][1] == 'v')) {
            vMode = true;
            dictionary_name = argv[2];
        }
        else {
            fprintf(stderr, "Wrong parameters. Try again.\n");
            return 0;
        }
    }
    else {
        fprintf(stderr, "Wrong parameters. Try again.\n");
        return 0;
    }
    struct dictionary *dict;// = dictionary_new();
    FILE *f = fopen(dictionary_name, "r");
    if (!f || !(dict = dictionary_load(f))) {
        fprintf(stderr, "Failed to load dictionary\n");
        exit(1);
    }
    fclose(f);
    //printf("dictionary loaded from file %s\n", dictionary_name);
    dict_check_command(dict, vMode);
    if (vMode)
        fprintf(stderr, "\n");
    dictionary_done(dict);
    return 0;
    
}
