#include <stdlib.h>
#include <stdio.h>
#include "list.h"

// fonction passée à list_process afin de la tester
// incrémente la valeur référencée dans node->data
int incremente(s_node * node, void * param) {
	// typecast
	int * data = (int *) list_get_data(node);
	int * intParam = (int *) param;
	// modification de la valeur
	* data += (* intParam);

	return 0;
}

int main(int argc, char * argv[], char * envp[]) {
	// tableau de données de test
	int tabTest[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	//char tabTest[10] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};

	// création d'une liste
	s_node * uneListe = list_create();
	// affichage
	list_dump(uneListe);
	// remplissage d'une liste
	for (int i = 0; i < 10; i++) 
		uneListe = list_append(uneListe, &tabTest[i]);
	// affichage
	printf("Liste 1 à 10 :\n");
	list_dump(uneListe);

	// création d'une seconde liste vide
	s_node * maListe = list_create();
	// affichage
	list_dump(maListe);
	// liste vide : ajout au début
	printf("list_insert\n");
	maListe = list_insert(maListe, &tabTest[0]);
	list_dump(maListe);
	printf("list_headRemove\n");
	maListe = list_headRemove(maListe);
	list_dump(maListe);
	// liste vide : ajout à la fin
	printf("list_append\n");
	maListe = list_append(maListe, &tabTest[1]);
	maListe = list_append(maListe, &tabTest[1]);
	list_dump(maListe);
	printf("list_remove\n");
	maListe = list_remove(maListe, &tabTest[1]);
	maListe = list_remove(maListe, &tabTest[1]);
	list_dump(maListe);
	// suppression de la liste
	printf("Suppression de la seconde liste\n");
	list_destroy(maListe);

	// test list_process
	printf("Test list_process sur une nouvelle liste\n");
	s_node * newListe = list_create();
	newListe = list_insert(newListe, &tabTest[4]);
	newListe = list_insert(newListe, &tabTest[5]);
	newListe = list_insert(newListe, &tabTest[6]);
	printf("Valeurs initiales :\n");
	list_dump(newListe);
	// appel list_process avec la fonction incremente
	int param = 1;
	s_node ** aLast;
	if ((aLast = (s_node **) malloc(sizeof(s_node*))) == NULL) return 1;
	int processReturn;
	if ((processReturn = list_process(newListe, &incremente, &param, aLast)) == 1)
		printf("Erreur list_process.\n");
	printf("Valeurs après list_process :\n");
	list_dump(newListe);

	return EXIT_SUCCESS;
}
