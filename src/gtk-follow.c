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
	// initialisation de la vue
	viewGTK * view = (viewGTK *) user_data;

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
		char * filename;
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

		// chargement du texte
		text * refText;
		if ((refText = text_load(filename)) == NULL)
			return;
		
		// association du texte à notre instance de follow
		view->texts->pRefText = refText;

		// tokenisation du texte
		text_tokenize(view->texts->map, view->texts->pRefText);

		// libération du nom de fichier
		g_free(filename);
	}

	gtk_widget_hide(GTK_WIDGET(dialog));
}

// ouverture du texte modifié
void open_new_event(GtkMenuItem * menuItem, gpointer user_data) {
	// initialisation de la vue
	viewGTK * view = (viewGTK *) user_data;

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
		char * filename;
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

		// chargement du texte
		text * refText;
		if ((refText = text_load(filename)) == NULL)
			return;
		
		// association du texte à notre instance de follow
		view->texts->pRefText = refText;

		// tokenisation du texte
		text_tokenize(view->texts->map, view->texts->pRefText);

		// libération du nom de fichier
		g_free(filename);
	}

	gtk_widget_hide(GTK_WIDGET(dialog));
}
