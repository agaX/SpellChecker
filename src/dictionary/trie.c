#include <stdio.h>
#include <wchar.h>
#include <stddef.h>
#include <locale.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node {
	wchar_t letter;
	int isItWord;
	int numberOfChildren;
	struct Node *children[32];
	struct Node *parent;
} Node;

int insert(Node *node, wchar_t word[100]) {
	Node *newNode;
	int indexOfArray = 0; //to compile
	int currentIndex = 0; 
	if (node != NULL) {
		while (word[currentIndex] != 0) {
			if  (node->children[indexOfArray] == NULL) {
				newNode = malloc(sizeof(Node));
				for (int i = 0; i < 32; i++)
					node->children[i] = NULL;
				newNode->numberOfChildren = 0;
				newNode->parent = node;
				if (word[currentIndex + 1] == 0) {
					newNode->isItWord = 1;
					return 1;
				}
				node->children[indexOfArray] = newNode;
				newNode->letter = word[currentIndex];
				currentIndex++;
				node->numberOfChildren++;
				node = newNode;
			}
			else {
				node = node->children[indexOfArray];
				if (word[currentIndex + 1] == 0) {
					if (node->isItWord == 1)
						return 0;
					else {
						node->isItWord = 1;
						return 1;
					}	
				}
				currentIndex++;
			}
		}
	}
	return 0;
}

int find(Node *node, wchar_t word[100]) {
	int stop = 0;
	int indexOfArray = 0; //to compile
	int currentIndex = 0;
	while (stop != 1) {
		if (node->children[indexOfArray] != NULL) 
			node = node->children[indexOfArray];
		else return 0;
		currentIndex++;
		if (word[currentIndex] == 0) {
			if (node->isItWord == 0)
				return 0;
			else
				return 1;
		}
	}
	return 0;
}

int delete(Node *node, wchar_t word[100]) {
	Node *flunkey;
	int stop = 0;
	int indexOfArray = 0; //to compile
	int currentIndex = 0;
	int wasCleaned = 0;
	while (stop != 1) {
		if (node->children[indexOfArray] != NULL) //indexOfArray, ma mieć nr odpowiadający wchar_T word[currentIndex] :)
			node = node->children[indexOfArray];
		else return 0;
		currentIndex++;
		if (word[currentIndex] == 0) {
			if (node->isItWord == 0)
				return 0;
			else {
				/* słowo się skończyło i jesteśmy w node'dzie, który jest słowem,
				jeśli ma synów, to tylko mówimy, że już nie jest końcem słowa,
				jeśli nie ma synów to go usuwamy */
				if (node->numberOfChildren == 0) {  
					flunkey = node;
					node = node->parent;
					free(flunkey);
					node->children[indexOfArray] = NULL;
					currentIndex--;
					node->numberOfChildren--;
				}
				else {
					/* zmieniliśmy, nie jest już słowem */
					node->isItWord = 0;
					return 1;
				}
				/*teraz jeśli jesteśmy w "końcu" słowa, to na pewno nie jest to nasz koniec słowa,
				więc wychodzimy z delete'a, a jeśli to nie jest koniec słowa, to sprawdzamy, czy ma dzieciaki,
				jeśli ma, to wychodzimy, jeśli nie ma, to kasujemy node'a*/
				while (node != NULL) {
					if (node->isItWord == 1)
						return 1;
					else {
						if (currentIndex >= 0) {
							if (node->numberOfChildren == 0) {  
								flunkey = node;
								node = node->parent;
								free(flunkey);
								node->children[indexOfArray] = NULL;
								currentIndex--;
								node->numberOfChildren--;
							}
							else
								return 1;
						}
						else
							return 1;
					}
				}
			}
		}
	}
	return 0;
}






/*

int indexOfArray() {

}
*/


/*
int main() {
	wchar_t ws1;
	wchar_t ws2;
	setlocale(LC_ALL, "en_PL.UTF-8");
	scanf("%lc",ws1);
	scanf("%ls",ws2);
	printf("wcscoll(const wchar_t *ws1, const wchar_t *ws2)");

}
*/

int main() {

    wchar_t string[100];

	setlocale(LC_ALL, "en_PL.UTF-8");

    printf ("Enter a string: ");
    scanf("%ls",string);

    printf("String Entered: %ls: length: %zu", string, wcslen(string));

    return 0;
}
