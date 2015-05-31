/** @defgroup dictionary Moduł dictionary
    Biblioteka obsługująca słownik.
  */
/** @file
    Interfejs wektora.

    @ingroup dictionary
    @author Jakub Pawlewicz <pan@mimuw.edu.pl>
    @copyright Uniwerstet Warszawski
    @date 2015-05-27
 */


#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <wchar.h>

#define VECTOR_INITIAL_CAPACITY 9


/**
  Struktura będąca węzłem drzewa PATRICIA.
 */
typedef struct Node {
	///Litery przechowywane w węźle.
	wchar_t letter;
	///Liczba synów (używane do save'a).
	int toBuild;
	///1 - jest słowe; 0 - nie jest.
	int isInWord;
	///Wektor wskaźników do node'ów.
	struct Vector *vector;
	///Wskaźnik na rodzica.
	struct Node *parent;
} Node;

/**
  Struktura definiująca wektor.
 */
typedef struct Vector {
  ///Aktualne liczba elementów w wektorze.
  int size;
  ///Aktualna pojemność wektora.
  int capacity;
  ///Tablica wskaźników do node'ów przechowywanych w wektorze.
  Node **data;
} Vector;

/**
  Inicjalizacja wektora
  @param[in]vector zostaje zainicjalizowany
  */
void vector_init(Vector *vector);

/**
  Dodanie elementu na koniec wektora
  @param[in]vector doniego wstawiamy element
  @param[in]node węzeł, który wstawiamy
  */
void vector_append(Vector *vector, Node *value);

/**
  Usuwanie konkretnego elementu z wektora
  @param[in]vector wektor, z którego zostanie usuniety element
  @param[in]node element do usunięcia
  */
void vector_remove(Vector *vector, Node *node);

/**
  Wstawienie node w konkretne miejsce
  @param[in]vector wektor, do którego zostanie wstawiony element
  @param[in]node który zostaje wstawiony
  @param[in]index indeks, na który wstawiamy element
  */
void insert_an_index(Vector *vector, Node *node, int index);

/**
  Podwojenie rozmatu wektora.
  @param[in]vector którego pojemność zwiększamy.
  */
void vector_double_capacity_if_full(Vector *vector);

/**
  Czyszczenie wektora.
  @param[in]vector, który zostanie wypisany.
  */
void vector_free(Vector *vector);

/**
  Wypisanie wektora
  @param[in]wektor, który zostaje wypisany
  */
void write_vector(Vector *vector);

#endif /* __VECTOR_H__ */
