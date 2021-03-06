#ifndef _LIST_
#define _LIST_

// noeud
typedef struct _list_node {
	void * data;
	struct _list_node * next;
} s_node;

// fonction de test
// affichage du contenu d'une liste d'entiers et de sa taille
void list_dump(s_node * head);
// affichage du contenu d'une liste de chaînes de caractères et de sa taille
void strlist_dump(s_node * head);

// création d'une nouvelle liste vide
s_node * list_create(void);

// lire ou écrire la donnée d'un noeud
void * list_get_data(s_node * node);
void list_set_data(s_node * node, void * data);

// création et insertion d'un noeud en tête de liste
// retourne la tête de liste
s_node * list_insert(s_node * head, void * data);

// création et ajout d'un noeud en queue de liste
// retourne la tête de liste
s_node * list_append(s_node * head, void * data);

// application d'une fonction sur les données enregistrées dans la liste
// last est le dernier noeud traité.
int list_process(s_node * head, int (* fct)(s_node * node, void * param), void * param, s_node ** last);

// suppression de la première instance d'une donnée dans la liste
// retourne la tête de liste
s_node * list_remove(s_node * head, void * data);

// suppression de la tête de liste
// retourne la nouvelle tête de liste
s_node * list_headRemove(s_node * head);

// destruction d'une liste
// la libération des données n'est pas prise en charge
void list_destroy(s_node * head);

// insertion d'une donnée dans une liste ordonnée
// une fonction passée en paramètre est appelée pour comparer deux données
// retourne l'élément inséré ou l'instance existante de cet élément
s_node * orderedList_insert(s_node ** head, int (* fct)(s_node * node, void * data), void * data);

#endif