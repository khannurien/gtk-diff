#include <stdlib.h>
#include <stdio.h>
#include "follow.h"
#include "strhash.h"

// allocation et chargement du texte
// retourne la structure text * pour un texte à partir de son chemin d'accès
text * text_load(const char * filename) {
	// ouverture du fichier
	FILE * fp;
	if ((fp = fopen(filename, "r")) == NULL)
		return NULL;

	// allocation de la structure text *
	text * fileText;
	if ((fileText = (text *) malloc(sizeof(text))) == NULL) {
		fclose(fp);
		return NULL;
	}

	// taille du texte initial, en caractères
	fseek(fp, 0, SEEK_END);
	int nbChar = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	// on estime grossièrement le nombre de mots au cinquième du nombre de caractères
	fileText->textSize = (nbChar / 5);

	// allocation de l'espace requis pour le texte et le caractère sentinelle
	if ((fileText->text = (char *) malloc((nbChar + 1) * sizeof(char))) == NULL) {
		fclose(fp);
		free(fileText);
		return NULL;
	}

	// copie du texte dans la structure
	fread(fileText->text, sizeof(char), nbChar, fp);

	// fermeture du fichier
	fclose(fp);

	// ajout du caractère sentinelle
	fileText->text[nbChar] = '\0';

	// allocation de tokenizedText
	fileText->nbTokens = 0;
	fileText->nbWordTokens = 0;
	if ((fileText->tokenizedText = (token **) malloc((fileText->textSize) * sizeof(token *))) == NULL) {
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

	// initialisation des pointeurs de text *
	newFollow->pRefText = NULL;
	newFollow->pNewText = NULL;
	newFollow->diffText = NULL;

	// on retourne la nouvelle instance
	return newFollow;
}

// fonction de tokenisation d'un mot
token * get_next_token(char * text, hashmap * map, int * offset, int * words) {
	// on alloue et on initialise un nouveau token
	token * newToken;
	if ((newToken = (token *) malloc(sizeof(token))) == NULL)
		return NULL;
	// on le positionne sur l'offset en cours
	newToken->textOffset = (* offset);

	// on se déplace sur le mot à lire
	char * word = (text += (* offset));

	// analyse du mot
	char * aChar = word;
	if ((aChar[0] == ' ') || (aChar[0] == '\t') || (aChar[0] == '\n') || (aChar[0] == '\r')) {
		// c'est au moins un délimiteur : on compte combien il y en a derrière
		int i = 1;
		while ((aChar[i] == ' ') || (aChar[i] == '\t') || (aChar[i] == '\n') || (aChar[i] == '\r'))
			i++;
	
		if (i <= 4) {	
			// espace court : on stocke les délimiteurs dans la donnée du token	
			newToken->type = SHORT_SPACE;
			int j;
			for (j = 0; j < i; j++)
				newToken->data.space[j] = aChar[j];
		} else if (i > 4) {
			// espace long : on change le type du token
			newToken->type = SPACE;
		}

		// décalage de l'offset
		(* offset) += i;
	} else if ((aChar[0] == ',') || (aChar[0] == ';') || (aChar[0] == ':') || (aChar[0] == '?') || (aChar[0] == '!') || (aChar[0] == '.')) {
		// le mot est un signe de ponctuation
		// on l'enregistre dans la hashmap
		char * aWord = hashmap_insert(map, &aChar[0]);

		// on met à jour le token
		newToken->type = WORD;
		newToken->data.word = aWord;

		// décalage de l'offset
		(* offset)++;
		// incrémentation du compteur de WORD
		(* words)++;
	} else {
		// sinon, c'est un mot
		newToken->type = WORD;

		// on alloue l'espace nécessaire à un buffer de vingt caractères
		char * buffer;
		if ((buffer = (char *) malloc(20 * sizeof(char))) == NULL) {
			free(newToken);
			return NULL;
		}

		// on lit le mot jusqu'à un délimiteur ou un signe de ponctuation
		int i = 0;
		while ((aChar[i] != ' ') && (aChar[i] != '\t') && (aChar[i] != '\n') && (aChar[i] != '\r') &&
			(aChar[i] != ',') && (aChar[i] != ';') && (aChar[i] != ':') && (aChar[i] != '?') && (aChar[i] != '!') && (aChar[i] != '.')) {
			// on augmente la taille du buffer si nécessaire
			if (i > 20)
				buffer = realloc(buffer, (i + 20) * sizeof(char));
			
			// on stocke chaque caractère dans le buffer
			buffer[i] = aChar[i];

			// compteur de caractères
			i++;
		}

		// on enregistre le mot dans la hashmap
		char * aWord = hashmap_insert(map, buffer);
		// on libère le buffer
		free(buffer);
		// on met à jour le pointeur data.word du token
		newToken->data.word = aWord;

		// décalage de l'offset
		(* offset) += i;
		// incrémentation du compteur de WORD
		(* words)++;
	}

	// on retourne le token
	return newToken;
}

// fonction de découpage d'un texte en tokens stockés dans une hashmap
// lecture séquentielle du texte pour appeler get_next_token
void text_tokenize(hashmap * map, text * textStruct) {
	// offset pour la lecture du mot suivant
	int i = 0;
	int * pOffset = &i;
	// compteur de WORD
	int words = 0;
	int * pWords = &words;

	// lecture séquentielle
	token * aToken;
	// tant que le texte n'est pas terminé, on le découpe en tokens
	while (textStruct->text[(* pOffset)] != '\0') {
		// la taille effective du texte atteint sa taille estimée (nbChar / 5)
		if ((* pOffset) == (textStruct->textSize) * 5) {
			// on réalloue de 20% supplémentaires la taille de tokenizedText
			textStruct->tokenizedText = realloc(textStruct->tokenizedText, ((1.2 * (textStruct->textSize)) * sizeof(token *)));
			textStruct->textSize *= 1.2;
		}

		// on demande le token suivant
		if ((aToken = get_next_token(textStruct->text, map, pOffset, pWords)) != NULL)
			textStruct->tokenizedText[(* pOffset)] = aToken;
	}

	// mise à jour de textStruct après tokenisation
	textStruct->nbTokens = i; // à corriger ?
	textStruct->nbWordTokens = words;
}