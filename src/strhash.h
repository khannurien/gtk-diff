#ifndef _STRHASH_
#define _STRHASH_
#include "list.h"

// superliste
typedef struct superlist {
	int size;
	s_node * head;
} superlist;

// table de hashage
typedef struct hashmap {
	int size;
	superlist * tab;
} hashmap;

// fonction de test : affichage d'une hashmap
void hash_dump(hashmap * map); 

// calcul du hash pour une chaîne de caractères
static unsigned int hash_calc(char * str, int mapSize);

// création d'une table de hashage
hashmap * hashmap_create(int mapSize);

// destruction d'une table de hashage
int hashmap_destroy(hashmap * map); 

// libération des données enregistrées dans une table de hashage
int hashmap_empty(hashmap * map);

// ajout d'un mot dans la table si le mot concerné ne s'y trouve pas déjà
char * hashmap_insert(hashmap * map, char * str);

// suppression d'un mot dans la table
int hashmap_remove(hashmap * map, char * str);

// affichage de données statistiques sur la table de hashage :
//   - nombre total d'éléments ;
//   - nombres min. et max. d'éléments par entrée
//   - écart type du nombre d'éléments par entrée
void hashmap_stats(hashmap * map);

#endif