#ifndef _STRHASH_
#define _STRHASH_
#include "list.h"

// 
typedef struct superlist {
	int size;
	s_node * head;
} superlist;

// table de hashage
typedef struct hashmap {
	int size;
	superlist * tab;
} hashmap;

// calcul du hash pour une chaîne de caractères
static int hash_calc(char * str, int mapSize);

// création d'une table de hashage

// destruction d'une table de hashage

// libération des données enregistrées dans une table de hashage

// ajout d'un mot dans la table si le mot concerné ne s'y trouve pas déjà

// suppression d'un mot dans la table

// affichage de données statistiques sur la table de hashage :
//   - nombre total d'éléments ;
//   - nombres min. et max. d'éléments par entrée
//   - écart type du nombre d'éléments par entrée

#endif