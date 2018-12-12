#include "gtk-follow.h"

// macro GTK
G_MODULE_EXPORT

// fonction de gestion des événements
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
	}

	gtk_widget_hide(GTK_WIDGET(dialog));
}

int main(int argc, char * argv[]) {
	GtkWindow * mainWindow;
	GtkBuilder * ihm;

	viewGTK view;
	follow texts;

	gtk_init(&argc, &argv);
	ihm = gtk_builder_new();

	GError * err = NULL;
	if (! gtk_builder_add_from_file(ihm, "glade/mainWindow.glade", &err)) {
		fprintf(stderr, "Error with GTK builder: \n%d - %s\n", err->code, err->message);
		return 1;
	}

	texts = * (follow_init());
	view.texts = &texts;

	view.mainWindow = GTK_WINDOW(gtk_builder_get_object(ihm, "mainWindow"));

	gtk_builder_connect_signals(ihm, &view);

	gtk_widget_show(GTK_WIDGET(view.mainWindow));

	gtk_main();

	return 0;
}
