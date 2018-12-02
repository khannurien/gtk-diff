#include <stdlib.h>
#include <stdio.h>
#include "follow.h"

// allocation et chargement du texte
// retourne la structure text * pour un texte à partir de son chemin d'accès
text * text_load(const char * filename) {
	// ouverture du fichier
	FILE * fp;
	if ((fp = fopen(filename, "r")) == NULL)
		return NULL;

	// allocation du texte
	text * fileText;
	if ((fileText = (text *) malloc(sizeof(text))) == NULL)
		return NULL;

	// taille du texte initial, en caractères
	fseek(fp, 0, SEEK_END);
	fileText->textSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// allocation de l'espace requis pour le texte et le caractère sentinelle
	if ((fileText->text = (char *) malloc((fileText->textSize + 1) * sizeof(char))) == NULL) {
		free(fileText);
		return NULL;
	}

	// copie du texte
	fread(fileText->text, sizeof(char), fileText->textSize, fp);

	// fermeture du fichier
	fclose(fp);

	// ajout du caractère sentinelle
	fileText->text[fileText->textSize] = '\0';

	// allocation de tokenizedText
	fileText->nbTokens = 0;
	fileText->nbWordTokens = 0;
	if ((fileText->tokenizedText = (token **) malloc(sizeof(token *))) == NULL) {
		free(fileText->text);
		free(fileText);
		return NULL;
	}

	// on retourne la structure
	return fileText;
}

// création de la structure follow et sa hashmap
// le reste des champs est à NULL
follow * follow_init(void) {
	// allocation de la nouvelle instance de follow
	follow * newFollow;
	if ((newFollow = (follow *) malloc(sizeof(follow))) == NULL)
		return NULL;

	// création de la hashmap
	if ((newFollow->map = hashmap_create(10)) == NULL) {
		free(newFollow);
		return NULL;
	}

	// on retourne la nouvelle instance
	return newFollow;
}

// fonction de tokenisation d'un mot
token * get_next_token(char * text, hashmap * map, int * offset) {
	// on alloue et on initialise un nouveau token
	token * newToken;
	// malloc... union dans un struct ?
	newToken->textOffset = (* offset);

	// on se déplace sur le mot à lire
	char * word = (text += (* offset));

	// analyse du mot
	char aChar;
	//	while (aChar != 'de quoi ?') {
		// on définit token->type
		//	if (aChar == '...') {
		//		token.type = ...
		//	}
		// on met à jour la donnée du token
		//	if ((token.type == WORD) || (...)) {
		//		token->data = ...
		//	}
	//	}

	// on incrémente l'offset
	(* offset)++;

	// on retourne le token
	return newToken;
}

// fonction de découpage d'un texte en tokens stockés dans une hashmap
// lecture séquentielle du texte pour appeler get_next_token
void text_tokenize(hashmap * map, text * textStruct) {
	// offset pour la lecture du mot suivant
	int i = 0;
	int * pOffset = &i;

	// lecture séquentielle
	token * aToken;
	// tant que le texte n'est pas terminé, on le découpe en tokens
	while ((* pOffset) < textStruct->textSize) {
		if ((aToken = get_next_token(textStruct->text, map, pOffset)) != NULL)
			textStruct->tokenizedText[(* pOffset)] = aToken;
	}
}