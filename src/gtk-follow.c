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

	gtk_text_buffer_create_tag(view.refBuffer, "blue_foreground", "foreground", "blue", NULL);
	gtk_text_buffer_create_tag(view.refBuffer, "red_foreground", "foreground", "red", NULL);
	gtk_text_buffer_create_tag(view.refBuffer, "green_foreground", "foreground", "green", NULL);

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

// ouverture du texte de référence
void open_ref_event(GtkMenuItem * menuItem, gpointer user_data) {
	printf("-3\n");

	// initialisation de la vue
	viewGTK * view = (viewGTK *) user_data;
	char * refFilename = NULL;
	char * newFilename = NULL;

	// menu
	GtkWidget * dialog;
	dialog = gtk_file_chooser_dialog_new("Choisir le document",
		view->mainWindow,
		GTK_FILE_CHOOSER_ACTION_OPEN,
		"_Cancel", GTK_RESPONSE_CANCEL,
		"_Open", GTK_RESPONSE_ACCEPT,
		NULL);

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		// chargement du fichier
		refFilename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
	}

	gtk_widget_hide(GTK_WIDGET(dialog));

	dialog = gtk_file_chooser_dialog_new("Choisir le document",
		view->mainWindow,
		GTK_FILE_CHOOSER_ACTION_OPEN,
		"_Cancel", GTK_RESPONSE_CANCEL,
		"_Open", GTK_RESPONSE_ACCEPT,
		NULL);

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		// chargement du fichier
		newFilename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
	}

	gtk_widget_hide(GTK_WIDGET(dialog));

	if (refFilename != NULL && newFilename != NULL) {

		// chargement du texte
		text * refText;
		if ((refText = text_load(refFilename)) == NULL)
			return;
		// chargement du texte
		text * newText;
		if ((newText = text_load(newFilename)) == NULL)
			return;
		printf("-2\n");

		// libération du nom de fichier
		//g_free(filename);
		
		// association du texte à notre instance de follow
		view->texts->pRefText = refText;
		view->texts->pNewText = newText;

		// tokenisation du texte
		printf("-1\n");
		text_tokenize(view->texts->map, view->texts->pRefText);
		printf("0\n");
		text_tokenize(view->texts->map, view->texts->pNewText);
		tokens_dump(view->texts->pRefText);
		tokens_dump(view->texts->pNewText);

		printf("1\n");

		// diff
		int ** lg = plsc(refText, newText);
		printf("2\n");
		view->texts->diffText = diff_create(lg, refText, newText);
		printf("3\n");
		tokens_dump(view->texts->diffText);
		printf("4\n");

		// buffer
		GtkTextIter iter, start;
		GtkTextBuffer * buffer = view->refBuffer;
		// RAZ
		gtk_text_buffer_get_start_iter(buffer, &start);
		gtk_text_buffer_get_end_iter(buffer, &iter);
		gtk_text_buffer_delete(buffer, &start, &iter);

		int i;
		for (i = 0; i < view->texts->diffText->nbTokens; i++) {
			gtk_text_buffer_get_end_iter(buffer, &iter);
			if ((view->texts->diffText->tokenizedText[i]->type == WORD) || (view->texts->diffText->tokenizedText[i]->type == SPACE)) {
				gtk_text_buffer_insert(view->refBuffer, &iter, view->texts->diffText->tokenizedText[i]->data.space, -1);
			} else if ((view->texts->diffText->tokenizedText[i]->type == ERASE)) {
				gtk_text_buffer_insert_with_tags_by_name(view->refBuffer, &iter, view->texts->diffText->tokenizedText[i]->data.word, -1, "red_foreground", NULL);
			} else if ((view->texts->diffText->tokenizedText[i]->type == INSERT)) {
				gtk_text_buffer_insert_with_tags_by_name(view->refBuffer, &iter, view->texts->diffText->tokenizedText[i]->data.word, -1, "green_foreground", NULL);
			} else if ((view->texts->diffText->tokenizedText[i]->type == REPLACE)) {
				gtk_text_buffer_insert_with_tags_by_name(view->refBuffer, &iter, view->texts->diffText->tokenizedText[i]->data.word, -1, "blue_foreground", NULL);
			} else if (view->texts->diffText->tokenizedText[i]->type == SHORT_SPACE) {
				int j = 0;
				while ((j <= 3) && (view->texts->diffText->tokenizedText[i]->data.space[j] != '\0' )) {
					j++;
				}
				gtk_text_buffer_insert(view->refBuffer, &iter, view->texts->diffText->tokenizedText[i]->data.space, j);
			}
		}
	}
}

