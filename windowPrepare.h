#ifndef WINDOW_PREPARE_H
#define WINDOW_PREPARE_H

#include <gtk/gtk.h>
#include <stdlib.h>

void handleError(GError**);
void activate(GtkApplication*, gpointer);

#endif //WINDOWPREPARE_H