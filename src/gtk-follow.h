#ifndef _GTKFOLLOW_
#define _GTKFOLLOW_
#include <gtk/gtk.h>
#include <glib.h>
#include "follow.h"

// structure pour la vue
typedef struct viewGTK {
	GtkWindow * mainWindow;
	GtkTextView * textLeft;
	GtkTextView * textRight;
	GtkTextBuffer * refBuffer;
	GtkTextBuffer * newBuffer;
	GtkTextBuffer * diffBuffer;
	follow * texts;
} viewGTK;

// fonctions de gestion des événements
// ouverture du texte de référence
void open_ref_event(GtkMenuItem * menuItem, gpointer user_data);
// ouverture du texte modifié
void open_new_event(GtkMenuItem * menuItem, gpointer user_data);

#endif
