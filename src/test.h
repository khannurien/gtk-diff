#ifndef _TEST_
#define _TEST_
#include "list.h"

// fonction passée à list_process afin de la tester
// incrémentation de la donnée d'un noeud
int incremente(s_node * node, void * param);

// fonction passée à orderedList_insert afin de comparer deux noeuds
// retourne 1 si la donnée du noeud est strictement supérieure à la donnée passée
// retourne 0 sur la donnée du noeud est égale à la donnée passée
// retourne -1 si la donnée du noeud est strictement inférieure à la donnée passée
// retourne -2 si les deux données n'ont pas pu être comparées
int node_compare(s_node * node, void * data);

// fonction de test sur les listes
int list_test(void);

// fonction de tests sur les hashmaps
int hash_test(void);

// fonction de test sur follow
int follow_test(void);

// fonction pour l'ensemble des tests
int test_all(void);

#endif