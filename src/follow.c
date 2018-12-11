#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "follow.h"
#include "strhash.h"

// fonction d'affichage des tokens d'un text
void tokens_dump(text * textStruct) {
	int i;
	for (i = 0; i < textStruct->nbTokens; i++) {
		if ((textStruct->tokenizedText[i]->type == WORD) || (textStruct->tokenizedText[i]->type == SPACE)) {
			printf("%s", textStruct->tokenizedText[i]->data.word);
		} else if (textStruct->tokenizedText[i]->type == SHORT_SPACE) {
			int j;
			while (j < 1) { // TODO: il faudrait avoir le nombre de délimiteurs dans le token
				printf("%c", textStruct->tokenizedText[i]->data.space[j]);
				j++;
			}
		}
	}
	// flush du buffer
	printf("\n\n");
}

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
	// on estime grossièrement le nombre de mots au tiers du nombre de caractères
	fileText->textSize = (nbChar / 3);

	// allocation de l'espace requis pour le texte et le caractère sentinelle
	if ((fileText->text = (char *) malloc((nbChar) * sizeof(char))) == NULL) {
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
token * get_next_token(char * text, hashmap * map, int * offset, int * tokens, int * words) {
	// on alloue et on initialise un nouveau token
	token * newToken;
	if ((newToken = (token *) malloc(sizeof(token))) == NULL)
		return NULL;
	// on le positionne sur l'offset en cours
	newToken->textOffset = (* offset);

	// on se déplace sur le mot à lire
	char * aChar = text + (* offset);
	// analyse du mot
	if ((aChar[0] == ' ') || (aChar[0] == '\t') || (aChar[0] == '\n') || (aChar[0] == '\r')) {
		// c'est au moins un délimiteur : on compte combien il y en a derrière
		int i = 1;
		while (((aChar[i] == ' ') || (aChar[i] == '\t') || (aChar[i] == '\n') || (aChar[i] == '\r')) && (aChar[i] != '\0'))
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

			// on crée un buffer pour i caractères et le caractère sentinelle
			char buffer[i + 1];

			// on stocke chaque caractère dans le buffer
			int j;
			for (j = 0; j < i; j++)
				buffer[j] = aChar[j];

			// on ajoute le caractère sentinelle
			buffer[i] = '\0';

			// on insère le token dans la hashmap
			newToken->data.word = hashmap_insert(map, buffer);

			// incrémentation du compteur de WORD
			(* words)++;
		}

		// décalage de l'offset
		(* offset) += i;
		// incrémentation du compteur de tokens
		(* tokens)++;
	} else if ((aChar[0] == ',') || (aChar[0] == ';') || (aChar[0] == ':') || (aChar[0] == '?') || (aChar[0] == '!') || (aChar[0] == '.') ||
				(aChar[0] == '(') || (aChar[0] == ')') || (aChar[0] == '[') || (aChar[0] == ']') || (aChar[0] == '{') || (aChar[0] == '}')) {
		// le mot est un signe de ponctuation
		// on met à jour le token
		newToken->type = WORD;

		// on l'enregistre avec son caractère sentinelle dans la hashmap
		char buffer[] = {aChar[0], '\0'};
		newToken->data.word = hashmap_insert(map, buffer);

		// décalage de l'offset
		(* offset)++;
		// incrémentation du compteur de WORD
		(* words)++;
		// incrémentation du compteur de tokens
		(* tokens)++;
	} else {
		// sinon, c'est un mot
		newToken->type = WORD;

		// on alloue l'espace nécessaire à un buffer pour vingt caractères
		char * buffer;
		int bSize = 20;
		if ((buffer = (char *) malloc((bSize + 1) * sizeof(char))) == NULL) {
			free(newToken);
			return NULL;
		}

		// on lit le mot jusqu'à l'EOF (cas du texte à un seul mot), un délimiteur ou un signe de ponctuation
		int i = 0;
		while ((aChar[i] != '\0') && (aChar[i] != ' ') && (aChar[i] != '\t') && (aChar[i] != '\n') && (aChar[i] != '\r') &&
			(aChar[i] != ',') && (aChar[i] != ';') && (aChar[i] != ':') && (aChar[i] != '?') && (aChar[i] != '!') && (aChar[i] != '.') &&
			(aChar[i] != '(') && (aChar[i] != ')') && (aChar[i] != '[') && (aChar[i] != ']') && (aChar[i] != '{') && (aChar[i] != '}')) {
			// on augmente la taille du buffer si nécessaire
			if (i > bSize) {
				// le mot le plus long en français fait 26 lettres
				bSize += 6;
				buffer = realloc(buffer, (bSize + 1) * sizeof(char));
			}
			
			// on stocke chaque caractère dans le buffer
			buffer[i] = aChar[i];

			// compteur de caractères
			i++;
		}

		// on insère le caractère sentinelle
		buffer[i] = '\0';

		// on enregistre le mot dans la hashmap et on met à jour le pointeur data.word du token
		newToken->data.word = hashmap_insert(map, buffer);
		// on libère le buffer
		free(buffer);

		// décalage de l'offset
		(* offset) += i;
		// incrémentation du compteur de WORD
		(* words)++;
		// incrémentation du compteur de tokens
		(* tokens)++;
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
	// compteur de tokens
	int tokens = 0;
	int * pTokens = &tokens;

	// lecture séquentielle
	token * aToken;
	// tant que le texte n'est pas terminé, on le découpe en tokens
	while (textStruct->text[(* pOffset)] != '\0') {
		// la taille effective du texte atteint sa taille estimée (nbChar / 3)
		while ((* pTokens) >= (textStruct->textSize)) {
			// on augmente de 20% la taille de tokenizedText
			textStruct->textSize = (textStruct->textSize) * 1.2 + 1;
			textStruct->tokenizedText = realloc(textStruct->tokenizedText, (textStruct->textSize * sizeof(token *)));
		}

		// on demande le token suivant
		if ((aToken = get_next_token(textStruct->text, map, pOffset, pTokens, pWords)) != NULL)
			textStruct->tokenizedText[(* pTokens) - 1] = aToken;
	}

	// mise à jour de textStruct après tokenisation
	textStruct->nbTokens = (* pTokens);
	textStruct->nbWordTokens = (* pWords);
}

// algo PLSC (Plus Longue Sous-Séquence Commune)
// les éléments communs doivent être dans le même ordre, mais pas nécessairement consécutifs
int ** plsc(text * refText, text * newText) {
	// allocation de la matrice à deux dimensions
	int ** lg;
	// en fonction du nombre de jetons de type WORD dans chaque structure
	int maxTokens = fmax(refText->nbWordTokens, newText->nbWordTokens);

	if ((lg = (int **) malloc((maxTokens + 1) * sizeof(int *))) == NULL)
		return NULL;

	int k, l;
	for (k = 0; k < (maxTokens + 1); k++) {
		if ((lg[k] = (int *) malloc((maxTokens + 1) * sizeof(int))) == NULL) {
			free(lg);
			return NULL;
		}
	}

	// initialisation des premières ligne et colonne de la matrice à 0
	for (k = 0; k < refText->nbWordTokens + 1; k++) {
		for (l = 0; l < newText->nbWordTokens + 1; l++) {
			lg[k][0] = 0;
			lg[0][l] = 0;
		}
	}

	// pour chaque mot de l'ancien texte (on démarre l'algo en 1,1)
	int i = 1, j = 1;
	int yToken, xToken;

	for (yToken = 0; yToken < refText->nbTokens; yToken++) {
		// si on dépasse les bornes de la matrice, on arrête
		if (i > refText->nbWordTokens) break;
		// si ce n'est pas un mot, on passe au token suivant
		if (refText->tokenizedText[yToken]->type != WORD) continue;

		// sinon, pour chaque mot du nouveau texte
		for (xToken = 0; xToken < newText->nbTokens; xToken++) {
			// si on dépasse les bornes de la matrice, on arrête
			if (j > newText->nbWordTokens) break;
			// si ce n'est pas un mot, on passe au token suivant
			if (newText->tokenizedText[xToken]->type != WORD) continue;

			// sinon, on remplit la matrice lg selon l'algorithme du cours
			if (refText->tokenizedText[yToken]->data.word == newText->tokenizedText[xToken]->data.word) {
				lg[i][j] = lg[i - 1][j - 1] + 1;
			} else {
				lg[i][j] = fmax(lg[i - 1][j], lg[i][j - 1]);
			}

			j++;
		}

		j = 1;
		i++;
	}

	// on retourne la matrice complétée
	return lg;
}

// algo de chaînage arrière pour construire le tableau de tokens faisant office de résultat de comparaison
// on se place en (n, n) correspondant aux dernières lettres de chaque chaîne
text * diff_create(int ** lg, text * refText, text * newText) {
	// indices dans les matrices, initialisés au max
	int i = refText->nbWordTokens;
	int j = newText->nbWordTokens;
	// nombre de tokens lus dans chaque texte
	int refTokenRd = refText->nbTokens - 1;
	int newTokenRd = newText->nbTokens - 1;
	// nombre de tokens écrits dans le tableau de résultat
	int diffTokenWr = refTokenRd + newTokenRd;
	// tableau de résultat
	token ** tDiff = (token **) malloc(diffTokenWr * sizeof(token *));
	// texte résultant
	text * diffText;
	diffText = (text *) malloc(sizeof(text));
	diffText->tokenizedText = tDiff;

	// parcours de la matrice
	while ((i > 0) && (j > 0)) {
		if ((newText->tokenizedText[newTokenRd]->type == SHORT_SPACE) || (newText->tokenizedText[newTokenRd]->type == SPACE)) {
			// si le token courant dans le nouveau texte est un espace
			tDiff[diffTokenWr] = newText->tokenizedText[newTokenRd];
			diffTokenWr--;
			newTokenRd--;
		} else if ((refText->tokenizedText[newTokenRd]->type == SHORT_SPACE) || (refText->tokenizedText[newTokenRd]->type == SPACE)) {
			// si le token courant dans l'ancien texte est un espace
			refTokenRd--;
		} else if ((lg[i][j] == lg[i - 1][j - 1] + 1) && (newText->tokenizedText[newTokenRd]->data.word == refText->tokenizedText[refTokenRd]->data.word)) {
			// mots identiques (appartenant à la PLSC)
			// on ajoute le token identique dans le tableau de résultat de la comparaison
			tDiff[diffTokenWr] = newText->tokenizedText[newTokenRd];

			// on décrémente les indices des tableaux de tokens concernés selon les cas :
			newTokenRd--;
			refTokenRd--;

			// on décrémente i et j
			i--;
			j--;
		} else if (lg[i][j] == lg[i - 1][j - 1]) {
			// remplacement
			// le jeton du nouveau texte passe en REPLACE et est ajouté au résultat
			// le jeton de l'ancien texte passe en ERASE et est ajouté au résultat
			// on décrémente les indices dans les tableaux de tokens concernés selon les cas

			// on décrémente i et j
			i--;
			j--;
		} else if (lg[i][j] == lg[i][j - 1]) {
			// insertion

			// ...
			// on décrémente j
			j--;

			// ...
		} else {
			// suppression

			// ...
			// on modifie le type du jeton du texte de référence en ERASE et on l'ajoute au résultat
			// ...

			// on décrémente i
			i--;
		}
	}

	// il reste des mots non-exploités dans le nouveau texte
	// ils ont été ajoutés
	while ((j > 0) && (newTokenRd >= 0)) {
		// ...
		j--;
	}

	// il reste des mots non-exploités dans l'ancien texte
	// ils ont été supprimés
	while ((i > 0) && (refTokenRd >= 0)) {
		// ...
		i--;
	}

	while (diffTokenWr > 0) {
		// malloc d'un jeton EMPTY
		diffTokenWr--;
	}

	// on retourne le nouveau texte
	return diffText;
}
