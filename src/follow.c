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

	// taille du texte initial
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

// fonctions de découpage
void text_tokenize(hashmap * map, text * textStruct) {
	// while next_token je prends le token et je l'envoie dans tokenizedText
}