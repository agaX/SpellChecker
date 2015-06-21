/** @file
  Dodatkowa funkcjonalność do gtk.
  @ingroup gtk-editor
  @author Agnieszka Kusnierz <ak332284@students.mimuw.edu.pl>
  @date 2015-06-15
  */

#include <gtk/gtk.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "editor.h"
#include "word_list.h"
#include "dictionary.h"
#include "conf.h"

void show_about () {
  GtkWidget *dialog = gtk_about_dialog_new();

  gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "Text Editor");
  //gtk_window_set_title(GTK_WINDOW(dialog), "About Text Editor");
  
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), 
     "Text Editor for IPP exercises\n");
    
  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
}

void show_help (void) {
  GtkWidget *help_window;
  GtkWidget *label;
  char help[5000];

  help_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW (help_window), "Help - Text Editor");
  gtk_window_set_default_size(GTK_WINDOW(help_window), 300, 300);
 
  strcpy(help,
         "\nAby podłączyć usługę spell-checkera do programu trzeba:\n\n"
         "Dołączyć ją do menu 'Spell' w menubar.\n\n"
         "Pobrać zawartość bufora tekstu z edytora: całą lub fragment,\n"
         "  zapamiętując pozycję.\n\n");
  strcat(help, "\0");

  label = gtk_label_new(help);
    
  gtk_container_add(GTK_CONTAINER(help_window), label); 

  gtk_widget_show_all(help_window);
}

/**
  Wyświetlenie listy dostępnych słowników i wczytanie jednego z nich.
  @param[in] item element menu.
  @param[in] data wskaźnik na wartość.
  */
void load_dictionary_from_menu(GtkMenuItem *item, gpointer data) {
  GtkWidget *dialog;
  GtkWidget *vbox, *label, *combo;
  dialog = gtk_dialog_new_with_buttons("Wczytaj słownik", NULL, 0, 
                                       GTK_STOCK_OK,
                                       GTK_RESPONSE_ACCEPT,
                                       GTK_STOCK_CANCEL,
                                       GTK_RESPONSE_REJECT,
                                       NULL);
  // W treści dialogu dwa elementy
  vbox = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  // Tekst
  label = gtk_label_new("Lista dostępnych słowników:");
  gtk_widget_show(label);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);

  // Spuszczane menu
  combo = gtk_combo_box_text_new();
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(CONF_PATH)) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      if (ent->d_name[0] != '.')
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), ent->d_name);
    }
    closedir(dir);
  }
  gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
  gtk_box_pack_start(GTK_BOX(vbox), combo, FALSE, FALSE, 1);
  gtk_widget_show(combo);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    dictionary_save_lang(dict, dictionary_name);
    dictionary_name = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));
    dictionary_done(dict);
    dict = dictionary_load_lang(dictionary_name);
  }
  gtk_widget_destroy(dialog);
}

/**
  Sprawdzenie słowa, na którym aktualnie znajduje się kursor. Ewentualne dodanie do słownika.
  @param[in] item element menu.
  @param[in] data wskaźnik na wartość.
  */
