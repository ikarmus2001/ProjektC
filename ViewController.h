#ifndef VIEW_CONTROLLER_H
#define VIEW_CONTROLLER_H

#include <gtk/gtk.h>
#include "Structures/structures.h"
#include "databaseOperations.h"

GtkWidget* getNewData(unsigned char);
// unsigned char getNewData(unsigned char source, GtkWidget** newTreeView);

#endif //VIEW_CONTROLLER_H