#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "strhash.h"

// fonction passée à orderedList_insert afin de comparer deux chaînes de caractères
// retourne :
// > 0 si nodeString > datastring
// < 0 si nodeString < dataString
// 0 si nodeString == dataString
int string_compare(s_node * node, void * data) {
	char * nodeString = (char *) list_get_data(node);

	return strcmp(nodeString, data);
}

// fonction de libération des données des listes passée à list_process
int list_free(s_node * node, void * data) {
	free(node);
	return 0;
}

// fonction passée à list_process afin de retourner le noeud à supprimer
// retourne 1 quand les chaînes de caractères sont identiques
int string_remove(s_node * node, void * data) {
	char * nodeString = (char *) list_get_data(node);

	if ((strcmp(nodeString, data)) == 0) {
		return 1;
	}

	return 0;
}

// fonction de test : affichage d'une hashmap
void hash_dump(hashmap * map) {
	printf("\n==============================\n\n");

	int i;
	int cpt = 0;

	superlist * chain = map->tab;
	for (i = 0; i < map->size; i++) {
		s_node * head = chain[i].head;
		strlist_dump(head);
		cpt++;
	}

	//printf("Nombre de listes : %d\n", cpt);
	printf("\n==============================\n\n");
}

// calcul du hash pour une chaîne de caractères
static unsigned int hash_calc(char * str, int mapSize) {
	// initialisation de la clef
	int key = 0;

	// à chaque caractère, on multiplie la clef par deux et on lui ajoute
	// le code ASCII du caractère
	while ((*str) != '\0') {
		//key *= 2; // décalage à gauche
		key <<= 1;
		key += (int) (*str);
		str++;
	}

	// on garde dans la clef le reste de sa division entière par la taille de la hashmap
	key %= mapSize;

	// on retourne le hash
	return key;
}

// création d'une table de hashage
hashmap * hashmap_create() {
	// allocation de la hashmap
	hashmap * map;
	if ((map = (hashmap *) malloc(sizeof(hashmap))) == NULL)
		return NULL;

	// initialisation d'une hashmap vide
	map->size = HASHMAP_INIT_SIZE;

	// allocation des listes chaînées
	if ((map->tab = (superlist *) malloc(HASHMAP_INIT_SIZE * sizeof(superlist))) == NULL) {
		free(map);
		return NULL;
	}

	// création des listes chaînées
	int i;
	for (i = 0; i < HASHMAP_INIT_SIZE; i++) {
		map->tab[i].head = list_create();
		map->tab[i].size = 0;
	}

	// on retourne la hashmap
	return map;
}

// destruction d'une table de hashage
void hashmap_destroy(hashmap * map) {
	// libération des listes chaînées de la hashmap
	hashmap_free(map);
	// libération de la superliste
	free(map->tab);
	// libération de la hashmap
	free(map);
}

// libération des données enregistrées dans une table de hashage
void hashmap_free(hashmap * map) {
	// pointeur vers la première superliste
	superlist * chain = map->tab;
	// parcours des listes chaînées
	int i;
	for (i = 0; i < map->size; i++) {
		// libération des données des listes chaînées
		s_node * last;
		if ((list_process(chain[i].head, &list_free, NULL, &chain[i].head)) == 1) {
			// on a supprimé un noeud
		}
	}
}

// ajout d'un mot dans la table si le mot concerné ne s'y trouve pas déjà
char * hashmap_insert(hashmap * map, char * str) {
	// calcul de la clef pour le mot
	unsigned int strHash = hash_calc(str, (map->size));

	// pointeur vers la première superliste
	superlist * chain = map->tab;

	// recherche ou insertion du mot
	// on copie la chaîne de caractères dans strDup
	char * strDup = strdup(str);
	s_node * insertedNode = orderedList_insert(&chain[strHash].head, &string_compare, strDup);

	// incrémente la taille de la superliste en cas d'insertion
	if (insertedNode->data == strDup) {
		chain[strHash].size++;
	} else {
		// on libère la chaîne dupliquée si elle n'a pas été insérée
		free(strDup);
	}

	// on retourne le mot inséré
	return insertedNode->data;
}

// suppression d'un mot dans la table
int hashmap_remove(hashmap * map, char * str) {
	// calcul de la clef pour le mot
	unsigned int strHash = hash_calc(str, (map->size));

	// pointeur vers la liste première superliste
	superlist * chain = map->tab;

	// list_process retrouve le noeud à supprimer et le stocke dans * nodeRemove
	s_node * nodeRemove;
	// TODO: tester la valeur de retour de list_process
	int testProcess;
	if ((testProcess = list_process(chain[strHash].head, &string_remove, str, &nodeRemove)) == 0) {
		// on n'a pas trouvé l'élément à supprimer
		// on retourne 1
		return 1;
	}
	// list_remove récupère le noeud à supprimer
	chain[strHash].head = list_remove(chain[strHash].head, nodeRemove->data);
	// on décrémente la taille de la liste chaînée
	chain[strHash].size--;

	// on retourne 0 si tout s'est bien passé
	return 0;
}

// affichage de données statistiques sur la table de hashage :
//   - nombre total d'éléments ;
//   - nombres min. et max. d'éléments par entrée
//   - écart type du nombre d'éléments par entrée
void hashmap_stats(hashmap * map) {
	int i;
	superlist * chain = map->tab;
	// min, max, total
	int min = chain[0].size;
	int max = chain[0].size;
	int total = 0;
	// on stocke la taille des différentes listes pour calculer la variance
	int taille[map->size];
	// moyenne, variance, écart type
	double sommeVariance = 0.0;
	double moyenne, variance, ecartType;

	// parcours de la superliste
	for (i = 0; i < map->size; i++) {
		taille[i] = chain[i].size;
		// actualisation du nombre total d'éléments
		total += chain[i].size;
		// calcul min et max éléments sur la liste en cours
		if (chain[i].size < min) min = chain[i].size;
		if (chain[i].size > max) max = chain[i].size;
	}

	// calcul de la moyenne des listes sur la hashmap
	moyenne = ((double) total/(double) (map->size));

	// calcul de la variance
	// somme des (tailles - moyenne)²
	for (i = 0; i < map->size; i++) {
		sommeVariance += pow((((double) taille[i] - (double) moyenne)), 2);
	}
	// variance
	variance = sommeVariance / ((double) map->size);
	// écart type
	ecartType = sqrt(variance);

	// affichage
	printf("Nombre minimum d'entrée(s) dans une liste : %d\n", min);
	printf("Nombre maximum d'entrée(s) dans une liste : %d\n", max);
	printf("Moyenne d'entrées dans les listes : %.2f\n", moyenne);
	printf("Variance : %.4f\n", variance);
	printf("Écart type : %.4f\n", ecartType);
}