static void WhatCheck (GtkMenuItem *item, gpointer data) {
  GtkWidget *dialog;
  GtkTextIter start, end;
  char *word;
  gunichar *wword;

  //load_dictionary_from_menu(&dict);
  
  // Znajdujemy pozycję kursora
  gtk_text_buffer_get_iter_at_mark(editor_buf, &start,
                                   gtk_text_buffer_get_insert(editor_buf));

  // Jeśli nie wewnątrz słowa, kończymy
  if (!gtk_text_iter_inside_word(&start)) {
    dialog = gtk_message_dialog_new(NULL, 0, GTK_MESSAGE_ERROR,
                                    GTK_BUTTONS_OK,
                                    "Kursor musi być w środku słowa");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }

  // Znajdujemy początek i koniec słowa, a potem samo słowo 
  end = start;
  gtk_text_iter_backward_word_start(&start);
  gtk_text_iter_forward_word_end(&end); 
  word = gtk_text_iter_get_text(&start, &end);

  // Zamieniamy na wide char (no prawie)
  wword = g_utf8_to_ucs4_fast(word, -1, NULL);
  
  if (!make_lowercase(wword)) {
    dialog = gtk_message_dialog_new(NULL, 0, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                    "Podane słowo nie jest słowem słownikowym.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  }
  else {
    // Sprawdzamy
    if (dictionary_find(dict, (wchar_t *)wword)) {
      dialog = gtk_message_dialog_new(NULL, 0, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                      "Wszystko w porządku,\nśpij spokojnie");
      gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
    }
    else {
      // Czas korekty
      GtkWidget *vbox, *label, *combo;
      struct word_list hints;
      int i;
      wchar_t **words;

      dictionary_hints(dict, (wchar_t *)wword, &hints);
      words = (wchar_t **) word_list_get(&hints);
      dialog = gtk_dialog_new_with_buttons("Korekta", NULL, 0, 
                                           GTK_STOCK_OK,
                                           GTK_RESPONSE_ACCEPT,
                                           GTK_STOCK_ADD,
                                           GTK_RESPONSE_APPLY,
                                           GTK_STOCK_CANCEL,
                                           GTK_RESPONSE_REJECT,
                                           NULL);
      // W treści dialogu dwa elementy
      vbox = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
      // Tekst
      label = gtk_label_new("Słowo nie znajduje się w słowniku. Wybierz \njedną z propozycji lub dodaj słowa do słownika.");
      gtk_widget_show(label);
      gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);

      // Spuszczane menu
      combo = gtk_combo_box_text_new();
      for (i = 0; i < word_list_size(&hints); i++) {
        // Combo box lubi mieć Gtk
        char *uword = g_ucs4_to_utf8((gunichar *)words[i], -1, NULL, NULL, NULL);

        // Dodajemy kolejny element
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), uword);
        g_free(uword);
      }
      gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
      gtk_box_pack_start(GTK_BOX(vbox), combo, FALSE, FALSE, 1);
      gtk_widget_show(combo);

      gint click = gtk_dialog_run(GTK_DIALOG(dialog));

      if (click == GTK_RESPONSE_ACCEPT) {
        char *korekta =
          gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));

        // Usuwamy stare
        gtk_text_buffer_delete(editor_buf, &start, &end);
        // Wstawiamy nowe
        gtk_text_buffer_insert(editor_buf, &start, korekta, -1);
        g_free(korekta);
      }
      // Proponujemy dodanie słowa do słownika
      else if (click == GTK_RESPONSE_APPLY)
        dictionary_insert(dict, wword);
      gtk_widget_destroy(dialog);
    }
  }
  g_free(word);
  g_free(wword);
}

/**
  Kolorowanie słów.
  Słowa nie znajdujące się w słowniku - potencjalnie błędne - zostają
  oznaczone na czerwono.
  */
static void show_errors() {
  GtkTextIter start, end, text_end;
  int i, range;
  char *word;
  gunichar *wword;

  gtk_text_buffer_create_tag(editor_buf, "red_fg", 
                             "foreground", "red", 
                             "weight", PANGO_WEIGHT_BOLD, NULL);
  gtk_text_buffer_get_end_iter(editor_buf, &text_end);
  gtk_text_buffer_get_start_iter(editor_buf, &end);
  range = gtk_text_buffer_get_char_count(editor_buf);
  while (!gtk_text_iter_is_end(&end)) {
    gtk_text_iter_forward_word_end(&end); 
    start = end;
    gtk_text_iter_backward_word_start(&start); 

    word = gtk_text_iter_get_text(&start, &end);
    wword = g_utf8_to_ucs4_fast(word, -1, NULL);
    if (make_lowercase(wword)) {
      if (!dictionary_find(dict, wword))
      gtk_text_buffer_apply_tag_by_name(editor_buf, "red_fg", 
                                      &start, &end);
    }
    g_free(word);
    g_free(wword);
  }

}

void extend_menu (GtkWidget *menubar) {
  GtkWidget *spell_menu_item, *spell_menu, *check_item, *load_dictionary_item, *show_errors_item;

  spell_menu_item = gtk_menu_item_new_with_label("Spell");
  spell_menu = gtk_menu_new();
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(spell_menu_item), spell_menu);
  gtk_widget_show(spell_menu_item);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), spell_menu_item);

  check_item = gtk_menu_item_new_with_label("Check Word");
  g_signal_connect(G_OBJECT(check_item), "activate", 
                   G_CALLBACK(WhatCheck), NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(spell_menu), check_item);
  gtk_widget_show(check_item);

  show_errors_item = gtk_menu_item_new_with_label("Check Whole Text");
  g_signal_connect(G_OBJECT(show_errors_item), "activate", 
                   G_CALLBACK(show_errors), NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(spell_menu), show_errors_item);
  gtk_widget_show(show_errors_item);

  load_dictionary_item = gtk_menu_item_new_with_label("Load Dictionary");
  g_signal_connect(G_OBJECT(load_dictionary_item), "activate", 
                   G_CALLBACK(load_dictionary_from_menu), NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(spell_menu), load_dictionary_item);
  gtk_widget_show(load_dictionary_item);

}

/*EOF*/
