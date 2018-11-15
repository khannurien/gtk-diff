#include <stdlib.h>
#include <stdio.h>
#include "list.h"

// fonction passée à list_process afin de la tester
// incrémentation de la donnée d'un noeud
int incremente(s_node * node, void * param) {
	// typecast
	int * data = (int *) list_get_data(node);
	int * intParam = (int *) param;
	// modification de la valeur
	* data += (* intParam);

	return 0;
}
// fonction passée à orderedList_insert afin de comparer deux noeuds
// retourne 1 si la donnée du noeud est strictement supérieure à la donnée passée
// retourne 0 sur la donnée du noeud est égale à la donnée passée
// retourne -1 si la donnée du noeud est strictement inférieure à la donnée passée
int node_compare(s_node * node, void * data) {
	if (*((int*) list_get_data(node)) > *((int*) data))
		return 1;
	
	if (*((int*) list_get_data(node)) == *((int*) data))
		return 0;

	if (*((int*) list_get_data(node)) < *((int*) data))
		return -1;
}

// main de tests sur les listes
int main(int argc, char * argv[], char * envp[]) {
	// tableau de données de test
	int tabTest[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int tabTestbis[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int tabTest2[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	//char tabTest[10] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
	int donneeSup = 11;

	// création d'une liste
	s_node * firstList;

	// tests
	if ((firstList = list_create()) == NULL) {
		printf("Liste vide OK. Affichage :\n");
		list_dump(firstList);
	} else {
		printf("Erreur lors de la création de la liste.\n");
	}

	// ajout en tête
	if ((firstList = list_insert(firstList, &tabTest[0])) != NULL) {
		printf("Insertion en tête OK. Affichage :\n");
		list_dump(firstList);
	} else {
		printf("Erreur insertion en tête de liste.\n");
	}
	// second ajout en tête
	if ((firstList = list_insert(firstList, &tabTest[1])) != NULL) {
		printf("Insertion en tête OK. Affichage :\n");
		list_dump(firstList);
	} else {
		printf("Erreur insertion en tête de liste.\n");
	}

	// ajout en queue de liste
	if ((firstList = list_append(firstList, &tabTest[2])) != NULL) {
		printf("Insertion en queue OK. Affichage :\n");
		list_dump(firstList);
	} else {
		printf("Erreur insertion en queue de liste.\n");
	}
	// second ajout en queue de liste
	if ((firstList = list_append(firstList, &tabTest[3])) != NULL) {
		printf("Insertion en queue OK. Affichage :\n");
		list_dump(firstList);
	} else {
		printf("Erreur insertion en queue de liste.\n");
	}

	// suppression d'une valeur connue dans la liste
	if ((firstList = list_remove(firstList, &tabTest[1])) != NULL) {
		printf("Suppression de '2' OK. Affichage :\n");
		list_dump(firstList);
	}

	// suppression des noeuds un à un
	while (firstList != NULL) {
		if ((firstList = list_headRemove(firstList)) != NULL) {
			printf("Noeud supprimé.\n");
		} else {
			printf("Dernier noeud supprimé. Affichage :\n");
			list_dump(firstList);
		}
	}

	// suppression de la liste
	list_destroy(firstList);
	printf("Liste supprimée.\n\n");

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
	if ((list_process(newListe, &incremente, &param, aLast)) == 1)
		printf("Erreur list_process.\n");
	printf("Valeurs après list_process :\n");
	list_dump(newListe);
	// suppression de la liste
	list_destroy(newListe);

	// création d'une liste ordonnée
	s_node * nodeTest;
	printf("Test orderedList_insert sur liste ordonnée\n");
	s_node * orderedList = list_create();
	for (int i = 0; i < 10; i++) 
		orderedList = list_append(orderedList, &tabTestbis[i]);
	// affichage
	printf("Liste ordonnée croissante de 1 à 10 :\n");
	list_dump(orderedList);
	// suppression d'une valeur
	printf("Suppression de la dernière valeur\n");
	orderedList = list_remove(orderedList, &tabTestbis[9]);
	list_dump(orderedList);
	// insertion d'une donnée
	printf("Insertion de la dernière valeur\n");
	nodeTest = orderedList_insert(&orderedList, &node_compare, &tabTestbis[9]);
	list_dump(orderedList);
	// suppression d'une valeur
	printf("Suppression de la première valeur\n");
	orderedList = list_remove(orderedList, &tabTestbis[0]);
	list_dump(orderedList);
	// insertion d'une donnée
	printf("Insertion de la première valeur\n");
	nodeTest = orderedList_insert(&orderedList, &node_compare, &tabTestbis[0]);
	list_dump(orderedList);
	// insertion d'une donnée plus grande
	printf("Insertion d'une donnée plus grande que toutes\n");
	nodeTest = orderedList_insert(&orderedList, &node_compare, &donneeSup);
	list_dump(orderedList);
	list_dump(nodeTest);

	// list_set_data
	printf("Test de list_set_data\n");
	list_set_data(orderedList, &tabTest[9]);
	list_dump(orderedList);
	// suppression de la dernière liste
	list_destroy(orderedList);

	return EXIT_SUCCESS;
}
