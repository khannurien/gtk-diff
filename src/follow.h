#ifndef _FOLLOW_
#define _FOLLOW_
#include "strhash.h"
#include "list.h"

typedef struct token {
	// espace : ' ', '\n', '\r' ou '\t'
	enum { WORD, SHORT_SPACE, SPACE, ERASE, INSERT, REPLACE, EMPTY } type;
	int textOffset; // position dans le texte (char *) d'origine
	union {
		char * word; // pointeur sur un mot de la table de hashage
		char space[4]; // jusqu'à 4 délimiteurs, on ne stocke pas dans la table de hashage
	} data; // accès via data.word OU data.space selon le type WORD ou SHORT_SPACE
} token;

typedef struct text {
	char * text; // texte brut (longueur divisée par i.e. 5)
	int textSize; // longueur texte brut
	struct token ** tokenizedText; // texte découpé, alloué dynamiquement, à realloc de 20% quand on a trop de mots
	int nbTokens; // nb total jetons
	int nbWordTokens; // nb jetons WORD
} text;

typedef struct follow {
	// table de hachage (une seule)
	hashmap * map;
	text * pTextRef; // doc référence
	text * pNewText; // doc représentant le nouveau texte
	text * diffText; // doc représentant la différence entre les deux
} follow;

// token_dump

// allocation et chargement du texte
// prend le chemin d'accès et renvoie le char * text
// text * text_load(const char * filename);

// get_next_token(...)

// fonction de découpage
// while next_token je prends le token et je l'envoie dans tokenizedText

// algo PLSC (Plus Longue Sous-Séquence Commune)
// les éléments communs doivent être dans le même ordre, mais pas nécessairement consécutifs
// fonction 1 : retourne la matrice PLSC int ** et prend les 2 struct text (ancien et nouveau)
// fonction 2 : on prend la matrice en [n][m] (max ligne/colonne)

#endif