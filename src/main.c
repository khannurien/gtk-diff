#include <stdlib.h>
#include <stdio.h>
#include "follow.h"
#include "strhash.h"
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
// retourne -2 si les deux données n'ont pas pu être comparées
int node_compare(s_node * node, void * data) {
	int nodeData = *((int *) list_get_data(node));
	int testData = *((int *) data);

	if (nodeData > testData)
		return 1;
	
	if (nodeData == testData)
		return 0;

	if (nodeData < testData)
		return -1;

	// erreur de comparaison
	return -2;
}

// fonction de test sur les listes
int list_test(void) {
	// tableau de données de test
	int tabTest[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int tabTestbis[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int tabTest2[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	//char tabTest[10] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
	int donneeInf = -1;
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

	// headRemove sur liste vide
	if ((firstList = list_headRemove(firstList)) == NULL) {
		printf("headRemove sur liste vide OK. Affichage :\n");
		list_dump(firstList);
	} else {
		printf("Erreur suppression de la tête de liste.\n");
	}

	// remove sur liste vide
	if ((firstList = list_remove(firstList, &donneeSup)) == NULL) {
		printf("remove sur liste vide OK. Affichage :\n");
		list_dump(firstList);
	} else {
		printf("Erreur suppression de la tête de liste.\n");
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

	// list_append sur liste vide
	if ((firstList = list_append(firstList, &tabTest[2])) != NULL) {
		printf("Insertion en queue OK. Affichage :\n");
		list_dump(firstList);
	} else {
		printf("Erreur insertion en queue de liste.\n");
	}
	// list_append
	if ((firstList = list_append(firstList, &tabTest[2])) != NULL) {
		printf("Insertion en queue OK. Affichage :\n");
		list_dump(firstList);
	} else {
		printf("Erreur insertion en queue de liste.\n");
	}
	// list_append
	if ((firstList = list_append(firstList, &tabTest[2])) != NULL) {
		printf("Insertion en queue OK. Affichage :\n");
		list_dump(firstList);
	} else {
		printf("Erreur insertion en queue de liste.\n");
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
	if ((aLast = (s_node **) malloc(sizeof(s_node *))) == NULL) return 1;
	if ((list_process(newListe, &incremente, &param, aLast)) == 1)
		printf("Erreur list_process.\n");
	printf("Valeurs après list_process :\n");
	list_dump(newListe);
	// suppression de la liste
	list_destroy(newListe);
	// libération de ** aLast
	free(aLast);

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
	printf("Suppression de la dernière valeur (10)\n");
	orderedList = list_remove(orderedList, &tabTestbis[9]);
	list_dump(orderedList);
	// insertion d'une donnée
	printf("Insertion de la dernière valeur (10)\n");
	nodeTest = orderedList_insert(&orderedList, &node_compare, &tabTestbis[9]);
	list_dump(orderedList);
	list_dump(nodeTest);
	// suppression d'une valeur
	printf("Suppression de la première valeur (1)\n");
	orderedList = list_remove(orderedList, &tabTestbis[0]);
	list_dump(orderedList);
	// insertion d'une donnée
	printf("Insertion de la première valeur (1)\n");
	nodeTest = orderedList_insert(&orderedList, &node_compare, &tabTestbis[0]);
	list_dump(orderedList);
	list_dump(nodeTest);
	// suppression d'une donnée en milieu de liste
	printf("Suppression d'une donnée en milieu de liste (5)\n");
	orderedList = list_remove(orderedList, &tabTestbis[4]);
	list_dump(nodeTest);
	// insertion d'une donnée en milieu de liste
	printf("Insertion d'une donnée en milieu de liste (5)\n");
	nodeTest = orderedList_insert(&orderedList, &node_compare, &tabTestbis[4]);
	list_dump(orderedList);
	list_dump(nodeTest);
	// insertion d'une donnée plus grande
	printf("Insertion d'une donnée plus grande que toutes\n");
	nodeTest = orderedList_insert(&orderedList, &node_compare, &donneeSup);
	list_dump(orderedList);
	list_dump(nodeTest);
	// insertion d'une donnée plus petite
	printf("Insertion d'une donnée plus petite que toutes\n");
	nodeTest = orderedList_insert(&orderedList, &node_compare, &donneeInf);
	list_dump(orderedList);
	list_dump(nodeTest);

	// list_set_data
	printf("Test de list_set_data\n");
	list_set_data(orderedList, &tabTest[9]);
	list_dump(orderedList);
	// suppression de la dernière liste
	list_destroy(orderedList);

	// tests unitaires OK :-)
	return 0;
}

int hash_test(void) {
	// données de test
	char * tabTest[] = {"héhé", "blablablablablabla", "coucoucoucou", "", "72438202355"};
	char * tabTest2[] = {"okokok", "ubo", "Aurevoir"};
	char * testChar = "testChar";

	// création d'une hashmap
	hashmap * newMap;

	// hashmap vide
	printf("Création et affichage d'une hashmap vide :\n");
	if ((newMap = hashmap_create(10)) == NULL) {
		printf("Erreur à l'allocation de la hashmap.\n");
		return 1;
	}
	hash_dump(newMap);

	hashmap_remove(newMap, tabTest[1]);

	// insertions
	printf("Insertion d'un élément (\"blablablablablabla\") dans une hashmap vide :\n");
	char * wordCheck;
	wordCheck = hashmap_insert(newMap, tabTest[1]);
	printf("Retour de hashmap_insert : %s\n", wordCheck);
	hash_dump(newMap);

	printf("Insertion du même élément (\"blablablablablabla\") une seconde fois :\n");
	hashmap_insert(newMap, tabTest[1]);
	hash_dump(newMap);

	printf("Suppression d'un élément (\"blablablablablabla\") sans pointeur :\n");
	int removeCheck;
	removeCheck = hashmap_remove(newMap, "blablablablablabla");
	printf("Retour de hashmap_remove : %d\n", removeCheck);
	hash_dump(newMap);

	printf("Insertion d'un autre élément (\"coucoucoucou\") :\n");
	hashmap_insert(newMap, tabTest[2]);
	hash_dump(newMap);

	printf("Insertion d'un autre élément (\"héhé\") :\n");
	hashmap_insert(newMap, tabTest[0]);
	hash_dump(newMap);

	printf("Insertion d'un autre élément (\"72438202355\"):\n");
	hashmap_insert(newMap, tabTest[4]);
	hash_dump(newMap);

	// données statistiques
	printf("Affichage de données statistiques :\n");
	hashmap_stats(newMap);

	// insertion des éléments du second tableau
	printf("Ajout de trois nouveaux éléments :\n");
	hashmap_insert(newMap, tabTest2[0]);
	hashmap_insert(newMap, tabTest2[1]);
	hashmap_insert(newMap, tabTest2[2]);
	hash_dump(newMap);

	// suppression des éléments du premier tableau
	printf("Suppression des anciens éléments :\n");
	hashmap_remove(newMap, tabTest[0]);
	hashmap_remove(newMap, tabTest[1]);
	hashmap_remove(newMap, tabTest[2]);
	hashmap_remove(newMap, tabTest[4]);
	hash_dump(newMap);

	// remise à zéro
	printf("Remise à zéro de la hashmap.\n");
	hashmap_empty(newMap);
	hash_dump(newMap);

	// seconde hashmap
	printf("Création d'une seconde hashmap :\n");
	hashmap * secondMap;
	if ((secondMap = hashmap_create(10)) == NULL) {
		printf("Erreur à l'allocation de la hashmap.\n");
		return 1;
	}
	hash_dump(secondMap);

	// suppression d'un élément dans la hashmap vide
	printf("Suppression d'un élément absent dans la hashmap vide (\"coucoucoucou\") :\n");
	hashmap_remove(secondMap, tabTest[2]);
	hash_dump(secondMap);

	// ré-insertion du même élément
	printf("Insertion d'un autre élément (chaîne vide) :\n");
	hashmap_insert(secondMap, tabTest[3]);
	hash_dump(secondMap);
	printf("Insertion du même élément (chaîne vide), à nouveau :\n");
	hashmap_insert(secondMap, tabTest[3]);
	hash_dump(secondMap);

	// suppression d'un élément absent
	printf("Suppression d'un élément absent de la hashmap (\"coucoucoucou\") :\n");
	hashmap_remove(secondMap, tabTest[2]);
	hash_dump(secondMap);

	// destruction de la hashmap
	printf("Destruction de la hashmap.\n");
	if (hashmap_destroy(newMap) == 1) {
		printf("Erreur lors de la libération des listes chaînées.\n");
		return 1;
	}

	// tests unitaires OK :-)
	return 0;
}

int follow_test(void) {
	// chemin des fichiers
	char * refFile = "data/testRef.txt";
	char * newFile = "data/testNew.txt";
	// chargement d'un premier texte
	text * refText;
	if ((refText = text_load(refFile)) == NULL)
		return 1;
	// et de sa version modifiée
	text * newText;
	if ((newText = text_load(newFile)) == NULL)
		return 1;

	// affichage du texte d'origine
	printf("%s\n", refText->text);
	printf("%d\n", refText->textSize);

	// création d'un nouveau follow
	follow * newFollow;
	newFollow = follow_init();
	// association au texte d'origine et à sa version modifiée
	newFollow->pRefText = refText;
	newFollow->pNewText = newText;

	// tokenisation du texte
	printf("Tokenisation du texte d'origine...\n");
	text_tokenize(newFollow->map, refText);
	printf("Tokenisation du texte modifié...\n");
	text_tokenize(newFollow->map, newText);

	// affichage de la hashmap
	printf("Affichage de la hashmap après tokenisation :\n");
	hash_dump(newFollow->map);

	// tests unitaires OK :-)
	return 0;
}

// ensemble des tests
int main(int argc, char * argv[], char * envp[]) {
	if ((list_test()) == 0)
		printf("Tests unitaires sur les listes : OK.\n\n");

	printf("\n==============================\n\n");

	if ((hash_test()) == 0)
		printf("Tests unitaires sur les hashmaps : OK.\n\n");

	printf("\n==============================\n\n");

	if ((follow_test()) == 0)
		printf("Tests unitaires sur follow : OK.\n\n");
	else if ((follow_test()) == 1)
		printf("[follow] Erreur de lecture des fichiers.\n\n");

	return 0;
}
