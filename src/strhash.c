#include <stdlib.h>
#include <stdio.h>
#include "strhash.h"

// calcul du hash pour une chaîne de caractères
static int hash_calc(char * str, int mapSize) {
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

// destruction d'une table de hashage

// libération des données enregistrées dans une table de hashage

// ajout d'un mot dans la table si le mot concerné ne s'y trouve pas déjà

// suppression d'un mot dans la table

// affichage de données statistiques sur la table de hashage :
//   - nombre total d'éléments ;
//   - nombres min. et max. d'éléments par entrée
//   - écart type du nombre d'éléments par entrée