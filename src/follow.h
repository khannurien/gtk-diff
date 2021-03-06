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
		char space[5]; // jusqu'à 4 délimiteurs, on ne stocke pas dans la table de hashage
	} data; // accès via data.word OU data.space selon le type WORD ou SHORT_SPACE
} token;

typedef struct text {
	char * text; // texte brut
	int textSize; // longueur grossière (i.e. 1/3 * nb caractères), en mots, du texte brut
	struct token ** tokenizedText; // texte découpé, alloué dynamiquement, à realloc de 20% quand on a trop de mots
	int nbTokens; // nb total jetons
	int nbWordTokens; // nb jetons WORD
} text;

typedef struct follow {
	hashmap * map;
	text * pRefText; // doc référence
	text * pNewText; // doc représentant le nouveau texte
	text * diffText; // doc représentant la différence entre les deux
} follow;

// fonction d'affichage des tokens d'un text
void tokens_dump(text * textStruct);

// création de la structure follow et sa hashmap
// le reste des champs est à NULL
follow * follow_init(void);

// allocation et chargement du texte
// retourne la structure text * pour un texte à partir de son chemin d'accès
text * text_load(const char * filename);

// fonction de tokenisation d'un mot
token * get_next_token(char * text, hashmap * map, int * offset, int * words, int * tokens);

// fonction de découpage d'un texte en tokens stockés dans une hashmap
// lecture séquentielle du texte pour appeler get_next_token
void text_tokenize(hashmap * map, text * textStruct);

// algo PLSC (Plus Longue Sous-Séquence Commune)
// les éléments communs doivent être dans le même ordre, mais pas nécessairement consécutifs
int ** plsc(text * refText, text * newText);

// algo de chaînage arrière pour construire le tableau de tokens faisant office de résultat de comparaison
// on se place en (n, n) correspondant aux dernières lettres de chaque chaîne
text * diff_create(int ** lg, text * refText, text * newText);

#endif