#ifndef VIEW_CONTROLLER_H
#define VIEW_CONTROLLER_H

#include <gtk/gtk.h>
#include "Structures/structures.h"
#include "databaseOperations.h"

GtkWidget* getNewData(unsigned char);
GtkWidget* createNewTreeView(JakasStruktura*, size_t);

#endif //VIEW_CONTROLLER_H