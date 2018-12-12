#ifndef _GTKFOLLOW_
#define _GTKFOLLOW_
#include <gtk/gtk.h>
#include <glib.h>
#include "follow.h"

// structure pour la vue
typedef struct viewGTK {
	GtkWindow * mainWindow;
	follow * texts;
} viewGTK;

// fonction de gestion des événements
void open_ref_event(GtkMenuItem * menuItem, gpointer user_data);

#endif
