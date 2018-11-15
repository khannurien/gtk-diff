#include <stdlib.h>
#include <stdio.h>
#include "list.h"

// fonctions de test
// affichage du contenu d'une liste et de sa taille
void list_dump(s_node * head) {
	if (head == NULL) {
		printf("Liste vide.\n");
	} else {
		s_node * curseur = head;
		int * maDonnee;
		int cpt = 0;

		printf("( ");

		// parcours de la liste
		while (curseur != NULL) {
			maDonnee = list_get_data(curseur);
			printf("%d ", *maDonnee);

			cpt++;
			curseur = curseur->next;
		}

		printf(")\n");

		printf("Taille de la liste : %d\n", cpt);
	}
}

// création d'une nouvelle liste vide
s_node * list_create(void) {
	return NULL;
}

// lire la donnée sur un noeud
void * list_get_data(s_node * node) {
	return node->data;
}

// écrire la donnée sur un noeud
void list_set_data(s_node * node, void * data) {
	node->data = data;
}

// création et insertion d'un noeud en tête de liste
// retourne la tête de liste
s_node * list_insert(s_node * head, void * data) {
	// nouveau noeud
	s_node * newNode;
	if ((newNode = (s_node *) malloc(sizeof(s_node))) == NULL)
		return NULL;
	list_set_data(newNode, data);

	// échange avec head
	newNode->next = head;

	return newNode;
}

// création et ajout d'un noeud en queue de liste
// retourne la tête de liste
s_node * list_append(s_node * head, void * data) {
	// nouveau noeud
	s_node * newNode;
	if ((newNode = (s_node *) malloc(sizeof(s_node))) == NULL)
		return NULL;
	list_set_data(newNode, data);
	newNode->next = NULL;

	// parcours jusqu'au dernier noeud
	// la liste est-elle vide ? si oui, on retourne le nouveau noeud
	if (head == NULL) {
		return newNode;
	} else {
		s_node * curseur = head;
		while (curseur->next != NULL) {
			curseur = curseur->next;
		}

		// ajout du nouveau noeud
		curseur->next = newNode;
	}

	return head;
}

// application d'une fonction sur les données enregistrées dans la liste
// last est le dernier noeud traité
int list_process(s_node * head, int (* fct)(s_node * node, void * param), void * param, s_node ** last) {
	// liste vide : on retourne 1
	if (head == NULL) return 1;

	// si la fonction est appliquée avec succès sur les données, on enregistre un pointeur
	// sur le dernier noeud traité dans *last
	// sinon, on retourne 1
	s_node * curseur = head;
	while (curseur != NULL) {
		if ((fct(curseur, param)) == 0) {
			*last = curseur;
		} else {
			return 1;
		}
		curseur = curseur->next;
	}

	// la fonction a été appliquée à l'ensemble des noeuds
	// *last contient un pointeur vers le dernier noeud traité
	// on retourne 0
	return 0;
}

// suppression de la première instance d'une donnée dans la liste
// retourne la tête de liste
s_node * list_remove(s_node * head, void * data) {
	// liste vide : on retourne NULL
	if (head == NULL) return NULL;

	// liste à un élément : on retourne la liste vide
	if (head->next == NULL) {
		free(head);
		return NULL;
	}

	// parcours de la liste jusqu'à la donnée
	s_node * curseur = head->next;
	s_node * precedent = head;
	while (curseur != NULL) {
		if (list_get_data(curseur) == data) {
			// décalage du noeud vers le suivant
			precedent->next = curseur->next;
			return head;
		} else {
			precedent = curseur;
			curseur = curseur->next;
		}
	}

	return head;
}

// suppression de la tête de liste
// retourne la nouvelle tête de liste
s_node * list_headRemove(s_node * head) {
	// liste vide : on retourne NULL
	if (head == NULL) return NULL;

	// liste à un seul élément : on retourne la liste vide
	if (head->next == NULL) {
		free(head);
		return NULL;
	}

	// décalage du début de liste
	s_node * newHead;
	newHead = head->next;
	free(head);

	return newHead;
}

// destruction d'une liste
// la libération des données n'est pas prise en charge
void list_destroy(s_node * head) {
	// on utilise headRemove jusqu'à épuisement de la liste
	while (head != NULL)
		head = list_headRemove(head);
}

// insertion d'une donnée dans une liste ordonnée
// une fonction passée en paramètre est appelée pour comparer deux données
s_node * orderedList_insert(s_node * head, int (*fct)(s_node * node, void * data), void * data) {
	// liste vide
	// on insère la donnée dans le premier noeud
	if (head == NULL) {
		head=list_insert(head, data);
		return head;
	}

	// comparaison avec la fonction node_compare
	if (fct(head, data) == 1) {
		head = list_insert(head, data);
		return head;
	}

	// liste à un seul élément
	// *data n'est pas inférieur à la donnée en tête de liste
	if (head->next == NULL) {
		head = list_append(head, data);
 		return head;
	}

	// création d'un nouveau noeud avec *data
	s_node * newNode;
	if ((newNode = (s_node *) malloc(sizeof(s_node))) == NULL)
		return NULL;

	list_set_data(newNode, data);
	
	// ouverture de deux curseurs
	s_node * nextNode = (head->next);
	s_node * prevNode = head;

	// parcours de la liste
	while (nextNode != NULL) {
		// on cherche le premier noeud avec *data supérieur à *intData
		// on insère le nouveau noeud avant
		if (fct(nextNode, data) == 1) {
			prevNode->next = newNode;
			newNode->next = nextNode;
			return head;
		}

		prevNode = nextNode;
		nextNode = nextNode->next;
	}

	// *data correspond à la plus grande donnée de la liste
	head = list_append(head, data);
 	return head;
}