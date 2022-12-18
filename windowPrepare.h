#ifndef WINDOW_PREPARE_H
#define WINDOW_PREPARE_H

#include <gtk/gtk.h>

// todo: zmiana kolejności funkcji będzie całkiem na miejscu

void handleError(GError**);
void activate(GtkApplication*, gpointer);

#endif //WINDOWPREPARE_H