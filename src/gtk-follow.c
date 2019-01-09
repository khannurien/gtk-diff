#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "gtk-follow.h"

// macro GLib de déclaration des fonctions
G_MODULE_EXPORT

int main(int argc, char * argv[]) {
	// déclaration de l'IHM
	GtkWindow * mainWindow;
	GtkBuilder * ihm;

	// déclaration de la vue et de l'instance follow
	viewGTK view;
	follow texts;

	// initialisation de l'IHM
	gtk_init(&argc, &argv);
	ihm = gtk_builder_new();

	GError * err = NULL;
	if (! gtk_builder_add_from_file(ihm, "glade/mainWindow.glade", &err)) {
		fprintf(stderr, "Error with GTK builder: \n%d - %s\n", err->code, err->message);
		return 1;
	}

	// initialisation de l'instance de follow
	texts = * (follow_init());

	// passage à la vue
	view.texts = &texts;

	// création de la fenêtre principale
	view.mainWindow = GTK_WINDOW(gtk_builder_get_object(ihm, "mainWindow"));
	view.textLeft = GTK_TEXT_VIEW(gtk_builder_get_object(ihm, "textLeft"));
	view.textRight = GTK_TEXT_VIEW(gtk_builder_get_object(ihm, "textRight"));
	// association des buffers
	view.refBuffer = gtk_text_view_get_buffer(view.textLeft);
	view.newBuffer = gtk_text_view_get_buffer(view.textRight);

	// tags GTK pour newBuffer
	gtk_text_buffer_create_tag(view.newBuffer, "blue_foreground", "foreground", "blue", NULL);
	gtk_text_buffer_create_tag(view.newBuffer, "red_foreground", "foreground", "red", NULL);
	gtk_text_buffer_create_tag(view.newBuffer, "green_foreground", "foreground", "green", NULL);

	// connexion de l'IHM aux signaux de la vue
	gtk_builder_connect_signals(ihm, &view);

	// affichage de la fenêtre
	gtk_widget_show(GTK_WIDGET(view.mainWindow));

	// application
	gtk_main();

	return 0;
}

// quitter l'application quand on ferme la fenêtre
void on_mainWindow_destroy() {
	gtk_main_quit();
}

// chargement consécutif du texte de référence, puis du texte modifié
void open_ref_event(GtkMenuItem * menuItem, gpointer user_data) {
	// initialisation de la vue
	viewGTK * view = (viewGTK *) user_data;
	char * refFilename = NULL;
	char * newFilename = NULL;

	// ouverture des fichiers texte
	GtkWidget * dialog;
	dialog = gtk_file_chooser_dialog_new("Choisir le document",
		view->mainWindow,
		GTK_FILE_CHOOSER_ACTION_OPEN,
		"_Cancel", GTK_RESPONSE_CANCEL,
		"_Open", GTK_RESPONSE_ACCEPT,
		NULL);

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		// chargement du fichier de référence
		refFilename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
	}

	gtk_widget_hide(GTK_WIDGET(dialog));

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		// chargement du fichier modifié
		newFilename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
	}

	gtk_widget_hide(GTK_WIDGET(dialog));

	if (refFilename != NULL && newFilename != NULL) {

		// chargement du texte de référence
		text * refText;
		if ((refText = text_load(refFilename)) == NULL)
			return;
		// chargement du texte modifié
		text * newText;
		if ((newText = text_load(newFilename)) == NULL)
			return;

		// libération du nom de fichier
		g_free(refFilename);
		g_free(newFilename);
		
		// association du texte à notre instance de follow
		view->texts->pRefText = refText;
		view->texts->pNewText = newText;

		// tokenisation du texte
		text_tokenize(view->texts->map, view->texts->pRefText);
		text_tokenize(view->texts->map, view->texts->pNewText);

		// création du diff
		int ** lg = plsc(refText, newText);
		view->texts->diffText = diff_create(lg, refText, newText);

		// buffer du fichier de référence
		GtkTextIter refIter, refStart;
		GtkTextBuffer * refBuffer = view->refBuffer;
		// RAZ
		gtk_text_buffer_get_start_iter(refBuffer, &refStart);
		gtk_text_buffer_get_end_iter(refBuffer, &refIter);
		gtk_text_buffer_delete(refBuffer, &refStart, &refIter);

		// buffer du fichier modifié
		GtkTextIter newIter, newStart;
		GtkTextBuffer * newBuffer = view->newBuffer;
		// RAZ
		gtk_text_buffer_get_start_iter(newBuffer, &newStart);
		gtk_text_buffer_get_end_iter(newBuffer, &newIter);
		gtk_text_buffer_delete(newBuffer, &newStart, &newIter);

		// boucles d'affichage
		// le buffer de gauche (refBuffer) contient le texte de référence
		int i, j;
		for (i = 0; i < view->texts->pRefText->nbTokens; i++) {
			gtk_text_buffer_get_end_iter(refBuffer, &refIter);
			if (view->texts->pRefText->tokenizedText[i]->type == SHORT_SPACE) {
				gtk_text_buffer_insert(view->refBuffer, &refIter, view->texts->pRefText->tokenizedText[i]->data.space, -1);
			} else {
				gtk_text_buffer_insert(view->refBuffer, &refIter, view->texts->pRefText->tokenizedText[i]->data.word, -1);
			}
		}

		// le buffer de droite (newBuffer) contient le résultat mis en forme du diff avec le texte modifié
		for (i = 0; i < view->texts->diffText->nbTokens; i++) {
			gtk_text_buffer_get_end_iter(newBuffer, &newIter);
			if ((view->texts->diffText->tokenizedText[i]->type == WORD) || (view->texts->diffText->tokenizedText[i]->type == SPACE)) {
				gtk_text_buffer_insert(view->newBuffer, &newIter, view->texts->diffText->tokenizedText[i]->data.word, -1);
			} else if (view->texts->diffText->tokenizedText[i]->type == ERASE) {
				gtk_text_buffer_insert_with_tags_by_name(view->newBuffer, &newIter, view->texts->diffText->tokenizedText[i]->data.word, -1, "red_foreground", NULL);
			} else if (view->texts->diffText->tokenizedText[i]->type == INSERT) {
				gtk_text_buffer_insert_with_tags_by_name(view->newBuffer, &newIter, view->texts->diffText->tokenizedText[i]->data.word, -1, "green_foreground", NULL);
			} else if (view->texts->diffText->tokenizedText[i]->type == REPLACE) {
				gtk_text_buffer_insert_with_tags_by_name(view->newBuffer, &newIter, view->texts->diffText->tokenizedText[i]->data.word, -1, "blue_foreground", NULL);
			} else if (view->texts->diffText->tokenizedText[i]->type == SHORT_SPACE) {
				gtk_text_buffer_insert(view->newBuffer, &newIter, view->texts->diffText->tokenizedText[i]->data.space, -1);
			}
		}
	}
}

