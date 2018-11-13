#include <stdlib.h>
#include <stdio.h>
#include "list.h"

int main(int argc, char * argv[], char * envp[]) {
	int test = 50;
	int test2 = 100;
	int test3 = 200;
	int test1337 = 1337;

	s_node * maListe;

	printf("Liste vide\n");
	// liste vide
	maListe = list_create();
	list_dump(maListe);
	// liste vide : ajout au début
	maListe = list_insert(maListe, &test);
	list_dump(maListe);
	maListe = list_headRemove(maListe);
	list_dump(maListe);
	// liste vide : ajout à la fin
	maListe = list_append(maListe, &test2);
	maListe = list_append(maListe, &test3);
	list_dump(maListe);
	maListe = list_remove(maListe, &test3);
	list_dump(maListe);
	maListe = list_remove(maListe, &test2);
	list_dump(maListe);
	// suppression de la liste
	list_destroy(maListe);

	printf("\nListe à 1 élément\n");
	// liste à 1 élément
	maListe = list_create();
	maListe = list_insert(maListe, &test);
	list_dump(maListe);
	// suppression au début
	maListe = list_headRemove(maListe);
	list_dump(maListe);
	// suppression d'un élément absent au début
	maListe = list_headRemove(maListe);
	list_dump(maListe);
	// suppression d'un élément quelconque absent
	maListe = list_remove(maListe, &test3);
	list_dump(maListe);
	// suppression de la liste
	list_destroy(maListe);

	printf("\nListe pleine\n");
	// liste pleine
	maListe = list_create();
	// insertions au début
	maListe = list_insert(maListe, &test);
	maListe = list_insert(maListe, &test);
	maListe = list_insert(maListe, &test);
	list_dump(maListe);
	// modification de la première valeur
	list_set_data(maListe, &test1337);
	list_dump(maListe);
	// insertions à la fin
	maListe = list_append(maListe, &test2);
	maListe = list_append(maListe, &test3);
	list_dump(maListe);
	// suppression de la tête
	maListe = list_headRemove(maListe);
	list_dump(maListe);
	// suppression de la première instance de test2
	maListe = list_remove(maListe, &test2);
	list_dump(maListe);
	// une deuxième fois
	maListe = list_remove(maListe, &test2);
	list_dump(maListe);
	// suppression de la liste
	//list_destroy(maListe);
	//list_dump(maListe);

	//int incremente(s_node * node, void * param) {
	//	node->val = node->val + param;
	//	return 0;
	//}
	// param = 1;
	// if (s_node ** last = (s_node **) malloc(sizeof(s_node*)) == NULL) return NULL;
	// list_process(maListe, (incremente(s_node * node, void * param)), &param, last);

	return EXIT_SUCCESS;
}